// -*- mode: c++ ; -*-
/* export_ascii_module.h
 * Author (s) :     Francois Mauger <mauger@lpccaen.in2p3.fr>
 * Creation date: 2012-11-09
 * Last modified: 2012-11-09
 *
 * License:
 *
 * Description:
 *
 *   Module for exporting calibrated Geiger hits in ASCII file(s)
 *
 * History:
 *
 */

#ifndef SNRECONSTRUCTION_PROCESSING_EXPORT_ASCII_MODULE_H_
#define SNRECONSTRUCTION_PROCESSING_EXPORT_ASCII_MODULE_H_ 1

#include <string>
#include <fstream>

#include <boost/scoped_ptr.hpp>

#include <dpp/base_module.h>

#include <snreconstruction/exports/event_exporter.h>

#include <datatools/smart_filename.h>

namespace snemo {

  namespace reconstruction {

    namespace exports {
      class export_ascii_event;
    }

    namespace processing {

      class export_ascii_module : public dpp::base_module
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
        export_ascii_module(datatools::logger::priority = datatools::logger::PRIO_FATAL);

        /// Destructor
        virtual ~export_ascii_module();

        /// Initialization
        virtual void initialize(const datatools::properties  & setup_,
                                datatools::service_manager   & service_manager_,
                                dpp::module_handle_dict_type & module_dict_);

        /// Reset
        virtual void reset();

        /// Data record processing
        virtual process_status process(datatools::things & data_);

      protected:

        process_status _store_ascii (const datatools::things & data_);

        /// Give default values to specific class members
        void _set_defaults ();

      private:

        exports::event_exporter   _exporter_;        //!< The exporter
        datatools::smart_filename _ascii_filenames_; //!< Filenames

        boost::scoped_ptr<exports::export_ascii_event> _ascii_event_;
        boost::scoped_ptr<std::ofstream>               _ascii_sink_;
        io_accounting_type                             _io_accounting_;

        // Macro to automate the registration of the module :
        DPP_MODULE_REGISTRATION_INTERFACE(export_ascii_module);

      };

    } // end of namespace processing

  } // end of namespace reconstruction

} // end of namespace snemo

#endif // SNRECONSTRUCTION_PROCESSING_EXPORT_ASCII_MODULE_H_

// end of export_ascii_module.h
