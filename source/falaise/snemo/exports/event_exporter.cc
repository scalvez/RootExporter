// -*- mode: c++ ; -*-
/* event_exporter.cc */

#include <falaise/snemo/exports/event_exporter.h>
#include <falaise/snemo/exports/export_event.h>

#include <mctools/simulated_data.h>
#include <falaise/snemo/datamodels/event_header.h>
#include <falaise/snemo/datamodels/calibrated_data.h>
#include <falaise/snemo/datamodels/tracker_clustering_data.h>
#include <falaise/snemo/datamodels/tracker_clustering_solution.h>
#include <falaise/snemo/datamodels/tracker_cluster.h>
#include <falaise/snemo/datamodels/tracker_trajectory_data.h>
#include <falaise/snemo/datamodels/tracker_trajectory_solution.h>
#include <falaise/snemo/datamodels/tracker_trajectory.h>

#include <mctools/utils.h>
// #include <sncore/utils/utils.h>

#include <geomtools/manager.h>
#include <datatools/things_macros.h>

#include <boost/algorithm/string/replace.hpp>
#include <boost/foreach.hpp>


namespace snemo {

  namespace reconstruction {

    namespace exports {

      // namespace scm = snemo::core::model;
      namespace sdm = snemo::datamodel;
      // namespace scu = snemo::core::utils;
      namespace sre = snemo::reconstruction::exports;

      bool event_exporter::is_initialized () const
      {
        return _initialized_;
      }

      bool event_exporter::has_geom_manager () const
      {
        return _geom_manager_ != 0;
      }

      const geomtools::manager & event_exporter::get_geom_manager () const
      {
        return *_geom_manager_;
      }

      void event_exporter::set_geom_manager (const geomtools::manager & gmgr_)
      {
        DT_THROW_IF (is_initialized (), std::logic_error, "Event exporter is already initialized ! ");
        _geom_manager_ = &gmgr_;

        // Check setup label:
        const std::string & setup_label = _geom_manager_->get_setup_label ();

        // DT_THROW_IF(setup_label != "snemo" && setup_label != "snemo::tracker_commissioning",
        //             std::logic_error, "Setup label '" << setup_label << "' is not supported !");

        _init_gid_infos_ ();
        return;
      }

      event_exporter::gid_info_type::gid_info_type ()
      {
        reset ();
        return;
      }

      void event_exporter::gid_info_type::reset ()
      {
        gid_gg_module_index    = geomtools::geom_id::INVALID_ADDRESS;
        gid_gg_side_index      = geomtools::geom_id::INVALID_ADDRESS;
        gid_gg_layer_index     = geomtools::geom_id::INVALID_ADDRESS;
        gid_gg_row_index       = geomtools::geom_id::INVALID_ADDRESS;

        gid_calo_geom_type     = geomtools::geom_id::INVALID_TYPE;
        gid_calo_module_index  = geomtools::geom_id::INVALID_ADDRESS;
        gid_calo_side_index    = geomtools::geom_id::INVALID_ADDRESS;
        gid_calo_column_index  = geomtools::geom_id::INVALID_ADDRESS;
        gid_calo_row_index     = geomtools::geom_id::INVALID_ADDRESS;

        gid_xcalo_geom_type    = geomtools::geom_id::INVALID_TYPE;
        gid_xcalo_module_index = geomtools::geom_id::INVALID_ADDRESS;
        gid_xcalo_side_index   = geomtools::geom_id::INVALID_ADDRESS;
        gid_xcalo_column_index = geomtools::geom_id::INVALID_ADDRESS;
        gid_xcalo_row_index    = geomtools::geom_id::INVALID_ADDRESS;
        gid_xcalo_wall_index   = geomtools::geom_id::INVALID_ADDRESS;

        gid_gveto_geom_type    = geomtools::geom_id::INVALID_TYPE;
        gid_gveto_module_index = geomtools::geom_id::INVALID_ADDRESS;
        gid_gveto_side_index   = geomtools::geom_id::INVALID_ADDRESS;
        gid_gveto_column_index = geomtools::geom_id::INVALID_ADDRESS;
        gid_gveto_wall_index   = geomtools::geom_id::INVALID_ADDRESS;

        return;
      }

      void event_exporter::_init_gid_infos_ ()
      {
        _gid_infos_.reset ();
        const geomtools::id_mgr & the_id_mgr = _geom_manager_->get_id_mgr ();

        // Geometry category types used for different kind of scintillator blocks :

        // CALO :
        _gid_infos_.gid_calo_geom_type  =
          the_id_mgr.get_category_info ("calorimeter_block").get_type (); // 1302;
        _gid_infos_.gid_calo_module_index
          = the_id_mgr.get_category_info ("calorimeter_block").get_subaddress_index ("module");
        _gid_infos_.gid_calo_side_index
          = the_id_mgr.get_category_info ("calorimeter_block").get_subaddress_index ("side");
        _gid_infos_.gid_calo_column_index
          = the_id_mgr.get_category_info ("calorimeter_block").get_subaddress_index ("column");
        _gid_infos_.gid_calo_row_index
          = the_id_mgr.get_category_info ("calorimeter_block").get_subaddress_index ("row");

        // XCALO :
        _gid_infos_.gid_xcalo_geom_type =
          the_id_mgr.get_category_info ("xcalo_block").get_type (); // 1232;
        _gid_infos_.gid_xcalo_module_index
          = the_id_mgr.get_category_info ("xcalo_block").get_subaddress_index ("module");
        _gid_infos_.gid_xcalo_side_index
          = the_id_mgr.get_category_info ("xcalo_block").get_subaddress_index ("side");
        _gid_infos_.gid_xcalo_wall_index
          = the_id_mgr.get_category_info ("xcalo_block").get_subaddress_index ("wall");
        _gid_infos_.gid_xcalo_column_index
          = the_id_mgr.get_category_info ("xcalo_block").get_subaddress_index ("column");
        _gid_infos_.gid_xcalo_row_index
          = the_id_mgr.get_category_info ("xcalo_block").get_subaddress_index ("row");

        // GVETO :
        _gid_infos_.gid_gveto_geom_type =
          the_id_mgr.get_category_info ("gveto_block").get_type (); // 1252;
        _gid_infos_.gid_gveto_module_index
          = the_id_mgr.get_category_info ("gveto_block").get_subaddress_index ("module");
        _gid_infos_.gid_gveto_side_index
          = the_id_mgr.get_category_info ("gveto_block").get_subaddress_index ("side");
        _gid_infos_.gid_gveto_wall_index
          = the_id_mgr.get_category_info ("gveto_block").get_subaddress_index ("wall");
        _gid_infos_.gid_gveto_column_index
          = the_id_mgr.get_category_info ("gveto_block").get_subaddress_index ("column");

        // Geometry category type used for Geiger drift cells :
        _gid_infos_.gid_gg_geom_type  =
          the_id_mgr.get_category_info ("drift_cell_core").get_type (); // 1204;
        _gid_infos_.gid_gg_module_index
          = the_id_mgr.get_category_info ("drift_cell_core").get_subaddress_index ("module");
        _gid_infos_.gid_gg_side_index
          = the_id_mgr.get_category_info ("drift_cell_core").get_subaddress_index ("side");
        _gid_infos_.gid_gg_layer_index
          = the_id_mgr.get_category_info ("drift_cell_core").get_subaddress_index ("layer");
        _gid_infos_.gid_gg_row_index
          = the_id_mgr.get_category_info ("drift_cell_core").get_subaddress_index ("row");

        return;
      }

      uint32_t event_exporter::get_export_flags () const
      {
        return _export_flags_;
      }

      void event_exporter::unset_exported (unsigned int export_bit_)
      {
        _export_flags_ &= ~export_bit_;
        return;
      }

      void event_exporter::set_exported (unsigned int export_bit_)
      {
        _export_flags_ |= export_bit_;
        return;
      }

      bool event_exporter::is_exported (unsigned int export_bit_) const
      {
        return (_export_flags_ & export_bit_);
      }

      bool event_exporter::are_cat_infos_exported () const
      {
        return _export_cat_infos_;
      }

      void event_exporter::set_cat_infos_exported (bool xci_)
      {
        _export_cat_infos_ = xci_;
      }

      int event_exporter::get_topic_export_level(const std::string & topic_label_) const
      {
        if (topic_label_ == "CAT")
          {
            if (_export_cat_infos_) return EXPORT_TOPIC_INCLUDE;
         }
        return EXPORT_TOPIC_NO_INCLUDE;
      }

      event_exporter::event_exporter ()
      {
        _initialized_ = false;
        _init_defaults ();
        return;
      }

      event_exporter::~event_exporter ()
      {
        if (is_initialized ())
          {
            reset ();
          }
        return;
      }

      void event_exporter::initialize (const datatools::properties & setup_)
      {
        {
          datatools::properties::keys_col_type bank_label_keys;
          setup_.keys_starting_with(bank_label_keys, "export.bank.label.");
          for (datatools::properties::keys_col_type::const_iterator i
                 = bank_label_keys.begin ();
               i != bank_label_keys.end ();
               i++)
            {
              std::string bank_label = *i;
              boost::algorithm::replace_first(bank_label, "export.bank.label.", "");
              _bank_labels_[bank_label] = setup_.fetch_string (*i);
            }
        }

        if (setup_.has_flag ("export.cat_infos"))
          {
            set_cat_infos_exported (true);
          }

        if (setup_.has_flag ("export.event_header"))
          {
            set_exported (sre::event_exporter::EXPORT_EVENT_HEADER);
          }

        if (setup_.has_flag ("export.true_particles"))
          {
            set_exported (sre::event_exporter::EXPORT_TRUE_PARTICLES);
          }

        if (setup_.has_flag ("export.true_step_hits"))
          {
            set_exported (sre::event_exporter::EXPORT_TRUE_STEP_HITS);
          }

        if (setup_.has_flag ("export.true_hits"))
          {
            set_exported (sre::event_exporter::EXPORT_TRUE_HITS);
          }

        if (setup_.has_flag ("export.calib_calorimeter_hits"))
          {
            set_exported (sre::event_exporter::EXPORT_CALIB_CALORIMETER_HITS);
          }

        if (setup_.has_flag ("export.calib_tracker_hits"))
          {
            set_exported (sre::event_exporter::EXPORT_CALIB_TRACKER_HITS);
          }

        if (setup_.has_flag ("export.tracker_clustering"))
          {
            set_exported (sre::event_exporter::EXPORT_TRACKER_CLUSTERING);
          }

        // If no export directive is given, export all data banks :
        if (_export_flags_ == sre::event_exporter::NO_EXPORT)
          {
            set_exported (sre::event_exporter::EXPORT_ALL);
          }

        //dump (std::clog);

        _initialized_ = true;
        return;
      }

      void event_exporter::reset ()
      {
        DT_THROW_IF (! is_initialized (), std::logic_error,  "Event exporter is not initialized ! ");
        _initialized_ = false;
        _init_defaults ();
        return;
      }

      void event_exporter::_set_default_bank_labels ()
      {
        _bank_labels_.clear ();
        // Default bank names for "EH", "SD", "CD", "TCD", "TTD" :
        _bank_labels_[sdm::data_info::EVENT_HEADER_LABEL]            = sdm::data_info::EVENT_HEADER_LABEL;
        _bank_labels_[sdm::data_info::SIMULATED_DATA_LABEL]          = sdm::data_info::SIMULATED_DATA_LABEL;
        _bank_labels_[sdm::data_info::CALIBRATED_DATA_LABEL]         = sdm::data_info::CALIBRATED_DATA_LABEL;
        _bank_labels_[sdm::data_info::TRACKER_CLUSTERING_DATA_LABEL] = sdm::data_info::TRACKER_CLUSTERING_DATA_LABEL;
        _bank_labels_[sdm::data_info::TRACKER_TRAJECTORY_DATA_LABEL] = sdm::data_info::TRACKER_TRAJECTORY_DATA_LABEL;
        return;
      }

      void event_exporter::_init_defaults ()
      {
        _set_default_bank_labels ();
        _export_flags_ = NO_EXPORT;
        _geom_manager_ = 0;
        _export_cat_infos_ = false;
        return;
      }

      //static
      std::string event_exporter::get_export_bit_label (unsigned int bit_)
      {
        if (bit_ == event_exporter::EXPORT_EVENT_HEADER) return "event_header";
        if (bit_ == event_exporter::EXPORT_TRUE_PARTICLES) return "true_particles";
        if (bit_ == event_exporter::EXPORT_TRUE_STEP_HITS) return "true_step_hits";
        if (bit_ == event_exporter::EXPORT_TRUE_HITS) return "true_hits";
        if (bit_ == event_exporter::EXPORT_CALIB_CALORIMETER_HITS) return "calib_calorimeter_hits";
        if (bit_ == event_exporter::EXPORT_CALIB_TRACKER_HITS) return "calib_tracker_hits";
        if (bit_ == event_exporter::EXPORT_TRACKER_CLUSTERING) return "tracker_clustering";
        if (bit_ == event_exporter::EXPORT_TRACKER_TRAJECTORIES) return "tracker_trajectories";
        return "";
      }

      void event_exporter::dump (std::ostream & out_) const
      {
        out_ << "Event exporter: " << std::endl;
        out_ << "|-- " << "Initialized : " << is_initialized () << std::endl;
        out_ << "|-- " << "Geometry manager : ";
        if (_geom_manager_ != 0 )
          {
            out_ << _geom_manager_->get_setup_label () << ' '
                 << _geom_manager_->get_setup_version () << ' ';
          }
        else
          {
            out_ << "<none>";
          }
        out_ << std::endl;
        out_ << "|-- " << "Bank labels : " << std::endl;
        for (std::map<std::string, std::string>::const_iterator i
               = _bank_labels_.begin();
             i != _bank_labels_.end();
             i++)
          {
            std::map<std::string, std::string>::const_iterator j = i;
            j++;
            out_ << "|   ";
            if (j == _bank_labels_.end())
              {
                out_ << "`-- ";
              }
            else
              {
                out_ << "|-- ";
              }
            out_ << i->first << " : " << "'" << i->second << "'" << std::endl;
          }

        out_ << "`-- " << "Export flags : " << '[' << _export_flags_ << ']' << std::endl;
        for (unsigned int bit = 1; bit <= event_exporter::EXPORT_LAST; bit *=2)
          {
            out_ << "    ";
            if (bit == event_exporter::EXPORT_LAST)
              {
                out_ << "`-- ";
              }
            else
              {
                out_ << "|-- ";
              }
            out_ << "Export '" << event_exporter::get_export_bit_label (bit) << "' : ";
            if (_export_flags_ & bit)
              {
                out_ << "yes";
              }
            else
              {
                out_ << "no";
              }
            out_ << std::endl;
            //if (bit == event_exporter::EXPORT_LAST) break;
          }
        return;
      }

      int event_exporter::run (const sdm::event_record & er_,
                               sre::export_event & ee_)
      {
        if (! is_initialized ())
          {
            std::ostringstream message;
            message << "snemo::reconstruction::exports::event_exporter::run: "
                    << "Event exporter is not initialized ! ";
            throw std::logic_error (message.str ());
          }

        ee_.clear_data ();
        if (is_exported (sre::event_exporter::EXPORT_EVENT_HEADER))
          {
            _export_event_header (er_, ee_);
          }

        if (is_exported (sre::event_exporter::EXPORT_TRUE_PARTICLES))
          {
            _export_true_particles (er_, ee_);
          }

        if (is_exported (sre::event_exporter::EXPORT_TRUE_STEP_HITS))
          {
            _export_true_step_hits (er_, ee_);
          }

        if (is_exported (sre::event_exporter::EXPORT_TRUE_HITS))
          {
            _export_true_hits (er_, ee_);
          }

        if (is_exported (sre::event_exporter::EXPORT_CALIB_CALORIMETER_HITS))
          {
            _export_calib_calorimeter_hits (er_, ee_);
          }

        if (is_exported (sre::event_exporter::EXPORT_CALIB_TRACKER_HITS))
          {
            _export_calib_tracker_hits (er_, ee_);
          }

        if (is_exported (sre::event_exporter::EXPORT_TRACKER_CLUSTERING))
          {
            _export_tracker_clustering (er_, ee_);
          }

        if (is_exported (sre::event_exporter::EXPORT_TRACKER_TRAJECTORIES))
          {
            _export_tracker_trajectories (er_, ee_);
          }

        // ee_.print (std::clog, "Export event", "DEVEL: ");
        return 0;
      }

      const std::map<std::string, std::string> &
      event_exporter::get_bank_labels () const
      {
        return _bank_labels_;
      }

      int event_exporter::_export_event_header (const datatools::things & er_,
                                                sre::export_event & ee_)
      {
        const std::string & eh_label = _bank_labels_[sdm::data_info::EVENT_HEADER_LABEL];
        if (! DATATOOLS_THINGS_CHECK_BANK(er_, eh_label, sdm::event_header))
          {
            DT_THROW_IF (true, std::logic_error, "Missing event header data to be processed !");
          }
        DATATOOLS_THINGS_CONST_BANK(er_, eh_label, sdm::event_header, EH);

        ee_.event_header.run_number   = EH.get_id ().get_run_number ();
        ee_.event_header.event_number = EH.get_id ().get_event_number ();
        ee_.event_header.simulated    = EH.is_simulated ();
        ee_.event_header.seconds      = EH.get_timestamp ().get_seconds ();
        ee_.event_header.picoseconds  = EH.get_timestamp ().get_picoseconds ();

        // Specific infos from CAT are stored in various places of the data model :
        if (are_cat_infos_exported ())
          {
            ee_.event_header.export_cat_infos = true;
          }
        return 0;
      }

      int event_exporter::_export_true_particles (const datatools::things & er_,
                                                  sre::export_event & ee_)
      {
        const std::string & sd_label = _bank_labels_ [sdm::data_info::SIMULATED_DATA_LABEL];
        if (! DATATOOLS_THINGS_CHECK_BANK(er_, sd_label, mctools::simulated_data))
          {
            DT_THROW_IF (true, std::logic_error, "Missing simulated data to be processed !");
          }
        DATATOOLS_THINGS_CONST_BANK(er_, sd_label, mctools::simulated_data, SD);

        // Vertex :
        true_vertex_type true_vertex;
        true_vertex.vertex_id = 0;
        true_vertex.x = SD.get_vertex().x() / CLHEP::mm;
        true_vertex.y = SD.get_vertex().y() / CLHEP::mm;
        true_vertex.z = SD.get_vertex().z() / CLHEP::mm;
        true_vertex.time = SD.get_primary_event ().get_time () / CLHEP::ns;
        ee_.true_vertices.push_back (true_vertex);

        // Primary particle :
        int primary_track_id = 0;
        for (::genbb::primary_event::particles_col_type::const_iterator i
               = SD.get_primary_event ().get_particles ().begin () ;
             i != SD.get_primary_event ().get_particles ().end ();
             i++)
          {
            const ::genbb::primary_particle & the_particle = *i;
            {
              true_particle_type dummy;
              ee_.true_particles.push_back (dummy);
            }
            true_particle_type & true_particle = ee_.true_particles.back ();
            true_particle.track_id = primary_track_id;
            true_particle.particle_type = the_particle.get_type();
            true_particle.px = the_particle.get_momentum().x() / CLHEP::keV;
            true_particle.py = the_particle.get_momentum().y() / CLHEP::keV;
            true_particle.pz = the_particle.get_momentum().z() / CLHEP::keV;
            true_particle.time = the_particle.get_time () / CLHEP::ns;
            true_particle.vertex_id = 0;
            primary_track_id++;
          }

        return 0;
      }

      int event_exporter::_export_true_step_hits (const datatools::things & er_,
                                                  sre::export_event & ee_)
      {
        const std::string & sd_label = _bank_labels_ [sdm::data_info::SIMULATED_DATA_LABEL];
        if (! DATATOOLS_THINGS_CHECK_BANK(er_, sd_label, mctools::simulated_data))
          {
            DT_THROW_IF (true, std::logic_error, "Missing simulated data to be processed !");
          }
        DATATOOLS_THINGS_CONST_BANK(er_, sd_label, mctools::simulated_data, SD);

        // Not implemented yet.

        return 0;
      }

      int event_exporter::_export_true_hits (const datatools::things & er_,
                                             sre::export_event & ee_)
      {
        const std::string & sd_label = _bank_labels_[sdm::data_info::SIMULATED_DATA_LABEL];
        if (! DATATOOLS_THINGS_CHECK_BANK(er_, sd_label, mctools::simulated_data))
          {
            DT_THROW_IF (true, std::logic_error, "Missing simulated data to be processed !");
          }
        DATATOOLS_THINGS_CONST_BANK(er_, sd_label, mctools::simulated_data, SD);

        // Calo true hits :
        const std::string calo_hit_label = "calo";
        if (SD.has_step_hits (calo_hit_label))
          {
            unsigned int ncalo = SD.get_number_of_step_hits (calo_hit_label);
            for (unsigned int ihit = 0; ihit < ncalo; ihit++)
              {
                const mctools::base_step_hit & sncore_true_scin_hit
                  = SD.get_step_hit (calo_hit_label, ihit);
                {
                  true_scin_hit_type dummy;
                  ee_.true_calo_hits.push_back (dummy);
                }
                true_scin_hit_type & true_scin_hit = ee_.true_calo_hits.back ();
                true_scin_hit.hit_id = sncore_true_scin_hit.get_hit_id();
                true_scin_hit.type = constants::CALO_TYPE;
                true_scin_hit.module = sncore_true_scin_hit.get_geom_id ().get (_gid_infos_.gid_calo_module_index);
                true_scin_hit.side = sncore_true_scin_hit.get_geom_id ().get (_gid_infos_.gid_calo_side_index);
                true_scin_hit.column = sncore_true_scin_hit.get_geom_id ().get (_gid_infos_.gid_calo_column_index);
                true_scin_hit.row = sncore_true_scin_hit.get_geom_id ().get (_gid_infos_.gid_calo_row_index);
                true_scin_hit.wall = constants::INVALID_ID;
                true_scin_hit.tfirst = sncore_true_scin_hit.get_time_start () / CLHEP::ns;
                true_scin_hit.tlast = sncore_true_scin_hit.get_time_stop () / CLHEP::ns;
                true_scin_hit.x1 = sncore_true_scin_hit.get_position_start ().x () / CLHEP::mm;
                true_scin_hit.y1 = sncore_true_scin_hit.get_position_start ().y () / CLHEP::mm;
                true_scin_hit.z1 = sncore_true_scin_hit.get_position_start ().z () / CLHEP::mm;
                true_scin_hit.x2 = sncore_true_scin_hit.get_position_stop ().x () / CLHEP::mm;
                true_scin_hit.y2 = sncore_true_scin_hit.get_position_stop ().y () / CLHEP::mm;
                true_scin_hit.z2 = sncore_true_scin_hit.get_position_stop ().z () / CLHEP::mm;
                true_scin_hit.delta_energy = sncore_true_scin_hit.get_energy_deposit () / CLHEP::keV;
              }
          }

        // Xcalo true hits :
        const std::string xcalo_hit_label = "xcalo";
        if (SD.has_step_hits (xcalo_hit_label))
          {
            unsigned int nxcalo = SD.get_number_of_step_hits (xcalo_hit_label);
            for (unsigned int ihit = 0; ihit < nxcalo; ihit++)
              {
                const mctools::base_step_hit & sncore_true_scin_hit
                  = SD.get_step_hit (xcalo_hit_label, ihit);
                {
                  true_scin_hit_type dummy;
                  ee_.true_xcalo_hits.push_back (dummy);
                }
                true_scin_hit_type & true_scin_hit = ee_.true_xcalo_hits.back ();
                true_scin_hit.hit_id = sncore_true_scin_hit.get_hit_id();
                true_scin_hit.type = constants::XCALO_TYPE;
                true_scin_hit.module = sncore_true_scin_hit.get_geom_id ().get (_gid_infos_.gid_xcalo_module_index);
                true_scin_hit.side = sncore_true_scin_hit.get_geom_id ().get (_gid_infos_.gid_xcalo_side_index);
                true_scin_hit.wall = sncore_true_scin_hit.get_geom_id ().get (_gid_infos_.gid_xcalo_wall_index);
                true_scin_hit.column = sncore_true_scin_hit.get_geom_id ().get (_gid_infos_.gid_xcalo_column_index);
                true_scin_hit.row = sncore_true_scin_hit.get_geom_id ().get (_gid_infos_.gid_xcalo_row_index);
                true_scin_hit.tfirst = sncore_true_scin_hit.get_time_start () / CLHEP::ns;
                true_scin_hit.tlast = sncore_true_scin_hit.get_time_stop () / CLHEP::ns;
                true_scin_hit.x1 = sncore_true_scin_hit.get_position_start ().x () / CLHEP::mm;
                true_scin_hit.y1 = sncore_true_scin_hit.get_position_start ().y () / CLHEP::mm;
                true_scin_hit.z1 = sncore_true_scin_hit.get_position_start ().z () / CLHEP::mm;
                true_scin_hit.x2 = sncore_true_scin_hit.get_position_stop ().x () / CLHEP::mm;
                true_scin_hit.y2 = sncore_true_scin_hit.get_position_stop ().y () / CLHEP::mm;
                true_scin_hit.z2 = sncore_true_scin_hit.get_position_stop ().z () / CLHEP::mm;
                true_scin_hit.delta_energy = sncore_true_scin_hit.get_energy_deposit () / CLHEP::keV;
              }
          }

        // Gveto true hits :
        const std::string gveto_hit_label = "gveto";
        if (SD.has_step_hits (gveto_hit_label))
          {
            unsigned int ngveto = SD.get_number_of_step_hits (gveto_hit_label);
            for (unsigned int ihit = 0; ihit < ngveto; ihit++)
              {
                const mctools::base_step_hit & sncore_true_scin_hit
                  = SD.get_step_hit (gveto_hit_label, ihit);
                {
                  true_scin_hit_type dummy;
                  ee_.true_gveto_hits.push_back (dummy);
                }
                true_scin_hit_type & true_scin_hit = ee_.true_gveto_hits.back ();
                true_scin_hit.hit_id = sncore_true_scin_hit.get_hit_id();
                true_scin_hit.type   = constants::GVETO_TYPE;
                true_scin_hit.module = sncore_true_scin_hit.get_geom_id ().get (_gid_infos_.gid_gveto_module_index);
                true_scin_hit.side   = sncore_true_scin_hit.get_geom_id ().get (_gid_infos_.gid_gveto_side_index);
                true_scin_hit.wall   = sncore_true_scin_hit.get_geom_id ().get (_gid_infos_.gid_gveto_wall_index);
                true_scin_hit.column = sncore_true_scin_hit.get_geom_id ().get (_gid_infos_.gid_gveto_column_index);
                true_scin_hit.row    = constants::INVALID_ID;
                true_scin_hit.tfirst = sncore_true_scin_hit.get_time_start () / CLHEP::ns;
                true_scin_hit.tlast  = sncore_true_scin_hit.get_time_stop () / CLHEP::ns;
                true_scin_hit.x1 = sncore_true_scin_hit.get_position_start ().x () / CLHEP::mm;
                true_scin_hit.y1 = sncore_true_scin_hit.get_position_start ().y () / CLHEP::mm;
                true_scin_hit.z1 = sncore_true_scin_hit.get_position_start ().z () / CLHEP::mm;
                true_scin_hit.x2 = sncore_true_scin_hit.get_position_stop ().x () / CLHEP::mm;
                true_scin_hit.y2 = sncore_true_scin_hit.get_position_stop ().y () / CLHEP::mm;
                true_scin_hit.z2 = sncore_true_scin_hit.get_position_stop ().z () / CLHEP::mm;
                true_scin_hit.delta_energy = sncore_true_scin_hit.get_energy_deposit () / CLHEP::keV;
              }
          }

        // Gg true hits :
        const std::string gg_hit_label = "gg";
        if (SD.has_step_hits (gg_hit_label))
          {
            unsigned int ngg = SD.get_number_of_step_hits (gg_hit_label);
            for (unsigned int ihit = 0; ihit < ngg; ihit++)
              {
                const mctools::base_step_hit & sncore_true_gg_hit
                  = SD.get_step_hit (gg_hit_label, ihit);
                {
                  true_gg_hit_type dummy;
                  ee_.true_gg_hits.push_back (dummy);
                }
                true_gg_hit_type & true_gg_hit = ee_.true_gg_hits.back ();
                true_gg_hit.hit_id = sncore_true_gg_hit.get_hit_id();
                true_gg_hit.module = sncore_true_gg_hit.get_geom_id ().get (_gid_infos_.gid_gg_module_index);
                true_gg_hit.side   = sncore_true_gg_hit.get_geom_id ().get (_gid_infos_.gid_gg_side_index);
                true_gg_hit.layer  = sncore_true_gg_hit.get_geom_id ().get (_gid_infos_.gid_gg_layer_index);
                true_gg_hit.row    = sncore_true_gg_hit.get_geom_id ().get (_gid_infos_.gid_gg_row_index);
                true_gg_hit.tionization = sncore_true_gg_hit.get_time_start () / CLHEP::ns;
                true_gg_hit.xionization = sncore_true_gg_hit.get_position_start ().x () / CLHEP::mm;
                true_gg_hit.yionization = sncore_true_gg_hit.get_position_start ().y () / CLHEP::mm;
                true_gg_hit.zionization = sncore_true_gg_hit.get_position_start ().z () / CLHEP::mm;
                true_gg_hit.pxionization = sncore_true_gg_hit.get_momentum_start ().x () / CLHEP::keV;
                true_gg_hit.pyionization = sncore_true_gg_hit.get_momentum_start ().y () / CLHEP::keV;
                true_gg_hit.pzionization = sncore_true_gg_hit.get_momentum_start ().z () / CLHEP::keV;
                true_gg_hit.xanode = sncore_true_gg_hit.get_position_stop ().x () / CLHEP::mm;
                true_gg_hit.yanode = sncore_true_gg_hit.get_position_stop ().y () / CLHEP::mm;
                true_gg_hit.zanode = sncore_true_gg_hit.get_position_stop ().z () / CLHEP::mm;
              }
          }

        return 0;
      }

      int event_exporter::_export_calib_calorimeter_hits (const datatools::things & er_,
                                                          sre::export_event & ee_)
      {
        const std::string & cd_label = _bank_labels_[sdm::data_info::CALIBRATED_DATA_LABEL];
        if (! DATATOOLS_THINGS_CHECK_BANK(er_, cd_label, sdm::calibrated_data))
          {
            DT_THROW_IF (true, std::logic_error, "Missing calibrated data to be processed !");
          }
        DATATOOLS_THINGS_CONST_BANK(er_, cd_label, sdm::calibrated_data, CD);

        BOOST_FOREACH (const sdm::calibrated_data::calorimeter_hit_handle_type & scin_handle,
                       CD.calibrated_calorimeter_hits ())
          {
            if (! scin_handle.has_data ()) continue;
            const sdm::calibrated_calorimeter_hit & sncore_scin_hit = scin_handle.get ();
            {
              sre::calib_calorimeter_hit_type dummy;
              ee_.calib_scin_hits.push_back (dummy);
            }
            sre::calib_calorimeter_hit_type & calib_scin_hit = ee_.calib_scin_hits.back ();
            calib_scin_hit.hit_id = sncore_scin_hit.get_hit_id ();
            calib_scin_hit.type = geomtools::geom_id::INVALID_TYPE;
            if (sncore_scin_hit.get_geom_id().get_type () == _gid_infos_.gid_calo_geom_type)
              {
                // CALO:
                calib_scin_hit.type = constants::CALO_TYPE;
                calib_scin_hit.module = sncore_scin_hit.get_geom_id().get(_gid_infos_.gid_calo_module_index);
                calib_scin_hit.side = sncore_scin_hit.get_geom_id().get(_gid_infos_.gid_calo_side_index);
                calib_scin_hit.column = sncore_scin_hit.get_geom_id().get(_gid_infos_.gid_calo_column_index);
                calib_scin_hit.row = sncore_scin_hit.get_geom_id().get(_gid_infos_.gid_calo_row_index);
                calib_scin_hit.wall = geomtools::geom_id::INVALID_ADDRESS;
              }
            else if (sncore_scin_hit.get_geom_id().get_type () == _gid_infos_.gid_xcalo_geom_type)
              {
                // XCALO:
                calib_scin_hit.type = constants::XCALO_TYPE;
                calib_scin_hit.module = sncore_scin_hit.get_geom_id().get(_gid_infos_.gid_xcalo_module_index);
                calib_scin_hit.side = sncore_scin_hit.get_geom_id().get(_gid_infos_.gid_xcalo_side_index);
                calib_scin_hit.column = sncore_scin_hit.get_geom_id().get(_gid_infos_.gid_xcalo_column_index);
                calib_scin_hit.row = sncore_scin_hit.get_geom_id().get(_gid_infos_.gid_xcalo_row_index);
                calib_scin_hit.wall = sncore_scin_hit.get_geom_id().get(_gid_infos_.gid_xcalo_wall_index);
              }
            else if (sncore_scin_hit.get_geom_id().get_type () == _gid_infos_.gid_gveto_geom_type)
              {
                // GVETO
                calib_scin_hit.type = constants::GVETO_TYPE;
                calib_scin_hit.module = sncore_scin_hit.get_geom_id().get(_gid_infos_.gid_gveto_module_index);
                calib_scin_hit.side = sncore_scin_hit.get_geom_id().get(_gid_infos_.gid_gveto_side_index);
                calib_scin_hit.column = sncore_scin_hit.get_geom_id().get(_gid_infos_.gid_gveto_column_index);
                calib_scin_hit.row = geomtools::geom_id::INVALID_ADDRESS;
                calib_scin_hit.wall = sncore_scin_hit.get_geom_id().get(_gid_infos_.gid_gveto_wall_index);
              }
            calib_scin_hit.time = sncore_scin_hit.get_time()/ CLHEP::ns;
            calib_scin_hit.sigma_time = sncore_scin_hit.get_sigma_time()/ CLHEP::ns;
            calib_scin_hit.energy = sncore_scin_hit.get_energy()/ CLHEP::keV;
            calib_scin_hit.sigma_energy = sncore_scin_hit.get_sigma_energy()/ CLHEP::keV;
            calib_scin_hit.true_hit_id = constants::INVALID_ID;
          }

        return 0;
      }

      int event_exporter::_export_calib_tracker_hits (const datatools::things & er_,
                                                      sre::export_event & ee_)
      {
        const std::string & cd_label = _bank_labels_[sdm::data_info::CALIBRATED_DATA_LABEL];
        if (! DATATOOLS_THINGS_CHECK_BANK(er_, cd_label, sdm::calibrated_data))
          {
            DT_THROW_IF (true, std::logic_error, "Missing calibrated data to be processed !");
          }
        DATATOOLS_THINGS_CONST_BANK(er_, cd_label, sdm::calibrated_data, CD);

        BOOST_FOREACH (const sdm::calibrated_data::tracker_hit_handle_type & gg_handle,
                       CD.calibrated_tracker_hits ())
          {
            if (! gg_handle.has_data ()) continue;
            const sdm::calibrated_tracker_hit & sncore_gg_hit = gg_handle.get ();
            {
              sre::calib_tracker_hit_type dummy;
              ee_.calib_gg_hits.push_back (dummy);
            }
            sre::calib_tracker_hit_type & calib_gg_hit = ee_.calib_gg_hits.back ();
            calib_gg_hit.hit_id = sncore_gg_hit.get_hit_id ();
            calib_gg_hit.module = sncore_gg_hit.get_geom_id().get(_gid_infos_.gid_gg_module_index);
            calib_gg_hit.side = sncore_gg_hit.get_geom_id().get(_gid_infos_.gid_gg_side_index);
            calib_gg_hit.layer = sncore_gg_hit.get_geom_id().get(_gid_infos_.gid_gg_layer_index);
            calib_gg_hit.row = sncore_gg_hit.get_geom_id().get(_gid_infos_.gid_gg_row_index);
            calib_gg_hit.noisy = sncore_gg_hit.is_noisy ();
            calib_gg_hit.delayed = sncore_gg_hit.is_delayed ();
            calib_gg_hit.missing_bottom_cathode = sncore_gg_hit.is_bottom_cathode_missing ();
            calib_gg_hit.missing_top_cathode = sncore_gg_hit.is_top_cathode_missing ();
            if (calib_gg_hit.delayed && sncore_gg_hit.has_delayed_time ())
              {
                calib_gg_hit.delayed_time = sncore_gg_hit.get_delayed_time () / CLHEP::ns;
                calib_gg_hit.delayed_time_error = sncore_gg_hit.get_delayed_time_error () / CLHEP::ns;
              }
            calib_gg_hit.x       = sncore_gg_hit.get_x() / CLHEP::mm;
            calib_gg_hit.y       = sncore_gg_hit.get_y() / CLHEP::mm;
            calib_gg_hit.z       = sncore_gg_hit.get_z() / CLHEP::mm;
            calib_gg_hit.sigma_z = sncore_gg_hit.get_sigma_z() / CLHEP::mm;
            calib_gg_hit.r       = sncore_gg_hit.get_r() / CLHEP::mm;
            calib_gg_hit.sigma_r = sncore_gg_hit.get_sigma_r() / CLHEP::mm;
            int tracker_true_hit_id = constants::INVALID_ID;
            {
              if (sncore_gg_hit.get_auxiliaries().has_key(mctools::hit_utils::HIT_MC_HIT_ID_KEY))
                {
                  // Extract the hit ID of the associated MC true Geiger hit :
                  tracker_true_hit_id = sncore_gg_hit.get_auxiliaries().fetch_integer (mctools::hit_utils::HIT_MC_HIT_ID_KEY);
                }
            }
            calib_gg_hit.true_hit_id = tracker_true_hit_id;

            calib_gg_hit.has_cat_infos = false;
            if (are_cat_infos_exported())
              {
                calib_gg_hit.has_cat_infos = true;
                if (sncore_gg_hit.get_auxiliaries ().has_flag("CAT_tangency_x"))
                  {
                    calib_gg_hit.cat_tangency_x = sncore_gg_hit.get_auxiliaries ().fetch_real("CAT_tangency_x");
                    calib_gg_hit.cat_tangency_y = sncore_gg_hit.get_auxiliaries ().fetch_real("CAT_tangency_y");
                    calib_gg_hit.cat_tangency_z = sncore_gg_hit.get_auxiliaries ().fetch_real("CAT_tangency_z");
                    calib_gg_hit.cat_tangency_x_error = sncore_gg_hit.get_auxiliaries ().fetch_real("CAT_tangency_x_error");
                    calib_gg_hit.cat_tangency_y_error = sncore_gg_hit.get_auxiliaries ().fetch_real("CAT_tangency_y_error");
                    calib_gg_hit.cat_tangency_z_error = sncore_gg_hit.get_auxiliaries ().fetch_real("CAT_tangency_z_error");
                  }

                if (sncore_gg_hit.get_auxiliaries ().has_flag("CAT_helix_x"))
                  {
                    calib_gg_hit.cat_helix_x = sncore_gg_hit.get_auxiliaries ().fetch_real("CAT_helix_x");
                    calib_gg_hit.cat_helix_y = sncore_gg_hit.get_auxiliaries ().fetch_real("CAT_helix_y");
                    calib_gg_hit.cat_helix_z = sncore_gg_hit.get_auxiliaries ().fetch_real("CAT_helix_z");
                    calib_gg_hit.cat_helix_x_error = sncore_gg_hit.get_auxiliaries ().fetch_real("CAT_helix_x_error");
                    calib_gg_hit.cat_helix_y_error = sncore_gg_hit.get_auxiliaries ().fetch_real("CAT_helix_y_error");
                    calib_gg_hit.cat_helix_z_error = sncore_gg_hit.get_auxiliaries ().fetch_real("CAT_helix_z_error");
                  }
              }
          }

        return 0;
      }

      int event_exporter::_export_tracker_clustering (const datatools::things & er_,
                                                      sre::export_event & ee_)
      {
        const datatools::logger::priority local_priority = datatools::logger::PRIO_WARNING;
        DT_LOG_TRACE (local_priority, "Entering...");
        const std::string & tcd_label = _bank_labels_[sdm::data_info::TRACKER_CLUSTERING_DATA_LABEL];
        if (! DATATOOLS_THINGS_CHECK_BANK(er_, tcd_label, sdm::tracker_clustering_data))
          {
            DT_THROW_IF (true, std::logic_error, "Missing tracker clustering data to be processed !");
          }
        DATATOOLS_THINGS_CONST_BANK(er_, tcd_label, sdm::tracker_clustering_data, TCD);
        ee_.tracker_clusters.clear();
        ee_.tracker_clustered_hits.clear();
        if (! TCD.has_default_solution ())
          {
            DT_LOG_TRACE (local_priority, "No default clustering solution.");
            return 0;
          }
        DT_LOG_TRACE (local_priority, "Loop on clusters...");
        const sdm::tracker_clustering_solution & TCS = TCD.get_default_solution ();
        int32_t TCS_id = TCS.get_solution_id ();
        DT_LOG_TRACE (local_priority, "TCS_id=" << TCS_id);
        ee_.tracker_clusters.reserve (TCS.get_clusters ().size ());
        DT_LOG_TRACE (local_priority, "# of clusters = " << TCS.get_clusters ().size ());
        BOOST_FOREACH (const sdm::tracker_clustering_solution::cluster_handle_type & cluster_handle,
                       TCS.get_clusters ())
          {
            if (! cluster_handle.has_data ()) continue;
            const sdm::tracker_cluster & sncore_cluster = cluster_handle.get ();
            DT_LOG_TRACE (local_priority, " --> Cluster : ");
            if (local_priority >= datatools::logger::PRIO_TRACE)
              {
                sncore_cluster.tree_dump(std::cerr);
              }

            if (! cluster_handle.has_data ()) continue;
            {
              sre::tracker_cluster_type dummy;
              ee_.tracker_clusters.push_back (dummy);
            }
            sre::tracker_cluster_type & TC = ee_.tracker_clusters.back ();
            TC.solution_id  = TCS_id;
            TC.cluster_id = sncore_cluster.get_cluster_id ();
            TC.module     = constants::INVALID_ID;
            TC.side       = constants::INVALID_ID;
            if (sncore_cluster.has_geom_id ())
              {
                TC.module     = sncore_cluster.get_geom_id ().get(_gid_infos_.gid_gg_module_index);
                TC.side       = sncore_cluster.get_geom_id ().get(_gid_infos_.gid_gg_side_index);
              }
            TC.delayed    = sncore_cluster.is_prompt ();
            TC.number_of_hits = sncore_cluster.get_hits ().size ();
            TC.has_cat_infos = false;
            if (are_cat_infos_exported())
              {
                _export_tracker_clustering_cat(sncore_cluster, TC);
              }
            DT_LOG_TRACE (local_priority, "# of hits in the current cluster = " << TC.number_of_hits);
            for (size_t hit = 0; hit < TC.number_of_hits; hit++)
              {
                DT_LOG_TRACE (local_priority, "  Hit #" << hit);
                {
                  sre::tracker_clustered_hit_type dummy;
                  ee_.tracker_clustered_hits.push_back (dummy);
                }
                sre::tracker_clustered_hit_type & chit = ee_.tracker_clustered_hits.back ();
                chit.hit_id = sncore_cluster.get_hits ().at (hit).get ().get_hit_id ();
                chit.cluster_id = TC.cluster_id;
              }
          }

        DT_LOG_TRACE (local_priority, "Loop on unclustered hits...");
        BOOST_FOREACH (const sdm::calibrated_data::tracker_hit_handle_type & uchit_handle,
                       TCS.get_unclustered_hits ())
          {
            if (! uchit_handle.has_data ()) continue;
            {
              sre::tracker_clustered_hit_type dummy;
              ee_.tracker_clustered_hits.push_back (dummy);
            }
            sre::tracker_clustered_hit_type & uchit = ee_.tracker_clustered_hits.back ();
            uchit.hit_id = uchit_handle.get ().get_hit_id ();
            uchit.cluster_id = -1;
          }

        DT_LOG_TRACE (local_priority, "Exiting.");
        return 0;
      }


        void event_exporter::_export_tracker_clustering_cat (const sdm::tracker_cluster & cluster_,
                                                             sre::tracker_cluster_type & tc_)
        {
          tc_.has_cat_infos = true;
          tc_.cat_has_momentum = false;
          if (cluster_.get_auxiliaries ().has_flag("CAT_has_momentum"))
            {
              tc_.cat_has_momentum = true;
              tc_.cat_momentum_x = cluster_.get_auxiliaries ().fetch_real("CAT_momentum_x");
              tc_.cat_momentum_y = cluster_.get_auxiliaries ().fetch_real("CAT_momentum_y");
              tc_.cat_momentum_z = cluster_.get_auxiliaries ().fetch_real("CAT_momentum_z");
            }

          tc_.cat_has_charge = false;
          if (cluster_.get_auxiliaries ().has_flag("CAT_charge"))
            {
              tc_.cat_has_charge = true;
              tc_.cat_charge = cluster_.get_auxiliaries ().fetch_real("CAT_charge");
            }

          tc_.cat_has_helix_vertex = false;
          if (cluster_.get_auxiliaries ().has_flag("CAT_has_helix_vertex"))
            {
              tc_.cat_has_helix_vertex = true;
              tc_.cat_helix_vertex_x = cluster_.get_auxiliaries ().fetch_real("CAT_helix_vertex_x");
              tc_.cat_helix_vertex_y = cluster_.get_auxiliaries ().fetch_real("CAT_helix_vertex_y");
              tc_.cat_helix_vertex_z = cluster_.get_auxiliaries ().fetch_real("CAT_helix_vertex_z");
              tc_.cat_helix_vertex_x_error = cluster_.get_auxiliaries ().fetch_real("CAT_helix_vertex_x_error");
              tc_.cat_helix_vertex_y_error = cluster_.get_auxiliaries ().fetch_real("CAT_helix_vertex_y_error");
              tc_.cat_helix_vertex_z_error = cluster_.get_auxiliaries ().fetch_real("CAT_helix_vertex_z_error");
            }

          tc_.cat_has_helix_decay_vertex = false;
          if (cluster_.get_auxiliaries ().has_flag("CAT_has_helix_decay_vertex"))
            {
              tc_.cat_has_helix_decay_vertex = true;
              tc_.cat_helix_decay_vertex_x = cluster_.get_auxiliaries ().fetch_real("CAT_helix_decay_vertex_x");
              tc_.cat_helix_decay_vertex_y = cluster_.get_auxiliaries ().fetch_real("CAT_helix_decay_vertex_y");
              tc_.cat_helix_decay_vertex_z = cluster_.get_auxiliaries ().fetch_real("CAT_helix_decay_vertex_z");
              tc_.cat_helix_decay_vertex_x_error = cluster_.get_auxiliaries ().fetch_real("CAT_helix_decay_vertex_x_error");
              tc_.cat_helix_decay_vertex_y_error = cluster_.get_auxiliaries ().fetch_real("CAT_helix_decay_vertex_y_error");
              tc_.cat_helix_decay_vertex_z_error = cluster_.get_auxiliaries ().fetch_real("CAT_helix_decay_vertex_z_error");
            }

          tc_.cat_has_tangent_vertex = false;
          if (cluster_.get_auxiliaries ().has_flag("CAT_has_tangent_vertex"))
            {
              tc_.cat_has_tangent_vertex = true;
              tc_.cat_tangent_vertex_x = cluster_.get_auxiliaries ().fetch_real("CAT_tangent_vertex_x");
              tc_.cat_tangent_vertex_y = cluster_.get_auxiliaries ().fetch_real("CAT_tangent_vertex_y");
              tc_.cat_tangent_vertex_z = cluster_.get_auxiliaries ().fetch_real("CAT_tangent_vertex_z");
              tc_.cat_tangent_vertex_x_error = cluster_.get_auxiliaries ().fetch_real("CAT_tangent_vertex_x_error");
              tc_.cat_tangent_vertex_y_error = cluster_.get_auxiliaries ().fetch_real("CAT_tangent_vertex_y_error");
              tc_.cat_tangent_vertex_z_error = cluster_.get_auxiliaries ().fetch_real("CAT_tangent_vertex_z_error");
            }

          tc_.cat_has_tangent_decay_vertex = false;
          if (cluster_.get_auxiliaries ().has_flag("CAT_has_tangent_decay_vertex"))
            {
              tc_.cat_has_tangent_decay_vertex = true;
              tc_.cat_tangent_decay_vertex_x = cluster_.get_auxiliaries ().fetch_real("CAT_tangent_decay_vertex_x");
              tc_.cat_tangent_decay_vertex_y = cluster_.get_auxiliaries ().fetch_real("CAT_tangent_decay_vertex_y");
              tc_.cat_tangent_decay_vertex_z = cluster_.get_auxiliaries ().fetch_real("CAT_tangent_decay_vertex_z");
              tc_.cat_tangent_decay_vertex_x_error = cluster_.get_auxiliaries ().fetch_real("CAT_tangent_decay_vertex_x_error");
              tc_.cat_tangent_decay_vertex_y_error = cluster_.get_auxiliaries ().fetch_real("CAT_tangent_decay_vertex_y_error");
              tc_.cat_tangent_decay_vertex_z_error = cluster_.get_auxiliaries ().fetch_real("CAT_tangent_decay_vertex_z_error");
            }

          return;
        }

      int event_exporter::_export_tracker_trajectories (const datatools::things & er_,
                                                        sre::export_event & ee_)
      {
        const std::string & ttd_label = _bank_labels_[sdm::data_info::TRACKER_TRAJECTORY_DATA_LABEL];
        if (! DATATOOLS_THINGS_CHECK_BANK(er_, ttd_label, sdm::tracker_trajectory_data))
          {
            DT_THROW_IF (true, std::logic_error, "Missing tracker trajectory data to be processed !");
          }
        DATATOOLS_THINGS_CONST_BANK(er_, ttd_label, sdm::tracker_trajectory_data, TTD);
        if (! TTD.has_default_solution ())
          {
            return 0;
          }
        const sdm::tracker_trajectory_solution & TTS = TTD.get_default_solution ();
        int32_t TTS_id = TTS.get_solution_id ();
        ee_.tracker_trajectories.reserve (TTS.get_trajectories ().size ());

        BOOST_FOREACH (const sdm::tracker_trajectory_solution::trajectory_handle_type & trajectory_handle,
                       TTS.get_trajectories ())
          {
            if (! trajectory_handle.has_data ()) continue;
            const sdm::tracker_trajectory & sncore_trajectory = trajectory_handle.get ();
            {
              sre::tracker_trajectory_type dummy;
              ee_.tracker_trajectories.push_back (dummy);
            }
            sre::tracker_trajectory_type & TT = ee_.tracker_trajectories.back ();
            TT.solution_id   = TTS_id;
            TT.trajectory_id = sncore_trajectory.get_trajectory_id ();
            if (sncore_trajectory.has_geom_id())
              {
                TT.module        = sncore_trajectory.get_geom_id ().get(_gid_infos_.gid_gg_module_index);
                TT.side          = sncore_trajectory.get_geom_id ().get(_gid_infos_.gid_gg_side_index);
              }
            TT.cluster_id    = sncore_trajectory.get_cluster ().get_cluster_id ();
            TT.delayed       = sncore_trajectory.get_cluster ().is_delayed ();
            TT.number_of_orphans = sncore_trajectory.get_orphans ().size ();
            BOOST_FOREACH (const sdm::calibrated_data::tracker_hit_handle_type & ohit_handle,
                           sncore_trajectory.get_orphans ())
              {
                if (! ohit_handle.has_data ()) continue;
                {
                  sre::tracker_trajectory_orphan_hit_type dummy;
                  ee_.tracker_trajectory_orphan_hits.push_back (dummy);
                }
                sre::tracker_trajectory_orphan_hit_type & ohit
                  = ee_.tracker_trajectory_orphan_hits.back ();
                ohit.hit_id = ohit_handle.get ().get_hit_id ();
                ohit.trajectory_id = TT.trajectory_id;
              }
          }

        return 0;
      }

    } // end of namespace exports

  } // end of namespace reconstruction

} // end of namespace snemo

// end of event_exporter.cc
