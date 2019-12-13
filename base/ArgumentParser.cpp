/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/ArgumentParser.h>
#include <base/string/ASCIITraits.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

ArgumentParser::Option::Option(
  const String& _name,
  unsigned int _flags) throw(OutOfDomain)
  : name(_name),
    flags(_flags)
{
}

ArgumentParser::Option::Option(
  const String& _name,
  char _shortName,
  unsigned int _flags) throw(OutOfDomain)
  : name(_name),
    shortName(_shortName),
    flags(_flags)
{
  if (!ASCIITraits::isAlphaNum(shortName)) {
    throw OutOfDomain(this);
  }
}

ArgumentParser::FlagOption::FlagOption(
  const String& name, unsigned int flags) throw(OutOfDomain)
  : Option(name, flags)
{
}

ArgumentParser::FlagOption::FlagOption(
  const String& name,
  char shortName,
  unsigned int flags) throw(OutOfDomain)
  : Option(name, shortName, flags)
{
}

ArgumentParser::StringOption::StringOption(
  const String& name, unsigned int flags) throw(OutOfDomain)
  : Option(name, flags)
{
}

ArgumentParser::StringOption::StringOption(
  const String& name,
  char shortName,
  unsigned int flags) throw(OutOfDomain)
  : Option(name, shortName, flags)
{
}

ArgumentParser::RegExpOption::RegExpOption(
  const String& name,
  const String& pattern,
  unsigned int flags) throw(OutOfDomain)
  : Option(name, flags), expression(pattern)
{
}

ArgumentParser::RegExpOption::RegExpOption(
  const String& name,
  char shortName,
  const String& pattern,
  unsigned int flags) throw(OutOfDomain)
  : Option(name, shortName, flags), expression(pattern)
{
}

bool ArgumentParser::RegExpOption::isValid(const String& value) const noexcept
{
  return expression.match(value).isComplete(value);
}

ArgumentParser::BooleanOption::BooleanOption(
  const String& name, unsigned int flags) throw(OutOfDomain)
  : Option(name, flags)
{
}

ArgumentParser::BooleanOption::BooleanOption(
  const String& name,
  char shortName,
  unsigned int flags) throw(OutOfDomain)
  : Option(name, shortName, flags)
{
}

ArgumentParser::CardinalOption::CardinalOption(
  const String& name, unsigned int flags) throw(OutOfDomain)
  : Option(name, flags)
{
}

ArgumentParser::CardinalOption::CardinalOption(
  const String& name,
  char shortName,
  unsigned int flags) throw(OutOfDomain)
  : Option(name, shortName, flags)
{
}

ArgumentParser::RealOption::RealOption(
  const String& name, unsigned int flags) throw(OutOfDomain)
  : Option(name, flags)
{
}

ArgumentParser::RealOption::RealOption(
  const String& name,
  char shortName,
  unsigned int flags) throw(OutOfDomain)
  : Option(name, shortName, flags)
{
}

bool ArgumentParser::RealOption::isValid(const String& value) const noexcept
{
  // TAG: sign
  String::ReadIterator i = value.getBeginReadIterator();
  String::ReadIterator end = value.getEndReadIterator();
  while ((i != end) && ASCIITraits::isDigit(*i)) {
    ++i;
  }
  if ((i != end) && (*i == '.')) {
    ++i; // skip '.'
    while ((i != end) && ASCIITraits::isDigit(*i)) {
      ++i;
    }
  }
  return i == end;
}

ArgumentParser::EnumOption::EnumOption(
  const String& name, unsigned int flags) throw(OutOfDomain)
  : Option(name, flags)
{
}

ArgumentParser::EnumOption::EnumOption(
  const String& name,
  char shortName,
  unsigned int flags) throw(OutOfDomain)
  : Option(name, shortName, flags)
{
}

ArgumentParser::ArgumentParser(unsigned int _flags)
  : flags(_flags),
    shortNames(25 + 25 + 10)
{
  MemorySize v = PrimitiveTraits<MemorySize>::MAXIMUM;
  fill(
    shortNames.getElements(),
    shortNames.getSize(),
    v // mark as unused
  );
}

void ArgumentParser::addOption(const Reference<Option>& option)
{
  bassert(
    !names.hasKey(option->getName()),
    AlreadyKeyException("name already registered", this)
  );
  names.add(option->getName(), options.getSize());
  if (option->hasShortName()) {
    MemorySize* lookup = shortNames.getElements();
    const uint8 shortNameIndex = getIndexOfShortName(option->getShortName());
    bassert(
      lookup[shortNameIndex] == PrimitiveTraits<unsigned int>::MAXIMUM,
      AlreadyKeyException("short name already registered", this)
    );
    lookup[shortNameIndex] = options.getSize();
  }
  options.append(option);
}

ArgumentParser::Argument* ArgumentParser::getArgument(
  const Reference<Option>& option,
  bool occured) const
{
  Argument* argument = nullptr;
  if (option.isType<FlagOption>()) {
    argument = new NamedArgument(option->getName(), occured ? Argument::EARLIER : 0);
  } else { // options which take a value
    if (option->getFlags() & Option::EXPLICIT) {
      throw bindCause(ArgumentException(this), ArgumentException::VALUE_MISSING);
    }
    if (option.isType<BooleanOption>()) {
      argument = new NamedArgument(
        option->getName(),
        (occured ? Argument::EARLIER : 0) | Argument::DEFAULT
      );
    } else if (option.isType<StringOption>()) {
      argument = new NamedArgument(
        option->getName(),
        (occured ? Argument::EARLIER : 0) | Argument::DEFAULT
      );
    } else if (option.isType<CardinalOption>()) {
      argument = new NamedArgument(
        option->getName(),
        (occured ? Argument::EARLIER : 0) | Argument::DEFAULT
      );
    } else if (option.isType<RealOption>()) {
      argument = new NamedArgument(
        option->getName(),
        (occured ? Argument::EARLIER : 0) | Argument::DEFAULT
      );
    } else if (option.isType<EnumOption>()) {
      argument = new NamedArgument(
        option->getName(),
        (occured ? Argument::EARLIER : 0) | Argument::DEFAULT
      );
    }
  }
  return argument;
}

ArgumentParser::Argument* ArgumentParser::getArgument(
  const Reference<Option>& option,
  const String& value,
  bool occured) const
{
  Argument* argument = nullptr;
  if (option.isType<FlagOption>()) {
    throw bindCause(ArgumentException(this), ArgumentException::VALUE_PRESENT);
  } else { // options which take a value
    if (option.isType<BooleanOption>()) {
      Reference<BooleanOption> temp = option.cast<BooleanOption>();
      bassert(
        temp.isValid(),
        bindCause(ArgumentException(this), ArgumentException::INVALID_VALUE)
      );
      argument = new StringArgument(
        option->getName(),
        value,
        occured ? Argument::EARLIER : 0
      );
    } else if (option.isType<StringOption>()) {
      argument = new StringArgument(
        option->getName(),
        value,
        occured ? Argument::EARLIER : 0
      );
    } else if (option.isType<CardinalOption>()) {
      Reference<CardinalOption> temp = option.cast<CardinalOption>();
      bassert(
        temp->isValid(value),
        bindCause(ArgumentException(this), ArgumentException::INVALID_VALUE)
      );
      argument = new StringArgument(
        option->getName(),
        value,
        occured ? Argument::EARLIER : 0
      );
    } else if (option.isType<RealOption>()) {
      Reference<RealOption> temp = option.cast<RealOption>();
      bassert(
        temp->isValid(value),
        bindCause(ArgumentException(this), ArgumentException::INVALID_VALUE)
      );
      argument = new StringArgument(
        option->getName(),
        value,
        occured ? Argument::EARLIER : 0
      );
    } else if (option.isType<EnumOption>()) {
      Reference<EnumOption> temp = option.cast<EnumOption>();
      bassert(
        temp->isValid(value),
        bindCause(ArgumentException(this), ArgumentException::INVALID_VALUE)
      );
      argument = new StringArgument(
        option->getName(),
        value,
        occured ? Argument::EARLIER : 0
      );
    }
  }
  return argument;
}

Array<ArgumentParser::Argument*> ArgumentParser::operator()(
  const Array<String>& arguments) const
{
  Array<Argument*> result;
  HashSet<String> occured;
  
  const MemorySize* lookup = shortNames.getElements();
  Array<String>::ReadIterator i = arguments.getBeginReadIterator();
  const Array<String>::ReadIterator endArgument =
    arguments.getEndReadIterator();
  
  while (i != endArgument) {
    String argument = *i++;
    if (argument.startsWith("-")) { // option
      if (argument.startsWith("--")) { // long name
        String name;
        String value;
        const MemoryDiff assignIndex = argument.indexOf("=");
        if (assignIndex >= 0) {
          name = argument.substring(2, assignIndex);
          value = argument.substring(assignIndex + 1);
        } else {
          name = argument.substring(2);
        }
        
        bassert(
          names.hasKey(name),
          bindCause(ArgumentException(this), ArgumentException::INVALID_OPTION)
        );
        Reference<Option> option = options[names[name]];
        const bool o = occured.hasValue(name);
        bassert(
          !(option->getFlags() & Option::STRICT) || !o,
          bindCause(ArgumentException(this), ArgumentException::STRICT_VIOLATION)
        );
        
        if (o) {
          Array<Argument*>::Iterator j = result.getEndIterator();
          const Array<Argument*>::Iterator begin = result.getBeginIterator();
          while (j != begin) {
            --j;
            if (dynamic_cast<NamedArgument*>(*j)) {
              NamedArgument* a = dynamic_cast<NamedArgument*>(*j);
              if (a->getName() == name) {
                if (option->getFlags() & Option::OVERRIDE) {
                  result.remove(j - begin);
                } else {
                  a->setFlags(a->getFlags() | Argument::LATER);
                }
                break; // no need to continue
              }
            }
          }
        } else {
          occured.add(name);
        }
        
        Argument* argument = nullptr;
        if (assignIndex >= 0) {
          argument = getArgument(option, value, o);
        } else {
          argument = getArgument(option, o);
        }
        if (argument) {
          result.append(argument);
        }
      } else if (argument.getLength() > 1) { // short
        String::ReadIterator j = argument.getBeginReadIterator();
        const String::ReadIterator endShortName =
          argument.getEndReadIterator();
        ++j; // skip -
        while (j != endShortName) {
          bassert(
            ASCIITraits::isAlphaNum(*j),
            bindCause(ArgumentException(this), ArgumentException::INVALID_SYMBOL)
          );
          const uint8 shortNameIndex = getIndexOfShortName(*j);
          bassert(
            lookup[shortNameIndex] != PrimitiveTraits<unsigned int>::MAXIMUM,
            bindCause(ArgumentException(this), ArgumentException::INVALID_OPTION)
          );
          Reference<Option> option = options[lookup[shortNameIndex]];
          const bool o = occured.hasValue(option->getName());
          bassert(
            !(option->getFlags() & Option::STRICT) || !o,
            bindCause(ArgumentException(this), ArgumentException::STRICT_VIOLATION)
          );
          
          if (o) {
            // set LATER flag for last argument
            Array<Argument*>::Iterator k = result.getEndIterator();
            const Array<Argument*>::Iterator begin = result.getBeginIterator();
            while (k != begin) {
              --k;
              if (dynamic_cast<NamedArgument*>(*k)) {
                NamedArgument* a = dynamic_cast<NamedArgument*>(*k);
                if (a->getName() == option->getName()) {
                  a->setFlags(a->getFlags() | Argument::LATER);
                  break; // no need to continue
                }
              }
            }
          } else {
            occured.add(option->getName());
          }
          
          Argument* argument = nullptr;
          if (argument) {
            result.append(argument);
          }
          ++j;
        }
        // value only allowed if one short option
      } else {
        // TAG: handle standalone "-"
        throw ArgumentException(this);
      }
      
    } else {
      // TAG: prefix options?
      bassert(
        this->flags & ORPHANS,
        bindCause(ArgumentException(this), ArgumentException::ORPHAN)
      );
      result.append(new OrphanArgument(argument));
    }
    
    // postcondition
    {
      Array<Reference<Option> >::ReadEnumerator enu =
        options.getReadEnumerator();
      while (enu.hasNext()) {
        Reference<Option> option = *enu.next();
        if (option->getFlags() & Option::MANDATORY) {
          bassert(
            occured.hasValue(option->getName()),
            bindCause(ArgumentException(this), ArgumentException::MANDATORY_VIOLATION)
          );
        }
      }
    }
  }
  return result;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
