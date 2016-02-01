// -*- mode: c++ ; -*-
/* root_utils.h 
 *
 * Author (s) :     Francois Mauger <mauger@lpccaen.in2p3.fr>
 * Creation date: 2012-12-03
 * Last modified: 2012-12-03
 *
 * License:
 *
 * Description:
 *
 *   Root utils
 *
 * History:
 *
 */

#ifndef SNRECONSTRUCTION_EXPORTS_ROOT_UTILS_H
#define SNRECONSTRUCTION_EXPORTS_ROOT_UTILS_H 1

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <boost/cstdint.hpp>
#include <camp/type.hpp>
#include <camp/value.hpp>

#include <Rtypes.h>

class TTree;
class TBranch;

namespace snemo {

  namespace reconstruction {

    namespace exports {

      struct branch_entry_type
      {
        enum branch_type
          {
            TYPE_UNDEFINED = 0,
            TYPE_BOOLEAN = 1,
            TYPE_CHAR    = 2,
            TYPE_UCHAR   = 3,
            TYPE_INT16   = 4,
            TYPE_UINT16  = 5,
            TYPE_INT32   = 6,
            TYPE_UINT32  = 7,
            TYPE_INT64   = 8,
            TYPE_UINT64  = 9,
            TYPE_FLOAT   = 10,
            TYPE_DOUBLE  = 11
          };

        static const bool SCALAR_DATA = false;
        static const bool ARRAY_DATA = true;
        static const unsigned int ARRAY_NO_FIXED_SIZE = 0;
        static const unsigned int DEFAULT_BUFFER_SIZE = 32000;
    
        static int get_branch_type_from_label (const std::string & label_);

        static char get_leaf_type_symbol (int); 

        static std::string get_leaf_type_name (int, bool); 
 
        branch_entry_type ();
 
        ~branch_entry_type ();

        branch_entry_type & set_name (const std::string & name_);

        branch_entry_type & set_parent_name (const std::string & parent_name_);

        branch_entry_type & set_leaf_name (const std::string & leaf_name_);

        branch_entry_type & set_title (const std::string & title_);

        branch_entry_type & set_unit (const std::string & unit_);

        branch_entry_type & set_topic (const std::string & topic_);

        branch_entry_type & set_array_size_name (const std::string & name_);

        branch_entry_type & set_type (int type_);

        branch_entry_type & set_array (bool array_);
 
        branch_entry_type & set_array_fixed_size (unsigned int fixed_size_ = 0);

        branch_entry_type & set_buffer_size (unsigned int buffer_size_);

        branch_entry_type & set (const std::string & name_,
                                 int type_,
                                 bool array_,
                                 unsigned int buffer_size_ = DEFAULT_BUFFER_SIZE );

        unsigned int get_store_bit () const;

        branch_entry_type & set_store_bit (unsigned int store_bit_); 

        void set_inhibit (bool);

        bool is_inhibited () const;

        bool is_activated () const;

        void lock ();

        void unlock ();

        bool is_locked () const;

        void reset ();

        bool is_valid () const;

        bool is_branch_created () const;

        bool is_array_fixed_size () const;

        bool has_fixed_size () const;
 
        const std::string & get_name () const;
 
        const std::string & get_parent_name () const;
 
        const std::string & get_leaf_name () const;
 
        const std::string & get_title () const;
 
        const std::string & get_topic () const;

        const std::string & get_unit () const;

        const std::string & get_array_size_name () const;

        int get_type () const;

        bool is_array () const;

        unsigned int get_buffer_size () const;

        unsigned int get_array_fixed_size () const;
       
        void set_size (unsigned int size_ = 0);

        void * get_address ();

        TBranch * make_branch (TTree * tree_, std::string size_name_ = ""); 

        void detach_branch ();

        void print (std::ostream & out_ = std::clog, 
                    const std::string & title_ = "",
                    const std::string & indent_ = "") const;

        void set_branch_value (const camp::Value & camp_value_, 
                               unsigned int rank_ = 0);
        
      protected:
        
        void _set_size (unsigned int size_);
        void _compute_address ();
        void _update_branch_address ();
        template<class T>
        void _set_value_in_vector (std::vector<T> & v_, 
                                   const T & value_address_, 
                                   unsigned int rank_);
 
      public:

        bool         _inhibit_; /// Inhibition flag
        bool         _locked_; /// Lock flag
        std::string  _name_;   /// Branch full name
        std::string  _parent_name_; /// Name of the parent object
        std::string  _leaf_name_; /// Name of the leaf branch
        std::string  _title_;  /// Branch title
        std::string  _unit_;  /// Branch unit
        std::string  _topic_;  /// Branch topic
        unsigned int _store_bit_; /// Store bit
        int          _type_;   /// Data type
        bool         _array_;  /// Array flag (implemented as a scalar or a std::vector<>)
        unsigned int _buffer_size_;        /// Branch buffer size
        unsigned int _array_fixed_size_;   /// Branch array's fixed size
        std::string  _array_size_name_;    /// The name of the branch that defined the size of an array
        std::vector<UChar_t>   _bvalues_;  /// Boolean value storage (as unsigned chars)
        std::vector<Char_t>    _cvalues_;  /// Char value storage
        std::vector<UChar_t>   _ucvalues_; /// Unsigned char value storage
        std::vector<Short_t>   _svalues_;  /// Short value storage
        std::vector<UShort_t>  _usvalues_; /// Unsigned short value storage
        std::vector<Int_t>     _ivalues_;  /// Int value storage
        std::vector<UInt_t>    _uivalues_; /// Unsigned int value storage
        std::vector<Long64_t>  _lvalues_;  /// Long value storage
        std::vector<ULong64_t> _ulvalues_; /// Unsigned long value storage
        std::vector<Float_t>   _fvalues_;  /// Float value storage
        std::vector<Double_t>  _dvalues_;  /// Double value storage
        void * _address_;   /// The current address to storage
        TBranch * _branch_; /// The current associated branch
      };

      struct branch_manager
      {
      public:
        typedef std::vector<branch_entry_type *> bi_col_type;
        static bool check_camp_type (const std::string & branch_name_,
                                     camp::Type camp_type_,
                                     const std::string & ctype_);
        branch_manager ();
        ~branch_manager ();
        const bi_col_type & get_branch_infos () const;
        bi_col_type & grab_branch_infos ();        
        branch_entry_type & add_branch_entry (const std::string & name_,
                                              int type_,
                                              bool array_);
        void init_bank_from_camp (const std::string & bank_name_,
                                  unsigned int store_bit_,
                                  int32_t bank_version_,
                                  const std::string & camp_class_id_,
                                  bool array_ = branch_entry_type::SCALAR_DATA,
                                  const std::string & branch_array_size_name_ = "");
        void reset ();
        bool is_debug () const;
        void set_debug (bool);
        bool has_branch (const std::string & branch_name_) const;
        branch_entry_type & grab_branch (const std::string & branch_name_);
        void add_topic (const std::string & topic_label_, int activity_level_ = 1);
        bool is_active_topic (const std::string & topic_label_) const;
      protected:
        bool _debug_;
        bi_col_type _branch_infos_;
        std::map<std::string,branch_entry_type*> _branch_dict_; 
        std::map<std::string, bool>              _active_topics_;
      };

    } // end of namespace exports

  } // end of namespace reconstruction

} // end of namespace snemo

#endif // SNRECONSTRUCTION_EXPORTS_ROOT_UTILS_H

// end of root_utils.h
