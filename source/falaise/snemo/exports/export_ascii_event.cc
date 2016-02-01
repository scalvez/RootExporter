// -*- mode: c++ ; -*-
/* export_ascii_event.cc */

#include <snreconstruction/exports/export_ascii_event.h>
#include <snreconstruction/exports/event_exporter.h>
#include <boost/foreach.hpp>
#include <boost/scoped_ptr.hpp>

#include <limits>

namespace snemo {

  namespace reconstruction {

    namespace exports {

      export_ascii_event::export_ascii_event ()
      {
        add_comments = false;
        return;
      }

      export_ascii_event::~export_ascii_event ()
      {
        return;
      }

      // static 
      void export_ascii_event::print_comment_data_tag_begin (std::ostream & out_,
                                                             const std::string & data_tag_)
      {
        out_ << '#' << "@record_begin: name=" << data_tag_ << std::endl;
        return;
      }

      // static 
      void export_ascii_event::print_comment_data_tag_end (std::ostream & out_,
                                                           const std::string & data_tag_)
      {
        out_ << '#' << "@record_end: name=" << data_tag_ << std::endl;
        return;
      }

      // static 
      void export_ascii_event::print_comment_data_info (std::ostream & out_,
                                                        const std::string & data_name_,
                                                        const std::string & data_type_,
                                                        const std::string & data_layout_,
                                                        const std::string & data_size_
                                                        )
      {
        out_ << '#' << "@data: " 
             << "name=" << data_name_ << " " 
             << "type=" << data_type_ << " ";
        if (data_type_ == "collection")
          {
            out_ << "size=" << data_size_ << " ";  
          }
        out_ << "layout=" << "store_version/uint32_t" << ';' << data_layout_ << " ";  
        
        out_ << std::endl;
        
        return;
      }

      void export_ascii_event::store (std::ostream & out_, 
                                      unsigned int store_bits_, 
                                      unsigned int store_version_) const
      {
        if (add_comments) print_comment_data_tag_begin (out_, "eventRecord");

        // STORE INFOS :
        // if (add_comments) print_comment_data_info (out_, 
        //                                            "store_version",
        //                                            "single",
        //                                            "store_version/int32_t");
        // out_ << store_version_ << ' ' << std::endl;

        if (store_version_ == 0)
          {
            if (add_comments) print_comment_data_info (out_, 
                                                       "eventRecord",
                                                       "single",
                                                       "store_bits/int32_t");
            out_ << store_version_ << ' ' << store_bits_ << ' ' << std::endl;
          }

        // EXPORT_EVENT_HEADER :
        if (store_bits_ & event_exporter::EXPORT_EVENT_HEADER)
          {
            if (add_comments) print_comment_data_tag_begin (out_, "eventRecord/header");

            if (add_comments) print_comment_data_info (out_, 
                                                       "header",
                                                       "single",
                                                       get_class_description<event_header_type>());
            store (out_, event_header, event_header_type::EXPORT_VERSION);
            out_ << std::endl;
            //std::clog << "DEVEL: " << "export_ascii_event::store: DONE !!!" << std::endl;
            if (add_comments) print_comment_data_tag_end (out_, "eventRecord/header");
          }

        // EXPORT_TRUE_PARTICLES :
        if (store_bits_ & event_exporter::EXPORT_TRUE_PARTICLES)
          {
            if (add_comments) print_comment_data_tag_begin (out_, "eventRecord/trueParticles");
            if (add_comments) print_comment_data_info (out_, 
                                                       "trueVertices",
                                                       "collection",
                                                       get_class_description<true_vertex_type>(),
                                                       "trueVertices@size/uint32_t");
            out_ << true_vertices.size () << ' ' << std::endl;
            BOOST_FOREACH (const true_vertex_type & true_vertex, true_vertices)
              {
                store (out_, true_vertex, true_vertex_type::EXPORT_VERSION);
                out_ << std::endl;
              }
 
            if (add_comments) print_comment_data_info (out_, 
                                                       "trueParticles",
                                                       "collection",   
                                                       get_class_description<true_particle_type>(),
                                                       "trueParticles@size/uint32_t");
            out_ << true_particles.size () << ' ' << std::endl;
            BOOST_FOREACH (const true_particle_type & true_particle, true_particles)
              {
                store (out_, true_particle, true_particle_type::EXPORT_VERSION);
                out_ << std::endl;
              }
            if (add_comments) print_comment_data_tag_end (out_, "eventRecord/trueParticles");
          }

        // EXPORT_TRUE_STEP_HITS :
        if (store_bits_ & event_exporter::EXPORT_TRUE_STEP_HITS)
          {
            if (add_comments) print_comment_data_tag_begin (out_, "eventRecord/trueStepHits");
            if (add_comments) print_comment_data_info (out_, 
                                                       "trueStepHits",
                                                       "collection",        
                                                       get_class_description<true_step_hit_type>(),
                                                       "trueStepHits@size/uint32_t");
            out_ << true_step_hits.size () << ' ' << std::endl;
            BOOST_FOREACH (const true_step_hit_type & true_step_hit, true_step_hits)
              {
                store (out_, true_step_hit, true_step_hit_type::EXPORT_VERSION);
                out_ << std::endl;
              }
            if (add_comments) print_comment_data_tag_end (out_, "eventRecord/trueStepHits");
         }

        // EXPORT_TRUE_HITS :
        if (store_bits_ & event_exporter::EXPORT_TRUE_HITS)
          {
            if (add_comments) print_comment_data_tag_begin (out_, "eventRecord/trueHits");
            if (add_comments) print_comment_data_info (out_, 
                                                       "trueCaloHits",
                                                       "collection",
                                                       get_class_description<true_scin_hit_type>(),
                                                       "trueCaloHits@size/uint32_t");
            out_ << true_calo_hits.size () << ' ' << std::endl;
            BOOST_FOREACH (const true_scin_hit_type & true_scin_hit, true_calo_hits)
              {
                store (out_, true_scin_hit, true_scin_hit_type::EXPORT_VERSION);
                out_ << std::endl;
              }

            if (add_comments) print_comment_data_info (out_, 
                                                       "trueXcaloHits",
                                                       "collection",
                                                       get_class_description<true_scin_hit_type>(),
                                                       "trueXcaloHits@size/uint32_t");
            out_ << true_xcalo_hits.size () << ' ' << std::endl;
            BOOST_FOREACH (const true_scin_hit_type & true_scin_hit, true_xcalo_hits)
              {
                store (out_, true_scin_hit, true_scin_hit_type::EXPORT_VERSION);
                out_ << std::endl;
              }

            if (add_comments) print_comment_data_info (out_, 
                                                       "trueGvetoHits",
                                                       "collection",
                                                        get_class_description<true_scin_hit_type>(),
                                                       "trueGvetoHits@size/uint32_t");
            out_ << true_gveto_hits.size () << ' ' << std::endl;
            BOOST_FOREACH (const true_scin_hit_type & true_scin_hit, true_gveto_hits)
              {
                store (out_, true_scin_hit, true_scin_hit_type::EXPORT_VERSION);
                out_ << std::endl;
              }
 
            if (add_comments) print_comment_data_info (out_, 
                                                       "trueGgHits",
                                                       "collection", 
                                                       get_class_description<true_gg_hit_type>(),
                                                       "trueGgHits@size/uint32_t");
            out_ << true_gg_hits.size () << ' ' << std::endl;
            BOOST_FOREACH (const true_gg_hit_type & true_gg_hit, true_gg_hits)
              {
                store (out_, true_gg_hit, true_gg_hit_type::EXPORT_VERSION);
                out_ << std::endl;
              }

            if (add_comments) print_comment_data_tag_end (out_, "eventRecord/true_hits");
          }

        // EXPORT_CALIB_CALORIMETER_HITS :
        if (store_bits_ & event_exporter::EXPORT_CALIB_CALORIMETER_HITS)
          {
            if (add_comments) print_comment_data_tag_begin (out_, "eventRecord/calibScinHits");
            if (add_comments) print_comment_data_info (out_, 
                                                       "calibScinHits",
                                                       "collection", 
                                                       get_class_description<calib_calorimeter_hit_type>(),
                                                       "calibScinHits@size/uint32_t");
            out_ << calib_scin_hits.size () << ' ' << std::endl;
            BOOST_FOREACH (const calib_calorimeter_hit_type & calib_scin_hit, calib_scin_hits)
              {
                store (out_, calib_scin_hit, calib_calorimeter_hit_type::EXPORT_VERSION);
                out_ << std::endl;
              }
            if (add_comments) print_comment_data_tag_end (out_, "eventRecord/calibScinHits");
          }

        // EXPORT_CALIB_TRACKER_HITS :
        if (store_bits_ & event_exporter::EXPORT_CALIB_TRACKER_HITS)
          {
            if (add_comments) print_comment_data_tag_begin (out_, "eventRecord/calibTrackerHits");
            if (add_comments) print_comment_data_info (out_, 
                                                       "calibTrackerHits",
                                                       "collection",   
                                                       get_class_description<calib_tracker_hit_type>(),
                                                       "calibTrackerHits@size/uint32_t");
            out_ << calib_gg_hits.size () << ' ' << std::endl;
            BOOST_FOREACH (const calib_tracker_hit_type & calib_gg_hit, calib_gg_hits)
              {
                store (out_, calib_gg_hit, calib_tracker_hit_type::EXPORT_VERSION);
                out_ << std::endl;
              }
            if (add_comments) print_comment_data_tag_end (out_, "eventRecord/calibTrackerHits");
          }
 
        if (add_comments) print_comment_data_tag_end (out_, "eventRecord");

        out_ << std::endl;
        return;
      }
      
    }  // end of namespace exports

  }  // end of namespace reconstruction

}  // end of namespace snemo

/* end of export_ascii_event.cc */
