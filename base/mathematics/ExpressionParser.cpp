/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/mathematics/ExpressionParser.h>
#include <stdlib.h>
#include <errno.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

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
    result.func.function = id;
    result.func.arguments = arguments;
    return result;
  }

  EvaluationNode makeNodeFromOperation(Operation opr) {
    EvaluationNode result;
    // must not be parenthesis
    if (opr.isBuiltin()) {
      result.type = BUILTIN;
      result.builtin = opr.getId();
    } else {
      result.type = FUNCTION;
      result.func.function = opr.getId();
      result.func.arguments = opr.getArguments();
    }
    return result;
  }

  // built-in unary operator
  class UnaryOperator : public Operation {
  public:
    UnaryOperator(unsigned int i, unsigned int p, Glue g, bool po) throw() : Operation(i, 1, p, g, true, po, false) {}
  };

  // built-in binary operator
  class BinaryOperator : public Operation {
  public:
    BinaryOperator(unsigned int i, unsigned int p, Glue g, bool po) throw() : Operation(i, 2, p, g, true, po, false) {}
  };

  class Function : public Operation {
  public:
    Function(unsigned int i, unsigned int a) throw() : Operation(i, a, 100, RIGHT, false, false, true) {}
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



ExpressionParser::ExpressionParser(const String& e, ExpressionProvider& p) throw() : expression(e), provider(p) {
  length = expression.getLength();
}

void ExpressionParser::pop() throw(ExpressionException) {
  Operation opr = stack.pop();
  if (operands >= opr.getArguments()) {
    nodes.append(makeNodeFromOperation(opr));
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
       (stack.peek().getGlue() == eval::LEFT)
  ) {
    pop();
  }

  stack.push(opr);
  unary = true;
}

void ExpressionParser::readIdentifier() throw(ExpressionException) {
  unsigned int begin = index;

  while (index < length) {
    char ch = expression[index];
    if (!(ASCIITraits::isLower(ch) || ASCIITraits::isDigit(ch))) {
      break;
    }
    ++index;
  }
  assert(index > begin, ExpressionException(begin, "Not an identifier")); // make sure an identifier has been read

  String identifier = expression.substring(begin, index);

  try {
    Node node = provider.getNode(identifier.getElements());
    switch (node.type) {
    case VARIABLE:
      nodes.append(node);
      ++operands;
      unary = false;
      break;
    case CONSTANT:
      nodes.append(node);
      ++operands;
      unary = false;
      break;
    case FUNCTION:
      push(Function(node.func.function, node.func.arguments));
      break;
    default:
      // internal knowledge - not returned by ExpressionProvider
      throw ExpressionException(index, "Identifier registered as unsupported type");
    }
  } catch(InvalidKey e) {
    throw ExpressionException(begin, "Identifier not recognized");
//    if (autoRegisterAsVariable) {
//      unknowns[name] = makeVariable(id???);
//      node = identifiers[name];
//    } else {
//    }
  }
}

void ExpressionParser::readValue() throw(ExpressionException) {
  unsigned int begin = index;
  bool digits = false;

  // read sign if present
  if (index < length) {
    if (expression[index] == '+') {
      ++index;
    } else if (expression[index] == '-') {
      ++index;
    }
  }

  // read digits if present before possible dot
  if ((index < length) && ASCIITraits::isDigit(expression[index])) {
    digits = true;
    while ((index < length) && ASCIITraits::isDigit(expression[index])) {
      ++index;
    }
  }

  // read dot if present
  if ((index < length) && (expression[index] == '.')) {
    ++index;

    // read digits if present after dot
    if ((index < length) && ASCIITraits::isDigit(expression[index])) {
      digits = true;
      while ((index < length) && ASCIITraits::isDigit(expression[index])) {
        ++index;
      }
    }
  }

  if (!digits) {
    throw ExpressionException(begin, "Not a number");
  }

  // read exponent if present
  if ((index < length) && (expression[index] == 'e')) {

    // read sign of exponent if present
    if (index < length) {
      if (expression[index] == '+') {
        ++index;
      } else if (expression[index] == '-') {
        ++index;
      }
    }

    // read exponent value
    if ((index < length) && ASCIITraits::isDigit(expression[index])) {
      while ((index < length) && ASCIITraits::isDigit(expression[index])) {
        ++index;
      }
    } else {
      throw ExpressionException(begin, "Not a number");
    }
  }

  //char buffer[index - begin + 1]; // i is index of character after number chars
  String buffer = expression.substring(begin, index);
  char* end;
  double value = strtod(buffer.getElements(), &end); // TAG: not the best solution - but it works for now
  if ((end != &buffer.getElements()[index - begin]) || (errno == ERANGE)) {
    throw ExpressionException(begin, "Not a number");
  }

  nodes.append(makeValueNode(value));
  ++operands;
  unary = false;
}

void ExpressionParser::parse() throw(ExpressionException) {
  unsigned int index = 0;
  unsigned int operands = 0;
  bool unary = true;
  stack.removeAll();
  nodes.removeAll();

  while (index < length) { // until all chars have been read
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

        while (!stack.isEmpty() && stack.peek().isPopable()) {
          pop();
        }

        if (!stack.isEmpty() && !stack.peek().isPopable()) {
          Operation opr = stack.pop(); // get object - function or parenthesis

          if (opr.isFunction()) {
            if (operands < opr.getArguments()) {
              throw ExpressionException(index, "Operand(s) missing from subexpression");
            }
            operands = operands - opr.getArguments() + 1;
            nodes.append(makeNodeFromOperation(opr));
          } else { // must be opening parenthesis
            // nothing should be added to the nodes list
          }
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

        if (!stack.isEmpty() && !stack.peek().isPopable()) {
          Operation opr = stack.peek(); // get function? - do not remove from stack

          if (opr.isFunction()) {
            if (opr.getArguments() == 0) {
              throw ExpressionException(index, "Function does not take any arguments");
            } else if (opr.getArguments() == 1) {
              throw ExpressionException(index, "Function only takes one argument");
            }
          } else {
            throw ExpressionException(index, "Opening parenthesis does not take argument list");
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
      if (ASCIITraits::isLower(ch)) { // is this an identifier
        if (unary) {
          readIdentifier();
        } else {
          throw ExpressionException(index, "Binary operator expected instead of operand/unary operation");
        }
      } else if (ASCIITraits::isDigit(ch) || (ch == '.')) { // is this a value
        if (unary) {
          readValue();
        } else {
          throw ExpressionException(index, "Binary operator expected instead of operand/unary operation");
        }
      } else {
        throw ExpressionException(index, "Invalid symbol");
      }
    }
  }

  while (!stack.isEmpty() && stack.peek().isPopable()) {
    pop();
  }

  if (!stack.isEmpty() || (operands != 1)) {
    throw ExpressionException(index, "Unexpected end of expression");
  }
}

//List<>::ListEnumeration ExpressionParser::getNodes() const throw() {
// return List<>::getEnumeration();
//}

ExpressionParser::~ExpressionParser() {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
