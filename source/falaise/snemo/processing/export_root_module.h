// -*- mode: c++ ; -*-
/* export_root_module.h
 * Author (s) :     Francois Mauger <mauger@lpccaen.in2p3.fr>
 * Creation date: 2012-11-15
 * Last modified: 2012-11-15
 *
 * License:
 *
 * Description:
 *
 *   Module for exporting the SuperNEMO event model in ROOT tree/file(s)
 *
 * History:
 *
 */

#ifndef __snreconstruction__processing__export_root_module_h
#define __snreconstruction__processing__export_root_module_h 1

#include <string>
#include <fstream>

#include <boost/scoped_ptr.hpp>

#include <dpp/base_module.h>

#include <snreconstruction/exports/event_exporter.h>

#include <datatools/smart_filename.h>

class TFile;
class TTree;

namespace snemo {

  namespace reconstruction {

    namespace exports {
      class export_root_event;
    }

    namespace processing {

      class export_root_module : public dpp::base_module
      {
      public:

        struct io_accounting_type
        {
          int max_records_per_file; //!< Maximum number of event records per file
          int max_records_total;    //!< Maximum number of event records to be processed
          int max_files;            //!< Maximum number of data files to be processed
          bool terminated;          //!< Termination flag
          int file_record_counter;  //!< Event record counter in the current file
          int record_counter;       //!< Total event record counter
          int file_index;           //!<Index of the current datafile index

          io_accounting_type ();
          void reset ();
        };

        bool is_terminated () const;

        /// Constructor
        export_root_module(datatools::logger::priority = datatools::logger::PRIO_FATAL);

        /// Destructor
        virtual ~export_root_module();

        /// Initialization
        virtual void initialize(const datatools::properties  & setup_,
                                datatools::service_manager   & service_manager_,
                                dpp::module_handle_dict_type & module_dict_);

        /// Reset
        virtual void reset();

        /// Data record processing
        virtual process_status process(datatools::things & data_);

      protected:

        process_status _process_event (const datatools::things & data_);

        process_status _open_file (const std::string & filename_);

        int _init_tree ();

        int _store_event (const datatools::things & data_);

        int _terminate_tree ();

        int _close_file ();

        /// Give default values to specific class members
        void _set_defaults ();

      private:

        exports::event_exporter   _exporter_;       //!< The exporter
        datatools::smart_filename _root_filenames_; //!< Filenames

        boost::scoped_ptr<exports::export_root_event> _root_event_;
        TFile *                                       _root_sink_;
        TTree *                                       _root_tree_;
        io_accounting_type                            _io_accounting_;

        // Macro to automate the registration of the module :
        DPP_MODULE_REGISTRATION_INTERFACE(export_root_module);

      };

    } // end of namespace processing

  } // end of namespace reconstruction

} // end of namespace snemo

#endif // __snreconstruction__processing__export_root_module_h

// end of export_root_module.h
