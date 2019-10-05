/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__ARGUMENT_PARSER_H
#define _DK_SDU_MIP__BASE__ARGUMENT_PARSER_H

#include <base/Object.h>
#include <base/Polymorphic.h>
#include <base/Primitives.h>
#include <base/string/String.h>
#include <base/string/InvalidFormat.h>
#include <base/collection/Array.h>
#include <base/collection/HashTable.h>
#include <base/collection/HashSet.h>
#include <base/collection/AlreadyKeyException.h>
#include <base/OutOfDomain.h>
#include <base/ValidifiedResult.h> // TAG: should be ValidifiedValue?
#include <base/ArgumentException.h>
#include <base/string/RegExp.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Argument parser.
  
  @short Argument parser.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class ArgumentParser : public Object {
public:

  enum Flag {
    ORPHANS = 1 /**< Allow orphans. */
  };
  
  class Option : public ReferenceCountedObject {
  public:
    
    enum Flag {
      MANDATORY = 1, /**< Option is required. */
      STRICT = 2, /**< Option must be occur at most once. */
      DEPRECATED = 4, /**< Option is deprecated. */
      EXPLICIT = 8, /**< Value must be given if option is specified. */
      OVERRIDE = 16 /**< The last occurrence is the only significant. */
    };
  private:

    /** The long name of the option. */
    String name;
    /** The short name of the option. */
    char shortName;
    /** Flags. */
    unsigned int flags;
  public:
    
    Option(const String& name, unsigned int flags = 0) throw(OutOfDomain);
    
    Option(
      const String& name,
      char shortName,
      unsigned int flags = 0) throw(OutOfDomain);
    
    /**
      Returns true if the option has a short name.
    */
    inline bool hasShortName() const throw() {
      return shortName != 0;
    }
    
    /**
      Returns the short name of the option.
    */
    inline char getShortName() const throw() {
      return shortName;
    }
    
    /**
      Returns the long name of the option.
    */
    inline String getName() const throw() {
      return name;
    }
    
    /**
      Returns the flags.
    */
    inline unsigned int getFlags() const throw() {
      return flags;
    }

    virtual ~Option() throw() {
    };
  };
  
  class FlagOption : public Option {
  public:
    
    FlagOption(const String& name, unsigned int flags = 0) throw(OutOfDomain);
    
    FlagOption(
      const String& name,
      char shortName,
      unsigned int flags = 0) throw(OutOfDomain);
  };
  
  class StringOption : public Option {
  private:

    ValidifiedResult<String> defaultValue;
  public:

    StringOption(
      const String& name, unsigned int flags = 0) throw(OutOfDomain);
    
    StringOption(
      const String& name,
      char shortName,
      unsigned int flags = 0) throw(OutOfDomain);

    inline bool hasDefaultValue() const throw() {
      return defaultValue.isValid();
    }
    
    inline String getDefaultValue() const throw() {
      return defaultValue.getValue();
    }
    
    inline void setDefaultValue(const String& defaultValue) throw() {
      this->defaultValue = defaultValue;
    }

    inline ~StringOption() throw() {
    }
  };
  
  class RegExpOption : public Option {
  private:

    RegExp expression;
    ValidifiedResult<String> defaultValue;
  public:

    RegExpOption(
      const String& name,
      const String& pattern,
      unsigned int flags = 0) throw(OutOfDomain);
    
    RegExpOption(
      const String& name,
      char shortName,
      const String& pattern,
      unsigned int flags = 0) throw(OutOfDomain);

    /**
      Returns true if the value is valid.
    */
    bool isValid(const String& value) const throw();
    
    inline ~RegExpOption() throw() {
    }
  };
  
  class BooleanOption : public Option {
  private:
    
    ValidifiedResult<bool> defaultValue;
  public:
    
    BooleanOption(
      const String& name, unsigned int flags = 0) throw(OutOfDomain);
    
    BooleanOption(
      const String& name,
      char shortName,
      unsigned int flags = 0) throw(OutOfDomain);
    
    inline bool hasDefaultValue() const throw() {
      return defaultValue.isValid();
    }
    
    inline bool getDefaultValue() const throw() {
      return defaultValue.getValue();
    }
    
    inline void setDefaultValue(bool defaultValue) throw() {
      this->defaultValue = defaultValue;
    }
    
    /**
      Returns true if the value is valid.
    */
    inline bool isValid(const String& value) const throw() {
      return (value == "true") || (value == "false");
    }
  };

  class CardinalOption : public Option {
  private:
    
    ValidifiedResult<int> defaultValue;
  public:

    // PREFIX, SIGN
    
    CardinalOption(
      const String& name, unsigned int flags = 0) throw(OutOfDomain);
    
    CardinalOption(
      const String& name,
      char shortName,
      unsigned int flags = 0) throw(OutOfDomain);
    
    /**
      Returns true if the option has a default value.
    */
    bool hasDefaultValue() const throw() {
      return defaultValue.isValid();
    }
    
    /**
      Returns the default value.
    */
    inline int getDefaultValue() const throw() {
      return defaultValue.getValue();
    }
    
    /**
      Sets the default value.
    */
    inline void setDefaultValue(int defaultValue) throw() {
      this->defaultValue = defaultValue;
    }

    /**
      Returns true if the value is valid.
    */
    inline bool isValid(const String& value) const throw() {
      // TAG: sign
      String::ReadIterator i = value.getBeginReadIterator();
      String::ReadIterator end = value.getEndReadIterator();
      while ((i != end) && ASCIITraits::isDigit(*i)) {
        ++i;
      }
      return i == end;
    }
  };

  class RealOption : public Option {
  private:

    ValidifiedResult<double> defaultValue;
  public:
    
    RealOption(const String& name, unsigned int flags = 0) throw(OutOfDomain);
    
    RealOption(
      const String& name,
      char shortName,
      unsigned int flags = 0) throw(OutOfDomain);
    
    /**
      Returns true if the option has a default value.
    */
    bool hasDefaultValue() const throw() {
      return defaultValue.isValid();
    }
    
    /**
      Returns the default value.
    */
    inline double getDefaultValue() const throw() {
      return defaultValue.getValue();
    }
    
    /**
      Sets the default value.
    */
    inline void setDefaultValue(double defaultValue) throw() {
      this->defaultValue = defaultValue;
    }

    /**
      Returns true if the value is valid.
    */
    bool isValid(const String& value) const throw();
  };
  
  class EnumOption : public Option {
  private:

    /** The valid values. */
    HashSet<String> enums;
  public:
    
    EnumOption(const String& name, unsigned int flags = 0) throw(OutOfDomain);
    
    EnumOption(
      const String& name,
      char shortName,
      unsigned int flags = 0) throw(OutOfDomain);    

    /** Registers a value. */
    inline void addValue(const String& value) throw(MemoryException) {
      enums.add(value);
    }

    /**
      Returns true if the value is valid.
    */
    inline bool isValid(const String& value) const throw() {
      return enums.hasValue(value);
    }
    
    inline ~EnumOption() throw() {
    }
  };
  
  class Argument : public Polymorphic {
  public:

    /** Argument flags. */
    enum Flag {
      EXPLICIT = 1, /**< The value was specified. */
      LATER = 2, /**< Argument is given later. */
      EARLIER = 4, /**< Argument is given earlier. */
      DEFAULT = 8 /**< Default value. */
    };
  private:
    
    /** Flags. */
    unsigned int flags;
  public:

    /**
      Initializes the argument with the specified flags.
    */
    inline Argument(unsigned int _flags = 0) throw()
      : flags(_flags) {
    }
    
    /**
      Returns true if the value was specified.
    */
    inline bool isExplicit() const throw() {
      return flags & EXPLICIT;
    }
    
    /**
      Returns true if the argument is a repetition of an earlier argument.
    */
    inline bool isEarlier() const throw() {
      return flags & EARLIER;
    }
    
    /**
      Returns true if the argument is repeted again later.
    */
    inline bool isLater() const throw() {
      return flags & LATER;
    }

    /**
      Returns the flags.
    */
    inline unsigned int getFlags() const throw() {
      return flags;
    }

    /**
      Sets the flags.
    */
    inline void setFlags(unsigned int flags) throw() {
      this->flags = flags;
    }

    inline ~Argument() throw() {
    }
  };

  class OrphanArgument : public Argument {
  private:
    
    /** The name of the option. */
    String value;
  public:

    inline OrphanArgument(const String& _value, unsigned int flags = 0) throw()
      : Argument(flags),
        value(_value) {
    }

    inline String getValue() const throw() {
      return value;
    }
    
    inline ~OrphanArgument() throw() {
    }
  };

  class NamedArgument : public Argument {
  private:
    
    /** The name of the option. */
    String name;
  public:
    
    inline NamedArgument(
      const String& _name,
      unsigned int flags = 0) throw()
      : Argument(flags),
        name(_name) {
    }
    
    inline String getName() const throw() {
      return name;
    }
    
    inline ~NamedArgument() throw() {
    }
  };
  
  class StringArgument : public NamedArgument {
  private:
    
    String value;
  public:
    
    inline StringArgument(
      const String& name,
      const String& _value,
      unsigned int flags = 0) throw()
      : NamedArgument(name, flags),
        value(_value) {
    }
    
    inline String getValue() const throw() {
      return value;
    }
    
    inline ~StringArgument() throw() {
    }
  };
private:

  /** Flags. */
  unsigned int flags;
  /** Map of long names into option descriptor index. */
  HashTable<String, unsigned int> names;
  /** Map of short names into option descriptor index. */
  Allocator<unsigned int> shortNames;
  /** The options. */
  Array<Reference<Option> > options;

  static inline uint8 getIndexOfShortName(char shortName) throw() {
    if ((shortName >= 'a') && (shortName <= 'z')) {
      return shortName - 'a';
    } else if ((shortName >= 'A') && (shortName <= 'Z')) {
      return shortName - 'A' + ('z' - 'a');
    } else {
      return shortName - '0' + ('z' - 'a') + ('Z' - 'A');
    }
  }
  
  Argument* getArgument(
    const Reference<Option>& option,
    bool occured) const throw(ArgumentException);

  Argument* getArgument(
    const Reference<Option>& option,
    const String& value,
    bool occured) const throw(ArgumentException);
public:
  
  /**
    Initializes the argument parser.

    @param flags The flags. The default is 0.
  */
  ArgumentParser(unsigned int flags = 0) throw(MemoryException);

  /**
    Registers the specified option.
  */
  void addOption(const Reference<Option>& option) throw(AlreadyKeyException);
  
  Array<Argument*> operator()(
    const Array<String>& arguments) const throw(ArgumentException);
  
  // TAG: destroy...?
};

// TAG: need AP for use with parse()?
// TAG: OPHAN for Group
// class Group : public Option {
// public:
//   // TAG: ordered group
//   enum Type {
//     ANY, /**< Any option. */
//     ONE, /**< Exactly one of the options. */
//     ONE_OR_MORE, /**< One or more of the options. */
//     MUTUAL_EXCLUSIVE /**< . */
//   };
// private:

//   Type type;
//   Array<Option*> options;
// public:
  
//   /**
//     Initializes the option group.
//   */
//   Group(Type _type) : type(_type) {
//   }

//   void add(Option* option) throw() {
//     options.append(option);
//   }
// }

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
