/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/UnsignedInteger.h>
#include <base/string/FormatOutputStream.h>
#include <base/string/StringOutputStream.h>
#include <base/mathematics/ExpressionParser.h>
#include <base/mathematics/Math.h>
#include <base/mathematics/Random.h>

using namespace dk::sdu::mip::base;

class EvaluateApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;  
  
  bool simple;
  bool parsed;
  
  enum Command {
    COMMAND_VERSION,
    COMMAND_HELP,
    COMMAND_CONSTANTS,
    COMMAND_FUNCTIONS,
    COMMAND_EVALUATE
  };

  enum Constant {
    CONSTANT_E,
    CONSTANT_LN2,
    CONSTANT_PI,
    CONSTANT_SQRT2,
    CONSTANT_EULER
  };

  struct Identifier {
    Literal name;
    unsigned int id;
  };
  
  enum Function {
    FUNCTION_RANDOM,
    
    FUNCTION_ABS,
    FUNCTION_SIGN,
    FUNCTION_FRAC,
    FUNCTION_FLOOR,
    FUNCTION_CEIL,
    FUNCTION_ROUND,
    FUNCTION_SQR,
    FUNCTION_SQRT,
    FUNCTION_EXP,
    FUNCTION_LN,
    FUNCTION_LOG,
    FUNCTION_LOG2,
    FUNCTION_SIN,
    FUNCTION_ASIN,
    FUNCTION_COS,
    FUNCTION_ACOS,
    FUNCTION_TAN,
    FUNCTION_ATAN,
    FUNCTION_COTAN,
    FUNCTION_ACOTAN,
    FUNCTION_COSEC,
    FUNCTION_ACOSEC,
    FUNCTION_SEC,
    FUNCTION_ASEC,
    FUNCTION_EXSEC,
    FUNCTION_AEXSEC,
    FUNCTION_VERS,
    FUNCTION_AVERS,
    FUNCTION_HAV,
    FUNCTION_AHAV,
    FUNCTION_COVERS,
    FUNCTION_ACOVERS,
    FUNCTION_SINH,
    FUNCTION_ASINH,
    FUNCTION_COSH,
    FUNCTION_ACOSH,
    FUNCTION_TANH,
    FUNCTION_ATANH,
    FUNCTION_COTANH,
    FUNCTION_ACOTANH,
    FUNCTION_SECH,
    FUNCTION_ASECH,
    FUNCTION_COSECH,
    FUNCTION_ACOSECH,
    
    FUNCTION_HYPOT,
    FUNCTION_PWR,
    FUNCTION_AVG,
    FUNCTION_MIN,
    FUNCTION_MAX,
    FUNCTION_LOGN,
    FUNCTION_ATAN2
  };

  static const Identifier CONSTANTS[CONSTANT_EULER + 1];
  static const Identifier NO_ARGUMENT_FUNCTIONS[FUNCTION_RANDOM - FUNCTION_RANDOM + 1];
  static const Identifier SINGLE_ARGUMENT_FUNCTIONS[FUNCTION_ACOSECH - FUNCTION_ABS + 1];
  static const Identifier DOUBLE_ARGUMENT_FUNCTIONS[FUNCTION_ATAN2 - FUNCTION_HYPOT + 1];
  
  class MyExpressionEvaluator : public ExpressionEvaluator {
  public:

    MyExpressionEvaluator() throw() {
    }

    double onConstant(unsigned int id) const throw(ExpressionException) {
      switch (id) {
      case CONSTANT_E:
        return constant::E;
      case CONSTANT_LN2:
        return constant::LN2;
      case CONSTANT_PI:
        return constant::PI;
      case CONSTANT_SQRT2:
        return constant::SQRT2;
      case CONSTANT_EULER:
        return constant::EULER;
      default:
        return ExpressionEvaluator::onConstant(id);
      }
    }
    
    double onFunction(
      unsigned int id,
      const double* value) const throw(ExpressionException) {
      switch (id) {
      case FUNCTION_RANDOM:
        return Random::getLongDouble();
      case FUNCTION_ABS:
        return Math::abs(value[0]);
      case FUNCTION_SIGN:
        return (value[0] > 0) ? 1 : ((value[0] < 0) ? -1 : 0); // Math::sign(value[0]);
      case FUNCTION_FRAC:
        return 0; // Math::frac(value[0]); // TAG: need Math::frac
      case FUNCTION_FLOOR:
        return Math::floor(value[0]);
      case FUNCTION_CEIL:
        return Math::ceil(value[0]);
      case FUNCTION_ROUND:
        return Math::round(value[0]);
      case FUNCTION_SQR:
        return value[0] * value[0];
      case FUNCTION_SQRT:
        if (value[0] < 0) {
          throw ExpressionException("Out of domain", this);
        }
        return Math::sqrt(value[0]);
      case FUNCTION_EXP:
        return Math::exp(value[0]);
      case FUNCTION_LN:
        return Math::ln(value[0]);
      case FUNCTION_LOG:
        return Math::log10(value[0]);
      case FUNCTION_LOG2:
        return Math::log2(value[0]);
     case FUNCTION_SIN:
        return Math::sin(value[0]);
      case FUNCTION_ASIN:
        return Math::asin(value[0]);
      case FUNCTION_COS:
        return Math::cos(value[0]);
      case FUNCTION_ACOS:
        return Math::acos(value[0]);
      case FUNCTION_TAN:
        return Math::tan(value[0]);
      case FUNCTION_ATAN:
        return Math::atan(value[0]);
      case FUNCTION_COTAN:
        return Math::cotan(value[0]);
      case FUNCTION_ACOTAN:
        return Math::acotan(value[0]);
      case FUNCTION_COSEC:
        return Math::cosec(value[0]);
      case FUNCTION_ACOSEC:
        return Math::acosec(value[0]);
      case FUNCTION_SEC:
        return Math::sec(value[0]);
      case FUNCTION_ASEC:
        return Math::asec(value[0]);
      case FUNCTION_EXSEC:
        return Math::exsec(value[0]);
      case FUNCTION_AEXSEC:
        return Math::aexsec(value[0]);
      case FUNCTION_VERS:
        return Math::vers(value[0]);
      case FUNCTION_AVERS:
        return Math::avers(value[0]);
      case FUNCTION_HAV:
        return Math::hav(value[0]);
      case FUNCTION_AHAV:
        return Math::ahav(value[0]);
      case FUNCTION_COVERS:
        return Math::covers(value[0]);
      case FUNCTION_ACOVERS:
        return Math::acovers(value[0]);
      case FUNCTION_SINH:
        return Math::sinh(value[0]);
      case FUNCTION_ASINH:
        return Math::asinh(value[0]);
      case FUNCTION_COSH:
        return Math::cosh(value[0]);
      case FUNCTION_ACOSH:
        return Math::acosh(value[0]);
      case FUNCTION_TANH:
        return Math::tanh(value[0]);
      case FUNCTION_ATANH:
        return Math::atanh(value[0]);
      case FUNCTION_COTANH:
        return Math::cotanh(value[0]);
      case FUNCTION_ACOTANH:
        return Math::acotanh(value[0]);
      case FUNCTION_SECH:
        return Math::sech(value[0]);
      case FUNCTION_ASECH:
        return Math::asech(value[0]);
      case FUNCTION_COSECH:
        return Math::cosech(value[0]);
      case FUNCTION_ACOSECH:
        return Math::acosech(value[0]);
      case FUNCTION_HYPOT:
        return Math::hypot(value[0], value[1]);
      case FUNCTION_PWR:
        return Math::power(value[0], value[1]);
      case FUNCTION_AVG:
        return (value[0] + value[1])/2;
      case FUNCTION_MIN:
        return (value[0] <= value[1]) ? value[0] : value[1];
      case FUNCTION_MAX:
        return (value[0] >= value[1]) ? value[0] : value[1];
      case FUNCTION_LOGN:
        if ((value[0] <= 0) || (value[1] == 0)) {
          throw ExpressionException("Out of domain", this);
        }
        return Math::logn(value[0], value[1]);
      case FUNCTION_ATAN2:
        return Math::atan2(value[0], value[1]);
      default:
        return ExpressionEvaluator::onFunction(id, value);
      }
    }
  };
public:
  
  EvaluateApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) throw()
    : Application("eval", numberOfArguments, arguments, environment) {
    simple = false;
    parsed = false;
  }
  
  void dumpConstants() throw() {
    if (simple) {
      fout << "Constants:";
      for (unsigned int i = 0; i < getArraySize(CONSTANTS); ++i) {
        fout << ' ' << CONSTANTS[i].name;
      }
    } else {
      fout << "Constants:" << EOL;
      for (unsigned int i = 0; i < getArraySize(CONSTANTS); ++i) {
        fout << indent(2) << CONSTANTS[i].name << EOL;
      }
    }
    fout << ENDL;
  }
  
  void dumpFunctions() throw() {
    if (simple) {
      fout << "Functions:";
      for (unsigned int i = 0; i < getArraySize(NO_ARGUMENT_FUNCTIONS); ++i) {
        fout << ' ' << NO_ARGUMENT_FUNCTIONS[i].name << "()";
      }
      for (unsigned int i = 0; i < getArraySize(SINGLE_ARGUMENT_FUNCTIONS); ++i) {
        fout << ' ' << SINGLE_ARGUMENT_FUNCTIONS[i].name << "(x)";
      }
      for (unsigned int i = 0; i < getArraySize(DOUBLE_ARGUMENT_FUNCTIONS); ++i) {
        fout << ' ' << DOUBLE_ARGUMENT_FUNCTIONS[i].name << "(x, y)";
      }
    } else {
      fout << "Functions:" << EOL;
      for (unsigned int i = 0; i < getArraySize(NO_ARGUMENT_FUNCTIONS); ++i) {
        fout << indent(2) << NO_ARGUMENT_FUNCTIONS[i].name << "()" << EOL;
      }
      for (unsigned int i = 0; i < getArraySize(SINGLE_ARGUMENT_FUNCTIONS); ++i) {
        fout << indent(2) << SINGLE_ARGUMENT_FUNCTIONS[i].name << "(x)" << EOL;
      }
      for (unsigned int i = 0; i < getArraySize(DOUBLE_ARGUMENT_FUNCTIONS); ++i) {
        fout << indent(2) << DOUBLE_ARGUMENT_FUNCTIONS[i].name << "(x, y)" << EOL;
      }
    }
    fout << ENDL;
  }
  
  void evaluate(const String& expression) throw() {
    ExpressionProvider provider;

    for (unsigned int i = 0; i < getArraySize(CONSTANTS); ++i) {
      provider.registerConstant(CONSTANTS[i].name, CONSTANTS[i].id);
    }
    
    for (unsigned int i = 0; i < getArraySize(NO_ARGUMENT_FUNCTIONS); ++i) {
      provider.registerFunction(
        NO_ARGUMENT_FUNCTIONS[i].name,
        NO_ARGUMENT_FUNCTIONS[i].id,
        0
      );
    }
    
    for (unsigned int i = 0; i < getArraySize(SINGLE_ARGUMENT_FUNCTIONS); ++i) {
      provider.registerFunction(
        SINGLE_ARGUMENT_FUNCTIONS[i].name,
        SINGLE_ARGUMENT_FUNCTIONS[i].id,
        1
      );
    }
    
    for (unsigned int i = 0; i < getArraySize(DOUBLE_ARGUMENT_FUNCTIONS); ++i) {
      provider.registerFunction(
        DOUBLE_ARGUMENT_FUNCTIONS[i].name,
        DOUBLE_ARGUMENT_FUNCTIONS[i].id,
        2
      );
    }
    
    ExpressionParser parser(expression, provider);
    parser.setAutoRegister(true);
    try {
      parser.parse();
    } catch (ExpressionException& e) {
      ferr << "Error: " << e.getMessage()
           << " at index " << e.getIndex() << ENDL;
      setExitCode(EXIT_CODE_ERROR);
      return;
    }
    
    if (parsed) {
      fout << "Parsed expression: " << parser.getString() << ENDL;
    }

    if (parser.hasUnknowns()) {
      HashTable<String, MyExpressionEvaluator::Node> unknowns = parser.getUnknowns();
      HashTable<String, MyExpressionEvaluator::Node>::ReadEnumerator enu =
        unknowns.getReadEnumerator();
      fout << "The expression contains the following unknowns:";
      while (enu.hasNext()) {
        fout << ' ' << *enu.next()->getKey();
      }
      fout << ENDL;
    } else {
      MyExpressionEvaluator evaluator;
      evaluator.setExpression(parser.getExpression());
      fout << "Result: " << evaluator.evaluate() << ENDL;
    }
  }

  void version() throw() {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "http://www.mip.sdu.dk/~fonseca/base" << EOL
         << "Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>" << EOL
         << ENDL;
  }
  
  void help() throw() {
    version();
    fout << "Usage: " << getFormalName() << " [OPTIONS] expression" << EOL
         << EOL
         << "Options:" << EOL
         << indent(2) << "--help      this message" << EOL
         << indent(2) << "--version   dump the version" << EOL
         << EOL
         << indent(2) << "--constants show the available constants" << EOL
         << indent(2) << "--functions show the available functions" << EOL
         << indent(2) << "--simple    simple list form" << EOL
         << indent(2) << "--parsed    show the parsed expression" << EOL
         << ENDL;
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
      } else if (argument == "--constants") {
        command = COMMAND_CONSTANTS;
      } else if (argument == "--functions") {
        command = COMMAND_FUNCTIONS;
      } else if (argument == "--simple") {
        simple = true;
      } else if (argument == "--parsed") {
        parsed = true;
      } else {
        if (expressionSpecified) {
          ferr << "Error: " << "Expression already specified" << ENDL;
          setExitCode(EXIT_CODE_ERROR);
          return;
        }
        expressionSpecified = true;
        expression = argument;
      }
    }    
    
    switch (command) {
    case COMMAND_VERSION:
      version();
      break;
    case COMMAND_HELP:
      help();
      break;
    case COMMAND_CONSTANTS:
      dumpConstants();
      break;
    case COMMAND_FUNCTIONS:
      dumpFunctions();
      break;
    case COMMAND_EVALUATE:
      if (!expressionSpecified) {
        ferr << "Error: " << "Expression not specified" << ENDL;
        setExitCode(EXIT_CODE_ERROR);
        return;
      }
      evaluate(expression);
      break;
    }
  }
};

const EvaluateApplication::Identifier
EvaluateApplication::CONSTANTS[CONSTANT_EULER + 1] = {
  {MESSAGE("E"), CONSTANT_E},
  {MESSAGE("LN2"), CONSTANT_LN2},
  {MESSAGE("PI"), CONSTANT_PI},
  {MESSAGE("SQRT2"), CONSTANT_SQRT2},
  {MESSAGE("EULER"), CONSTANT_EULER} 
};

const EvaluateApplication::Identifier
EvaluateApplication::NO_ARGUMENT_FUNCTIONS[FUNCTION_RANDOM - FUNCTION_RANDOM + 1] = {
  {MESSAGE("random"), FUNCTION_RANDOM}
};

const EvaluateApplication::Identifier
EvaluateApplication::SINGLE_ARGUMENT_FUNCTIONS[FUNCTION_ACOSECH - FUNCTION_ABS + 1] = {
  {MESSAGE("abs"), FUNCTION_ABS},
  {MESSAGE("sign"), FUNCTION_SIGN},
  {MESSAGE("frac"), FUNCTION_FRAC},
  {MESSAGE("floor"), FUNCTION_FLOOR},
  {MESSAGE("ceil"), FUNCTION_CEIL},
  {MESSAGE("round"), FUNCTION_ROUND},
  {MESSAGE("sqr"), FUNCTION_SQR},
  {MESSAGE("sqrt"), FUNCTION_SQRT},
  {MESSAGE("exp"), FUNCTION_EXP},
  {MESSAGE("ln"), FUNCTION_LN},
  {MESSAGE("log"), FUNCTION_LOG},
  {MESSAGE("log2"), FUNCTION_LOG2},
  {MESSAGE("sin"), FUNCTION_SIN},
  {MESSAGE("asin"), FUNCTION_ASIN},
  {MESSAGE("cos"), FUNCTION_COS},
  {MESSAGE("acos"), FUNCTION_ACOS},
  {MESSAGE("tan"), FUNCTION_TAN},
  {MESSAGE("atan"), FUNCTION_ATAN},
  {MESSAGE("cotan"), FUNCTION_COTAN},
  {MESSAGE("acotan"), FUNCTION_ACOTAN},
  {MESSAGE("cosec"), FUNCTION_COSEC},
  {MESSAGE("acosec"), FUNCTION_ACOSEC},
  {MESSAGE("sec"), FUNCTION_SEC},
  {MESSAGE("asec"), FUNCTION_ASEC},
  {MESSAGE("exsec"), FUNCTION_EXSEC},
  {MESSAGE("aexsec"), FUNCTION_AEXSEC},
  {MESSAGE("vers"), FUNCTION_VERS},
  {MESSAGE("avers"), FUNCTION_AVERS},
  {MESSAGE("hav"), FUNCTION_HAV},
  {MESSAGE("ahav"), FUNCTION_AHAV},
  {MESSAGE("covers"), FUNCTION_COVERS},
  {MESSAGE("acovers"), FUNCTION_ACOVERS},
  {MESSAGE("sinh"), FUNCTION_SINH},
  {MESSAGE("asinh"), FUNCTION_ASINH},
  {MESSAGE("cosh"), FUNCTION_COSH},
  {MESSAGE("acosh"), FUNCTION_ACOSH},
  {MESSAGE("tanh"), FUNCTION_TANH},
  {MESSAGE("atanh"), FUNCTION_ATANH},
  {MESSAGE("cotanh"), FUNCTION_COTANH},
  {MESSAGE("acotanh"), FUNCTION_ACOTANH},
  {MESSAGE("sech"), FUNCTION_SECH},
  {MESSAGE("asech"), FUNCTION_ASECH},
  {MESSAGE("cosech"), FUNCTION_COSECH},
  {MESSAGE("acosech"), FUNCTION_ACOSECH}
};

const EvaluateApplication::Identifier
EvaluateApplication::DOUBLE_ARGUMENT_FUNCTIONS[FUNCTION_ATAN2 - FUNCTION_HYPOT + 1] = {
  {MESSAGE("hypot"), FUNCTION_HYPOT},
  {MESSAGE("pwr"), FUNCTION_PWR},
  {MESSAGE("avg"), FUNCTION_AVG},
  {MESSAGE("min"), FUNCTION_MIN},
  {MESSAGE("max"), FUNCTION_MAX},
  {MESSAGE("logn"), FUNCTION_LOGN},
  {MESSAGE("atan2"), FUNCTION_ATAN2}
};

STUB(EvaluateApplication);
