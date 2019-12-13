/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/string/FormatOutputStream.h>
#include <base/math/ExpressionParser.h>
#include <base/string/StringOutputStream.h>
#include <stdlib.h> // TAG: remove when done
#include <errno.h> // TAG: remove when done

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

ExpressionEvaluator::ExpressionEvaluator() noexcept
{
}

double ExpressionEvaluator::onConstant(
  unsigned int constant) const throw(ExpressionException)
{
  throw ExpressionException("Invalid constant.", this);
}

double ExpressionEvaluator::onFunction(
  unsigned int function,
  const double* value) const throw(ExpressionException)
{
  throw ExpressionException("Invalid function.", this);
}

double ExpressionEvaluator::evaluate() const throw(ExpressionException)
{
  List<Node>::ReadEnumerator enu = nodes.getReadEnumerator();
  PrimitiveStackArray<double> stack(nodes.getSize());
  MemoryDiff index = -1;
  while (enu.hasNext()) {
    Node node = *enu.next();
    switch (node.type) {
    case ExpressionEvaluator::VALUE:
      stack[++index] = node.value;
      break;
    case ExpressionEvaluator::VARIABLE:
      throw ExpressionException("Variable in constant expression.", this);
      // stack[++index] = lookup node.variable;
    case ExpressionEvaluator::CONSTANT:
      stack[++index] = onConstant(node.constant);
      break;
    case ExpressionEvaluator::BUILTIN:
      switch (node.builtin) {
      case PLUS:
        // stack[index] = stack[index];
        break;
      case MINUS:
        stack[index] = -stack[index];
        break;
      case ADD:
        stack[index - 1] += stack[index];
        --index;
        break;
      case SUBTRACT:
        stack[index - 1] -= stack[index];
        --index;
        break;
      case MULTIPLY:
        stack[index - 1] *= stack[index];
        --index;
        break;
      case DIVIDE:
        if (stack[index] == 0) {
          throw ExpressionException("Division by zero.", this);
        }
        stack[index - 1] /= stack[index];
        --index;
        break;
      case POWER:
        stack[index - 1] = Math::power(stack[index - 1], stack[index]);
        --index;
        break;
      case PARENTHESIS:
        break;
      default:
        throw ExpressionException("Invalid built-in operator.", this);
      }
      break;
    case ExpressionEvaluator::FUNCTION:
      if (node.function.arguments == 0) {
        stack[++index] = onFunction(node.function.id, 0);
      } else {
        if (static_cast<unsigned int>(index + 1) < node.function.arguments) {
          throw ExpressionException("Arguments exhausted.", this);
        }
        index -= node.function.arguments - 1;
        stack[index] = onFunction(node.function.id, &stack[index]);
      }
      break;
    case ExpressionEvaluator::UNKNOWN:
      throw ExpressionException("Unknown.", this);
    }
  }
  return stack[0];
}
  
double ExpressionEvaluator::evaluate(const double* variables) const throw(ExpressionException)
{
  List<Node>::ReadEnumerator enu = nodes.getReadEnumerator();
  PrimitiveStackArray<double> stack(nodes.getSize());
  MemoryDiff index = -1;
  while (enu.hasNext()) {
    Node node = *enu.next();
    switch (node.type) {
    case ExpressionEvaluator::VALUE:
      stack[++index] = node.value;
      break;
    case ExpressionEvaluator::VARIABLE:
      stack[++index] = variables[node.variable];
      break;
    case ExpressionEvaluator::CONSTANT:
      stack[++index] = onConstant(node.constant);
      break;
    case ExpressionEvaluator::BUILTIN:
      switch (node.builtin) {
      case PLUS:
        // stack[index] = stack[index];
        break;
      case MINUS:
        stack[index] = -stack[index];
        break;
      case ADD:
        stack[index - 1] += stack[index];
        --index;
        break;
      case SUBTRACT:
        stack[index - 1] -= stack[index];
        --index;
        break;
      case MULTIPLY:
        stack[index - 1] *= stack[index];
        --index;
        break;
      case DIVIDE:
        if (stack[index] == 0) {
          throw ExpressionException("Division by zero.", this);
        }
        stack[index - 1] /= stack[index];
        --index;
        break;
      case POWER:
        stack[index - 1] = Math::power(stack[index - 1], stack[index]);
        --index;
        break;
      case PARENTHESIS:
        break;
      default:
        throw ExpressionException("Invalid built-in operator.", this);
      }
      break;
    case ExpressionEvaluator::FUNCTION:
      if (node.function.arguments == 0) {
        stack[++index] = onFunction(node.function.id, 0);
      } else {
        if (static_cast<unsigned int>(index + 1) < node.function.arguments) {
          throw ExpressionException("Arguments exhausted.", this);
        }
        index -= node.function.arguments - 1;
        stack[index] = onFunction(node.function.id, &stack[index]);
      }
      break;
    case ExpressionEvaluator::UNKNOWN:
      throw ExpressionException("Unknown.", this);
    }
  }
  return stack[0];
}

void ExpressionEvaluator::evaluate(const double* variables, double* results, unsigned int count) const throw(ExpressionException)
{
  PrimitiveStackArray<double> stack(nodes.getSize());
  for (unsigned int i = 0; i < count; ++i) {
    List<Node>::ReadEnumerator enu = nodes.getReadEnumerator();
    MemoryDiff index = -1;
    while (enu.hasNext()) {
      Node node = *enu.next();
      switch (node.type) {
      case ExpressionEvaluator::VALUE:
        stack[++index] = node.value;
        break;
      case ExpressionEvaluator::VARIABLE:
        stack[++index] = variables[node.variable];
        break;
      case ExpressionEvaluator::CONSTANT:
        stack[++index] = onConstant(node.constant);
        break;
      case ExpressionEvaluator::BUILTIN:
        switch (node.builtin) {
        case PLUS:
          // stack[index] = stack[index];
          break;
        case MINUS:
          stack[index] = -stack[index];
          break;
        case ADD:
          stack[index - 1] += stack[index];
          --index;
          break;
        case SUBTRACT:
          stack[index - 1] -= stack[index];
          --index;
          break;
        case MULTIPLY:
          stack[index - 1] *= stack[index];
          --index;
          break;
        case DIVIDE:
          if (stack[index] == 0) {
            throw ExpressionException("Division by zero.", this);
          }
          stack[index - 1] /= stack[index];
          --index;
          break;
        case POWER:
          stack[index - 1] = Math::power(stack[index - 1], stack[index]);
          --index;
          break;
        case PARENTHESIS:
          break;
        default:
          throw ExpressionException("Invalid built-in operator.", this);
        }
        break;
      case ExpressionEvaluator::FUNCTION:
        if (node.function.arguments == 0) {
          stack[++index] = onFunction(node.function.id, nullptr);
        } else {
          if (static_cast<unsigned int>(index + 1) < node.function.arguments) {
            throw ExpressionException("Arguments exhausted.", this);
          }
          index -= node.function.arguments - 1;
          stack[index] = onFunction(node.function.id, &stack[index]);
        }
        break;
      case ExpressionEvaluator::UNKNOWN:
        throw ExpressionException("Unknown.", this);
      }
    }
    *results++ = stack[0];
  }
}

ExpressionEvaluator::~ExpressionEvaluator() noexcept
{
}

ExpressionProvider::ExpressionProvider() noexcept
{
}

void ExpressionProvider::registerConstant(
  const String& name,
  unsigned int id) throw(AmbiguousRegistration)
{
  if (identifiers.hasKey(name)) {
    throw AmbiguousRegistration(this);
  }
  identifiers[name] = ExpressionEvaluator::makeConstantNode(id);
  constants[id] = name;
}

void ExpressionProvider::registerVariable(
  const String& name,
  unsigned int id) throw(AmbiguousRegistration)
{
  if (identifiers.hasKey(name)) {
    throw AmbiguousRegistration(this);
  }
  identifiers[name] = ExpressionEvaluator::makeVariableNode(id);
  variables[id] = name;
}

void ExpressionProvider::registerFunction(
  const String& name,
  unsigned int id,
  unsigned int arguments) throw(AmbiguousRegistration)
{
  if (identifiers.hasKey(name)) {
    throw AmbiguousRegistration(this);
  }
  identifiers[name] = ExpressionEvaluator::makeFunctionNode(id, arguments);
  functions[id] = name;
}

ExpressionEvaluator::Node ExpressionProvider::getNode(
  const String& name) const throw(InvalidKey)
{
  return identifiers.getValue(name);
}

ExpressionProvider::~ExpressionProvider() noexcept
{
}



ExpressionParser::ExpressionParser(
  const String& _expression,
  ExpressionProvider& _provider) noexcept
  : expression(_expression),
    provider(_provider)
{
  autoRegister = false;
  index = 0;
  length = static_cast<unsigned int>(expression.getLength());
}

void ExpressionParser::pop() throw(ExpressionException) {
  Operation operation = stack.pop();
  if (operands < operation.getArguments()) {
    throw ExpressionException(index, "Operand expected.");
  }
  nodes.append(ExpressionEvaluator::makeNodeFromOperation(operation));
  operands = operands - operation.getArguments() + 1;
}

void ExpressionParser::push(Operation operation) throw(ExpressionException) {
  while (!stack.isEmpty() &&
         stack.peek().isPopable() &&
         (stack.peek().getPrecedence() > operation.getPrecedence())) {
    pop();
  }
  
  if (!stack.isEmpty() &&
      stack.peek().isPopable() &&
      (stack.peek().getPrecedence() == operation.getPrecedence()) &&
      (stack.peek().getGlue() == ExpressionEvaluator::LEFT)) {
    pop();
  }
  stack.push(operation);
  unary = true;
}

void ExpressionParser::readIdentifier() throw(ExpressionException) {
  unsigned int begin = index;
  bassert(
    (index < length) && ASCIITraits::isAlpha(expression[index]),
    ExpressionException(begin, "Not an identifier.")
  );
  ++index;
  
  while (index < length) {
    char ch = expression[index];
    if (!(ASCIITraits::isAlphaNum(ch))) {
      break;
    }
    ++index;
  }
  
  String identifier = expression.substring(begin, index);
  
  if (!provider.isIdentifier(identifier)) {
    if (!autoRegister) {
      throw ExpressionException(begin, "Identifier not recognized.");
    }
    if (unknowns.hasKey(identifier)) {
      nodes.append(unknowns[identifier]);
    } else {
      Node node = ExpressionEvaluator::makeUnknownNode(static_cast<unsigned int>(unknowns.getSize()));
      unknowns[identifier] = node;
      unknownsId[node.unknown] = identifier;
      nodes.append(node);
    }
    ++operands;
    unary = false;
    return;
  }
  
  Node node = provider.getNode(identifier);
  switch (node.type) {
  case ExpressionEvaluator::VARIABLE:
    nodes.append(node);
    ++operands;
    unary = false;
    break;
  case ExpressionEvaluator::CONSTANT:
    nodes.append(node);
    ++operands;
    unary = false;
    break;
  case ExpressionEvaluator::FUNCTION:
    push(
      ExpressionEvaluator::Function(node.function.id, node.function.arguments)
    );
    break;
  default:
    // internal knowledge - not returned by ExpressionProvider
    throw ExpressionException(
      index,
      "Identifier registered as unsupported type."
    );
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
    throw ExpressionException(begin, "Not a number.");
  }

  // read exponent if present
  if ((index < length) && (expression[index] == 'e')) {
    ++index;
    
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
      throw ExpressionException(begin, "Not a number.");
    }
  }
  
  String buffer = expression.substring(begin, index);
  char* end = nullptr;
  // TAG: not the best solution - but it works for now
  double value = strtod(buffer.getElements(), &end);
  if ((end != &buffer.getElements()[index - begin]) || (errno == ERANGE)) {
    throw ExpressionException(begin, "Not a number.");
  }
  nodes.append(ExpressionEvaluator::makeValueNode(value));
  ++operands;
  unary = false;
}

void ExpressionParser::parse() throw(ExpressionException) {
  const ExpressionEvaluator::UnaryOperator plus(
    ExpressionEvaluator::PLUS,
    3,
    ExpressionEvaluator::RIGHT,
    true
  );
  
  const ExpressionEvaluator::UnaryOperator minus(
    ExpressionEvaluator::MINUS,
    3,
    ExpressionEvaluator::RIGHT,
    true
  );
  
  const ExpressionEvaluator::BinaryOperator add(
    ExpressionEvaluator::ADD,
    0,
    ExpressionEvaluator::LEFT,
    true
  );
  
  const ExpressionEvaluator::BinaryOperator subtract(
    ExpressionEvaluator::SUBTRACT,
    0,
    ExpressionEvaluator::LEFT,
    true
  );
  
  const ExpressionEvaluator::BinaryOperator multiply(
    ExpressionEvaluator::MULTIPLY,
    1,
    ExpressionEvaluator::LEFT,
    true
  );
  
  const ExpressionEvaluator::BinaryOperator divide(
    ExpressionEvaluator::DIVIDE,
    1,
    ExpressionEvaluator::LEFT,
    true
  );
  
  const ExpressionEvaluator::BinaryOperator power(
    ExpressionEvaluator::POWER,
    2,
    ExpressionEvaluator::RIGHT,
    true
  );
  
  const ExpressionEvaluator::UnaryOperator parenthesis(
    ExpressionEvaluator::PARENTHESIS,
    100,
    ExpressionEvaluator::RIGHT,
    false
  );
  
  index = 0;
  operands = 0;
  unary = true;
  
  unknowns.removeAll();
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
        throw ExpressionException(
          index,
          "Left operand expected by multiplication operator"
        );
      } else {
        push(multiply);
      }
      ++index;
      break;
    case '/':
      if (unary) {
        throw ExpressionException(
          index,
          "Left operand expected by division operator"
        );
      } else {
        push(divide);
      }
      ++index;
      break;
    case '^':
      if (unary) {
        throw ExpressionException(
          index,
          "Left operand expected by power operator"
        );
      } else {
        push(power);
      }
      ++index;
      break;
    case '(':
      if (unary) {
        push(parenthesis);
      } else {
        throw ExpressionException(
          index,
          "Operator expected instead of opening parenthesis"
        );
      }
      ++index;
      break;
    case ')':
      if (unary) {
        throw ExpressionException(
          index,
          "Operand expected before closing parenthesis"
        );
      } else {
        while (!stack.isEmpty() && stack.peek().isPopable()) {
          pop();
        }
        if (stack.isEmpty() || (stack.peek().getId() != ExpressionEvaluator::PARENTHESIS)) {
          throw ExpressionException(
            index,
            "Closing parenthesis has no matching opening parenthesis"
          );
        }
        stack.pop(); // eat parenthesis
        if (!stack.isEmpty() && stack.peek().isFunction()) {
          Operation operation = stack.pop();
          if (operands < operation.getArguments()) {
            throw ExpressionException(
              index,
              "Operand(s) missing from subexpression"
            );
          }
          operands = operands - operation.getArguments() + 1;
          nodes.append(ExpressionEvaluator::makeNodeFromOperation(operation));
        }
      }
      ++index;
      break;
    case ';':
      if (unary) {
        throw ExpressionException(
          index,
          "Operand expected before argument separator"
        );
      } else {
        while (!stack.isEmpty() && stack.peek().isPopable()) {
          pop();
        }
        if (stack.isEmpty() || (stack.peek().getId() != ExpressionEvaluator::PARENTHESIS)) {
          throw ExpressionException(
            index,
            "Argument separator outside of argument list"
          );
        }
        stack.pop(); // eat parenthesis
        if (stack.isEmpty() || !stack.peek().isFunction()) {
          throw ExpressionException(
            index,
            "Argument separator outside of argument list"
          );
        }
        Operation operation = stack.peek(); // do not remove from stack
        if (operation.getArguments() == 0) {
          throw ExpressionException(
            index,
            "Function does not take any arguments"
          );
        } else if (operation.getArguments() == 1) {
          throw ExpressionException(
            index,
            "Function only takes one argument"
          );
        }
        stack.push(parenthesis);
        unary = true;
      }
      ++index;
      break;
    default:
      char ch = expression[index];
      if (ASCIITraits::isAlpha(ch)) { // is this an identifier
        if (unary) {
          readIdentifier();
        } else {
          throw ExpressionException(
            index,
            "Binary operator expected instead of operand/unary operation"
          );
        }
      } else if (ASCIITraits::isDigit(ch) || (ch == '.')) { // is this a value
        if (unary) {
          readValue();
        } else {
          throw ExpressionException(
            index,
            "Binary operator expected instead of operand/unary operation"
          );
        }
      } else {
        throw ExpressionException(index, "Invalid symbol.");
      }
    }
  }
  
  while (!stack.isEmpty() && stack.peek().isPopable()) {
    pop();
  }
  
  if (!stack.isEmpty() || (operands != 1)) {
    throw ExpressionException(index, "Unexpected end of expression.");
  }
}

String ExpressionParser::getString() const throw(ExpressionException) {
  List<Node>::ReadEnumerator enu = nodes.getReadEnumerator();
  Stack<String> stack;
  while (enu.hasNext()) {
    Node node = *enu.next();
    switch (node.type) {
    case ExpressionEvaluator::VALUE:
      {
        StringOutputStream stream;
        stream << node.value << FLUSH;
        stack.push(stream.getString());
      }
      break;
    case ExpressionEvaluator::VARIABLE:
      {
        StringOutputStream stream;
        stream << provider.getVariable(node.variable) << FLUSH;
        stack.push(stream.getString());
      }
      break;
    case ExpressionEvaluator::CONSTANT:
      {
        StringOutputStream stream;
        stream << provider.getConstant(node.constant) << FLUSH;
        stack.push(stream.getString());
      }
      break;
    case ExpressionEvaluator::BUILTIN:
      switch (node.builtin) {
      case ExpressionEvaluator::PLUS:
        {
          String argument = stack.pop();
          stack.push(MESSAGE("+") + argument);
        }
        break;
      case ExpressionEvaluator::MINUS:
        {
          String argument = stack.pop();
          stack.push(MESSAGE("-") + argument);
        }
        break;
      case ExpressionEvaluator::ADD:
        {
          String right = stack.pop();
          String left = stack.pop();
          stack.push(MESSAGE("(") + left + MESSAGE("+") + right + MESSAGE(")"));
        }
        break;
      case ExpressionEvaluator::SUBTRACT:
        {
          String right = stack.pop();
          String left = stack.pop();
          stack.push(MESSAGE("(") + left + MESSAGE("-") + right + MESSAGE(")"));
        }
        break;
      case ExpressionEvaluator::MULTIPLY:
        {
          String right = stack.pop();
          String left = stack.pop();
          stack.push(MESSAGE("(") + left + MESSAGE("*") + right + MESSAGE(")"));
        }
        break;
      case ExpressionEvaluator::DIVIDE:
        {
          String right = stack.pop();
          String left = stack.pop();
          stack.push(MESSAGE("(") + left + MESSAGE("/") + right + MESSAGE(")"));
        }
        break;
      case ExpressionEvaluator::POWER:
        {
          String right = stack.pop();
          String left = stack.pop();
          stack.push(MESSAGE("(") + left + MESSAGE("^") + right + MESSAGE(")"));
        }
        break;
      case ExpressionEvaluator::PARENTHESIS:
        break;
      default:
        throw ExpressionException("Invalid built-in operator.", this);
      }
      break;
    case ExpressionEvaluator::FUNCTION:
      {
        if (static_cast<unsigned int>(index + 1) < node.function.arguments) {
          throw ExpressionException("Arguments exhausted.", this);
        }
        String arguments;
        if (node.function.arguments > 0) {
          for (unsigned int i = 0; (i + 1) < node.function.arguments; ++i) {
            arguments = String("; ") + stack.pop() + arguments;
          }
          arguments = stack.pop() + arguments;
        }
        StringOutputStream stream;
        stream << provider.getFunction(node.function.id)
               << '(' << arguments << ')' << FLUSH;
        stack.push(stream.getString());
      }
      break;
    case ExpressionEvaluator::UNKNOWN:
      stack.push(unknownsId[node.unknown]);
      break;
    }
  }
  return stack.pop();
}

ExpressionParser::~ExpressionParser() noexcept
{
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
