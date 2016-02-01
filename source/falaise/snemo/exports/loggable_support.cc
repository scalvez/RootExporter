// -*- mode: c++ ; -*-
/* loggable_support.cc
 */

#include <falaise/snemo/exports/loggable_support.h>

#include <string>

#include <datatools/properties.h>
#include <datatools/exception.h>

namespace snemo {

  namespace reconstruction {

    namespace exports {

      loggable_support::loggable_support()
      {
        _logging_priority = datatools::logger::PRIO_WARNING;
      }

      loggable_support::loggable_support(datatools::logger::priority p)
      {
        _logging_priority = p;
      }

      datatools::logger::priority loggable_support::get_logging_priority() const
      {
        return _logging_priority;
      }

      void loggable_support::set_debug (bool debug_)
      {
        if (debug_) {
          if (_logging_priority < datatools::logger::PRIO_DEBUG) {
            set_logging_priority(datatools::logger::PRIO_DEBUG);
          }
        } else {
          set_logging_priority(datatools::logger::PRIO_WARNING);
        }
      }

      bool loggable_support::is_debug () const
      {
        return _logging_priority >= datatools::logger::PRIO_DEBUG;
      }

      void loggable_support::set_verbose (bool verbose_)
      {
        if (verbose_) {
          if (_logging_priority < datatools::logger::PRIO_NOTICE) {
            set_logging_priority(datatools::logger::PRIO_NOTICE);
          }
        } else {
          set_logging_priority(datatools::logger::PRIO_WARNING);
        }
      }

      bool loggable_support::is_verbose () const
      {
        return _logging_priority >= datatools::logger::PRIO_NOTICE;
      }

      void loggable_support::set_logging_priority(const std::string & priority_label_)
      {
        datatools::logger::priority p = datatools::logger::get_priority(priority_label_);
        DT_THROW_IF(p == datatools::logger::PRIO_UNDEFINED,
                    std::logic_error,
                    "Invalid logging priority label '" << priority_label_ << "' !");
        set_logging_priority(p);
      }

      void loggable_support::set_logging_priority(datatools::logger::priority p)
      {
        _logging_priority = p;
      }

      void loggable_support::_initialize_logging_support(const datatools::properties & config_)
      {
        set_logging_priority(datatools::logger::extract_logging_configuration(config_,
                                                                              datatools::logger::PRIO_WARNING));
        return;
      }

    }// end of namespace exports

  } // end of namespace reconstruction

} // end of namespace snemo

  // end of loggable_support.cc
