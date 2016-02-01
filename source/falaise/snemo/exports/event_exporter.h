// -*- mode: c++ ; -*-
/* event_exporter.h
 *
 * Author (s) :     Francois Mauger <mauger@lpccaen.in2p3.fr>
 * Creation date: 2012-11-09
 * Last modified: 2012-11-09
 *
 * License:
 *
 * Description:
 *
 *   Event exporter
 *
 * History:
 *
 */

#ifndef SNRECONSTRUCTION_EXPORTS_EVENT_EXPORTER_H
#define SNRECONSTRUCTION_EXPORTS_EVENT_EXPORTER_H 1

#include <map>
#include <string>

#include <boost/cstdint.hpp>

#include <datatools/bit_mask.h>
#include <falaise/snemo/datamodels/data_model.h>

namespace geomtools {
  class manager;
}
namespace datatools {
  class properties;
}

namespace snemo {

    namespace datamodel {
      class tracker_cluster;
    }

  namespace reconstruction {

    namespace exports {

      class export_event;
      class tracker_cluster_type;

      struct event_exporter
      {
      public:
        enum export_bit_type
          {
            NO_EXPORT = 0,
            // Needs 'Event Header' :
            EXPORT_EVENT_HEADER           = datatools::bit_mask::bit00,
            // Needs 'Simulated Data' :
            EXPORT_TRUE_PARTICLES         = datatools::bit_mask::bit01,
            EXPORT_TRUE_STEP_HITS         = datatools::bit_mask::bit02,
            EXPORT_TRUE_HITS              = datatools::bit_mask::bit03,
            // Needs 'Calibrated Data' :
            EXPORT_CALIB_CALORIMETER_HITS = datatools::bit_mask::bit04,
            EXPORT_CALIB_TRACKER_HITS     = datatools::bit_mask::bit05,
            EXPORT_TRACKER_CLUSTERING     = datatools::bit_mask::bit06,
            EXPORT_TRACKER_TRAJECTORIES   = datatools::bit_mask::bit07,

            EXPORT_ALL =
            EXPORT_EVENT_HEADER
            | EXPORT_TRUE_PARTICLES
            | EXPORT_TRUE_STEP_HITS
            | EXPORT_TRUE_HITS
            | EXPORT_CALIB_CALORIMETER_HITS
            | EXPORT_CALIB_TRACKER_HITS
            | EXPORT_TRACKER_CLUSTERING
            | EXPORT_TRACKER_TRAJECTORIES,

            EXPORT_LAST = EXPORT_TRACKER_TRAJECTORIES,
          };

        enum export_topic_level
          {
            EXPORT_TOPIC_EXCLUDE    = -1,
            EXPORT_TOPIC_NO_INCLUDE = 0,
            EXPORT_TOPIC_INCLUDE    = 1,
          };

        struct gid_info_type
        {
          unsigned int gid_gg_geom_type;
          unsigned int gid_gg_module_index;
          unsigned int gid_gg_side_index;
          unsigned int gid_gg_layer_index;
          unsigned int gid_gg_row_index;

          unsigned int gid_calo_geom_type;
          unsigned int gid_calo_module_index;
          unsigned int gid_calo_side_index;
          unsigned int gid_calo_column_index;
          unsigned int gid_calo_row_index;

          unsigned int gid_xcalo_geom_type;
          unsigned int gid_xcalo_module_index;
          unsigned int gid_xcalo_side_index;
          unsigned int gid_xcalo_column_index;
          unsigned int gid_xcalo_row_index;
          unsigned int gid_xcalo_wall_index;

          unsigned int gid_gveto_geom_type;
          unsigned int gid_gveto_module_index;
          unsigned int gid_gveto_side_index;
          unsigned int gid_gveto_column_index;
          unsigned int gid_gveto_row_index;
          unsigned int gid_gveto_wall_index;
        public:
          gid_info_type ();
          void reset ();
        };

        static std::string get_export_bit_label (unsigned int bit_);

      public:

        bool is_initialized () const;

        bool has_geom_manager () const;

        void set_geom_manager (const geomtools::manager & gmgr_);

        const geomtools::manager & get_geom_manager () const;

        void unset_exported (unsigned int store_bit_);

        void set_exported (unsigned int store_bit_);

        bool is_exported (unsigned int store_bit_) const;

        bool are_cat_infos_exported () const;

        void set_cat_infos_exported (bool);

        int get_topic_export_level(const std::string & topic_label_) const;

        event_exporter ();

        ~event_exporter ();

        void initialize (const datatools::properties & config_);

        void reset ();

        int run (const snemo::datamodel::event_record &,
                 snemo::reconstruction::exports::export_event &);

        uint32_t get_export_flags () const;

        void dump (std::ostream & = std::clog) const;

      protected:

        void _set_default_bank_labels ();

        void _init_defaults ();

        void _init_gid_infos_ ();

        int _export_event_header (const datatools::things &,
                                  snemo::reconstruction::exports::export_event &);

        int _export_true_particles (const datatools::things &,
                                    snemo::reconstruction::exports::export_event &);

        int _export_true_step_hits (const datatools::things &,
                                    snemo::reconstruction::exports::export_event &);

        int _export_true_hits (const datatools::things &,
                               snemo::reconstruction::exports::export_event &);

        int _export_calib_calorimeter_hits (const datatools::things &,
                                            snemo::reconstruction::exports::export_event &);

        int _export_calib_tracker_hits (const datatools::things &,
                                        snemo::reconstruction::exports::export_event &);


        int _export_tracker_clustering (const datatools::things &,
                                        snemo::reconstruction::exports::export_event &);

        void _export_tracker_clustering_cat (const snemo::datamodel::tracker_cluster & cluster_,
                                             snemo::reconstruction::exports::tracker_cluster_type & tc_);


        int _export_tracker_trajectories (const datatools::things &,
                                          snemo::reconstruction::exports::export_event &);

         const std::map<std::string, std::string> & get_bank_labels () const;

      private:

        bool _initialized_; //!< Initialization flag
        const geomtools::manager * _geom_manager_; //!< The geometry manager
        gid_info_type _gid_infos_;

        std::map<std::string, std::string> _bank_labels_; //!< The labels of the bank in the event records
        uint32_t _export_flags_;
        bool     _export_cat_infos_; // Topic = "CAT"

      };

    } // end of namespace exports

  } // end of namespace reconstruction

} // end of namespace snemo

#endif // SNRECONSTRUCTION_EXPORTS_EVENT_EXPORTER_H

// end of event_exporter.h
