/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/UnsignedInteger.h>
#include <base/mathematics/Math.h>
#include <base/string/FormatOutputStream.h>
#include <base/string/StringOutputStream.h>
#include <base/mathematics/ExpressionParser.h>

using namespace base;

class EvaluateApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;  
  
  enum Command {
    COMMAND_VERSION,
    COMMAND_HELP,
    COMMAND_EVALUATE
  };
public:
  
  EvaluateApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) throw()
    : Application(MESSAGE("eval"), numberOfArguments, arguments, environment) {
  }
  
  void evaluate(const String& expression) throw() {
    ExpressionProvider provider;
    
    provider.registerConstant(MESSAGE("E"), 1 + 256);
    provider.registerConstant(MESSAGE("PI"), 2 + 256);
    
    provider.registerFunction(MESSAGE("sin"), 1 + 1024, 1);
    provider.registerFunction(MESSAGE("cos"), 2 + 1024, 1);
    provider.registerFunction(MESSAGE("tan"), 3 + 1024, 1);

    ExpressionParser parser(expression, provider);
    try {
      parser.parse();
    } catch (ExpressionException& e) {
      ferr << MESSAGE("Error: ") << e.getMessage()
           << MESSAGE(" at index ") << e.getIndex() << ENDL;
      setExitCode(EXIT_CODE_ERROR);
      return;
    }
    
    // ExpressionEvaluator evaluator;
    fout << MESSAGE("Result: ") << 0 << ENDL;
  }

  void version() throw() {
    fout << getFormalName() << MESSAGE(" version ")
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << MESSAGE("The Base Framework (Test Suite)") << EOL
         << MESSAGE("http://www.mip.sdu.dk/~fonseca/base") << EOL
         << MESSAGE("Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>") << EOL
         << ENDL;
  }
  
  void help() throw() {
    version();
    fout << getFormalName() << MESSAGE(" [OPTIONS] expression") << ENDL;
  }
  
  void main() throw() {
    Command command = COMMAND_EVALUATE;
    bool expressionSpecified = false;
    String expression;
    
    const Array<String> arguments = getArguments();
    Array<String>::ReadEnumerator enu = arguments.getReadEnumerator();
    while (enu.hasNext()) {
      String argument = *enu.next();
      if (argument == "--help") {
        command = COMMAND_HELP;
      } else if (argument == "--version") {
        command = COMMAND_VERSION;
      } else {
        if (expressionSpecified) {
          ferr << MESSAGE("Error: ") << MESSAGE("Expression already specified") << ENDL;
          setExitCode(EXIT_CODE_ERROR);
          return;
        }
        expressionSpecified = true;
        expression = argument;
      }
    }
    
    if (!expressionSpecified) {
      ferr << MESSAGE("Error: ") << MESSAGE("Expression not specified") << ENDL;
      setExitCode(EXIT_CODE_ERROR);
      return;
    }
    
    switch (command) {
    case COMMAND_VERSION:
      version();
      break;
    case COMMAND_HELP:
      help();
      break;
    case COMMAND_EVALUATE:
      evaluate(expression);
      break;
    }
  }
};

STUB(EvaluateApplication);
