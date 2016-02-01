// -*- mode: c++ ; -*-
/* export_ascii_module.cc
 */

#include <stdexcept>
#include <sstream>

#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>

#include <snreconstruction/processing/export_ascii_module.h>
#include <snreconstruction/exports/export_ascii_event.h>

#include <datatools/service_manager.h>
#include <datatools/utils.h>

#include <sncore/models/data_model.h>
#include <sncore/models/event_header.h>
#include <sncore/models/calibrated_data.h>

#include <geomtools/geometry_service.h>
#include <geomtools/manager.h>


namespace snemo {

  namespace reconstruction {

    namespace processing {

      // Registration instantiation macro :
      DPP_MODULE_REGISTRATION_IMPLEMENT(export_ascii_module,
                                          "snemo::reconstruction::processing::export_ascii_module");

      bool export_ascii_module::is_terminated () const
      {
        return _io_accounting_.terminated;
      }

      export_ascii_module::io_accounting_type::io_accounting_type ()
      {
        reset ();
        return;
      }

      void export_ascii_module::io_accounting_type::reset ()
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

      void export_ascii_module::_set_defaults ()
      {
        _ascii_filenames_.reset ();
        _ascii_sink_.reset (0);
        _ascii_event_.reset (0);
        _io_accounting_.reset ();
        return;
      }

      void export_ascii_module::initialize(const datatools::properties  & setup_,
                                           datatools::service_manager   & service_manager_,
                                           dpp::module_handle_dict_type & module_dict_)
      {
        DT_THROW_IF (is_initialized (), std::logic_error,
                     "Module '" << get_name () << "' is already initialized ! ");

        dpp::base_module::_common_initialize (setup_);

        bool use_ascii_comments = false;
        if (setup_.has_flag ("use_ascii_comments"))
          {
            use_ascii_comments = true;
          }

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
        if (_ascii_filenames_.is_valid ())
          {
            _ascii_filenames_.reset ();
          }
        _ascii_filenames_.initialize (setup_);
        DT_THROW_IF (! _ascii_filenames_.is_valid (), std::logic_error,
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
        geomtools::geometry_service & Geo
          = service_manager_.get<geomtools::geometry_service> (geo_label);
        _exporter_.set_geom_manager (Geo.get_geom_manager ());

        // Initialize the exporter :
        datatools::properties exporter_setup;
        setup_.export_starting_with (exporter_setup, "export.");
        _exporter_.initialize (exporter_setup);

        // Initialize the export ASCII event :
        _ascii_event_.reset (new snemo::reconstruction::exports::export_ascii_event);
        _ascii_event_.get ()->add_comments = use_ascii_comments;

        _set_initialized (true);
        return;
      }

      void export_ascii_module::reset()
      {
        DT_THROW_IF (! is_initialized (), std::logic_error,
                     "Module '" << get_name () << "' is not initialized !");

        // Reset accounting info :
        _io_accounting_.reset ();

        // Reset the list of ASCII filenames :
        _ascii_filenames_.reset ();

        // Reset the export ASCII event :
        _ascii_event_.reset (0);

        // Reset the output file :
        if (_ascii_sink_.get () != 0)
          {
            // _ascii_sink_.get ()->close ();
            _ascii_sink_.reset (0);
          }

        _set_defaults ();
        _set_initialized (false);
        return;
      }

      // Constructor :
      export_ascii_module::export_ascii_module(datatools::logger::priority logging_priority)
      {
        _set_defaults ();
        return;
      }

      // Destructor :
      export_ascii_module::~export_ascii_module()
      {
        if (is_initialized()) export_ascii_module::reset();
        return;
      }


      // Processing :
      dpp::base_module::process_status export_ascii_module::process(datatools::things & data_record_)
      {
        DT_THROW_IF (! is_initialized (), std::logic_error,
                     "Module '" << get_name () << "' is not initialized !");

        // Main processing method :
        return _store_ascii (data_record_);
      }

      dpp::base_module::process_status export_ascii_module::_store_ascii (const datatools::things & data_record_)
      {
        dpp::base_module::process_status store_status = dpp::base_module::PROCESS_SUCCESS;
        DT_LOG_TRACE (get_logging_priority (), "Entering...");

        if (_io_accounting_.terminated)
          {
            // The module has now finished its job : we do not process this event
            store_status = dpp::base_module::PROCESS_STOP;
            return store_status;
          }

        if (_ascii_sink_.get () == 0)
          {
            _io_accounting_.file_index++;
            if (_io_accounting_.file_index >= (int)_ascii_filenames_.size ())
              {
                store_status = dpp::base_module::PROCESS_FATAL;
                return store_status;
              }
            const std::string sink_label = _ascii_filenames_[_io_accounting_.file_index];
            DT_LOG_DEBUG (get_logging_priority (), "Opening ASCII sink '" << sink_label << "'...");

            boost::filesystem::path sink_path(sink_label.c_str ());
            boost::filesystem::path sink_dir_path = sink_path.parent_path();
            const std::string sink_dir_str = boost::filesystem::basename (sink_dir_path);
            if (! sink_dir_str.empty () )
              {
                DT_THROW_IF (boost::filesystem::exists (sink_dir_path)
                             && ! boost::filesystem::is_directory (sink_dir_path),
                             std::logic_error,
                             "Path '" << sink_dir_path << "' is not a directory !");
                if (! boost::filesystem::is_directory (sink_dir_path))
                  {
                    DT_LOG_NOTICE (get_logging_priority (), "Creating base directory for ASCII sink '" << sink_label << "'...");
                    boost::filesystem::create_directories(sink_dir_path);
                  }
                else
                  {
                    DT_LOG_DEBUG (get_logging_priority (), "Base directory for ASCII sink '" << sink_label << "' already exists...");
                  }
              }
            _ascii_sink_.reset (new std::ofstream);
            _ascii_sink_.get ()->open (sink_label.c_str ());
            DT_THROW_IF (! *_ascii_sink_.get (), std::logic_error,
                         "Cannot open the ASCII sink '" << sink_label << "' !");
            // store_status = FATAL;
            // return store_status;
            _io_accounting_.file_record_counter = 0;
          }

        // force storage of the current event record :
        const bool store_it = true;
        // store action :
        if (store_it)
          {
            DT_LOG_TRACE (get_logging_priority (), "Store_it...");
            DT_THROW_IF (_ascii_sink_.get () == 0, std::logic_error,
                         "No available data sink ! This is a bug !");

            // Get a mutable reference to the ASCII export event data structure :
            snemo::reconstruction::exports::export_ascii_event & EE = *_ascii_event_.get ();

            // Algo here:
            _exporter_.run (data_record_, EE);

            DT_LOG_TRACE (get_logging_priority (), "Event storing...");
            EE.store (*_ascii_sink_.get (), _exporter_.get_export_flags ());
            DT_LOG_TRACE (get_logging_priority (), "Event stored !");
            DT_THROW_IF (! *_ascii_sink_.get (), std::logic_error,
                         "Cannot store the event record ! This is a bug !");
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
            if (_ascii_sink_.get () != 0)
              {
                _ascii_sink_.reset ();
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
            if ((_io_accounting_.file_index + 1) >= (int)_ascii_filenames_.size ())
              {
                stop_output = true;
                DT_LOG_NOTICE (get_logging_priority (),
                               "Module '" << get_name () << "' has filled "
                               << "the last requested output file (total is " << _ascii_filenames_.size () << " files)!");
              }
          }

        if (stop_output)
          {
            _io_accounting_.terminated = true;
          }


        DT_LOG_TRACE (get_logging_priority (), "Exiting.");
        return store_status;
      }

    } // end of namespace processing

  } // end of namespace reconstruction

} // end of namespace snemo

// end of export_ascii_module.cc
