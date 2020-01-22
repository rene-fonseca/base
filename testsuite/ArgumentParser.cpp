/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/ArgumentParser.h>

using namespace com::azure::dev::base;

class ArgumentApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  ArgumentApplication()
    : Application("Argument")
  {
  }
  
  void main()
  {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << ENDL;
    
    ArgumentParser parser(ArgumentParser::ORPHANS);
    
    parser.addOption(new ArgumentParser::FlagOption("help", 'h'));
    parser.addOption(new ArgumentParser::FlagOption("version"));
    parser.addOption(new ArgumentParser::FlagOption("verbose", 'v'));
    parser.addOption(new ArgumentParser::FlagOption("enable", 'e'));
    parser.addOption(new ArgumentParser::FlagOption("disable", 'd'));
    parser.addOption(
      new ArgumentParser::CardinalOption(
        "cardinal",
        'c',
        ArgumentParser::CardinalOption::OVERRIDE
      )
    );
    parser.addOption(
      new ArgumentParser::StringOption(
        "file",
        'f',
        ArgumentParser::StringOption::STRICT |
        ArgumentParser::StringOption::EXPLICIT
      )
    );
    
    ArgumentParser::EnumOption* o = new ArgumentParser::EnumOption("mode", 'm');
    o->addValue("compile");
    o->addValue("link");
    o->addValue("install");
    parser.addOption(o);
    
    Array<ArgumentParser::Argument*> result = parser(getArguments());
    
    Array<ArgumentParser::Argument*>::ReadEnumerator enu =
      result.getReadEnumerator();

    while (enu.hasNext()) {
      ArgumentParser::Argument* argument = *enu.next();
      if (dynamic_cast<ArgumentParser::NamedArgument*>(argument)) {
        const ArgumentParser::NamedArgument* a =
          dynamic_cast<ArgumentParser::NamedArgument*>(argument);
        fout << "NamedArgument: " << a->getName() << SP << HEX << a->getFlags()
             << ENDL;
      } else if (dynamic_cast<ArgumentParser::OrphanArgument*>(argument)) {
        const ArgumentParser::OrphanArgument* a =
          dynamic_cast<ArgumentParser::OrphanArgument*>(argument);
        fout << "OrphanArgument: " << a->getValue() << ENDL;
      } else if (dynamic_cast<ArgumentParser::StringArgument*>(argument)) {
        const ArgumentParser::StringArgument* a =
          dynamic_cast<ArgumentParser::StringArgument*>(argument);
        fout << "StringArgument: " << a->getName() << "=" << a->getValue()
             << ENDL;
      }
    }
    
  }
};

APPLICATION_STUB(ArgumentApplication);
