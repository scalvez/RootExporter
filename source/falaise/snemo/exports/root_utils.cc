// -*- mode: c++ ; -*-
/* root_utils.cc */

#include <falaise/snemo/exports/root_utils.h>

#include <sstream>
#include <stdexcept>
#include <limits>
#include <typeinfo>

#include <TTree.h>
#include <TBranch.h>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <camp/class.hpp>

#include <datatools/exception.h>
#include <datatools/logger.h>

namespace snemo {

  namespace reconstruction {

    namespace exports {

      void branch_entry_type::set_inhibit (bool i_)
      {
        _inhibit_ = i_;
      }

      bool branch_entry_type::is_inhibited () const
      {
        return _inhibit_;
      }

      bool branch_entry_type::is_activated () const
      {
        return ! is_inhibited ();
      }

      void branch_entry_type::lock ()
      {
        _locked_ = true;
        return;
      }

      void branch_entry_type::unlock ()
      {
        _locked_ = false;
        return;
      }

      bool branch_entry_type::is_locked () const
      {
        return _locked_;
      }

      // static
      int branch_entry_type::get_branch_type_from_label (const std::string & label_)
      {
        if (label_ == "bool")
          {
            return TYPE_BOOLEAN;
          }
        if (label_ == "int8_t" || label_ == "char")
          {
            return TYPE_CHAR;
          }
        if (label_ == "uint8_t" || label_ == "uchar")
          {
            return TYPE_UCHAR;
          }
        if (label_ == "int16_t")
          {
            return TYPE_INT16;
          }
        if (label_ == "uint16_t")
          {
            return TYPE_UINT16;
          }
        if (label_ == "int32_t")
          {
            return TYPE_INT32;
          }
        if (label_ == "uint32_t")
          {
            return TYPE_UINT32;
          }
        if (label_ == "int64_t")
          {
            return TYPE_INT64;
          }
        if (label_ == "uint64_t")
          {
            return TYPE_UINT64;
          }
        if (label_ == "float")
          {
            return TYPE_FLOAT;
          }
        if (label_ == "double")
          {
            return TYPE_DOUBLE;
          }
        return TYPE_UNDEFINED;
      }


      // static
      char branch_entry_type::get_leaf_type_symbol (int bt_)
      {
        switch (bt_)
          {
          case TYPE_BOOLEAN : return 'b';
          case TYPE_CHAR    : return 'B';
          case TYPE_UCHAR   : return 'b';
          case TYPE_INT16   : return 'S';
          case TYPE_UINT16  : return 's';
          case TYPE_INT32   : return 'I';
          case TYPE_UINT32  : return 'i';
          case TYPE_INT64   : return 'L';
          case TYPE_UINT64  : return 'l';
          case TYPE_FLOAT   : return 'F';
          case TYPE_DOUBLE  : return 'D';
          }
        return 0;
      }

      // static
      std::string branch_entry_type::get_leaf_type_name (int bt_, bool array_)
      {
        std::string ltn;
        switch (bt_)
          {
          case TYPE_BOOLEAN : ltn = "Bool_t"; break;
          case TYPE_CHAR    : ltn = "Char_t"; break;
          case TYPE_UCHAR   : ltn = "UChar_t"; break;
          case TYPE_INT16   : ltn = "Short_t"; break;
          case TYPE_UINT16  : ltn = "UShort_t"; break;
          case TYPE_INT32   : ltn = "Int_t"; break;
          case TYPE_UINT32  : ltn = "UInt_t"; break;
          case TYPE_INT64   : ltn = "Long64_t"; break;
          case TYPE_UINT64  : ltn = "ULong64_t"; break;
          case TYPE_FLOAT   : ltn = "Float_t"; break;
          case TYPE_DOUBLE  : ltn = "Double_t"; break;
          }
        return ltn;
      }

      branch_entry_type::branch_entry_type()
      {
        _inhibit_ = false;
        _locked_ = false;
        _name_ = "";
        _parent_name_ = "";
        _leaf_name_ = "";
        _title_ = "";
        _unit_ = "";
        _topic_ = "";

        _store_bit_ = 0;
        _type_ = TYPE_UNDEFINED;
        _array_ = false;
        _buffer_size_ = DEFAULT_BUFFER_SIZE;
        _array_fixed_size_ = 0;
        _array_size_name_ = "";
        _address_ = 0;
        _branch_ = 0;
        return;
      }

      branch_entry_type::~branch_entry_type()
      {
        reset ();
        return;
      }

      void branch_entry_type::reset ()
      {
        detach_branch ();
        _inhibit_ = false;
        _locked_ = false;
        _name_ = "";
        _parent_name_ = "";
        _leaf_name_ = "";
        _title_ = "";
        _unit_ = "";
        _topic_ = "";

        _store_bit_ = 0;
        _type_ = TYPE_UNDEFINED;
        _array_ = false;
        _buffer_size_ = DEFAULT_BUFFER_SIZE;
        _array_fixed_size_ = 0;
        _array_size_name_ = "";
        _address_ = 0;
        _branch_ = 0;
        _bvalues_.clear ();
        _cvalues_.clear ();
        _ucvalues_.clear ();
        _svalues_.clear ();
        _usvalues_.clear ();
        _ivalues_.clear ();
        _uivalues_.clear ();
        _lvalues_.clear ();
        _ulvalues_.clear ();
        _fvalues_.clear ();
        _dvalues_.clear ();
        return;
      }

      bool branch_entry_type::is_branch_created () const
      {
        return _branch_ != 0;
      }

      bool branch_entry_type::is_valid () const
      {
        return !_name_.empty ()
          && _type_ != TYPE_UNDEFINED;
      }

      branch_entry_type & branch_entry_type::set_name (const std::string & name_)
      {
        DT_THROW_IF (name_.empty (), std::logic_error, "Empty name is not allowed !");
        _name_ = name_;
        return *this;
      }

      branch_entry_type & branch_entry_type::set_parent_name (const std::string & parent_name_)
      {
        DT_THROW_IF (parent_name_.empty (), std::logic_error, "Empty parent name is not allowed !");
        _parent_name_ = parent_name_;
        return *this;
      }

      branch_entry_type & branch_entry_type::set_leaf_name (const std::string & leaf_name_)
      {
        DT_THROW_IF (leaf_name_.empty (), std::logic_error, "Empty leaf name is not allowed !");
        _leaf_name_ = leaf_name_;
        return *this;
      }

      branch_entry_type & branch_entry_type::set_title (const std::string & title_)
      {
        _title_ = title_;
        return *this;
      }

      branch_entry_type & branch_entry_type::set_unit (const std::string & unit_)
      {
        _unit_ = unit_;
        return *this;
      }

      branch_entry_type & branch_entry_type::set_topic (const std::string & topic_)
      {
        _topic_ = topic_;
        return *this;
      }

      unsigned int branch_entry_type::get_store_bit () const
      {
        return _store_bit_;
      }

      branch_entry_type &
      branch_entry_type::set_store_bit (unsigned int store_bit_)
      {
        _store_bit_ = store_bit_;
        return *this;
      }

      branch_entry_type & branch_entry_type::set_array_size_name (const std::string & array_size_name_)
      {
        DT_THROW_IF (array_size_name_.empty (), std::logic_error, "Empty size name is not allowed !");
        _array_size_name_ = array_size_name_;
        return *this;
      }

      void branch_entry_type::print (std::ostream & out_,
                                     const std::string & title_,
                                     const std::string & indent_) const
      {
        if (!title_.empty ())
          {
            out_ << indent_ << title_ << " : " << std::endl;
          }
        out_ << indent_ << "|-- " << "Valid       : '" << is_valid () << "'" << std::endl;
        out_ << indent_ << "|-- " << "Activated   : '" << is_activated () << "'" << std::endl;
        out_ << indent_ << "|-- " << "Name        : '" << _name_ << "'" << std::endl;
        out_ << indent_ << "|-- " << "Parent name : '" << _parent_name_ << "'" << std::endl;
        out_ << indent_ << "|-- " << "Leaf name   : '" << _leaf_name_ << "'" << std::endl;
        out_ << indent_ << "|-- " << "Title       : '" << _title_ << "'" << std::endl;
        out_ << indent_ << "|-- " << "Unit        : '" << _unit_ << "'" << std::endl;
        out_ << indent_ << "|-- " << "Topic       : '" << _topic_ << "'" << std::endl;
        out_ << indent_ << "|-- " << "Store bit   : " << _store_bit_ << std::endl;
        out_ << indent_ << "|-- " << "Type        : '" << branch_entry_type::get_leaf_type_name (_type_, false) << "'" << std::endl;
        out_ << indent_ << "|-- " << "Array       : '" << (_array_ ? "Yes" : "No") << "'" << std::endl;
        out_ << indent_ << "|-- " << "Buffer size : " << _buffer_size_ << std::endl;
        if (_array_)
          {
            if (_array_fixed_size_ > 0)
              {
                out_ << indent_ << "|-- " << "Array fixed size : " << _array_fixed_size_ << std::endl;
              }
            else
              {
                out_ << indent_ << "|-- " << "Array size name  : '" << _array_size_name_ << "'" << std::endl;
              }
          }
        out_ << indent_ << "|-- " << "Address     : " << _address_ << std::endl;
        out_ << indent_ << "|-- " << "Values      : " << std::endl;
        out_ << indent_ << "`-- " << "Branch      : " << _branch_ << std::endl;
        return;
      }

      branch_entry_type & branch_entry_type::set_type (int type_)
      {
        switch (type_)
          {
          case TYPE_BOOLEAN :
            _type_ = type_;
            break;
          case TYPE_CHAR :
            _type_ = type_;
            break;
          case TYPE_UCHAR :
            _type_ = type_;
            break;
          case TYPE_INT16 :
            _type_ = type_;
            break;
          case TYPE_UINT16 :
            _type_ = type_;
            break;
          case TYPE_INT32 :
            _type_ = type_;
            break;
          case TYPE_UINT32 :
            _type_ = type_;
            break;
          case TYPE_INT64 :
            _type_ = type_;
            break;
          case TYPE_UINT64 :
            _type_ = type_;
            break;
          case TYPE_FLOAT :
            _type_ = type_;
            break;
          case TYPE_DOUBLE :
            _type_ = type_;
            break;
          default:
            _type_ = TYPE_UNDEFINED;
            DT_THROW_IF (true, std::logic_error, "Type '" << type_ << "' is not allowed !");
          }
        _type_ = type_;
        return *this;
      }

      branch_entry_type & branch_entry_type::set_array (bool array_)
      {
        _array_ = array_;
        if (! _array_)
          {
            _set_size (1);
          }
        return *this;
      }

      branch_entry_type & branch_entry_type::set_array_fixed_size (unsigned int fixed_size_ )
      {
        set_array (true);
        if (fixed_size_ > 0)
          {
            _array_fixed_size_ = fixed_size_;
          }
        return *this;
      }

      bool branch_entry_type::has_fixed_size () const
      {
        if (! _array_) return true; // scalar data has fixed size==1
        return _array_fixed_size_ != ARRAY_NO_FIXED_SIZE;
      }

      unsigned int branch_entry_type::get_array_fixed_size () const
      {
        return _array_fixed_size_;
      }

      branch_entry_type & branch_entry_type::set_buffer_size (unsigned int buffer_size_)
      {
        _buffer_size_ = buffer_size_;
        return *this;
      }

      const std::string & branch_entry_type::get_name () const
      {
        return _name_;
      }

      const std::string & branch_entry_type::get_parent_name () const
      {
        return _parent_name_;
      }

      const std::string & branch_entry_type::get_leaf_name () const
      {
        return _leaf_name_;
      }

      const std::string & branch_entry_type::get_title () const
      {
        return _title_;
      }

      const std::string & branch_entry_type::get_topic () const
      {
        return _topic_;
      }

      const std::string & branch_entry_type::get_unit () const
      {
        return _unit_;
      }

      const std::string & branch_entry_type::get_array_size_name () const
      {
        return _array_size_name_;
      }

      int branch_entry_type::get_type () const
      {
        return _type_;
      }

      bool branch_entry_type::is_array () const
      {
        return _array_;
      }

      unsigned int branch_entry_type::get_buffer_size () const
      {
        return _buffer_size_;
      }

      branch_entry_type &
      branch_entry_type::set (const std::string & name_,
                              int type_,
                              bool array_,
                              unsigned int buffer_size_)
      {
        set_name (name_);
        set_type (type_);
        set_array (array_);
        set_buffer_size (buffer_size_);
        return *this;
      }

      template<class T>
      void branch_entry_type::_set_value_in_vector (std::vector<T> & v_,
                                                    const T & value_address_,
                                                    unsigned int rank_)
      {
        if (rank_ >= v_.size ())
          {
            T * current_addr = v_.data();
            v_.resize (rank_ + 1);
            if (v_.data() != current_addr)
              {
                /*
                std::clog << "_set_value_in_vector: "
                          << "New address for vector of type '" << typeid(T).name () << "'."
                          << "\n";
                */
                this->_compute_address ();
              }
          }
        v_[rank_] = value_address_;
        /*
        if(sizeof(T) == 4 && typeid(T) == typeid(int32_t))
          {
            std::clog << "************** v_[size=" << v_.size () << "] : add "
                      << value_address_ << " at rank " << rank_ << std::endl;
          for (int i = 0; i <= rank_; i++)
          {
          std::clog << "  Contents:  v_[" << i << "] = ";
          if (sizeof (v_[i]) == 1 )
          {
          std::clog << (int) v_[i];
          }
          else
          {
          std::clog << v_[i];
          }
          std::clog << std::endl;
          }
          }
        */
        return;
      }

      bool branch_entry_type::is_array_fixed_size () const
      {
        return _array_fixed_size_ != ARRAY_NO_FIXED_SIZE;
      }

      void branch_entry_type::set_branch_value (const camp::Value & camp_value_,
                                                unsigned int rank_)
      {
        DT_THROW_IF (is_inhibited (), std::logic_error, "Branch entry is inhibited ! Cannot set value !");
        DT_THROW_IF (! is_locked (), std::logic_error, "Branch entry '" << get_name () << "' is not locked ! Cannot set value !");
        DT_THROW_IF (! _array_ && rank_ > 0, std::logic_error, "Rank > 0 (" << rank_ << ") is not allowed for scalar value !");
        if (_array_ && is_array_fixed_size ())
          {
            DT_THROW_IF (rank_ >= _array_fixed_size_, std::logic_error,
                         "Array rank overflow (" << rank_ << ">=" << _array_fixed_size_
                         << ") is not allowed for branch '" << get_name () << " !");
          }
        switch (_type_)
          {
          case TYPE_BOOLEAN :
            {
              bool val = camp_value_.to<bool>();
              UChar_t boolchar = val ? 1 : 0;
              _set_value_in_vector<UChar_t> (_bvalues_, boolchar, rank_);
            }
            break;
          case TYPE_CHAR :
            {
              Char_t val = camp_value_.to<Char_t>();
              _set_value_in_vector<Char_t> (_cvalues_, val, rank_);
            }
            break;
          case TYPE_UCHAR :
            {
              UChar_t val = camp_value_.to<UChar_t>();
              _set_value_in_vector<UChar_t> (_ucvalues_, val, rank_);
            }
            break;
          case TYPE_INT16 :
            {
              Short_t val = camp_value_.to<Short_t>();
              _set_value_in_vector<Short_t> (_svalues_, val, rank_);
            }
            break;
          case TYPE_UINT16 :
            {
              UShort_t val = camp_value_.to<UShort_t>();
              _set_value_in_vector<UShort_t> (_usvalues_, val, rank_);
            }
            break;
          case TYPE_INT32 :
            {
              Int_t val = camp_value_.to<Int_t>();
              _set_value_in_vector<Int_t> (_ivalues_, val, rank_);
            }
            break;
          case TYPE_UINT32 :
            {
              UInt_t val = camp_value_.to<UInt_t>();
              _set_value_in_vector<UInt_t> (_uivalues_, val, rank_);
            }
            break;
          case TYPE_INT64 :
            {
              Long64_t val = camp_value_.to<Long64_t>();
              _set_value_in_vector<Long64_t> (_lvalues_, val, rank_);
            }
            break;
          case TYPE_UINT64 :
            {
              ULong64_t val = camp_value_.to<ULong64_t>();
              _set_value_in_vector<ULong64_t> (_ulvalues_, val, rank_);
            }
            break;
          case TYPE_FLOAT :
            {
              Float_t val = camp_value_.to<Float_t>();
              _set_value_in_vector<Float_t> (_fvalues_, val, rank_);
            }
            break;
          case TYPE_DOUBLE :
            {
              Double_t val = camp_value_.to<Double_t>();
              _set_value_in_vector<Double_t> (_dvalues_, val, rank_);
            }
            break;
          }
        return;
      }

      void branch_entry_type::set_size (unsigned int size_)
      {
        DT_THROW_IF (!is_locked (), std::logic_error, "Branch entry is not locked ! Cannot set size !");
        DT_THROW_IF (! _array_, std::logic_error, "Branch '" << get_name () << "' is not an array !");
        DT_THROW_IF (has_fixed_size () && (size_ != _array_fixed_size_), std::range_error,
                     "Cannot resize fixed size array data for branch '" << get_name () << "' !");
        _set_size (size_);
        return;
      }

      void branch_entry_type::_set_size (unsigned int size_)
      {
        if (size_ == 0)
          {
            return;
          }
        switch (_type_)
          {
          case TYPE_BOOLEAN :
            if (size_ == 0)
              {
                _bvalues_.clear ();
                break;
              }
            _bvalues_.assign (size_, 0);
            break;
          case TYPE_CHAR :
            if (size_ == 0)
              {
                _cvalues_.clear ();
                break;
              }
            _cvalues_.assign (size_, 0);
            break;
          case TYPE_UCHAR :
            if (size_ == 0)
              {
                _ucvalues_.clear ();
                break;
              }
            _ucvalues_.assign (size_, 0);
            break;
          case TYPE_INT16 :
            if (size_ == 0)
              {
                _svalues_.clear ();
                break;
              }
            _svalues_.assign (size_, -1);
            break;
          case TYPE_UINT16 :
            if (size_ == 0)
              {
                _usvalues_.clear ();
                break;
              }
            _usvalues_.assign (size_, -1);
            break;
          case TYPE_INT32 :
            if (size_ == 0)
              {
                _ivalues_.clear ();
                break;
              }
            _ivalues_.assign (size_, -1);
            break;
          case TYPE_UINT32 :
            if (size_ == 0)
              {
                _uivalues_.clear ();
                break;
              }
            _uivalues_.assign (size_, 0xFFFFFFFF);
            break;
          case TYPE_INT64 :
            if (size_ == 0)
              {
                _lvalues_.clear ();
                break;
              }
            _lvalues_.assign (size_, -1L);
            break;
          case TYPE_UINT64 :
            if (size_ == 0)
              {
                _ulvalues_.clear ();
                break;
              }
            _ulvalues_.assign (size_, 0xFFFFFFFFFFFFFFFF);
            break;
          case TYPE_FLOAT :
            if (size_ == 0)
              {
                _fvalues_.clear ();
                break;
              }
            _fvalues_.assign (size_, std::numeric_limits<float>::quiet_NaN());
            break;
          case TYPE_DOUBLE :
            if (size_ == 0)
              {
                _dvalues_.clear ();
                break;
              }
            _dvalues_.assign (size_, std::numeric_limits<double>::quiet_NaN());
            break;
          }
        _compute_address ();
        return;
      }


      void * branch_entry_type::get_address ()
      {
        if (_address_ == 0)
          {
            _compute_address ();
          }
        return _address_;
      }

      void branch_entry_type::_update_branch_address ()
      {
        if (_branch_ != 0 && _address_ != 0)
          {
            _branch_->SetAddress(_address_);
          }
        return;
      }

      void branch_entry_type::_compute_address ()
      {
        switch (_type_)
          {
          case TYPE_BOOLEAN :
            {
              _bvalues_.reserve (1);
              _address_ = static_cast<void *>(_bvalues_.data ());
            }
            break;
          case TYPE_CHAR :
            {
              _cvalues_.reserve (1);
              _address_ = static_cast<void *>(_cvalues_.data ());
            }
            break;
          case TYPE_UCHAR :
            {
              _ucvalues_.reserve (1);
              _address_ = static_cast<void *>(_ucvalues_.data ());
            }
            break;
          case TYPE_INT16 :
            {
              _svalues_.reserve (1);
              _address_ = static_cast<void *>(_svalues_.data ());
            }
            break;
          case TYPE_UINT16 :
            {
              _usvalues_.reserve (1);
              _address_ = static_cast<void *>(_usvalues_.data ());
            }
            break;
          case TYPE_INT32 :
            {
              _ivalues_.reserve (1);
              _address_ = static_cast<void *>(_ivalues_.data ());
            }
            break;
          case TYPE_UINT32 :
            {
              _uivalues_.reserve (1);
              _address_ = static_cast<void *>(_uivalues_.data ());
            }
            break;
          case TYPE_INT64 :
            {
              _lvalues_.reserve (1);
              _address_ = static_cast<void *>(_lvalues_.data ());
            }
            break;
          case TYPE_UINT64 :
            {
              _ulvalues_.reserve (1);
              _address_ = static_cast<void *>(_ulvalues_.data ());
            }
            break;
          case TYPE_FLOAT :
            {
              _fvalues_.reserve (1);
              _address_ = static_cast<void *>(_fvalues_.data ());
            }
            break;
          case TYPE_DOUBLE :
            {
              _dvalues_.reserve (1);
              _address_ = static_cast<void *>(_dvalues_.data ());
            }
            break;
          }
        _update_branch_address ();
        return;
      }

      void branch_entry_type::detach_branch ()
      {
        _branch_ = 0;
        return;
      }

      TBranch * branch_entry_type::make_branch (TTree * tree_, std::string size_name_)
      {
        DT_THROW_IF (! is_valid (), std::logic_error, "Invalid setup ! Cannot make a branch !");
        DT_THROW_IF (is_inhibited (), std::logic_error, "Branch entry is inhibited ! Cannot make a branch !");
        DT_THROW_IF (! is_locked (), std::logic_error, "Branch entry is not locked ! Cannot make a branch !");
        TBranch * br = 0;
        void * address = get_address ();
        if (! _array_)
          {
            std::ostringstream leaflist;
            leaflist << _name_ << "/" << get_leaf_type_symbol (_type_);
            br = tree_->Branch(_name_.c_str (),
                               address,
                               leaflist.str ().c_str (),
                               _buffer_size_);
          }
        else
          {
            std::string branch_size_name;
            std::ostringstream leaflist;
            leaflist << _name_ << '[';
            if (has_fixed_size ())
              {
                leaflist << _array_fixed_size_;
              }
            else
              {
                if (! size_name_.empty ())
                  {
                    branch_size_name = size_name_;
                  }
                else
                  {
                    branch_size_name = _array_size_name_;
                  }
                DT_THROW_IF (branch_size_name.empty (), std::logic_error,
                             "Missing branch size name ! Cannot make a branch !");
                leaflist << branch_size_name;
              }
            leaflist << ']' << '/' << get_leaf_type_symbol (_type_);
            switch (_type_)
              {
              default :
                br = tree_->Branch(_name_.c_str (),
                                   address,
                                   leaflist.str ().c_str (),
                                   _buffer_size_);
                break;
              }
          }
        std::ostringstream branch_title_oss;
        if (! _title_.empty ())
          {
            branch_title_oss << _title_;
          }
        else
          {
            branch_title_oss << br->GetTitle ();
          }
        if (! _unit_.empty ())
          {
            branch_title_oss << ' ' << '[' << _unit_<< ']';
          }
        br->SetTitle (branch_title_oss.str ().c_str ());
        _branch_ = br;
        return br;
      }

      bool branch_manager::is_debug () const
      {
        return _debug_;
      }

      void branch_manager::set_debug (bool debug_)
      {
        _debug_ = debug_;
        return;
      }

      branch_manager::branch_manager ()
      {
        _debug_ = false;
        return;
      }

      branch_manager::~branch_manager ()
      {
        reset ();
        return;
      }

      const branch_manager::bi_col_type & branch_manager::get_branch_infos () const
      {
        return _branch_infos_;
      }

      branch_manager::bi_col_type & branch_manager::grab_branch_infos ()
      {
        return _branch_infos_;
      }

      branch_entry_type &
      branch_manager::add_branch_entry (const std::string & name_,
                                        int type_,
                                        bool array_)
      {
        DT_THROW_IF (_branch_dict_.find (name_) != _branch_dict_.end(), std::logic_error,
                     "Branch named '" << name_ << "' already exists !");
        _branch_infos_.push_back ( new branch_entry_type);
        branch_entry_type & be = *_branch_infos_.back ();
        be.set (name_, type_, array_);
        _branch_dict_[name_] = &be;
        return be;
      }

      void branch_manager::reset ()
      {
        _branch_dict_.clear ();
        for (size_t i = 0; i < _branch_infos_.size (); i++)
          {
            delete _branch_infos_[i];
          }
        _branch_infos_.clear ();
        _active_topics_.clear ();
        return;
      }

      void branch_manager::add_topic(const std::string & topic_label_, int activity_level_)
      {
        DT_THROW_IF (topic_label_.empty (), std::logic_error, "Empty topic label is not allowed !");
        _active_topics_[topic_label_] = activity_level_;
        return;
      }

      bool branch_manager::is_active_topic (const std::string & topic_label_) const
      {
        std::map<std::string, bool>::const_iterator found = _active_topics_.find (topic_label_);
        if (found == _active_topics_.end()) return false;
        if (found->second == 1) return true;
        return false;
      }

      // static
      bool branch_manager::check_camp_type (const std::string & branch_name_,
                                            camp::Type camp_type_,
                                            const std::string & ctype_)
      {
        if (camp_type_ == camp::boolType)
          {
            return ctype_ == "bool";
          }
        else if (camp_type_ == camp::intType)
          {
            return ctype_ == "int8_t"
              ||  ctype_ == "char"
              ||  ctype_ == "uint8_t"
              ||  ctype_ == "uchar"
              ||  ctype_ == "int16_t"
              ||  ctype_ == "uint16_t"
              ||  ctype_ == "int32_t"
              ||  ctype_ == "uint32_t"
              ||  ctype_ == "int64_t"
              ||  ctype_ == "uint64_t"
              ;
          }
        else if (camp_type_ == camp::realType)
          {
            return ctype_ == "float" || ctype_ == "double";
          }
        return false;
      }

      void branch_manager::init_bank_from_camp (const std::string & bank_name_,
                                                unsigned int store_bit_,
                                                int32_t bank_version_,
                                                const std::string & camp_class_id_,
                                                bool array_,
                                                const std::string & branch_array_size_name_)
      {
        const camp::Class & meta_class = camp::classByName (camp_class_id_);
        std::string branch_array_size_name;
        if (array_)
          {
            if (branch_array_size_name_.empty ())
              {
                std::ostringstream branch_array_size_oss;
                branch_array_size_oss << bank_name_
                                      << "@size";
                branch_array_size_name = branch_array_size_oss.str ();
              }
            else
              {
                branch_array_size_name = branch_array_size_name_;
              }
            branch_entry_type & be_array_size =
              add_branch_entry (branch_array_size_name,
                                branch_entry_type::TYPE_UINT32,
                                branch_entry_type::SCALAR_DATA);
            be_array_size.set_store_bit (store_bit_);
            be_array_size.lock ();
          }
        {
          std::ostringstream branch_version_oss;
          branch_version_oss << bank_name_
                             << "@version";
          std::string branch_version_name = branch_version_oss.str ();
          branch_entry_type & be_version =
            add_branch_entry (branch_version_name,
                              branch_entry_type::TYPE_UINT32,
                              branch_entry_type::SCALAR_DATA);
          be_version.set_store_bit (store_bit_);
          be_version.lock ();
          camp::Value bankVersionVal = bank_version_;
          be_version.set_branch_value (bankVersionVal, 0);
        }

        std::size_t nb_branches = meta_class.propertyCount();

        for (size_t ibranch = 0; ibranch < nb_branches; ibranch++)
          {
            const camp::Property & branch_prop =  meta_class.property(ibranch);
            const std::string & branch_name = branch_prop.name();
            std::ostringstream branch_label_oss;
            branch_label_oss << bank_name_ << '.'
                             << branch_name;
            std::string branch_label = branch_label_oss.str();
            camp::Type branch_camp_type = branch_prop.type();
            std::string branch_type = "int32_t";
            if (branch_prop.hasTag ("ctype"))
              {
                branch_type = branch_prop.tag ("ctype").to<std::string>();
              }
            DT_THROW_IF (! check_camp_type (branch_label, branch_camp_type, branch_type), std::logic_error,
                         "CAMP type " << branch_camp_type << " and C-type '" << branch_type
                         << "' do not match for branch '" << branch_label << "' !");
            std::string branch_unit;
            if (branch_prop.hasTag ("unit"))
              {
                branch_unit = branch_prop.tag ("unit").to<std::string>();
              }
            std::string branch_title;
            if (branch_prop.hasTag ("title"))
              {
                branch_title = branch_prop.tag ("title").to<std::string>();
              }
            std::string branch_topic;
            if (branch_prop.hasTag ("topic"))
              {
                branch_topic = branch_prop.tag ("topic").to<std::string>();
              }

            branch_entry_type & be =
              add_branch_entry (branch_label,
                                branch_entry_type::get_branch_type_from_label(branch_type),
                                array_);
            if (array_)
              {
                be.set_array_size_name (branch_array_size_name);
              }
            if (! branch_unit.empty ())
              {
                be.set_unit (branch_unit);
              }
            if (! branch_title.empty ())
              {
                be.set_title (branch_title);
              }
            if (! branch_topic.empty ())
              {
                be.set_topic (branch_topic);
                if (! is_active_topic(branch_topic))
                  {
                    be.set_inhibit(true);
                  }
              }
            be.set_store_bit (store_bit_);
            be.set_parent_name (bank_name_);
            be.set_leaf_name (branch_name);
            be.lock ();
            if (is_debug ())
              {
                DT_LOG_DEBUG (datatools::logger::PRIO_DEBUG, "Branch entry :");
                be.print (std::clog);
              }
          }
        return;
      }

      bool branch_manager::has_branch (const std::string & branch_name_) const
      {
        return _branch_dict_.find (branch_name_) != _branch_dict_.end ();
      }

      branch_entry_type & branch_manager::grab_branch (const std::string & branch_name_)
      {
        std::map<std::string,branch_entry_type*>::iterator found
          = _branch_dict_.find (branch_name_);
        DT_THROW_IF (found == _branch_dict_.end (), std::logic_error,
                     "Branch named '" << branch_name_ << "' does not exist !");
        return *(found->second);
      }

    }  // end of namespace exports

  }  // end of namespace reconstruction

}  // end of namespace snemo

/* end of root_utils.cc */
