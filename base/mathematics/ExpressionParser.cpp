/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "evaluate.h"
#include <stdlib.h>
#include <errno.h>

namespace eval {

  EvaluationNode makeValueNode(double value) {
    EvaluationNode result;
    result.type = VALUE;
    result.value = value;
    return result;
  }

  EvaluationNode makeConstantNode(unsigned int id) {
    EvaluationNode result;
    result.type = CONSTANT;
    result.constant = id;
    return result;
  }

  EvaluationNode makeVariableNode(unsigned int id) {
    EvaluationNode result;
    result.type = VARIABLE;
    result.variable = id;
    return result;
  }

  EvaluationNode makeFunctionNode(unsigned int id, unsigned int arguments) {
    EvaluationNode result;
    result.type = FUNCTION;
    result.function = id;
    result.arguments = arguments;
    return result;
  }

  EvaluationNode makeNodeFromOperation(Operation opr) {
    EvaluationNode result;
    if (opr.isBuiltin()) {
      result.type = BUILTIN;
      result.builtin = opr.getId();
    } else {
      result.type = FUNCTION;
      result.function = opr.getId();
      result.arguments = opr.getArguments();
    }
    return result;
  }

  class UnaryOperator : public Operation {
  public:
    UnaryOperator(unsigned int i, unsigned int p, Glue g, bool po) throw() : Operation(i, 1, p, g, true, po) {}
  };

  class BinaryOperator : public Operation {
  public:
    BinaryOperator(unsigned int i, unsigned int p, Glue g, bool po) throw() : Operation(i, 2, p, g, true, po) {}
  };

  // built-in operations
  UnaryOperator plus(0, 2, RIGHT, true);
  UnaryOperator minus(1, 2, RIGHT, true);
  BinaryOperator add(2, 0, LEFT, true);
  BinaryOperator subtract(3, 0, LEFT, true);
  BinaryOperator multiply(4, 1, LEFT, true);
  BinaryOperator divide(5, 1, LEFT, true);
  BinaryOperator power(6, 3, RIGHT, true);
  UnaryOperator parenthesis(7, 100, RIGHT, false);
}

using namespace eval;



ExpressionProvider::ExpressionProvider() throw() {
}

void ExpressionProvider::registerConstant(const char* name, unsigned int id) throw(AmbiguousRegistration) {
  if (identifiers.isKey(name)) {
    throw AmbiguousRegistration();
  }
  identifiers[name] = makeConstantNode(id);
}

void ExpressionProvider::registerVariable(const char* name, unsigned int id) throw(AmbiguousRegistration) {
  if (identifiers.isKey(name)) {
    throw AmbiguousRegistration();
  }
  identifiers[name] = makeVariableNode(id);
}

void ExpressionProvider::registerFunction(const char* name, unsigned int id, unsigned int arguments) throw(AmbiguousRegistration) {
  if (identifiers.isKey(name)) {
    throw AmbiguousRegistration();
  }
  identifiers[name] = makeFunctionNode(id, arguments);
}

eval::EvaluationNode ExpressionProvider::getNode(const char* name) const throw(InvalidKey) {
  return identifiers.getValue(name);
}

ExpressionProvider::~ExpressionProvider() {
}



ExpressionParser::ExpressionParser(const String<>& e, ExpressionProvider& p) throw() : expression(e), provider(p) {
}

void ExpressionParser::pop() throw(ExpressionException) {
  Operation opr = stack.pop();
  if (operands >= opr.getArguments()) {
    nodes.add(makeNodeFromOperation(opr));
    operands = operands - opr.getArguments() + 1;
  } else {
    throw ExpressionException(index, "Operand expected");
  }
}

void ExpressionParser::push(Operation opr) throw(ExpressionException) {
  while (!stack.isEmpty() && (stack.peek().getPrecedence() > opr.getPrecedence())) {
    pop();
  }

  if (!stack.isEmpty() &&
       (stack.peek().getPrecedence() == opr.getPrecedence()) &&
       (stack.peek().getGlue() == LEFT)
  ) {
    pop();
  }

  stack.push(opr);
  unary = true;
}

void ExpressionParser::readIdentifier() throw(ExpressionException) {

  unsigned int i = index;

  while (i <= last) {
    char ch = expression[index];
    if (!(((ch >= 'a') && (ch <= 'z')) || ((ch >= '0') && (ch <= '9')))) {
      break;
    }
    ++i;
  }
  if (i == index) { // has something been read
    throw ExpressionException(index, "Not an identifier");
  }

  char identifier[index - i + 1];
  for (unsigned int j = 0; j < index - i; ++j) {
    identifier[j] = expression[i + j];
  }
  identifier[index - i + 1] = '\0';

  try {
    Node node = provider.getNode(identifier);
  } catch(InvalidKey e) {
//    if (autoRegisterAsVariable) {
//      unknowns[name] = makeVariable(id???);
//      node = identifiers[name];
//    } else {
      throw ExpressionException(index, "Identifier not recognized");
//    }
  }

  index = i; // char after identifier
}

void ExpressionParser::readValue() throw(ExpressionException) {

  unsigned int i = index;
  bool digits = false;

  // read sign if present
  if (i <= last) {
    if (expression[i] == '+') {
      ++i;
    } else if (expression[i] == '-') {
      ++i;
    }
  }

  // read digits if present before possible dot
  if ((i <= last) && ((expression[i] >= '0') || (expression[i] <= '9'))) {
    digits = true;
    while ((i <= last) && ((expression[i] >= '0') || (expression[i] <= '9'))) {
      ++i;
    }
  }

  // read dot if present
  if ((i <= last) && (expression[i] == '.')) {
    ++i;

    // read digits if present after dot
    if ((i <= last) && ((expression[i] >= '0') || (expression[i] <= '9'))) {
      digits = true;
      while ((i <= last) && ((expression[i] >= '0') || (expression[i] <= '9'))) {
        ++i;
      }
    }
  }

  if (!digits) {
    throw ExpressionException(index, "Not a number");
  }

  // read exponent if present
  if ((i <= last) && (expression[i] == 'e')) {

    // read sign of exponent if present
    if (i <= last) {
      if (expression[i] == '+') {
        ++i;
      } else if (expression[i] == '-') {
        ++i;
      }
    }

    // read exponent value
    if ((i <= last) && ((expression[i] >= '0') || (expression[i] <= '9'))) {
      while ((i <= last) && ((expression[i] >= '0') || (expression[i] <= '9'))) {
        ++i;
      }
    } else {
      throw ExpressionException(index, "Not a number");
    }
  }

  char buffer[i - index + 1]; // i is index of character after number chars
  for (unsigned int j = 0; j < i - index; ++j) {
    buffer[j] = expression[index + j];
  }
  buffer[i - index] = '\0';
  char* end;
  double value = strtod((char*)&buffer, &end); // not the best solution - but it works for now
  if ((end != &buffer[i - index]) || (errno == ERANGE)) {
    throw ExpressionException(index, "Not a number");
  }

  nodes.add(makeValueNode(value));
  ++operands;
  unary = false;
  index = i; // char after value
}

void ExpressionParser::parse() throw(ExpressionException) {
  unsigned int index = 0;
  unsigned int last = expression.length() - 1;
  unsigned int operands = 0;
  bool unary = true;
  stack.removeAll();
  nodes.removeAll();

  while (index <= last) { // until all chars have been read
    switch (expression[index]) {
    case ' ':
      ++index;
      break; // ignore ordinary spaces
    case '+':
      if (unary) {
        push(plus);
      } else {
        push(add);
      }
      ++index;
      break;
    case '-':
      if (unary) {
        push(minus);
      } else {
        push(subtract);
      }
      ++index;
      break;
    case '*':
      if (unary) {
        throw ExpressionException(index, "Left operand expected by multiplication operator");
      } else {
        push(multiply);
      }
      ++index;
      break;
    case '/':
      if (unary) {
        throw ExpressionException(index, "Left operand expected by division operator");
      } else {
        push(divide);
      }
      ++index;
      break;
    case '^':
      if (unary) {
        throw ExpressionException(index, "Left operand expected by power operator");
      } else {
        push(power);
      }
      ++index;
      break;
    case '(':
      if (unary) {
        push(parenthesis);
      } else {
        throw ExpressionException(index, "Operator expected instead of opening parenthesis");
      }
      ++index;
      break;
    case ')':
      if (unary) {
        throw ExpressionException(index, "Operand expected before closing parenthesis");
      } else {
        while (!stack.isEmpty() && (stack.peek().isPopable())) {
          pop();
        }

        if (!stack.isEmpty() && (!stack.peek().isPopable())) {
          Operation opr = stack.pop(); // get object

          if (operands < opr.getArguments()) {
            throw ExpressionException(index, "Operand(s) missing from subexpression");
          }
          operands = operands - opr.getArguments() + 1;
          nodes.add(makeNodeFromOperation(opr));
        } else {
          throw ExpressionException(index, "Closing parenthesis has no matching opening parenthesis");
        }
      }
      ++index;
      break;
    case ';':
      if (unary) {
        throw ExpressionException(index, "Operand expected before argument separator");
      } else {

        while (!stack.isEmpty() && stack.peek().isPopable()) {
          pop();
        }

        if (!stack.isEmpty() && (!stack.peek().isPopable())) {
          Operation opr = stack.peek(); // get object - do not remove from stack
          // does not handle parenthesis'
          if (opr.getArguments() == 0) {
            throw ExpressionException(index, "Function does not take any arguments");
          } else if (opr.getArguments() == 1) {
            throw ExpressionException(index, "Function only takes one argument");
          }
        } else {
          throw ExpressionException(index, "Argument separator used outside function argument list");
        }

        unary = true;
      }
      ++index;
      break;
    default:
      char ch = expression[index];
      if ((ch >= 'a') && (ch <= 'z')) { // is this an identifier
        if (unary) {
          readIdentifier();
        } else {
          throw ExpressionException(index, "Binary operator expected instead of operand/unary operation");
        }
      } else if ((ch >= '0') && (ch <= '9') || (ch == '.')) { // is this a value
        if (unary) {
          readValue();
        } else {
          throw ExpressionException(index, "Binary operator expected instead of operand/unary operation");
        }
      } else {
        throw ExpressionException(index, "Invalid symbol");
      }
  }

  while (!stack.isEmpty() && stack.peek().isPopable()) {
    pop();
  }

  if (!stack.isEmpty() || (operands != 1)) {
    throw ExpressionException(index, "Unexpected end of expression");
  }
}

//ExpressionParser::getNodes() const throw() {
//}

ExpressionParser::~ExpressionParser() {
}
