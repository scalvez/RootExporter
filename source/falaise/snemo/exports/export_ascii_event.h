// -*- mode: c++ ; -*-
/* export_ascii_event.h 
 *
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

#ifndef SNRECONSTRUCTION_EXPORTS_EXPORT_ASCII_EVENT_H
#define SNRECONSTRUCTION_EXPORTS_EXPORT_ASCII_EVENT_H 1

#include <iostream>

#include <snreconstruction/exports/export_event.h>

namespace snemo {

  namespace reconstruction {

    namespace exports {

      struct export_ascii_event : public export_event
      {
      public:

        template<class Type>
        void store (std::ostream & out_, const Type & object_, 
                    int version_ = Type::EXPORT_VERSION) const;
 
        void store (std::ostream &, 
                    unsigned int store_bits_ = 0xFFFFFFFF, 
                    unsigned int store_version_ = export_event::EXPORT_VERSION) const;

        export_ascii_event ();

        virtual ~export_ascii_event ();

        static void print_comment_data_tag_begin (std::ostream & out_,
                                                  const std::string & data_tag_);

        static void print_comment_data_tag_end (std::ostream & out_,
                                                const std::string & data_tag_);

        static void print_comment_data_info (std::ostream & out_,
                                             const std::string & data_name_,
                                             const std::string & data_type_,
                                             const std::string & data_layout_,
                                             const std::string & data_size_ = ""
                                             );

      public:
        bool add_comments;
        char separator;
      };

      
      template<class Type>
      void export_ascii_event::store (std::ostream & out_, const Type & object_, 
                                      int version_) const
      {
        const camp::Class & meta_class = camp::classByType< Type > ();
        out_ << version_ << ' ';
        for (int i = 0; i < meta_class.propertyCount(); i++)
          {
            const camp::Property & prop = meta_class.property (i);
            camp::UserObject obj (object_);
            camp::Value val = prop.get (obj);
            out_ << val << ' ';
          }
        return;
        
      }
 
    } // end of namespace exports

  } // end of namespace reconstruction

} // end of namespace snemo

#endif // SNRECONSTRUCTION_EXPORTS_EXPORT_ASCII_EVENT_H

// end of export_ascii_event.h
