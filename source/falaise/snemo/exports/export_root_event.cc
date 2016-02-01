// -*- mode: c++ ; -*-
/* export_root_event.cc */

#include <falaise/snemo/exports/export_root_event.h>
#include <falaise/snemo/exports/event_exporter.h>

#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

#include <limits>

#include <TTree.h>

namespace snemo {

  namespace reconstruction {

    namespace exports {

      export_root_event::export_root_event ()
      {
        _store_bits_ = 0;
        return;
      }

      export_root_event::~export_root_event ()
      {
        _branch_manager_.reset ();
        _store_bits_ = 0;
        return;
      }

      void export_root_event::construct (unsigned int store_bits_,
                                         const std::map<std::string,int> topics_,
                                         unsigned int store_version_)
      {
        _store_bits_ = store_bits_;

        for (std::map<std::string,int>::const_iterator i = topics_.begin();
             i != topics_.end();
             i++)
          {
            _branch_manager_.add_topic(i->first, i->second);
          }

        // EXPORT_EVENT_HEADER :
        if (_store_bits_ & event_exporter::EXPORT_EVENT_HEADER)
          {
            std::string bank_description;
            int32_t bank_version;
            bank_description = "event_header_type";
            bank_export_version<event_header_type>(bank_version);
            _branch_manager_.init_bank_from_camp ("header",
                                                  event_exporter::EXPORT_EVENT_HEADER,
                                                  bank_version,
                                                  bank_description,
                                                  branch_entry_type::SCALAR_DATA);
          }

        // EXPORT_TRUE_PARTICLES :
        if (_store_bits_ & event_exporter::EXPORT_TRUE_PARTICLES)
          {
            std::string bank_description;
            int32_t bank_version;
            bank_description = "true_vertex_type";
            bank_export_version<true_vertex_type>(bank_version);
            _branch_manager_.init_bank_from_camp ("trueVertices",
                                                  event_exporter::EXPORT_TRUE_PARTICLES,
                                                  bank_version,
                                                  bank_description,
                                                  branch_entry_type::ARRAY_DATA);

            bank_description = "true_particle_type";
            bank_export_version<true_particle_type>(bank_version);
            _branch_manager_.init_bank_from_camp ("trueParticles",
                                                  event_exporter::EXPORT_TRUE_PARTICLES,
                                                  bank_version,
                                                  bank_description,
                                                  branch_entry_type::ARRAY_DATA);
          }

        // EXPORT_TRUE_STEP_HITS :
        if (_store_bits_ & event_exporter::EXPORT_TRUE_STEP_HITS)
          {
            std::string bank_description;
            int32_t bank_version;
            bank_description = "true_step_hit_type";
            bank_export_version<true_step_hit_type>(bank_version);
            _branch_manager_.init_bank_from_camp ("trueStepHits",
                                                  event_exporter::EXPORT_TRUE_STEP_HITS,
                                                  bank_version,
                                                  bank_description,
                                                  branch_entry_type::ARRAY_DATA);
          }

        // EXPORT_TRUE_HITS :
        if (_store_bits_ & event_exporter::EXPORT_TRUE_HITS)
          {
            std::string bank_description;
            int32_t bank_version;
            bank_description = "true_scin_hit_type";
            bank_export_version<true_scin_hit_type>(bank_version);
            _branch_manager_.init_bank_from_camp ("trueCaloHits",
                                                  event_exporter::EXPORT_TRUE_HITS,
                                                  bank_version,
                                                  bank_description,
                                                  branch_entry_type::ARRAY_DATA);
            _branch_manager_.init_bank_from_camp ("trueXcaloHits",
                                                  event_exporter::EXPORT_TRUE_HITS,
                                                  bank_version,
                                                  bank_description,
                                                  branch_entry_type::ARRAY_DATA);
            _branch_manager_.init_bank_from_camp ("trueGvetoHits",
                                                  event_exporter::EXPORT_TRUE_HITS,
                                                  bank_version,
                                                  bank_description,
                                                  branch_entry_type::ARRAY_DATA);

            bank_description = "true_gg_hit_type";
            bank_export_version<true_gg_hit_type>(bank_version);
            _branch_manager_.init_bank_from_camp ("trueGgHits",
                                                  event_exporter::EXPORT_TRUE_HITS,
                                                  bank_version,
                                                  bank_description,
                                                  branch_entry_type::ARRAY_DATA);
          }

        // EXPORT_CALIB_CALORIMETER_HITS :
        if (_store_bits_ & event_exporter::EXPORT_CALIB_CALORIMETER_HITS)
          {
            std::string bank_description;
            int32_t bank_version;
            bank_description = "calib_calorimeter_hit_type";
            bank_export_version<calib_calorimeter_hit_type>(bank_version);
            _branch_manager_.init_bank_from_camp ("calibScinHits",
                                                  event_exporter::EXPORT_CALIB_CALORIMETER_HITS,
                                                  bank_version,
                                                  bank_description,
                                                  branch_entry_type::ARRAY_DATA);
          }

        // EXPORT_CALIB_TRACKER_HITS :
        if (_store_bits_ & event_exporter::EXPORT_CALIB_TRACKER_HITS)
          {
            std::string bank_description;
            int32_t bank_version;
            bank_description = "calib_tracker_hit_type";
            bank_export_version<calib_tracker_hit_type>(bank_version);
            _branch_manager_.init_bank_from_camp ("calibTrackerHits",
                                                  event_exporter::EXPORT_CALIB_TRACKER_HITS,
                                                  bank_version,
                                                  bank_description,
                                                  branch_entry_type::ARRAY_DATA);
          }

        // EXPORT_TRACKER_CLUSTERING :
        if (_store_bits_ & event_exporter::EXPORT_TRACKER_CLUSTERING)
          {
            std::string bank_description;
            int32_t bank_version;

            bank_description = "tracker_cluster_type";
            bank_export_version<tracker_cluster_type>(bank_version);
            _branch_manager_.init_bank_from_camp ("trackerClusters",
                                                  event_exporter::EXPORT_TRACKER_CLUSTERING,
                                                  bank_version,
                                                  bank_description,
                                                  branch_entry_type::ARRAY_DATA);

            bank_description = "tracker_clustered_hit_type";
            bank_export_version<tracker_clustered_hit_type>(bank_version);
            _branch_manager_.init_bank_from_camp ("trackerClusteredHits",
                                                  event_exporter::EXPORT_TRACKER_CLUSTERING,
                                                  bank_version,
                                                  bank_description,
                                                  branch_entry_type::ARRAY_DATA);
        }

        // EXPORT_TRACKER_TRAJECTORIES :
        if (_store_bits_ & event_exporter::EXPORT_TRACKER_TRAJECTORIES)
          {
            std::string bank_description;
            int32_t bank_version;

            bank_description = "tracker_trajectory_type";
            bank_export_version<tracker_trajectory_type>(bank_version);
            _branch_manager_.init_bank_from_camp ("trackerTrajectories",
                                                  event_exporter::EXPORT_TRACKER_TRAJECTORIES,
                                                  bank_version,
                                                  bank_description,
                                                  branch_entry_type::ARRAY_DATA);

            bank_description = "tracker_trajectory_orphan_hit_type";
            bank_export_version<tracker_trajectory_orphan_hit_type>(bank_version);
            _branch_manager_.init_bank_from_camp ("trackerTrajectoryOrphanHits",
                                                  event_exporter::EXPORT_TRACKER_TRAJECTORIES,
                                                  bank_version,
                                                  bank_description,
                                                  branch_entry_type::ARRAY_DATA);

            bank_description = "tracker_trajectory_pattern_type";
            bank_export_version<tracker_trajectory_pattern_type>(bank_version);
            _branch_manager_.init_bank_from_camp ("trackerTrajectoryPatterns",
                                                  event_exporter::EXPORT_TRACKER_TRAJECTORIES,
                                                  bank_version,
                                                  bank_description,
                                                  branch_entry_type::ARRAY_DATA);
        }

        return;
      }

      void export_root_event::detach_branches ()
      {
        branch_manager::bi_col_type & bis = _branch_manager_.grab_branch_infos ();
        for (size_t i = 0; i < bis.size (); i++)
          {
            branch_entry_type & bi = *(bis[i]);
            bi.detach_branch ();
          }
        return;
      }

      void export_root_event::setup_tree (TTree * tree_)
      {
        DT_LOG_TRACE (get_logging_priority (), "Entering...");
        if (is_debug())
          {
            DT_LOG_DEBUG (get_logging_priority (), "Export ROOT event");
            print ();
          }
        branch_manager::bi_col_type & bis = _branch_manager_.grab_branch_infos ();
        for (size_t i = 0; i < bis.size (); i++)
          {
            branch_entry_type & bi = *(bis[i]);
            if (bi.is_activated ())
              {
                bi.make_branch (tree_);
              }
          }
        DT_LOG_TRACE (get_logging_priority (), "Exiting.");
        return;
      }

      void export_root_event::print (std::ostream & out_,
                                     const std::string & title_,
                                     const std::string & indent_) const
      {

        if (! title_.empty ())
          {
            out_ << indent_ << title_ << ": \n";
          }
        const branch_manager::bi_col_type & bis = _branch_manager_.get_branch_infos ();
        for (size_t i = 0; i < bis.size (); i++)
          {
            std::string tag = "|-- ";
            std::string tag2 = "|   ";
            if (i+1 == bis.size ())
              {
                tag = "`-- ";
                tag2 = "    ";
              }
            const branch_entry_type & bi = *(bis[i]);
            out_ << indent_ << tag
                 << "Branch '" << bi.get_name () << "'\n";
            std::ostringstream indent_oss;
            indent_oss << indent_ << tag2;
            bi.print (out_, "", indent_oss.str ());
          }
        return;
      }

      void export_root_event::fill_branch_memory (branch_entry_type & branch_info_)
      {
        DT_LOG_TRACE (get_logging_priority (), "Entering...");
        const std::string & bi_name = branch_info_.get_name ();
        if (branch_info_.is_inhibited ())
          {
            DT_LOG_TRACE (get_logging_priority (), "Branch '" << bi_name << " is inhibited ! Exiting.");
            return;
          }
        if (boost::ends_with (bi_name, "@version"))
          {
            DT_LOG_TRACE (get_logging_priority (), "Branch '" << bi_name << "' doesn't need filling ! Exiting.");
            return;
          }
        const camp::Class & event_class = camp::classByName ("export_event");
        const export_event & EE = static_cast<const export_event &>(*this);
        camp::UserObject proxyEE (EE);
        if (boost::ends_with (bi_name, "@size"))
          {
            DT_LOG_TRACE (get_logging_priority (), "Branch '" << bi_name << "' is the size of a data array !");
            camp::Value bankSizeVal = event_class.property(bi_name).get (proxyEE);
            DT_LOG_TRACE (get_logging_priority (), "Set array size '" << bankSizeVal << "'");
            branch_info_.set_branch_value (bankSizeVal);
            DT_LOG_TRACE (get_logging_priority (), "Exiting.");
            return;
          }
        const std::string & branch_parent_name = branch_info_.get_parent_name ();
        DT_THROW_IF (! event_class.hasProperty (branch_parent_name), std::logic_error,
                     "Cannot find parent for branch '" << bi_name
                     << "' as a property named '" << branch_parent_name << "' !");
        if (get_logging_priority () >= datatools::logger::PRIO_TRACE)
          {
            DT_LOG_TRACE (get_logging_priority (), "Go branch '" << bi_name << "'...");
            branch_info_.print (std::clog);
          }
        const bool array = branch_info_.is_array ();
        const camp::Property & propParent = event_class.property(branch_parent_name);
        if (array)
          {
            // Check if the parent property is an empty array :
            if ((dynamic_cast<const camp::ArrayProperty &>(propParent)).size (proxyEE) == 0)
              {
                DT_LOG_TRACE (get_logging_priority (), "Array branch '" << bi_name << "' has no element ! Exiting.");
                return;
              }
          }
        camp::Value parentVal = propParent.get (proxyEE);
        if (get_logging_priority () >= datatools::logger::PRIO_TRACE)
          {
            DT_LOG_TRACE (get_logging_priority (), "Go branch '" << bi_name << "'...");
            branch_info_.print (std::clog);
          }
        camp::UserObject proxyParent = parentVal.to<camp::UserObject>();
        if (array)
          {
            // Array data :
            DT_LOG_TRACE (get_logging_priority (), "This is the array branch '" << bi_name << "'...");
            const std::string & propName = branch_info_.get_leaf_name ();
            DT_LOG_TRACE (get_logging_priority (), "Property name : '" << propName << "'");
            const camp::Class & parent_class = proxyParent.getClass();
            DT_LOG_TRACE (get_logging_priority (), "Parent class name is '" << parent_class.name () << "'.");

            unsigned int array_size = 0;
            if (branch_info_.is_array_fixed_size ())
              {
                array_size = branch_info_.get_array_fixed_size ();
              }
            else
              {
                const std::string & size_prop_name = branch_info_.get_array_size_name ();
                std::ostringstream getter_func_oss;
                getter_func_oss << branch_parent_name << "@get";
                std::string getter_func_name = getter_func_oss.str ();
                DT_THROW_IF (! event_class.hasProperty (size_prop_name), std::logic_error,
                             "Cannot find size leaf named '" << size_prop_name << "' for branch '" << bi_name
                             << "' as a property of class '" << event_class.name () << "' !");
                camp::Value arraySizeVal = event_class.property(size_prop_name).get (proxyEE);
                DT_LOG_TRACE (get_logging_priority (), "Array size '" << arraySizeVal << "'.");
                array_size = arraySizeVal.to<unsigned int> ();
                for (size_t i = 0; i < array_size; i++)
                  {
                    DT_THROW_IF (! event_class.hasFunction (getter_func_name), std::logic_error,
                                 "Cannot find getter function named '" << getter_func_name << "' for branch '" << bi_name
                                 << "' as a function of class '" << event_class.name () << "' !");
                    const camp::Function & getterFunc = event_class.function (getter_func_name);
                    camp::Value objVal =  getterFunc.call (proxyEE, camp::Args (i));
                    camp::UserObject obj = objVal.to<camp::UserObject>();
                    camp::Value leafVal = parent_class.property(propName).get (obj);
                    DT_LOG_TRACE (get_logging_priority (),
                                  "Branch '" << bi_name << "' : setting array [" << i << "] value ("
                                  << leafVal << ")");
                    branch_info_.set_branch_value (leafVal, i);
                  }
              }
            DT_LOG_TRACE (get_logging_priority (), "Array size is : '" << array_size << "'.");
          }
        else
          {
            // Non-array data :
            DT_LOG_TRACE (get_logging_priority (), "Not an array branch...");
            const std::string & propName = branch_info_.get_leaf_name ();
            DT_LOG_TRACE (get_logging_priority (), "Property name : '" << propName << "'");
            const camp::Class & parent_class = proxyParent.getClass();
            DT_THROW_IF (! parent_class.hasProperty (propName), std::logic_error,
                         "Cannot find leaf named '" << propName << "' for branch '" << bi_name
                         << "' as a property of class '" << parent_class.name () << "' !");;
            DT_LOG_TRACE (get_logging_priority (), "Found property '" << propName << "' in class '" << parent_class.name () << "'.");
            camp::Value leafVal = parent_class.property(propName).get (proxyParent);
            DT_LOG_TRACE (get_logging_priority (), "Branch '" << bi_name << "' : setting scalar value ("
                          << leafVal << ")");
            branch_info_.set_branch_value (leafVal);
          }

        DT_LOG_TRACE (get_logging_priority (), "Exiting.");
       return;
      }

      void export_root_event::fill_memory ()
      {
        //const camp::Class & event_class = camp::classByName ("export_event");

        const export_event & EE = static_cast<const export_event &>(*this);
        camp::UserObject proxyEE (EE);

        // Loop on branch infos:
        for (branch_manager::bi_col_type::iterator bi_iter
               = _branch_manager_.grab_branch_infos ().begin ();
             bi_iter != _branch_manager_.grab_branch_infos ().end ();
             bi_iter++)
          {
            branch_entry_type & bi = **bi_iter;
            DT_LOG_TRACE (get_logging_priority (), "Branch name = " << bi.get_name ());
            // Check if this branch is stored :
            if (! (bi.get_store_bit () & _store_bits_))
              {
                DT_LOG_TRACE (get_logging_priority (), "Branch '" << bi.get_name () << "' is not stored !");
                continue;
              }
            fill_branch_memory (bi);
          }
        return;
      }

    }  // end of namespace exports

  }  // end of namespace reconstruction

}  // end of namespace snemo

/* end of export_root_event.cc */
