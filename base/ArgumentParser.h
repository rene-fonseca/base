/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

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

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Argument parser.
  
  @short Argument parser.
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API ArgumentParser : public Object {
public:

  enum Flag {
    ORPHANS = 1 /**< Allow orphans. */
  };
  
  class _COM_AZURE_DEV__BASE__API Option : public ReferenceCountedObject {
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
    char shortName = 0;
    /** Flags. */
    unsigned int flags = 0;
  public:
    
    Option(const String& name, unsigned int flags = 0);
    
    Option(
      const String& name,
      char shortName,
      unsigned int flags = 0);
    
    /**
      Returns true if the option has a short name.
    */
    inline bool hasShortName() const noexcept
    {
      return shortName != 0;
    }
    
    /**
      Returns the short name of the option.
    */
    inline char getShortName() const noexcept
    {
      return shortName;
    }
    
    /**
      Returns the long name of the option.
    */
    inline const String& getName() const noexcept
    {
      return name;
    }
    
    /**
      Returns the flags.
    */
    inline unsigned int getFlags() const noexcept
    {
      return flags;
    }

    virtual ~Option() noexcept
    {
    };
  };
  
  class _COM_AZURE_DEV__BASE__API FlagOption : public Option {
  public:
    
    FlagOption(const String& name, unsigned int flags = 0);
    
    FlagOption(
      const String& name,
      char shortName,
      unsigned int flags = 0);
  };
  
  class _COM_AZURE_DEV__BASE__API StringOption : public Option {
  private:

    ValidifiedResult<String> defaultValue;
  public:

    StringOption(
      const String& name, unsigned int flags = 0);
    
    StringOption(
      const String& name,
      char shortName,
      unsigned int flags = 0);

    inline bool hasDefaultValue() const noexcept
    {
      return defaultValue.isValid();
    }
    
    inline const String& getDefaultValue() const noexcept
    {
      return defaultValue.getValue();
    }
    
    inline void setDefaultValue(const String& defaultValue) noexcept
    {
      this->defaultValue = defaultValue;
    }

    inline ~StringOption() noexcept
    {
    }
  };
  
  class _COM_AZURE_DEV__BASE__API RegExpOption : public Option {
  private:

    RegExp expression;
    ValidifiedResult<String> defaultValue;
  public:

    RegExpOption(
      const String& name,
      const String& pattern,
      unsigned int flags = 0);
    
    RegExpOption(
      const String& name,
      char shortName,
      const String& pattern,
      unsigned int flags = 0);

    /**
      Returns true if the value is valid.
    */
    bool isValid(const String& value) const noexcept;
    
    inline ~RegExpOption() noexcept
    {
    }
  };
  
  class _COM_AZURE_DEV__BASE__API BooleanOption : public Option {
  private:
    
    ValidifiedResult<bool> defaultValue;
  public:
    
    BooleanOption(
      const String& name, unsigned int flags = 0);
    
    BooleanOption(
      const String& name,
      char shortName,
      unsigned int flags = 0);
    
    inline bool hasDefaultValue() const noexcept
    {
      return defaultValue.isValid();
    }
    
    inline bool getDefaultValue() const noexcept
    {
      return defaultValue.getValue();
    }
    
    inline void setDefaultValue(bool defaultValue) noexcept
    {
      this->defaultValue = defaultValue;
    }
    
    /**
      Returns true if the value is valid.
    */
    inline bool isValid(const String& value) const noexcept
    {
      return (value == "true") || (value == "false");
    }
  };

  class _COM_AZURE_DEV__BASE__API CardinalOption : public Option {
  private:
    
    ValidifiedResult<int> defaultValue;
  public:

    // PREFIX, SIGN
    
    CardinalOption(
      const String& name, unsigned int flags = 0);
    
    CardinalOption(
      const String& name,
      char shortName,
      unsigned int flags = 0);
    
    /**
      Returns true if the option has a default value.
    */
    bool hasDefaultValue() const noexcept
    {
      return defaultValue.isValid();
    }
    
    /**
      Returns the default value.
    */
    inline int getDefaultValue() const noexcept
    {
      return defaultValue.getValue();
    }
    
    /**
      Sets the default value.
    */
    inline void setDefaultValue(int defaultValue) noexcept
    {
      this->defaultValue = defaultValue;
    }

    /**
      Returns true if the value is valid.
    */
    inline bool isValid(const String& value) const noexcept
    {
      // TAG: sign
      String::ReadIterator i = value.getBeginReadIterator();
      String::ReadIterator end = value.getEndReadIterator();
      while ((i != end) && ASCIITraits::isDigit(*i)) {
        ++i;
      }
      return i == end;
    }
  };

  class _COM_AZURE_DEV__BASE__API RealOption : public Option {
  private:

    ValidifiedResult<double> defaultValue;
  public:
    
    RealOption(const String& name, unsigned int flags = 0);
    
    RealOption(
      const String& name,
      char shortName,
      unsigned int flags = 0);
    
    /**
      Returns true if the option has a default value.
    */
    bool hasDefaultValue() const noexcept
    {
      return defaultValue.isValid();
    }
    
    /**
      Returns the default value.
    */
    inline double getDefaultValue() const noexcept
    {
      return defaultValue.getValue();
    }
    
    /**
      Sets the default value.
    */
    inline void setDefaultValue(double defaultValue) noexcept
    {
      this->defaultValue = defaultValue;
    }

    /**
      Returns true if the value is valid.
    */
    bool isValid(const String& value) const noexcept;
  };
  
  class _COM_AZURE_DEV__BASE__API EnumOption : public Option {
  private:

    /** The valid values. */
    HashSet<String> enums;
  public:
    
    EnumOption(const String& name, unsigned int flags = 0);
    
    EnumOption(
      const String& name,
      char shortName,
      unsigned int flags = 0);

    /** Registers a value. */
    inline void addValue(const String& value)
    {
      enums.add(value);
    }

    /**
      Returns true if the value is valid.
    */
    inline bool isValid(const String& value) const noexcept
    {
      return enums.hasValue(value);
    }
    
    inline ~EnumOption() noexcept
    {
    }
  };
  
  class _COM_AZURE_DEV__BASE__API Argument : public Polymorphic {
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
    unsigned int flags = 0;
  public:

    /**
      Initializes the argument with the specified flags.
    */
    inline Argument(unsigned int _flags = 0) noexcept
      : flags(_flags)
    {
    }
    
    /**
      Returns true if the value was specified.
    */
    inline bool isExplicit() const noexcept
    {
      return flags & EXPLICIT;
    }
    
    /**
      Returns true if the argument is a repetition of an earlier argument.
    */
    inline bool isEarlier() const noexcept
    {
      return flags & EARLIER;
    }
    
    /**
      Returns true if the argument is repeted again later.
    */
    inline bool isLater() const noexcept
    {
      return flags & LATER;
    }

    /**
      Returns the flags.
    */
    inline unsigned int getFlags() const noexcept
    {
      return flags;
    }

    /**
      Sets the flags.
    */
    inline void setFlags(unsigned int flags) noexcept
    {
      this->flags = flags;
    }

    inline ~Argument() noexcept
    {
    }
  };

  class _COM_AZURE_DEV__BASE__API OrphanArgument : public Argument {
  private:
    
    /** The name of the option. */
    String value;
  public:

    inline OrphanArgument(const String& _value, unsigned int flags = 0) noexcept
      : Argument(flags),
        value(_value)
    {
    }

    inline const String& getValue() const noexcept
    {
      return value;
    }
    
    inline ~OrphanArgument() noexcept
    {
    }
  };

  class _COM_AZURE_DEV__BASE__API NamedArgument : public Argument {
  private:
    
    /** The name of the option. */
    String name;
  public:
    
    inline NamedArgument(
      const String& _name,
      unsigned int flags = 0) noexcept
      : Argument(flags),
        name(_name)
    {
    }
    
    inline const String& getName() const noexcept
    {
      return name;
    }
    
    inline ~NamedArgument() noexcept
    {
    }
  };
  
  class _COM_AZURE_DEV__BASE__API StringArgument : public NamedArgument {
  private:
    
    String value;
  public:
    
    inline StringArgument(
      const String& name,
      const String& _value,
      unsigned int flags = 0) noexcept
      : NamedArgument(name, flags),
        value(_value)
    {
    }
    
    inline const String& getValue() const noexcept
    {
      return value;
    }
    
    inline ~StringArgument() noexcept
    {
    }
  };
private:

  /** Flags. */
  unsigned int flags = 0;
  /** Map of long names into option descriptor index. */
  HashTable<String, MemorySize> names;
  /** Map of short names into option descriptor index. */
  Allocator<MemorySize> shortNames;
  /** The options. */
  Array<Reference<Option> > options;

  static inline uint8 getIndexOfShortName(char shortName) noexcept
  {
    if ((shortName >= 'a') && (shortName <= 'z')) {
      return shortName - 'a';
    } else if ((shortName >= 'A') && (shortName <= 'Z')) {
      return shortName - 'A' + ('z' - 'a');
    } else {
      return shortName - '0' + ('z' - 'a') + ('Z' - 'A');
    }
  }
  
  Argument* getArgument(const Reference<Option>& option, bool occured) const;

  Argument* getArgument(
    const Reference<Option>& option,
    const String& value,
    bool occured) const;
public:
  
  /**
    Initializes the argument parser.

    @param flags The flags. The default is 0.
  */
  ArgumentParser(unsigned int flags = 0);

  /**
    Registers the specified option.
  */
  void addOption(const Reference<Option>& option);
  
  Array<Argument*> operator()(const Array<String>& arguments) const;
  
  // TAG: destroy...?
};

// TAG: need AP for use with parse()?
// TAG: OPHAN for Group
// class _COM_AZURE_DEV__BASE__API Group : public Option {
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

//   void add(Option* option) noexcept {
//     options.append(option);
//   }
// }

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
