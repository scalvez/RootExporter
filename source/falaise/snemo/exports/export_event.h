// -*- mode: c++ ; -*-
/* export_event.h
 *
 * Author (s) :     Francois Mauger <mauger@lpccaen.in2p3.fr>
 * Creation date: 2012-11-09
 * Last modified: 2012-12-10
 *
 * License:
 *
 * Description:
 *
 *   Module for exporting the SuperNEMO event model(s)
 *
 * History:
 *
 */

#ifndef SNRECONSTRUCTION_EXPORTS_EXPORT_EVENT_H
#define SNRECONSTRUCTION_EXPORTS_EXPORT_EVENT_H 1

#include <string>
#include <vector>
#include <iostream>

#include <boost/cstdint.hpp>
#include <camp/camptype.hpp>
#include <camp/class.hpp>

namespace snemo {

  namespace reconstruction {

    namespace exports {

      template<class Type>
      void bank_export_version (int32_t & bank_version_)
      {
        bank_version_ = Type::EXPORT_VERSION;
        return;
      }

      template<class Type>
      bool bank_check_export_version (int32_t bank_version_)
      {
        return bank_version_ >= 0 && bank_version_ <= Type::EXPORT_VERSION;
      }

      template<class Type>
      const std::string & describe_class ();

      struct constants
      {
        static const int32_t INVALID_ID;
        static const int16_t INVALID_INTEGER16;
        static const int32_t INVALID_INTEGER;
        static const int64_t INVALID_INTEGER64;
        static const double  INVALID_DOUBLE;
        enum calo_block_type
          {
            CALO_TYPE  = 0, // GID has: module/side/column/row
            XCALO_TYPE = 1, // GID has: module/side/wall/column/row
            GVETO_TYPE = 2  // GID has: module/side/wall/column
          };
      };

      struct event_header_type
      {
      public:
        static const int32_t EXPORT_VERSION = 0;
        event_header_type ();
        void reset ();
      public:
        int32_t run_number;   // >=0
        int32_t event_number; // >=0
        bool    simulated;    // simulated event=true, real event=false
        int64_t seconds;      // >=0 [s]
        int64_t picoseconds;  // >=0 [ps]

        // Auxiliary properties :
        bool export_cat_infos;

      };

      struct true_vertex_type
      {
      public:
        static const int32_t EXPORT_VERSION = 0;
        true_vertex_type ();
        void reset ();
      public:
        int32_t vertex_id;
        double  x; // mm
        double  y; // mm
        double  z; // mm
        double  time; // ns
      };

      struct true_particle_type
      {
      public:
        static const int32_t EXPORT_VERSION = 0;
        true_particle_type ();
        void reset ();
      public:
        int32_t track_id;
        int32_t particle_type;
        double  px; // mm
        double  py; // mm
        double  pz; // mm
        double  time; // ns
        int32_t vertex_id;
      };

      struct true_step_hit_type
      {
      public:
        static const int32_t EXPORT_VERSION = 0;
        true_step_hit_type ();
        void reset ();
       public:
        int32_t hit_id;  // >=0
        double  tstart;  // ns
        double  xstart;  // mm
        double  ystart;  // mm
        double  zstart;  // mm
        double  pxstart; // keV
        double  pystart; // keV
        double  pzstart; // keV
        double  tstop;   // ns
        double  xstop;   // mm
        double  ystop;   // mm
        double  zstop;   // mm
        double  pxstop;  // keV
        double  pystop;  // keV
        double  pzstop;  // keV
        double  delta_energy;  // keV
      };

      struct true_gg_hit_type
      {
      public:
        static const int32_t EXPORT_VERSION = 0;
        true_gg_hit_type ();
        void reset ();
      public:
        int32_t hit_id;  // >=0
        int32_t module;  // >=0
        int32_t side;    // 0 for x<0, 1 for x>0
        int32_t layer;   // [0..8]
        int32_t row;     // [0..112]
        double  tionization;  // ns
        double  xionization;  // mm
        double  yionization;  // mm
        double  zionization;  // mm
        double  pxionization; // keV
        double  pyionization; // keV
        double  pzionization; // keV
        double  xanode;       // mm
        double  yanode;       // mm
        double  zanode;       // mm
      };

      struct true_scin_hit_type
      {
      public:
        static const int32_t EXPORT_VERSION = 0;
        true_scin_hit_type ();
        void reset ();
      public:
        int32_t hit_id;  // >=0
        int32_t type;    // >=0
        int32_t module;  // >=0
        int32_t side;    // 0 for x<0, 1 for x>0
        int32_t column;  // [0..8]
        int32_t row;     // [0..112]
        int32_t wall;    // >=0
        double  tfirst;  // ns
        double  tlast;   // ns
        double  x1;  // mm
        double  y1;  // mm
        double  z1;  // mm
        double  x2;  // mm
        double  y2;  // mm
        double  z2;  // mm
        double  delta_energy; // keV
      };

      struct calib_tracker_hit_type
      {
      public:
        static const int32_t EXPORT_VERSION = 0;
        calib_tracker_hit_type ();
        void reset ();
        void reset_cat ();
      public:
        int32_t hit_id;  // >=0
        int32_t true_hit_id;  // >=0
        int32_t module;  // >=0
        int32_t side;    // 0 for x<0, 1 for x>0
        int32_t layer;   // [0..8]
        int32_t row;     // [0..112]
        bool    noisy;
        bool    missing_bottom_cathode;
        bool    missing_top_cathode;
        bool    delayed;
        double  delayed_time; // ns
        double  delayed_time_error; // ns
        double  x;       // mm
        double  y;       // mm
        double  z;       // mm
        double  sigma_z; // mm
        double  r;       // mm
        double  sigma_r; // mm

        // CAT specific auxilairies :
        bool     has_cat_infos;
        double   cat_tangency_x;
        double   cat_tangency_y;
        double   cat_tangency_z;
        double   cat_tangency_x_error;
        double   cat_tangency_y_error;
        double   cat_tangency_z_error;

        double   cat_helix_x;
        double   cat_helix_y;
        double   cat_helix_z;
        double   cat_helix_x_error;
        double   cat_helix_y_error;
        double   cat_helix_z_error;
       };

      struct calib_calorimeter_hit_type
      {
      public:
        static const int32_t EXPORT_VERSION = 0;
        calib_calorimeter_hit_type ();
        void reset ();
      public:
        int32_t hit_id; // >=0
        int32_t true_hit_id;  // >=0
        int32_t type;   // >=0
        int32_t module; // >=0
        int32_t side;   // 0 for x<0, 1 for x>0
        int32_t column; // >=0
        int32_t row;    // >=0
        int32_t wall;   // >=0
        double  time;         // ns
        double  sigma_time;   // ns
        double  energy;       // keV
        double  sigma_energy; // keV
      };

      struct tracker_cluster_type
      {
      public:
         tracker_cluster_type ();
        static const int32_t EXPORT_VERSION = 0;
        void reset ();
        void reset_cat ();
      public:
        int32_t  solution_id; // >=0
        int32_t  cluster_id ; // >=0
        int32_t  module;      // >=0
        int32_t  side;        // >=0
        bool     delayed;
        uint32_t number_of_hits; // >=0

         // CAT specific properties :
        bool     has_cat_infos;

        bool     cat_has_charge;
        double   cat_charge;

        bool     cat_has_momentum;
        double   cat_momentum_x;
        double   cat_momentum_y;
        double   cat_momentum_z;

        bool     cat_has_helix_vertex;
        double   cat_helix_vertex_x;
        double   cat_helix_vertex_y;
        double   cat_helix_vertex_z;
        double   cat_helix_vertex_x_error;
        double   cat_helix_vertex_y_error;
        double   cat_helix_vertex_z_error;

        bool     cat_has_helix_decay_vertex;
        double   cat_helix_decay_vertex_x;
        double   cat_helix_decay_vertex_y;
        double   cat_helix_decay_vertex_z;
        double   cat_helix_decay_vertex_x_error;
        double   cat_helix_decay_vertex_y_error;
        double   cat_helix_decay_vertex_z_error;

        bool     cat_has_tangent_vertex;
        double   cat_tangent_vertex_x;
        double   cat_tangent_vertex_y;
        double   cat_tangent_vertex_z;
        double   cat_tangent_vertex_x_error;
        double   cat_tangent_vertex_y_error;
        double   cat_tangent_vertex_z_error;

        bool     cat_has_tangent_decay_vertex;
        double   cat_tangent_decay_vertex_x;
        double   cat_tangent_decay_vertex_y;
        double   cat_tangent_decay_vertex_z;
        double   cat_tangent_decay_vertex_x_error;
        double   cat_tangent_decay_vertex_y_error;
        double   cat_tangent_decay_vertex_z_error;
     };

      struct tracker_clustered_hit_type
      {
      public:
        static const int32_t EXPORT_VERSION = 0;
        tracker_clustered_hit_type ();
        void reset ();
      public:
        int32_t solution_id; // >=0
        int32_t cluster_id;  // >=0
        int32_t hit_id;      // >=0
      };

      enum trajectory_1d_pattern_type
        {
          PATTERN_NONE     = -1,
          PATTERN_LINE     = 0,
          PATTERN_POLYLINE = 1,
          PATTERN_HELIX    = 2,
        };

      struct tracker_trajectory_type
      {
      public:
        tracker_trajectory_type ();
        static const int32_t EXPORT_VERSION = 0;
        void reset ();
      public:
        int32_t  solution_id;       // >=0
        int32_t  trajectory_id;     // >=0
        int32_t  module;            // >=0
        int32_t  side;              // >=0
        int32_t  cluster_id;        // >=0
        bool     delayed;           //
        uint32_t number_of_orphans; // >=0
        int32_t  pattern_id;        // >=0
      };

      struct tracker_trajectory_orphan_hit_type
      {
      public:
        static const int32_t EXPORT_VERSION = 0;
        tracker_trajectory_orphan_hit_type ();
        void reset ();
      public:
        int32_t solution_id;   // >=0
        int32_t trajectory_id; // >=0
        int32_t hit_id;        // >=0
      };

      struct vertex_type
      {
      public:
        static const int32_t EXPORT_VERSION = 0;
        vertex_type ();
        void reset ();
      public:
        int32_t vertex_id;   // >=0
        int32_t parent_type; // >=0
        int32_t parent_id;   // >=0
        double  x;
        double  y;
        double  z;
        double  x_error;
        double  y_error;
        double  z_error;
      };

      struct polyline_type
      {
      public:
        static const int32_t EXPORT_VERSION = 0;
        polyline_type ();
        void reset ();
      public:
        int32_t polyline_id;         // >=0
        uint32_t number_of_vertexes; // >=0
      };

      struct helix_type
      {
      public:
        static const int32_t EXPORT_VERSION = 0;
        helix_type ();
        void reset ();
      public:
        int32_t helix_id; // >=0
        double  x0;
        double  y0;
        double  z0;
        double  r;
        double  step;
        double  t0;
        double  t1;
      };

      struct tracker_trajectory_pattern_type
      {
      public:
        static const int32_t EXPORT_VERSION = 0;
        tracker_trajectory_pattern_type ();
        void reset ();
      public:
        int32_t  pattern_id;    // >=0
        int32_t  solution_id;   // >=0
        int32_t  trajectory_id; // >=0
        int32_t  pattern_type;  // >=0
        double   length;
        int32_t  vertex0_id;
        int32_t  vertex1_id;
        int32_t  line_id;
        int32_t  helix_id;
        int32_t  polyline_id;
      };

      template <class Type>
      void audit_vector (const std::vector< Type > & v_, int i_)
      {
        // std::clog << "**** AUDIT VECTOR " << typeid(Type).name () << " : \n";
        // std::clog << "     Size     : " << v_.size () << "\n";
        // std::clog << "     Capacity : " << v_.capacity () << "\n";
        // std::clog << "     Index    : " << i_ << "\n";
        return;
      }

      struct export_event
      {
      public:
        static const int32_t EXPORT_VERSION = 0;
        export_event ();
        virtual ~export_event ();
        void reset ();
        void clear_data ();

        const true_vertex_type & get_true_vertex (int i_) const;

        const true_particle_type & get_true_particle (int i_) const;

        const true_step_hit_type & get_true_step_hit (int i_) const;

        const true_scin_hit_type & get_true_calo_hit (int i_) const;

        const true_scin_hit_type & get_true_xcalo_hit (int i_) const;

        const true_scin_hit_type & get_true_gveto_hit (int i_) const;

        const true_gg_hit_type & get_true_gg_hit (int i_) const;

        const calib_calorimeter_hit_type & get_calib_scin_hit (int i_) const;

        const calib_tracker_hit_type & get_calib_gg_hit (int i_) const;

        const tracker_clustered_hit_type & get_tracker_clustered_hit (int i_) const;

        const tracker_cluster_type & get_tracker_cluster (int i_) const;

        const tracker_trajectory_type & get_tracker_trajectory (int i_) const;

        const tracker_trajectory_orphan_hit_type & get_tracker_trajectory_orphan_hit (int i_) const;

        const tracker_trajectory_pattern_type & get_tracker_trajectory_pattern (int i_) const;

        const vertex_type & get_tracker_trajectory_vertex (int i_) const;

        const polyline_type & get_tracker_trajectory_polyline (int i_) const;

        const helix_type & get_tracker_trajectory_helix (int i_) const;

        static void implement_introspection ();

        struct introspection_activator
        {
          introspection_activator ();
        };

        void print (std::ostream & out_ = std::clog,
                    const std::string & title_ = "",
                    const std::string & indent_ = "") const;

      public:

        // Event header data :
        event_header_type                       event_header;    /// Event header

        // True (MC) data :
        std::vector<true_vertex_type>           true_vertices;   /// True particles
        std::vector<true_particle_type>         true_particles;  /// True particles
        std::vector<true_step_hit_type>         true_step_hits;  /// True particles
        std::vector<true_scin_hit_type>         true_calo_hits;  /// True particles
        std::vector<true_scin_hit_type>         true_xcalo_hits; /// True particles
        std::vector<true_scin_hit_type>         true_gveto_hits; /// True particles
        std::vector<true_gg_hit_type>           true_gg_hits;    /// True particles

        // Calibrated data :
        std::vector<calib_calorimeter_hit_type> calib_scin_hits; /// Calibrated scintillator hits
        std::vector<calib_tracker_hit_type>     calib_gg_hits;   /// Calibrated tracker hits

        // Tracker clustering data:
        std::vector<tracker_cluster_type>       tracker_clusters;       /// Tracker clusters
        std::vector<tracker_clustered_hit_type> tracker_clustered_hits; /// Tracker clustered hits

        // Tracker trajectories data:
        std::vector<tracker_trajectory_type>    tracker_trajectories;   /// Tracker trajectories
        std::vector<vertex_type>                tracker_trajectory_vertices;             /// Vertices
        std::vector<polyline_type>              tracker_trajectory_polylines;            /// Polylines
        std::vector<helix_type>                 tracker_trajectory_helices;              /// Helices
        std::vector<tracker_trajectory_orphan_hit_type>  tracker_trajectory_orphan_hits; /// Tracker trajectory orphan hit
        std::vector<tracker_trajectory_pattern_type>  tracker_trajectory_patterns;       /// Tracker trajectory patterns

      };

      template<class Type>
      const std::string & get_class_description ()
      {
        static boost::scoped_ptr<std::string> g_description (0);
        if (g_description.get () == 0)
          {
            std::ostringstream oss;

            const camp::Class & meta_class = camp::classByType<Type> ();
            for (int i = 0; i < meta_class.propertyCount(); i++)
              {
                const camp::Property & prop = meta_class.property (i);
                if (i > 0) oss << ';';
                oss << prop.name ();
                if (prop.hasTag ("ctype"))
                  {
                    oss << '/' << prop.tag ("ctype");
                  }
                if (prop.hasTag ("unit"))
                  {
                    oss << '[' << prop.tag ("unit") << ']';
                  }
              }

            g_description.reset (new std::string (oss.str()));
          }
        return *g_description.get();
      }

    } // end of namespace exports

  } // end of namespace reconstruction

} // end of namespace snemo

CAMP_TYPE (snemo::reconstruction::exports::event_header_type);
CAMP_TYPE (snemo::reconstruction::exports::true_vertex_type);
CAMP_TYPE (snemo::reconstruction::exports::true_particle_type);
CAMP_TYPE (snemo::reconstruction::exports::true_step_hit_type);
CAMP_TYPE (snemo::reconstruction::exports::true_gg_hit_type);
CAMP_TYPE (snemo::reconstruction::exports::true_scin_hit_type);
CAMP_TYPE (snemo::reconstruction::exports::calib_tracker_hit_type);
CAMP_TYPE (snemo::reconstruction::exports::calib_calorimeter_hit_type);
CAMP_TYPE (snemo::reconstruction::exports::tracker_cluster_type);
CAMP_TYPE (snemo::reconstruction::exports::tracker_clustered_hit_type);
CAMP_TYPE (snemo::reconstruction::exports::tracker_trajectory_type);
CAMP_TYPE (snemo::reconstruction::exports::tracker_trajectory_orphan_hit_type);
CAMP_TYPE (snemo::reconstruction::exports::vertex_type);
CAMP_TYPE (snemo::reconstruction::exports::polyline_type);
CAMP_TYPE (snemo::reconstruction::exports::helix_type);
CAMP_TYPE (snemo::reconstruction::exports::tracker_trajectory_pattern_type);
CAMP_TYPE (snemo::reconstruction::exports::export_event);

#endif // SNRECONSTRUCTION_EXPORTS_EXPORT_EVENT_H

// end of export_event.h
