// -*- mode: c++ ; -*-
/* export_root_event.h
 *
 * Author (s) :     Francois Mauger <mauger@lpccaen.in2p3.fr>
 * Creation date: 2012-11-09
 * Last modified: 2012-11-09
 *
 * License:
 *
 * Description:
 *
 *   Module for exporting calibrated Geiger hits in ROOT file(s)
 *
 * History:
 *
 */

#ifndef SNRECONSTRUCTION_EXPORTS_EXPORT_ROOT_EVENT_H
#define SNRECONSTRUCTION_EXPORTS_EXPORT_ROOT_EVENT_H 1

#include <iostream>

#include <snreconstruction/exports/export_event.h>
#include <snreconstruction/exports/root_utils.h>
#include <snreconstruction/exports/loggable_support.h>

class TTree;

namespace snemo {

  namespace reconstruction {

    namespace exports {

      struct export_root_event : public export_event,
                                 public loggable_support
      {
      public:

        /// Default constructor
        export_root_event ();

        /// Destructor
        virtual ~export_root_event ();

        /// Construct the ROOT tree branch structure
        void construct (unsigned int store_bits_,
                        const std::map<std::string,int> topics_,
                        unsigned int store_version_ = 0);

        /// Setup a ROOT tree from the internal structure of branches
        void setup_tree (TTree * tree_);

        /// Detach ROOT tree branches
        void detach_branches ();

        /// Fill all branches
        void fill_memory ();

        /// Fill the memory associated to a given branch
        void fill_branch_memory (branch_entry_type &);

        /// Smart print
        void print (std::ostream & out_ = std::clog,
                    const std::string & title_ = "",
                    const std::string & indent_ = "") const;

      private:
        uint32_t       _store_bits_; /// Store bits
        branch_manager _branch_manager_; /// Branch manager

      };

    } // end of namespace exports

  } // end of namespace reconstruction

} // end of namespace snemo

#endif // SNRECONSTRUCTION_EXPORTS_EXPORT_ROOT_EVENT_H

// end of export_root_event.h
