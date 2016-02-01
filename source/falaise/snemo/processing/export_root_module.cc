// -*- mode: c++ ; -*-
/* export_root_module.cc
 */

#include <stdexcept>
#include <sstream>

#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>

#include <falaise/snemo/processing/export_root_module.h>
#include <falaise/snemo/exports/export_root_event.h>

#include <datatools/service_manager.h>
#include <datatools/utils.h>

#include <falaise/snemo/datamodels/data_model.h>
#include <falaise/snemo/datamodels/event_header.h>
#include <falaise/snemo/datamodels/calibrated_data.h>

#include <geomtools/geometry_service.h>
#include <geomtools/manager.h>

#include <TFile.h>
#include <TTree.h>

namespace snemo {

  namespace reconstruction {

    namespace processing {

      // Registration instantiation macro :
      DPP_MODULE_REGISTRATION_IMPLEMENT(export_root_module,
                                        "snemo::reconstruction::processing::export_root_module");

      bool export_root_module::is_terminated () const
      {
        return _io_accounting_.terminated;
      }

      export_root_module::io_accounting_type::io_accounting_type ()
      {
        reset ();
        return;
      }

      void export_root_module::io_accounting_type::reset ()
      {
        max_records_per_file = 0;
        max_records_total = 0;
        max_files = 0;
        terminated = false;
        file_record_counter = 0;
        record_counter = 0;
        file_index = -1;
        return;
      }

      void export_root_module::_set_defaults ()
      {
        _root_filenames_.reset ();
        _root_event_.reset (0);
        _io_accounting_.reset ();
        return;
      }

      void export_root_module::initialize(const datatools::properties  & setup_,
                                          datatools::service_manager   & service_manager_,
                                          dpp::module_handle_dict_type & module_dict_)
      {
        DT_THROW_IF (is_initialized (), std::logic_error,
                     "Module '" << get_name () << "' is already initialized ! ");

        dpp::base_module::_common_initialize (setup_);

        // I/O accounting :
        if (setup_.has_key ("max_records_total"))
          {
            _io_accounting_.max_records_total = setup_.fetch_integer ("max_records_total");
            if (_io_accounting_.max_records_total < 0) _io_accounting_.max_records_total = 0;
          }

        if (setup_.has_key ("max_records_per_file"))
          {
            _io_accounting_.max_records_per_file = setup_.fetch_integer ("max_records_per_file");
            if (_io_accounting_.max_records_per_file < 0) _io_accounting_.max_records_per_file = 0;
          }

        if (setup_.has_key ("max_files"))
          {
            _io_accounting_.max_files = setup_.fetch_integer ("max_files");
            if (_io_accounting_.max_files < 0) _io_accounting_.max_files = 0;
          }

        // File names :
        if (_root_filenames_.is_valid ())
          {
            _root_filenames_.reset ();
          }
        _root_filenames_.initialize (setup_);
        DT_THROW_IF (! _root_filenames_.is_valid (), std::logic_error,
                     "Module '" << get_name () << "' : invalid list of filenames !");

        // Service labels :
        DT_THROW_IF (! setup_.has_key ("Geo_label"), std::logic_error,
                     "Module '" << get_name () << "' has no valid '" << "Geo_label" << "' property !");
        const std::string geo_label = setup_.fetch_string ("Geo_label");
        // Geometry manager :
        DT_THROW_IF (! service_manager_.has (geo_label) ||
                     ! service_manager_.is_a<geomtools::geometry_service> (geo_label),
                     std::logic_error,
                     "Module '" << get_name () << "' has no '" << geo_label << "' service !");
        const geomtools::geometry_service & Geo
          = service_manager_.get<geomtools::geometry_service> (geo_label);
        _exporter_.set_geom_manager (Geo.get_geom_manager ());

        // Initialize the exporter :
        datatools::properties exporter_setup;
        setup_.export_starting_with (exporter_setup, "export.");
        _exporter_.initialize (exporter_setup);

        // Initialize the export event :
        _root_event_.reset (new snemo::reconstruction::exports::export_root_event);
        std::map<std::string,int> topics;
        if (_exporter_.get_topic_export_level("CAT")
            == snemo::reconstruction::exports::event_exporter::EXPORT_TOPIC_INCLUDE)
          {
            topics["CAT"] = snemo::reconstruction::exports::event_exporter::EXPORT_TOPIC_INCLUDE;
          }
        _root_event_.get()->construct (_exporter_.get_export_flags (),
                                       topics);

        _set_initialized (true);
        return;
      }

      void export_root_module::reset()
      {
        DT_THROW_IF (! is_initialized (), std::logic_error,
                     "Module '" << get_name () << "' is not initialized !");

        // Reset the output file :
        _close_file ();

        _set_defaults ();

        _set_initialized (false);
        return;
      }

      // Constructor :
      export_root_module::export_root_module(datatools::logger::priority logging_priority_)
        : dpp::base_module(logging_priority_)
      {
        _set_defaults ();
        _root_sink_ = 0;
        _root_tree_ = 0;
        return;
      }

      // Destructor :
      export_root_module::~export_root_module()
      {
        if (is_initialized ()) export_root_module::reset ();
        return;
      }

      // Processing :
      dpp::base_module::process_status export_root_module::process(datatools::things & data_record_)
      {
        DT_THROW_IF (! is_initialized (), std::logic_error,
                     "Module '" << get_name () << "' is not initialized !");

        // Main processing method :
        return _process_event (data_record_);
      }

      dpp::base_module::process_status export_root_module::_process_event (const datatools::things & data_record_)
      {
        dpp::base_module::process_status store_status = dpp::base_module::PROCESS_SUCCESS;
        DT_LOG_TRACE (get_logging_priority (), "Entering...");

        if (_io_accounting_.terminated)
          {
            // The module has now finished its job : we do not process this event
            store_status = dpp::base_module::PROCESS_STOP;
            return store_status;
          }

        if (_root_sink_ == 0)
          {
            _io_accounting_.file_index++;
            if (_io_accounting_.file_index >= (int)_root_filenames_.size ())
              {
                store_status = dpp::base_module::PROCESS_FATAL;
                return store_status;
              }
            DT_LOG_TRACE (get_logging_priority (), "_io_accounting_.file_index === "
                          << _io_accounting_.file_index << "'...");
            std::string sink_label = _root_filenames_[_io_accounting_.file_index];
            DT_LOG_DEBUG (get_logging_priority (), "Opening ROOT sink '" << sink_label << "'...");
            boost::filesystem::path sink_path(sink_label.c_str ());
            boost::filesystem::path sink_dir_path = sink_path.parent_path();
            std::string sink_dir_str = boost::filesystem::basename (sink_dir_path);
            if (! sink_dir_str.empty () )
              {
                DT_THROW_IF ( boost::filesystem::exists (sink_dir_path)
                              && ! boost::filesystem::is_directory (sink_dir_path),
                              std::logic_error,
                              "Path '" << sink_dir_path << "' is not a directory !");
                if (! boost::filesystem::is_directory (sink_dir_path))
                  {
                    DT_LOG_NOTICE (get_logging_priority (),
                                   "Creating base directory for ROOT sink '" << sink_label << "'...");
                    boost::filesystem::create_directories(sink_dir_path);
                  }
                else
                  {
                    DT_LOG_DEBUG (get_logging_priority (), "Base directory for ROOT sink '"
                                  << sink_label << "' already exists...");
                  }
              }
            _open_file (sink_label);
            _io_accounting_.file_record_counter = 0;
          }

        // force storage of the current event record :
        const bool store_it = true;
        // store action :
        if (store_it)
          {
            DT_THROW_IF (_root_sink_ == 0, std::logic_error,
                         "No available data sink ! This is a bug !");
            // Invoke the effective storage of the event data :
            _store_event (data_record_);

            // Statistics :
            _io_accounting_.file_record_counter++;
            _io_accounting_.record_counter++;
          }

        bool stop_file   = false;
        bool stop_output = false;

        if (_io_accounting_.max_records_total > 0)
          {
            if (_io_accounting_.record_counter >= _io_accounting_.max_records_total)
              {
                stop_output = true;
                stop_file   = true;
                DT_LOG_NOTICE (get_logging_priority (),
                               "Module '" << get_name () << "' has reached the maximum number of records "
                               << "stored in the output data source (" << _io_accounting_.max_records_total << ") !");
              }
          }

        if (_io_accounting_.max_records_per_file > 0)
          {
            if (_io_accounting_.file_record_counter >= _io_accounting_.max_records_per_file)
              {
                stop_file = true;
                DT_LOG_NOTICE (get_logging_priority (),
                               "Module '" << get_name () << "' has reached the maximum number of records "
                               << "to be stored in the current output file (" <<_io_accounting_.max_records_per_file << ") !");
              }
          }

        if (stop_file)
          {
            if (_root_sink_ != 0)
              {
                _close_file ();
              }
            _io_accounting_.file_record_counter = 0;
            if (_io_accounting_.max_files > 0)
              {
                if ((_io_accounting_.file_index + 1) >= _io_accounting_.max_files)
                  {
                    stop_output = true;
                    DT_LOG_NOTICE (get_logging_priority (),
                                   "Module '" << get_name () << "' has reached "
                                   << "the requested maximum number of output files (" << _io_accounting_.max_files << ") !");
                  }
              }
            if ((_io_accounting_.file_index + 1) >= (int)_root_filenames_.size ())
              {
                stop_output = true;
                DT_LOG_NOTICE (get_logging_priority (),
                               "Module '" << get_name () << "' has filled "
                               << "the last requested output file (total is " << _root_filenames_.size () << " files)!");
              }
          }

        if (stop_output)
          {
            _io_accounting_.terminated = true;
          }

        DT_LOG_TRACE (get_logging_priority (), "Exiting.");
        return store_status;
      }


      dpp::base_module::process_status export_root_module::_open_file (const std::string & sink_label_)
      {
        DT_LOG_TRACE (get_logging_priority (), "Entering...");
        DT_THROW_IF (_root_sink_ != 0, std::logic_error,
                     "Former ROOT file is not NULL ! This is a bug !");
        _root_sink_ = new TFile(sink_label_.c_str (),
                                "RECREATE",
                                "SuperNEMO event record ROOT export");
        if (_root_sink_ == 0)
          {
            DT_LOG_ERROR (get_logging_priority (), "Cannot open the ROOT file ('" << sink_label_ << "') !");
            // Hmmm throw ?
            //throw std::logic_error (message.str ());
            return dpp::base_module::PROCESS_FATAL;
          }
        if (! _root_sink_->IsWritable ())
          {
            DT_LOG_ERROR (get_logging_priority (), "Cannot write ROOT output file ('" << sink_label_ << "') !");
            //throw std::runtime_error (message.str());
            return dpp::base_module::PROCESS_FATAL;
          }
        _root_sink_->cd ();
        _init_tree ();
        DT_LOG_TRACE (get_logging_priority (), "Exiting.");
        return dpp::base_module::PROCESS_SUCCESS;
      }

      int export_root_module::_terminate_tree ()
      {
        DT_LOG_TRACE (get_logging_priority (), "Entering...");
        if (_root_tree_ != 0)
          {
            _root_tree_->Print ();
            snemo::reconstruction::exports::export_root_event & EE = *_root_event_.get ();
            EE.detach_branches ();
            _root_tree_->SetDirectory (_root_sink_);
            // Ensure the ROOT file is the current directory :
            _root_sink_->cd ();
            _root_tree_->Write ();
            DT_LOG_TRACE (get_logging_priority (), "ROOT tree was writen.");
            _root_tree_->SetDirectory (0);
            _root_tree_ = 0;
          }
        DT_LOG_TRACE (get_logging_priority (), "Exiting.");
        return 0;
      }

      int export_root_module::_init_tree ()
      {
        DT_LOG_TRACE (get_logging_priority (), "Entering...");
        _root_tree_ = new TTree("snemodata","SuperNEMO event model");
        _root_tree_->SetDirectory (_root_sink_); // make even more sure...

        snemo::reconstruction::exports::export_root_event & EE = *_root_event_.get ();
        EE.setup_tree (_root_tree_);
        _root_tree_->Print ();
        DT_LOG_TRACE (get_logging_priority (), "Exiting.");
        return 0;
      }

      int export_root_module::_close_file ()
      {
        DT_LOG_TRACE (get_logging_priority (), "Entering...");
        _terminate_tree ();
        if (_root_sink_ != 0)
          {
            _root_sink_->cd();
            _root_sink_->Write();
            _root_sink_->Close();
            delete _root_sink_;
            _root_sink_ = 0;
            DT_LOG_DEBUG (get_logging_priority (), "ROOT file is closed.");
          }
        DT_LOG_TRACE (get_logging_priority (), "Exiting.");
        return 0;
      }


      int export_root_module::_store_event (const snemo::datamodel::event_record & event_record_)
      {
        DT_LOG_TRACE (get_logging_priority (), "Entering...");
        // Get a reference to the export ROOT event :
        snemo::reconstruction::exports::export_root_event & EE = *_root_event_.get ();
        DT_LOG_DEBUG (get_logging_priority (), "Reference to exported ROOT event is ok.");

        // Export the SN@ilWare event data model to the export event:
        _exporter_.run (event_record_, EE);
        DT_LOG_DEBUG (get_logging_priority (), "SN@ilWare event has been exported.");

        // Fill the memory addressed by branches in the ROOT tree:
        EE.fill_memory ();
        DT_LOG_DEBUG (get_logging_priority (), "Exported ROOT event has been pushed in branched memory.");

        // Final store, using the 'export setup' of the exporter  :
        _root_tree_->SetDirectory (_root_sink_);
        _root_tree_->Fill ();
        DT_LOG_TRACE (get_logging_priority (), "Exiting.");
        return 0;
      }

    } // end of namespace processing

  } // end of namespace reconstruction

} // end of namespace snemo

// end of export_root_module.cc
