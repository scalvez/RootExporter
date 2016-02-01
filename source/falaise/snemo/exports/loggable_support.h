/* loggable_support.h
 * Author(s)     : Francois Mauger <mauger@lpccaen.in2p3.fr>
 * Creation date : 2013-06-03
 * Last modified : 2013-06-03
 */

#ifndef SNRECONSTRUCTION_EXPORTS_LOGGABLE_SUPPORT_H_
#define SNRECONSTRUCTION_EXPORTS_LOGGABLE_SUPPORT_H_ 1

#include <datatools/logger.h>

namespace datatools {
  class properties;
}

namespace snemo {

  namespace reconstruction {

    namespace exports {

      class loggable_support {

      public:

        loggable_support();

        explicit loggable_support(datatools::logger::priority);

        datatools::logger::priority get_logging_priority() const;

        void set_logging_priority(const std::string & priority_label_);

        void set_logging_priority(datatools::logger::priority);

        bool is_debug () const;

        void set_debug (bool);

        bool is_verbose () const;

        void set_verbose (bool);

      protected:

        void _initialize_logging_support(const datatools::properties &);

      protected:

        datatools::logger::priority _logging_priority; //!< Logging priority threshold

      };

    } // end of namespace exports

  } // end of namespace reconstruction

} // end of namespace snemo

#endif //SNRECONSTRUCTION_EXPORTS_LOGGABLE_SUPPORT_H_

// end of track_history.h
/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** End: --
*/
