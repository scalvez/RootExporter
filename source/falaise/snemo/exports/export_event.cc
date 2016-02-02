// -*- mode: c++ ; -*-
/* export_event.cc */

#include <falaise/snemo/exports/export_event.h>
#include <falaise/snemo/exports/event_exporter.h>

#include <sstream>
#include <limits>

namespace snemo {

  namespace reconstruction {

    namespace exports {

      const int32_t constants::INVALID_ID        = -1;
      const int16_t constants::INVALID_INTEGER16 = -1;
      const int32_t constants::INVALID_INTEGER   = -1;
      const int64_t constants::INVALID_INTEGER64 = -1;
      const double  constants::INVALID_DOUBLE    = std::numeric_limits<double>::quiet_NaN ();

      /***********************************************/

      event_header_type::event_header_type ()
      {
        reset ();
        return;
      }

      void event_header_type::reset ()
      {
        run_number   = constants::INVALID_ID;
        event_number = constants::INVALID_ID;
        simulated    = false;
        seconds      = constants::INVALID_INTEGER64;
        picoseconds  = constants::INVALID_INTEGER64;

        export_cat_infos = false;
        return;
      }

      /***********************************************/

      true_vertex_type::true_vertex_type ()
      {
        reset ();
        return;
      }

      void true_vertex_type::reset ()
      {
        vertex_id = constants::INVALID_ID;
        x = constants::INVALID_DOUBLE;
        y = constants::INVALID_DOUBLE;
        z = constants::INVALID_DOUBLE;
        time = constants::INVALID_DOUBLE;
        return;
      }

      /***********************************************/

      true_particle_type::true_particle_type ()
      {
        reset ();
        return;
      }

      void true_particle_type::reset ()
      {
        track_id = constants::INVALID_ID;
        particle_type = constants::INVALID_ID;
        px = constants::INVALID_DOUBLE;
        py = constants::INVALID_DOUBLE;
        pz = constants::INVALID_DOUBLE;
        time = constants::INVALID_DOUBLE;
        vertex_id = constants::INVALID_ID;
        return;
      }

      /***********************************************/

      calib_calorimeter_hit_type::calib_calorimeter_hit_type ()
      {
        reset ();
        return;
      }

      void calib_calorimeter_hit_type::reset ()
      {
        hit_id = constants::INVALID_ID;
        true_hit_id = constants::INVALID_ID;
        type = constants::INVALID_ID;
        module = constants::INVALID_ID;
        side = constants::INVALID_ID;
        column = constants::INVALID_ID;
        row = constants::INVALID_ID;
        wall = constants::INVALID_ID;
        time = constants::INVALID_DOUBLE;
        sigma_time = constants::INVALID_DOUBLE;
        energy = constants::INVALID_DOUBLE;
        sigma_energy = constants::INVALID_DOUBLE;
        return;
      }

      /***********************************************/

      true_step_hit_type::true_step_hit_type ()
      {
        reset ();
        return;
      }

      void true_step_hit_type::reset ()
      {
        hit_id = constants::INVALID_ID;

        tstart = constants::INVALID_DOUBLE;
        xstart = constants::INVALID_DOUBLE;
        ystart = constants::INVALID_DOUBLE;
        zstart = constants::INVALID_DOUBLE;
        pxstart = constants::INVALID_DOUBLE;
        pystart = constants::INVALID_DOUBLE;
        pzstart = constants::INVALID_DOUBLE;

        tstop = constants::INVALID_DOUBLE;
        xstop = constants::INVALID_DOUBLE;
        ystop = constants::INVALID_DOUBLE;
        zstop = constants::INVALID_DOUBLE;
        pxstop = constants::INVALID_DOUBLE;
        pystop = constants::INVALID_DOUBLE;
        pzstop = constants::INVALID_DOUBLE;

        delta_energy = constants::INVALID_DOUBLE;

        return;
      }

      /***********************************************/

      true_gg_hit_type::true_gg_hit_type ()
      {
        reset ();
        return;
      }

      void true_gg_hit_type::reset ()
      {
        hit_id = constants::INVALID_ID;

        module = constants::INVALID_ID;
        side = constants::INVALID_ID;
        layer = constants::INVALID_ID;
        row = constants::INVALID_ID;

        tionization = constants::INVALID_DOUBLE;
        xionization = constants::INVALID_DOUBLE;
        yionization = constants::INVALID_DOUBLE;
        zionization = constants::INVALID_DOUBLE;
        pxionization = constants::INVALID_DOUBLE;
        pyionization = constants::INVALID_DOUBLE;
        pzionization = constants::INVALID_DOUBLE;
        xanode = constants::INVALID_DOUBLE;
        yanode = constants::INVALID_DOUBLE;
        zanode = constants::INVALID_DOUBLE;

        return;
      }

      /***********************************************/

      true_scin_hit_type::true_scin_hit_type ()
      {
        reset ();
        return;
      }

      void true_scin_hit_type::reset ()
      {
        hit_id = constants::INVALID_ID;
        type = constants::INVALID_ID;
        module = constants::INVALID_ID;
        side = constants::INVALID_ID;
        column = constants::INVALID_ID;
        row = constants::INVALID_ID;
        wall = constants::INVALID_ID;

        tfirst = constants::INVALID_DOUBLE;
        tlast = constants::INVALID_DOUBLE;
        x1 = constants::INVALID_DOUBLE;
        y1 = constants::INVALID_DOUBLE;
        z1 = constants::INVALID_DOUBLE;
        x2 = constants::INVALID_DOUBLE;
        y2 = constants::INVALID_DOUBLE;
        z2 = constants::INVALID_DOUBLE;
        delta_energy = constants::INVALID_DOUBLE;

        return;
      }

      /***********************************************/

      calib_tracker_hit_type::calib_tracker_hit_type ()
      {
        reset ();
        return;
      }

      void calib_tracker_hit_type::reset ()
      {
        hit_id = constants::INVALID_ID;
        true_hit_id = constants::INVALID_ID;
        module = constants::INVALID_ID;
        side = constants::INVALID_ID;
        layer = constants::INVALID_ID;
        row = constants::INVALID_ID;
        noisy = false;
        missing_bottom_cathode = false;
        missing_top_cathode = false;
        delayed = false;
        delayed_time = constants::INVALID_DOUBLE;
        delayed_time_error = constants::INVALID_DOUBLE;
        x = constants::INVALID_DOUBLE;
        y = constants::INVALID_DOUBLE;
        z = constants::INVALID_DOUBLE;
        sigma_z = constants::INVALID_DOUBLE;
        r = constants::INVALID_DOUBLE;
        sigma_r = constants::INVALID_DOUBLE;
        // CAT specific :
        reset_cat ();
        return;
      }

      void calib_tracker_hit_type::reset_cat ()
      {
        has_cat_infos = false;

        cat_tangency_x = constants::INVALID_DOUBLE;
        cat_tangency_y = constants::INVALID_DOUBLE;
        cat_tangency_z = constants::INVALID_DOUBLE;
        cat_tangency_x_error = constants::INVALID_DOUBLE;
        cat_tangency_y_error = constants::INVALID_DOUBLE;
        cat_tangency_z_error = constants::INVALID_DOUBLE;

        cat_helix_x = constants::INVALID_DOUBLE;
        cat_helix_y = constants::INVALID_DOUBLE;
        cat_helix_z = constants::INVALID_DOUBLE;
        cat_helix_x_error = constants::INVALID_DOUBLE;
        cat_helix_y_error = constants::INVALID_DOUBLE;
        cat_helix_z_error = constants::INVALID_DOUBLE;
        return;
      }

      /***********************************************/

      tracker_clustered_hit_type::tracker_clustered_hit_type ()
      {
        reset ();
        return;
      }

      void tracker_clustered_hit_type::reset ()
      {
        solution_id = constants::INVALID_ID;
        cluster_id = constants::INVALID_ID;
        hit_id = constants::INVALID_ID;
        return;
      }

      /***********************************************/

      tracker_cluster_type::tracker_cluster_type ()
      {
        reset ();
        return;
      }

      void tracker_cluster_type::reset ()
      {
        solution_id = constants::INVALID_ID;
        cluster_id = constants::INVALID_ID;
        module = constants::INVALID_ID;
        side  = constants::INVALID_ID;
        delayed = false;
        number_of_hits = 0;
        // CAT specific properties :
        reset_cat();
        return;
      }

      void tracker_cluster_type::reset_cat ()
      {
        // CAT specific properties :
        has_cat_infos = false;

        cat_has_charge = false;
        cat_charge = constants::INVALID_DOUBLE;

        cat_has_momentum = false;
        cat_momentum_x = constants::INVALID_DOUBLE;
        cat_momentum_y = constants::INVALID_DOUBLE;
        cat_momentum_z = constants::INVALID_DOUBLE;

        cat_has_helix_vertex = false;
        cat_helix_vertex_x = constants::INVALID_DOUBLE;
        cat_helix_vertex_y = constants::INVALID_DOUBLE;
        cat_helix_vertex_z = constants::INVALID_DOUBLE;
        cat_helix_vertex_x_error = constants::INVALID_DOUBLE;
        cat_helix_vertex_y_error = constants::INVALID_DOUBLE;
        cat_helix_vertex_z_error = constants::INVALID_DOUBLE;

        cat_has_helix_decay_vertex = false;
        cat_helix_decay_vertex_x = constants::INVALID_DOUBLE;
        cat_helix_decay_vertex_y = constants::INVALID_DOUBLE;
        cat_helix_decay_vertex_z = constants::INVALID_DOUBLE;
        cat_helix_decay_vertex_x_error = constants::INVALID_DOUBLE;
        cat_helix_decay_vertex_y_error = constants::INVALID_DOUBLE;
        cat_helix_decay_vertex_z_error = constants::INVALID_DOUBLE;

        cat_has_tangent_vertex = false;
        cat_tangent_vertex_x = constants::INVALID_DOUBLE;
        cat_tangent_vertex_y = constants::INVALID_DOUBLE;
        cat_tangent_vertex_z = constants::INVALID_DOUBLE;
        cat_tangent_vertex_x_error = constants::INVALID_DOUBLE;
        cat_tangent_vertex_y_error = constants::INVALID_DOUBLE;
        cat_tangent_vertex_z_error = constants::INVALID_DOUBLE;

        cat_has_tangent_decay_vertex = false;
        cat_tangent_decay_vertex_x = constants::INVALID_DOUBLE;
        cat_tangent_decay_vertex_y = constants::INVALID_DOUBLE;
        cat_tangent_decay_vertex_z = constants::INVALID_DOUBLE;
        cat_tangent_decay_vertex_x_error = constants::INVALID_DOUBLE;
        cat_tangent_decay_vertex_y_error = constants::INVALID_DOUBLE;
        cat_tangent_decay_vertex_z_error = constants::INVALID_DOUBLE;

        return;
      }

      /***********************************************/

      vertex_type::vertex_type ()
      {
        reset ();
        return;
      }

      void vertex_type::reset ()
      {
        vertex_id = constants::INVALID_ID;
        parent_type = constants::INVALID_ID;
        parent_id = constants::INVALID_ID;
        x = constants::INVALID_DOUBLE;
        z = constants::INVALID_DOUBLE;
        x = constants::INVALID_DOUBLE;
        x_error = constants::INVALID_DOUBLE;
        y_error = constants::INVALID_DOUBLE;
        z_error = constants::INVALID_DOUBLE;
        return;
      }

      /***********************************************/

      polyline_type::polyline_type ()
      {
        reset ();
        return;
      }

      void polyline_type::reset ()
      {
        polyline_id = constants::INVALID_ID;
        number_of_vertexes = 0;
        return;
      }

      /***********************************************/

      helix_type::helix_type ()
      {
        reset ();
        return;
      }

      void helix_type::reset ()
      {
        helix_id = constants::INVALID_ID;
        x0 = constants::INVALID_DOUBLE;
        y0 = constants::INVALID_DOUBLE;
        z0 = constants::INVALID_DOUBLE;
        r = constants::INVALID_DOUBLE;
        step = constants::INVALID_DOUBLE;
        t0 = constants::INVALID_DOUBLE;
        t1 = constants::INVALID_DOUBLE;
        return;
      }

      /***********************************************/

      tracker_trajectory_type::tracker_trajectory_type ()
      {
        reset ();
        return;
      }

      void tracker_trajectory_type::reset ()
      {
        solution_id = constants::INVALID_ID;
        trajectory_id = constants::INVALID_ID;
        module = constants::INVALID_ID;
        side  = constants::INVALID_ID;
        cluster_id = constants::INVALID_ID;
        delayed = false;
        number_of_orphans = 0;
        pattern_id = constants::INVALID_ID;
        return;
      }

      /***********************************************/

      tracker_trajectory_orphan_hit_type::tracker_trajectory_orphan_hit_type ()
      {
        reset ();
        return;
      }

      void tracker_trajectory_orphan_hit_type::reset ()
      {
        solution_id = constants::INVALID_ID;
        trajectory_id = constants::INVALID_ID;
        hit_id = constants::INVALID_ID;
        return;
      }

      /***********************************************/

      tracker_trajectory_pattern_type::tracker_trajectory_pattern_type ()
      {
        reset ();
        return;
      }

      void tracker_trajectory_pattern_type::reset ()
      {
        solution_id   = constants::INVALID_ID;
        trajectory_id = constants::INVALID_ID;
        pattern_id = constants::INVALID_ID;
        pattern_type  = constants::INVALID_ID;
        length        = constants::INVALID_DOUBLE;
        vertex0_id    = constants::INVALID_ID;
        vertex1_id    = constants::INVALID_ID;
        line_id     = constants::INVALID_ID;
        helix_id    = constants::INVALID_ID;
        polyline_id = constants::INVALID_ID;
        return;
      }

      /***********************************************/

      export_event::export_event ()
      {
        introspection_activator IA;
        reset ();
        true_vertices.reserve (1);
        true_particles.reserve (5);
        true_step_hits.reserve (300);
        true_gg_hits.reserve (100);
        true_calo_hits.reserve (5);
        true_xcalo_hits.reserve (5);
        true_gveto_hits.reserve (5);
        calib_scin_hits.reserve (5);
        calib_gg_hits.reserve (100);
        tracker_clusters.reserve (10);
        tracker_clustered_hits.reserve (100);
        tracker_trajectories.reserve (10);
        tracker_trajectory_orphan_hits.reserve (20);
        tracker_trajectory_patterns.reserve (10);
        return;
      }

      export_event::~export_event ()
      {
        return;
      }

      void export_event::reset ()
      {
        clear_data ();
        return;
      }

      void export_event::clear_data ()
      {
        event_header.reset ();
        true_particles.clear ();
        true_vertices.clear ();
        true_step_hits.clear ();
        true_gg_hits.clear ();
        true_calo_hits.clear ();
        true_xcalo_hits.clear ();
        true_gveto_hits.clear ();
        calib_scin_hits.clear ();
        calib_gg_hits.clear ();
        tracker_clusters.clear ();
        tracker_clustered_hits.clear ();
        tracker_trajectories.clear ();
        tracker_trajectory_orphan_hits.clear ();
        tracker_trajectory_vertices.clear ();
        tracker_trajectory_polylines.clear ();
        tracker_trajectory_helices.clear ();
        tracker_trajectory_patterns.clear ();
        return;
      }

      void export_event::print (std::ostream & out_,
                                const std::string & title_,
                                const std::string & indent_) const
      {
        if (! title_.empty ())
          {
            out_ << indent_ << title_ << " : \n";
          }
        out_ << indent_ << "|-- "
             << "EH: " << event_header.run_number << " " << event_header.event_number
             << std::endl;

        out_ << indent_ << "|-- "
             << "True vertices: " << true_vertices.size ()
             << std::endl;
        out_ << indent_ << "|-- "
             << "True particles: " << true_particles.size ()
             << std::endl;
        out_ << indent_ << "|-- "
             << "True step hits: " << true_step_hits.size ()
             << std::endl;
        out_ << indent_ << "|-- "
             << "True calo hits: " << true_calo_hits.size ()
             << std::endl;
        out_ << indent_ << "|-- "
             << "True xcalo hits: " << true_xcalo_hits.size ()
             << std::endl;
        out_ << indent_ << "|-- "
             << "True gveto hits: " << true_gveto_hits.size ()
             << std::endl;
        out_ << indent_ << "|-- "
             << "True gg hits: " << true_gg_hits.size ()
             << std::endl;

        out_ << indent_ << "|-- "
             << "Calib scin hits: " << calib_scin_hits.size ()
             << std::endl;
        out_ << indent_ << "|-- "
             << "Calib gg hits: " << calib_gg_hits.size ()
             << std::endl;

        out_ << indent_ << "|-- "
             << "Tracker clusters: " << tracker_clusters.size ()
             << std::endl;
        out_ << indent_ << "|-- "
             << "Tracker clustered hits: " << tracker_clustered_hits.size ()
             << std::endl;

        out_ << indent_ << "|-- "
             << "Tracker trajectories: " << tracker_trajectories.size ()
             << std::endl;
        out_ << indent_ << "|-- "
             << "Tracker trajectory vertices: " << tracker_trajectory_vertices.size ()
             << std::endl;
        out_ << indent_ << "|-- "
             << "Tracker trajectory polylines: " << tracker_trajectory_polylines.size ()
             << std::endl;
        out_ << indent_ << "|-- "
             << "Tracker trajectory helices: " << tracker_trajectory_helices.size ()
             << std::endl;
        out_ << indent_ << "|-- "
             << "Tracker trajectory orphan hits: " << tracker_trajectory_orphan_hits.size ()
             << std::endl;
        out_ << indent_ << "`-- "
             << "Tracker trajectory patterns: " << tracker_trajectory_patterns.size ()
             << std::endl;

        return;
      }

      const true_vertex_type &
      export_event::get_true_vertex (int i_) const
      {
        audit_vector<true_vertex_type> (true_vertices, i_);
        return true_vertices.at(i_);
      }

      const true_particle_type &
      export_event::get_true_particle (int i_) const
      {
        audit_vector<true_particle_type> (true_particles, i_);
        return true_particles.at(i_);
      }

      const true_step_hit_type &
      export_event::get_true_step_hit (int i_) const
      {
        audit_vector<true_step_hit_type> (true_step_hits, i_);
        return true_step_hits.at(i_);
      }

      const true_scin_hit_type &
      export_event::get_true_calo_hit (int i_) const
      {
        audit_vector<true_scin_hit_type> (true_calo_hits, i_);
        return true_calo_hits.at(i_);
      }

      const true_scin_hit_type &
      export_event::get_true_xcalo_hit (int i_) const
      {
        audit_vector<true_scin_hit_type> (true_xcalo_hits, i_);
        return true_xcalo_hits.at(i_);
      }

      const true_scin_hit_type &
      export_event::get_true_gveto_hit (int i_) const
      {
        audit_vector<true_scin_hit_type> (true_gveto_hits, i_);
        return true_gveto_hits.at(i_);
      }

      const true_gg_hit_type &
      export_event::get_true_gg_hit (int i_) const
      {
        audit_vector<true_gg_hit_type> (true_gg_hits, i_);
        return true_gg_hits.at(i_);
      }

      const calib_calorimeter_hit_type &
      export_event::get_calib_scin_hit (int i_) const
      {
        audit_vector<calib_calorimeter_hit_type> (calib_scin_hits, i_);
        return calib_scin_hits.at(i_);
      }

      const calib_tracker_hit_type &
      export_event::get_calib_gg_hit (int i_) const
      {
        audit_vector<calib_tracker_hit_type> (calib_gg_hits, i_);
        return calib_gg_hits.at(i_);
      }

      const tracker_clustered_hit_type &
      export_event::get_tracker_clustered_hit (int i_) const
      {
        audit_vector<tracker_clustered_hit_type> (tracker_clustered_hits, i_);
        return tracker_clustered_hits.at(i_);
      }

      const tracker_cluster_type &
      export_event::get_tracker_cluster (int i_) const
      {
        audit_vector<tracker_cluster_type> (tracker_clusters, i_);
        return tracker_clusters.at(i_);
      }

      const tracker_trajectory_type &
      export_event::get_tracker_trajectory (int i_) const
      {
        audit_vector<tracker_trajectory_type> (tracker_trajectories, i_);
        return tracker_trajectories.at(i_);
      }

      const tracker_trajectory_orphan_hit_type &
      export_event::get_tracker_trajectory_orphan_hit (int i_) const
      {
        audit_vector<tracker_trajectory_orphan_hit_type> (tracker_trajectory_orphan_hits, i_);
        return tracker_trajectory_orphan_hits.at(i_);
      }

      const tracker_trajectory_pattern_type &
      export_event::get_tracker_trajectory_pattern (int i_) const
      {
        audit_vector<tracker_trajectory_pattern_type> (tracker_trajectory_patterns, i_);
        return tracker_trajectory_patterns.at(i_);
      }

      const vertex_type &
      export_event::get_tracker_trajectory_vertex (int i_) const
      {
        audit_vector<vertex_type> (tracker_trajectory_vertices, i_);
        return tracker_trajectory_vertices.at(i_);
      }

      const polyline_type &
      export_event::get_tracker_trajectory_polyline (int i_) const
      {
        audit_vector<polyline_type> (tracker_trajectory_polylines, i_);
        return tracker_trajectory_polylines.at(i_);
      }

      const helix_type &
      export_event::get_tracker_trajectory_helix (int i_) const
      {
        audit_vector<helix_type> (tracker_trajectory_helices, i_);
        return tracker_trajectory_helices.at(i_);
      }

      export_event::introspection_activator::introspection_activator ()
      {
        static bool activated = false;
        if (! activated)
          {
            implement_introspection ();
            activated = true;
          }
        return;
      }

      void export_event::implement_introspection ()
      {
        std::clog << "NOTICE: export_event::implement_introspection: Entering...\n";

        try{
          camp::Class::declare< event_header_type >("event_header_type")
            .tag ("version", 0)
            .constructor0()
            .property ("runNumber", &event_header_type::run_number)
            .tag ("ctype", "int32_t")
            .property ("eventNumber", &event_header_type::event_number)
            .tag ("ctype", "int32_t")
            .property ("simulated", &event_header_type::simulated)
            .tag ("ctype", "bool")
            .property ("seconds", &event_header_type::seconds)
            .tag ("ctype", "int64_t")
            .property ("picoseconds", &event_header_type::picoseconds)
            .tag ("ctype", "int64_t")
            .property ("export_cat_infos", &event_header_type::export_cat_infos)
            .tag ("ctype", "bool")
            ;

          camp::Class::declare< true_vertex_type >("true_vertex_type")
            .tag ("version", 0)
            .constructor0()
            .property ("vertexId", &true_vertex_type::vertex_id)
            .tag ("ctype", "int32_t")
            .property ("x", &true_vertex_type::x)
            .tag ("ctype", "double")
            .tag ("unit", "mm")
            .property ("y", &true_vertex_type::y)
            .tag ("ctype", "double")
            .tag ("unit", "mm")
            .property ("z", &true_vertex_type::z)
            .tag ("ctype", "double")
            .tag ("unit", "mm")
            .property ("time", &true_vertex_type::time)
            .tag ("ctype", "double")
            .tag ("unit", "ns")
            ;

          camp::Class::declare< true_particle_type >("true_particle_type")
            .tag ("version", 0)
            .constructor0()
            .property ("trackId", &true_particle_type::track_id)
            .tag ("ctype", "int32_t")
            .property ("particle_type", &true_particle_type::particle_type)
            .tag ("ctype", "int32_t")
            .property ("px", &true_particle_type::px)
            .tag ("ctype", "double")
            .tag ("unit", "keV")
            .property ("py", &true_particle_type::py)
            .tag ("ctype", "double")
            .tag ("unit", "keV")
            .property ("pz", &true_particle_type::pz)
            .tag ("ctype", "double")
            .tag ("unit", "keV")
            .property ("time", &true_particle_type::time)
            .tag ("ctype", "double")
            .tag ("unit", "ns")
            .property ("vertexId", &true_particle_type::vertex_id)
            .tag ("ctype", "int32_t")
            ;

          camp::Class::declare< true_step_hit_type >("true_step_hit_type")
            .tag ("version", 0)
            .constructor0()
            .property ("hitId", &true_step_hit_type::hit_id)
            .tag ("ctype", "int32_t")
            .property ("tStart", &true_step_hit_type::tstart)
            .tag ("ctype", "double")
            .tag ("unit", "ns")
            .property ("xStart", &true_step_hit_type::xstart)
            .tag ("ctype", "double")
            .tag ("unit", "mm")
            .property ("yStart", &true_step_hit_type::ystart)
            .tag ("ctype", "double")
            .tag ("unit", "mm")
            .property ("zStart", &true_step_hit_type::zstart)
            .tag ("ctype", "double")
            .tag ("unit", "mm")
            .property ("pxStart", &true_step_hit_type::pxstart)
            .tag ("ctype", "double")
            .tag ("unit", "keV")
            .property ("pyStart", &true_step_hit_type::pystart)
            .tag ("ctype", "double")
            .tag ("unit", "keV")
            .property ("pzStart", &true_step_hit_type::pzstart)
            .tag ("ctype", "double")
            .tag ("unit", "keV")
            .property ("tStop", &true_step_hit_type::tstop)
            .tag ("ctype", "double")
            .tag ("unit", "ns")
            .property ("xStop", &true_step_hit_type::xstop)
            .tag ("ctype", "double")
            .tag ("unit", "mm")
            .property ("yStop", &true_step_hit_type::ystop)
            .tag ("ctype", "double")
            .tag ("unit", "mm")
            .property ("zStop", &true_step_hit_type::zstop)
            .tag ("ctype", "double")
            .tag ("unit", "mm")
            .property ("pxStop", &true_step_hit_type::pxstop)
            .tag ("ctype", "double")
            .tag ("unit", "keV")
            .property ("pyStop", &true_step_hit_type::pystop)
            .tag ("ctype", "double")
            .tag ("unit", "keV")
            .property ("pzStop", &true_step_hit_type::pzstop)
            .tag ("ctype", "double")
            .tag ("unit", "keV")
            .property ("deltaEnergy", &true_step_hit_type::delta_energy)
            .tag ("ctype", "double")
            .tag ("unit", "keV")
            ;

          camp::Class::declare< true_gg_hit_type >("true_gg_hit_type")
            .tag ("version", 0)
            .constructor0()
            .property ("hitId", &true_gg_hit_type::hit_id)
            .tag ("ctype", "int32_t")
            .property ("module", &true_gg_hit_type::module)
            .tag ("ctype", "int32_t")
            .property ("side", &true_gg_hit_type::side)
            .tag ("ctype", "int32_t")
            .property ("layer", &true_gg_hit_type::layer)
            .tag ("ctype", "int32_t")
            .property ("row", &true_gg_hit_type::row)
            .tag ("ctype", "int32_t")
            .property ("tIonization", &true_gg_hit_type::tionization)
            .tag ("ctype", "double")
            .tag ("unit", "ns")
            .property ("xIonization", &true_gg_hit_type::xionization)
            .tag ("ctype", "double")
            .tag ("unit", "mm")
            .property ("yIonization", &true_gg_hit_type::yionization)
            .tag ("ctype", "double")
            .tag ("unit", "mm")
            .property ("zIonization", &true_gg_hit_type::zionization)
            .tag ("ctype", "double")
            .tag ("unit", "mm")
            .property ("pxIonization", &true_gg_hit_type::pxionization)
            .tag ("ctype", "double")
            .tag ("unit", "keV")
            .property ("pyIonization", &true_gg_hit_type::pyionization)
            .tag ("ctype", "double")
            .tag ("unit", "keV")
            .property ("pzIonization", &true_gg_hit_type::pzionization)
            .tag ("ctype", "double")
            .tag ("unit", "keV")
            .property ("xAnode", &true_gg_hit_type::xanode)
            .tag ("ctype", "double")
            .tag ("unit", "mm")
            .property ("yAnode", &true_gg_hit_type::yanode)
            .tag ("ctype", "double")
            .tag ("unit", "mm")
            .property ("zAnode", &true_gg_hit_type::zanode)
            .tag ("ctype", "double")
            .tag ("unit", "mm")
            ;

          camp::Class::declare< true_scin_hit_type >("true_scin_hit_type")
            .tag ("version", 0)
            .constructor0()
            .property ("hitId", &true_scin_hit_type::hit_id)
            .tag ("ctype", "int32_t")
            .property ("type", &true_scin_hit_type::type)
            .tag ("ctype", "int32_t")
            .property ("module", &true_scin_hit_type::module)
            .tag ("ctype", "int32_t")
            .property ("side", &true_scin_hit_type::side)
            .tag ("ctype", "int32_t")
            .property ("column", &true_scin_hit_type::column)
            .tag ("ctype", "int32_t")
            .property ("row", &true_scin_hit_type::row)
            .tag ("ctype", "int32_t")
            .property ("wall", &true_scin_hit_type::wall)
            .tag ("ctype", "int32_t")
            .property ("tFirst", &true_scin_hit_type::tfirst)
            .tag ("ctype", "double")
            .tag ("unit", "ns")
            .property ("tLast", &true_scin_hit_type::tlast)
            .tag ("ctype", "double")
            .tag ("unit", "ns")
            .property ("x1", &true_scin_hit_type::x1)
            .tag ("ctype", "double")
            .tag ("unit", "mm")
            .property ("y1", &true_scin_hit_type::y1)
            .tag ("ctype", "double")
            .tag ("unit", "mm")
            .property ("z1", &true_scin_hit_type::z1)
            .tag ("ctype", "double")
            .tag ("unit", "mm")
            .property ("x2", &true_scin_hit_type::x2)
            .tag ("ctype", "double")
            .tag ("unit", "mm")
            .property ("y2", &true_scin_hit_type::y2)
            .tag ("ctype", "double")
            .tag ("unit", "mm")
            .property ("z2", &true_scin_hit_type::z2)
            .tag ("ctype", "double")
            .tag ("unit", "mm")
            .property ("deltaEnergy", &true_scin_hit_type::delta_energy)
            .tag ("ctype", "double")
            .tag ("unit", "keV")
            ;

          camp::Class::declare< calib_tracker_hit_type >("calib_tracker_hit_type")
            .tag ("version", 0)
            .constructor0()
            .property ("hitId", &calib_tracker_hit_type::hit_id)
            .tag ("ctype", "int32_t")
            .property ("trueHitId", &calib_tracker_hit_type::true_hit_id)
            .tag ("ctype", "int32_t")
            .property ("module", &calib_tracker_hit_type::module)
            .tag ("ctype", "int32_t")
            .property ("side", &calib_tracker_hit_type::side)
            .tag ("ctype", "int32_t")
            .property ("layer", &calib_tracker_hit_type::layer)
            .tag ("ctype", "int32_t")
            .property ("row", &calib_tracker_hit_type::row)
            .tag ("ctype", "int32_t")
            .property ("noisy", &calib_tracker_hit_type::noisy)
            .tag ("ctype", "bool")
            .property ("missingBottomCathode", &calib_tracker_hit_type::missing_bottom_cathode)
            .tag ("ctype", "bool")
            .property ("missingTopCathode", &calib_tracker_hit_type::missing_top_cathode)
            .tag ("ctype", "bool")
            .property ("delayed", &calib_tracker_hit_type::delayed)
            .tag ("ctype", "bool")
            .property ("delayedTime", &calib_tracker_hit_type::delayed_time)
            .tag ("ctype", "double")
            .tag ("unit", "ns")
            .property ("delayedTimeError", &calib_tracker_hit_type::delayed_time_error)
            .tag ("ctype", "double")
            .tag ("unit", "ns")
            .property ("x", &calib_tracker_hit_type::x)
            .tag ("ctype", "double")
            .tag ("unit", "mm")
            .property ("y", &calib_tracker_hit_type::y)
            .tag ("ctype", "double")
            .tag ("unit", "mm")
            .property ("z", &calib_tracker_hit_type::z)
            .tag ("ctype", "double")
            .tag ("unit", "mm")
            .property ("sigmaZ", &calib_tracker_hit_type::sigma_z)
            .tag ("ctype", "double")
            .tag ("unit", "mm")
            .property ("r", &calib_tracker_hit_type::r)
            .tag ("ctype", "double")
            .tag ("unit", "mm")
            .property ("sigmaR", &calib_tracker_hit_type::sigma_r)
            .tag ("ctype", "double")
            .tag ("unit", "mm")

            // Special CAT clustering infos :
            .property ("hasCatInfos", &calib_tracker_hit_type::has_cat_infos)
            .tag ("ctype", "bool")
            .tag ("topic", "CAT")

            .property ("catTangencyX", &calib_tracker_hit_type::cat_tangency_x)
            .tag ("ctype", "double")
            .tag ("topic", "CAT")
            .property ("catTangencyY", &calib_tracker_hit_type::cat_tangency_y)
            .tag ("ctype", "double")
            .tag ("topic", "CAT")
            .property ("catTangencyZ", &calib_tracker_hit_type::cat_tangency_z)
            .tag ("topic", "CAT")
            .tag ("ctype", "double")
            .tag ("topic", "CAT")
            .property ("catTangencyXError", &calib_tracker_hit_type::cat_tangency_x_error)
            .tag ("ctype", "double")
            .tag ("topic", "CAT")
            .property ("catTangencyYError", &calib_tracker_hit_type::cat_tangency_y_error)
            .tag ("ctype", "double")
            .tag ("topic", "CAT")
            .property ("catTangencyZError", &calib_tracker_hit_type::cat_tangency_z_error)
            .tag ("ctype", "double")
            .tag ("topic", "CAT")
            .property ("catHelixX", &calib_tracker_hit_type::cat_helix_x)
            .tag ("ctype", "double")
            .tag ("topic", "CAT")
            .property ("catHelixY", &calib_tracker_hit_type::cat_helix_y)
            .tag ("ctype", "double")
            .tag ("topic", "CAT")
            .property ("catHelixZ", &calib_tracker_hit_type::cat_helix_z)
            .tag ("ctype", "double")
            .tag ("topic", "CAT")
            .property ("catHelixXError", &calib_tracker_hit_type::cat_helix_x_error)
            .tag ("ctype", "double")
            .tag ("topic", "CAT")
            .property ("catHelixYError", &calib_tracker_hit_type::cat_helix_y_error)
            .tag ("ctype", "double")
            .tag ("topic", "CAT")
            .property ("catHelixZError", &calib_tracker_hit_type::cat_helix_z_error)
            .tag ("ctype", "double")
            .tag ("topic", "CAT")
             ;

          camp::Class::declare< calib_calorimeter_hit_type >("calib_calorimeter_hit_type")
            .tag ("version", 0)
            .constructor0()
            .property ("hitId", &calib_calorimeter_hit_type::hit_id)
            .tag ("ctype", "int32_t")
            .property ("trueHitId", &calib_calorimeter_hit_type::true_hit_id)
            .tag ("ctype", "int32_t")
            .property ("type", &calib_calorimeter_hit_type::type)
            .tag ("ctype", "int32_t")
            .property ("module", &calib_calorimeter_hit_type::module)
            .tag ("ctype", "int32_t")
            .property ("side", &calib_calorimeter_hit_type::side)
            .tag ("ctype", "int32_t")
            .property ("column", &calib_calorimeter_hit_type::column)
            .tag ("ctype", "int32_t")
            .property ("row", &calib_calorimeter_hit_type::row)
            .tag ("ctype", "int32_t")
            .property ("wall", &calib_calorimeter_hit_type::wall)
            .tag ("ctype", "int32_t")
            .property ("time", &calib_calorimeter_hit_type::time)
            .tag ("ctype", "double")
            .tag ("unit", "ns")
            .property ("sigmaTime", &calib_calorimeter_hit_type::sigma_time)
            .tag ("ctype", "double")
            .tag ("unit", "ns")
            .property ("energy", &calib_calorimeter_hit_type::energy)
            .tag ("ctype", "double")
            .tag ("unit", "keV")
            .property ("sigmaEnergy", &calib_calorimeter_hit_type::sigma_energy)
            .tag ("ctype", "double")
            .tag ("unit", "keV")
            ;

          camp::Class::declare< tracker_clustered_hit_type >("tracker_clustered_hit_type")
            .tag ("version", 0)
            .constructor0()
            .property ("solutionId", &tracker_clustered_hit_type::solution_id)
            .tag ("ctype", "int32_t")
            .property ("clusterId", &tracker_clustered_hit_type::cluster_id)
            .tag ("ctype", "int32_t")
            .property ("hitId", &tracker_clustered_hit_type::hit_id)
            .tag ("ctype", "int32_t")
           ;

          camp::Class::declare< tracker_cluster_type >("tracker_cluster_type")
            .tag ("version", 0)
            .constructor0()
            .property ("solutionId", &tracker_cluster_type::solution_id)
            .tag ("ctype", "int32_t")
            .property ("clusterId", &tracker_cluster_type::cluster_id)
            .tag ("ctype", "int32_t")
            .property ("module", &tracker_cluster_type::module)
            .tag ("ctype", "int32_t")
            .property ("side", &tracker_cluster_type::side)
            .tag ("ctype", "int32_t")
            .property ("delayed", &tracker_cluster_type::delayed)
            .tag ("ctype", "bool")
            .property ("numberOfHits", &tracker_cluster_type::number_of_hits)
            .tag ("ctype", "uint32_t")


            // Special CAT clustering infos :
            .property ("hasCatInfos", &tracker_cluster_type::has_cat_infos)
            .tag ("ctype", "bool")
            .tag ("topic", "CAT")

            .property ("catHasCharge", &tracker_cluster_type::cat_has_charge)
            .tag ("ctype", "bool")
            .tag ("topic", "CAT")
            .property ("catCharge", &tracker_cluster_type::cat_charge)
            .tag ("ctype", "double")
            .tag ("topic", "CAT")

            .property ("catHasMomentum", &tracker_cluster_type::cat_has_momentum)
            .tag ("ctype", "bool")
            .tag ("topic", "CAT")
            .property ("catMomentumX", &tracker_cluster_type::cat_momentum_x)
            .tag ("ctype", "double")
            .tag ("topic", "CAT")
            .property ("catMomentumY", &tracker_cluster_type::cat_momentum_y)
            .tag ("ctype", "double")
            .tag ("topic", "CAT")
            .property ("catMomentumZ", &tracker_cluster_type::cat_momentum_z)
            .tag ("ctype", "double")
            .tag ("topic", "CAT")

            .property ("catHasHelixVertex", &tracker_cluster_type::cat_has_helix_vertex)
            .tag ("ctype", "bool")
            .tag ("topic", "CAT")
            .property ("caTHelixVertexX", &tracker_cluster_type::cat_helix_vertex_x)
            .tag ("ctype", "double")
            .tag ("topic", "CAT")
            .property ("catHelixVertexY", &tracker_cluster_type::cat_helix_vertex_y)
            .tag ("ctype", "double")
            .tag ("topic", "CAT")
            .property ("catHelixVertexZ", &tracker_cluster_type::cat_helix_vertex_z)
            .tag ("ctype", "double")
            .tag ("topic", "CAT")
            .property ("catHelixVertexXError", &tracker_cluster_type::cat_helix_vertex_x_error)
            .tag ("ctype", "double")
            .tag ("topic", "CAT")
            .property ("catHelixVertexYError", &tracker_cluster_type::cat_helix_vertex_y_error)
            .tag ("ctype", "double")
            .tag ("topic", "CAT")
            .property ("catHelixVertexZError", &tracker_cluster_type::cat_helix_vertex_z_error)
            .tag ("ctype", "double")
            .tag ("topic", "CAT")

            .property ("catHasHelixDecayVertex", &tracker_cluster_type::cat_has_helix_decay_vertex)
            .tag ("ctype", "bool")
            .tag ("topic", "CAT")
            .property ("catHelixDecayVertexX", &tracker_cluster_type::cat_helix_decay_vertex_x)
            .tag ("ctype", "double")
            .tag ("topic", "CAT")
            .property ("catHelixDecayVertexY", &tracker_cluster_type::cat_helix_decay_vertex_y)
            .tag ("ctype", "double")
            .tag ("topic", "CAT")
            .property ("catHelixDecayVertexZ", &tracker_cluster_type::cat_helix_decay_vertex_z)
            .tag ("ctype", "double")
            .tag ("topic", "CAT")
            .property ("catHelixDecayVertexXError", &tracker_cluster_type::cat_helix_decay_vertex_x_error)
            .tag ("ctype", "double")
            .tag ("topic", "CAT")
            .property ("catHelixDecayVertexYError", &tracker_cluster_type::cat_helix_decay_vertex_y_error)
            .tag ("ctype", "double")
            .tag ("topic", "CAT")
            .property ("catHelixDecayVertexZError", &tracker_cluster_type::cat_helix_decay_vertex_z_error)
            .tag ("ctype", "double")
            .tag ("topic", "CAT")

            .property ("catHasTangentVertex", &tracker_cluster_type::cat_has_tangent_vertex)
            .tag ("ctype", "bool")
            .tag ("topic", "CAT")
            .property ("catTangentVertexX", &tracker_cluster_type::cat_tangent_vertex_x)
            .tag ("ctype", "double")
            .tag ("topic", "CAT")
            .property ("catTangentVertexY", &tracker_cluster_type::cat_tangent_vertex_y)
            .tag ("ctype", "double")
            .tag ("topic", "CAT")
            .property ("catTangentVertexZ", &tracker_cluster_type::cat_tangent_vertex_z)
            .tag ("ctype", "double")
            .tag ("topic", "CAT")
            .property ("catTangentVertexXError", &tracker_cluster_type::cat_tangent_vertex_x_error)
            .tag ("ctype", "double")
            .tag ("topic", "CAT")
            .property ("catTangentVertexYError", &tracker_cluster_type::cat_tangent_vertex_y_error)
            .tag ("ctype", "double")
            .tag ("topic", "CAT")
            .property ("catTangentVertexZError", &tracker_cluster_type::cat_tangent_vertex_z_error)
            .tag ("ctype", "double")
            .tag ("topic", "CAT")

            .property ("catHasTangentDecayVertex", &tracker_cluster_type::cat_has_tangent_decay_vertex)
            .tag ("ctype", "bool")
            .tag ("topic", "CAT")
            .property ("catTangentDecayVertexX", &tracker_cluster_type::cat_tangent_decay_vertex_x)
            .tag ("ctype", "double")
            .tag ("topic", "CAT")
            .property ("catTangentDecayVertexY", &tracker_cluster_type::cat_tangent_decay_vertex_y)
            .tag ("ctype", "double")
            .tag ("topic", "CAT")
            .property ("catTangentDecayVertexZ", &tracker_cluster_type::cat_tangent_decay_vertex_z)
            .tag ("ctype", "double")
            .tag ("topic", "CAT")
            .property ("catTangentDecayVertexXError", &tracker_cluster_type::cat_tangent_decay_vertex_x_error)
            .tag ("ctype", "double")
            .tag ("topic", "CAT")
            .property ("catTangentDecayVertexYError", &tracker_cluster_type::cat_tangent_decay_vertex_y_error)
            .tag ("ctype", "double")
            .tag ("topic", "CAT")
            .property ("catTangentDecayVertexZError", &tracker_cluster_type::cat_tangent_decay_vertex_z_error)
            .tag ("ctype", "double")
            .tag ("topic", "CAT")
           ;

          camp::Class::declare< vertex_type >("vertex_type")
            .tag ("version", 0)
            .constructor0()
            .property ("vertexId", &vertex_type::vertex_id)
            .tag ("ctype", "int32_t")
            .property ("parentType", &vertex_type::parent_type)
            .tag ("ctype", "int32_t")
            .property ("parentId", &vertex_type::parent_id)
            .tag ("ctype", "int32_t")
            .property ("x", &vertex_type::x)
            .tag ("ctype", "double")
            .tag ("unit", "mm")
            .property ("y", &vertex_type::y)
            .tag ("ctype", "double")
            .tag ("unit", "mm")
            .property ("z", &vertex_type::z)
            .tag ("ctype", "double")
            .tag ("unit", "mm")
            .property ("xError", &vertex_type::x_error)
            .tag ("ctype", "double")
            .tag ("unit", "mm")
            .property ("yError", &vertex_type::y_error)
            .tag ("ctype", "double")
            .tag ("unit", "mm")
            .property ("zError", &vertex_type::z_error)
            .tag ("ctype", "double")
            .tag ("unit", "mm")
            ;

          camp::Class::declare< polyline_type >("polyline_type")
            .tag ("version", 0)
            .constructor0()
            .property ("polylineId", &polyline_type::polyline_id)
            .tag ("ctype", "int32_t")
            .property ("numberOfVertexes", &polyline_type::number_of_vertexes)
            .tag ("ctype", "uint32_t")
            ;

          camp::Class::declare< helix_type >("helix_type")
            .tag ("version", 0)
            .constructor0()
            .property ("helixId", &helix_type::helix_id)
            .tag ("ctype", "int32_t")
            .property ("x0", &helix_type::x0)
            .tag ("ctype", "double")
            .property ("y0", &helix_type::y0)
            .tag ("ctype", "double")
            .property ("z0", &helix_type::z0)
            .tag ("ctype", "double")
            .property ("r", &helix_type::r)
            .tag ("ctype", "double")
            .property ("step", &helix_type::step)
            .tag ("ctype", "double")
            .property ("t0", &helix_type::t0)
            .tag ("ctype", "double")
            .property ("t1", &helix_type::t1)
            .tag ("ctype", "double")
           ;

          camp::Class::declare< tracker_trajectory_type >("tracker_trajectory_type")
            .tag ("version", 0)
            .constructor0()
            .property ("solutionId", &tracker_trajectory_type::solution_id)
            .tag ("ctype", "int32_t")
            .property ("trajectoryId", &tracker_trajectory_type::trajectory_id)
            .tag ("ctype", "int32_t")
            .property ("module", &tracker_trajectory_type::module)
            .tag ("ctype", "int32_t")
            .property ("side", &tracker_trajectory_type::side)
            .tag ("ctype", "int32_t")
            .property ("clusterId", &tracker_trajectory_type::cluster_id)
            .tag ("ctype", "int32_t")
            .property ("delayed", &tracker_trajectory_type::delayed)
            .tag ("ctype", "bool")
            .property ("numberOfOrphans", &tracker_trajectory_type::number_of_orphans)
            .tag ("ctype", "uint32_t")
            .property ("patternId", &tracker_trajectory_type::pattern_id)
            .tag ("ctype", "int32_t")
           ;

          camp::Class::declare< tracker_trajectory_orphan_hit_type >("tracker_trajectory_orphan_hit_type")
            .tag ("version", 0)
            .constructor0()
            .property ("solutionId", &tracker_trajectory_orphan_hit_type::solution_id)
            .tag ("ctype", "int32_t")
            .property ("trajectoryId", &tracker_trajectory_orphan_hit_type::trajectory_id)
            .tag ("ctype", "int32_t")
            .property ("hitId", &tracker_trajectory_orphan_hit_type::hit_id)
            .tag ("ctype", "int32_t")
            ;

          camp::Class::declare< tracker_trajectory_pattern_type >("tracker_trajectory_pattern_type")
            .tag ("version", 0)
            .constructor0()
            .property ("patternId", &tracker_trajectory_pattern_type::pattern_id)
            .tag ("ctype", "int32_t")
            .property ("solutionId", &tracker_trajectory_pattern_type::solution_id)
            .tag ("ctype", "int32_t")
            .property ("trajectoryId", &tracker_trajectory_pattern_type::trajectory_id)
            .tag ("ctype", "int32_t")
            .property ("patternType", &tracker_trajectory_pattern_type::pattern_type)
            .tag ("ctype", "int32_t")
            .property ("length", &tracker_trajectory_pattern_type::length)
            .tag ("ctype", "double")
            .tag ("unit", "mm")
            .property ("vertext0Id", &tracker_trajectory_pattern_type::vertex0_id)
            .tag ("ctype", "int32_t")
            .property ("vertext1Id", &tracker_trajectory_pattern_type::vertex1_id)
            .tag ("ctype", "int32_t")
            .property ("lineId", &tracker_trajectory_pattern_type::line_id)
            .tag ("ctype", "int32_t")
            .property ("helixId", &tracker_trajectory_pattern_type::helix_id)
            .tag ("ctype", "int32_t")
            .property ("polylineId", &tracker_trajectory_pattern_type::polyline_id)
            .tag ("ctype", "int32_t")
            ;

           /*************************************************************************/

          camp::Class::declare< export_event >("export_event")
            .tag ("version", 0)
            .constructor0()
            // Event header :
            .property ("header", &export_event::event_header)

            // True vertices :
            .property ("trueVertices",      &export_event::true_vertices)
            .property ("trueVertices@size",
                       &std::vector<true_vertex_type>::size,
                       &export_event::true_vertices)
            .function ("trueVertices@get",  &export_event::get_true_vertex)

            // True particles :
            .property ("trueParticles",     &export_event::true_particles)
            .property ("trueParticles@size",
                       &std::vector<true_particle_type>::size,
                       &export_event::true_particles)
            .function ("trueParticles@get", &export_event::get_true_particle)

            // True step hits :
            .property ("trueStepHits",      &export_event::true_step_hits)
            .property ("trueStepHits@size",
                       &std::vector<true_step_hit_type>::size,
                       &export_event::true_step_hits)
            .function ("trueStepHits@get",  &export_event::get_true_step_hit)

            // True calo hits :
            .property ("trueCaloHits",      &export_event::true_calo_hits)
            .property ("trueCaloHits@size",
                       &std::vector<true_scin_hit_type>::size,
                       &export_event::true_calo_hits)
            .function ("trueCaloHits@get",  &export_event::get_true_calo_hit)

            // True xcalo hits :
            .property ("trueXcaloHits",     &export_event::true_xcalo_hits)
            .property ("trueXcaloHits@size",
                       &std::vector<true_scin_hit_type>::size,
                       &export_event::true_xcalo_hits)
            .function ("trueXcaloHits@get", &export_event::get_true_xcalo_hit)

            // True gveto hits :
            .property ("trueGvetoHits",     &export_event::true_gveto_hits)
            .property ("trueGvetoHits@size",
                       &std::vector<true_scin_hit_type>::size,
                       &export_event::true_gveto_hits)
            .function ("trueGvetoHits@get", &export_event::get_true_gveto_hit)

            // True gg hits :
            .property ("trueGgHits",        &export_event::true_gg_hits)
            .property ("trueGgHits@size",
                       &std::vector<true_gg_hit_type>::size,
                       &export_event::true_gg_hits)
            .function ("trueGgHits@get",    &export_event::get_true_gg_hit)

            // Calibrated scintillator hits :
            .property ("calibScinHits",     &export_event::calib_scin_hits)
            .property ("calibScinHits@size",
                       &std::vector<calib_calorimeter_hit_type>::size,
                       &export_event::calib_scin_hits)
            .function ("calibScinHits@get", &export_event::get_calib_scin_hit)

            // Calibrated tracker hits :
            .property ("calibTrackerHits",       &export_event::calib_gg_hits)
            .property ("calibTrackerHits@size",
                       &std::vector<calib_tracker_hit_type>::size,
                       &export_event::calib_gg_hits)
            .function ("calibTrackerHits@get",   &export_event::get_calib_gg_hit)

            // Tracker clusters :
            .property ("trackerClusters",       &export_event::tracker_clusters)
            .property ("trackerClusters@size",
                       &std::vector<tracker_cluster_type>::size,
                       &export_event::tracker_clusters)
            .function ("trackerClusters@get",   &export_event::get_tracker_cluster)

            // Tracker clustered hits :
            .property ("trackerClusteredHits",       &export_event::tracker_clustered_hits)
            .property ("trackerClusteredHits@size",
                       &std::vector<tracker_clustered_hit_type>::size,
                       &export_event::tracker_clustered_hits)
            .function ("trackerClusteredHits@get",   &export_event::get_tracker_clustered_hit)

            // Tracker trajectories :
            .property ("trackerTrajectories",       &export_event::tracker_trajectories)
            .property ("trackerTrajectories@size",
                       &std::vector<tracker_trajectory_type>::size,
                       &export_event::tracker_trajectories)
            .function ("trackerTrajectories@get",   &export_event::get_tracker_trajectory)

            // Tracker trajectory orphan hits:
            .property ("trackerTrajectoryOrphanHits",       &export_event::tracker_trajectory_orphan_hits)
            .property ("trackerTrajectoryOrphanHits@size",
                       &std::vector<tracker_trajectory_orphan_hit_type>::size,
                       &export_event::tracker_trajectory_orphan_hits)
            .function ("trackerTrajectoryOrphanHits@get",   &export_event::get_tracker_trajectory_orphan_hit)

            // Tracker trajectory patterns:
            .property ("trackerTrajectoryPatterns",       &export_event::tracker_trajectory_patterns)
            .property ("trackerTrajectoryPatterns@size",
                       &std::vector<tracker_trajectory_pattern_type>::size,
                       &export_event::tracker_trajectory_patterns)
            .function ("trackerTrajectoryPatterns@get",   &export_event::get_tracker_trajectory_pattern)

            // Tracker trajectory vertices:
            .property ("trackerTrajectoryVertices",       &export_event::tracker_trajectory_vertices)
            .property ("trackerTrajectoryVertices@size",
                       &std::vector<vertex_type>::size,
                       &export_event::tracker_trajectory_vertices)
            .function ("trackerTrajectoryVertices@get",   &export_event::get_tracker_trajectory_vertex)

            // Tracker trajectory polyline:
            .property ("trackerTrajectoryPolyline",       &export_event::tracker_trajectory_polylines)
            .property ("trackerTrajectoryPolyline@size",
                       &std::vector<polyline_type>::size,
                       &export_event::tracker_trajectory_polylines)
            .function ("trackerTrajectoryPolyline@get",   &export_event::get_tracker_trajectory_polyline)

            // Tracker trajectory helix:
            .property ("trackerTrajectoryHelix",       &export_event::tracker_trajectory_helices)
            .property ("trackerTrajectoryHelix@size",
                       &std::vector<helix_type>::size,
                       &export_event::tracker_trajectory_helices)
            .function ("trackerTrajectoryHelix@get",   &export_event::get_tracker_trajectory_helix)

            // more...
            ;

        }
        catch(std::exception & x)
          {
            std::cerr << "ERROR: export_event::implement_introspection: "
                      << x.what () << std::endl;
          }
        catch(...)
          {
            std::cerr << "ERROR: export_event::implement_introspection: "
                      << "Unexpected error !" << std::endl;
          }
        std::clog << "NOTICE: export_event::implement_introspection: Exiting.\n";
      }

    }  // end of namespace exports

  }  // end of namespace reconstruction

}  // end of namespace snemo

/* end of export_event.cc */
