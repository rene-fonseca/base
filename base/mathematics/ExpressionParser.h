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
#include <base/string/String.h>
#include <base/collection/AmbiguousRegistration.h>
#include <base/collection/Stack.h>
#include <base/collection/List.h>
#include <base/collection/HashTable.h>
#include <base/mathematics/ExpressionException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Arithmetic expression evaluator.
  
  @short Expression evaluator
  @see ExpressionParser
  @ingroup mathematics
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API ExpressionEvaluator : public Object {
public:

  /** The valid types of a node in an expression. */
  enum NodeType {
    VALUE, /**< A value. */
    VARIABLE, /**< A variable. */
    CONSTANT, /**< A constant. */
    BUILTIN, /**< Built-in. */
    FUNCTION, /**< A function. */
    UNKNOWN /**< Unknown. */
  };

  /* Node of expression. */
  struct Node {
    NodeType type;
    union {
      double value; // type should be determined by template argument
      unsigned int variable;
      unsigned int constant;
      unsigned int builtin;
      struct {
        unsigned int id;
        unsigned int arguments;
      } function;
      unsigned int unknown;
    };
  };
  
  /* Determines the order when operators have same precedence. */
  enum Glue {
    LEFT, /**< Bind to the left. */
    RIGHT /**< Bind to the right. */
  };

  /** Built-in operators. */
  enum Builtin {
    PLUS = 0, /**< Plus. */
    MINUS = 1, /**< Minus. */
    ADD = 2, /**< Add. */
    SUBTRACT = 3, /**< Subtract. */
    MULTIPLY = 4, /**< Multiply. */
    DIVIDE = 5, /**< Divide. */
    POWER = 6, /**< Power. */
    PARENTHESIS = 7 /**< Parenthesis. */
  };
  
  class Operation {
  private:
    
    unsigned int id = 0;
    unsigned int arguments = 0;
    unsigned int precedence = 0;
    Glue glue = LEFT;
    bool builtin = false;
    bool popable = false;
    bool function = false;
  public:
    
    inline Operation(
      unsigned int _id,
      unsigned int _arguments,
      unsigned int _precedence,
      Glue _glue,
      bool _builtin,
      bool _popable,
      bool _function) throw()
      : id(_id),
        arguments(_arguments),
        precedence(_precedence),
        glue(_glue),
        builtin(_builtin),
        popable(_popable),
        function(_function) {
    }
    
    inline unsigned int getArguments() const throw() {
      return arguments;
    }
    
    inline unsigned int getPrecedence() const throw() {
      return precedence;
    }
    
    inline Glue getGlue() const throw() {
      return glue;
    }
    
    inline bool isBuiltin() const throw() {
      return builtin;
    }
    
    inline bool isPopable() const throw() {
      return popable;
    }

    inline bool isFunction() const throw() {
      return function;
    }

    inline unsigned int getId() const throw() {
      return id;
    }
  };
  
  /* Built-in unary operator. */
  class UnaryOperator : public Operation {
  public:
    
    inline UnaryOperator(
      unsigned int id,
      unsigned int precedence,
      Glue glue,
      bool popable) throw()
      : Operation(id, 1, precedence, glue, true, popable, false) {
    }
  };
  
  /* Built-in binary operator. */
  class BinaryOperator : public Operation {
  public:
    
    inline BinaryOperator(
      unsigned int id,
      unsigned int precedence,
      Glue glue,
      bool popable) throw()
      : Operation(id, 2, precedence, glue, true, popable, false) {
    }
  };
  
  /* Function. */
  class Function : public Operation {
  public:
    
    inline Function(unsigned int id, unsigned int arguments) throw()
      : Operation(id, arguments, 100, RIGHT, false, false, true) {
    }
  };
  
  static inline Node makeValueNode(double value) throw() {
    Node result;
    result.type = VALUE;
    result.value = value;
    return result;
  }
  
  static inline Node makeConstantNode(unsigned int id) throw() {
    Node result;
    result.type = CONSTANT;
    result.constant = id;
    return result;
  }
  
  static inline Node makeVariableNode(unsigned int id) throw() {
    Node result;
    result.type = VARIABLE;
    result.variable = id;
    return result;
  }  
 
  static inline Node makeFunctionNode(unsigned int id, unsigned int arguments) throw() {
    Node result;
    result.type = FUNCTION;
    result.function.id = id;
    result.function.arguments = arguments;
    return result;
  }
  
  static inline Node makeUnknownNode(unsigned int id) throw() {
    Node result;
    result.type = UNKNOWN;
    result.unknown = id;
    return result;
  }
  
  static inline Node makeNodeFromOperation(Operation operation) throw() {
    Node result;
    // must not be parenthesis
    if (operation.isBuiltin()) {
      result.type = BUILTIN;
      result.builtin = operation.getId();
    } else {
      result.type = FUNCTION;
      result.function.id = operation.getId();
      result.function.arguments = operation.getArguments();
    }
    return result;
  }
private:

  /** Container for the names of the variables in the formula. */
  List<String> variables;
  /** Container for the parsed formula. */
  List<Node> nodes;
public:
  
  virtual double onConstant(
    unsigned int constant) const throw(ExpressionException);
  
  virtual double onFunction(
    unsigned int function,
    const double* value) const throw(ExpressionException);
  
  /**
    Initializes expression evaluator.
  */
  ExpressionEvaluator() throw();

  /**
    Returns the expression.
  */
  inline List<Node> getExpression() const throw() {
    return nodes;
  }

  /**
    Sets the expression.
  */
  inline void setExpression(const List<Node>& nodes) throw() {
    this->nodes = nodes;
  }

  /**
    Returns the names of variables of the formula in the order expected by the
    evaluate methods.
  */
  inline List<String> getVariables() const throw() {
    return variables;
  }
  
  /**
    Sets the variables.
  */
  inline void setVariables(const List<String>& variables) throw() {
    this->variables = variables;
  }
  
  /**
    Evaluates the constant expression once.
    
    @return The result of the evaluation.
  */
  double evaluate() const throw(ExpressionException);
  
  /**
    Evaluates the formula once.

    @param variables The array containing the values of the variables (e.g. x0,
    y0, z0).
    @return The result of the evaluation.
  */
  double evaluate(const double* variables) const throw(ExpressionException);
  
  /**
    Evaluates the formula several times.

    @param variable The values of the variables (e.g. x0, y0, z0, x1, y1, z1,
    x2, ...).
    @param results The array to store the results in. (e.g. res0, res1, res3,
    ...).
    @param count The desired number of evaluations.
  */
  void evaluate(
    const double* variables,
    double* results,
    unsigned int count) const throw(ExpressionException);
  
  virtual ~ExpressionEvaluator() throw();
};



/**
  The expression provider is responsible for mapping identifiers into constants,
  variables, and functions.

  @short Expression provider.
  @see ExpressionParser
  @version 1.0
*/

class ExpressionProvider : public Object {
protected:
  
  /** Collection holding the registered identifiers. */
  HashTable<String, ExpressionEvaluator::Node> identifiers;
  /** Constants. */
  HashTable<unsigned int, String> constants;
  /** Variables. */
  HashTable<unsigned int, String> variables;
  /** Functions. */
  HashTable<unsigned int, String> functions;
public:
  
  /**
    Initializes an expression provider.
  */
  ExpressionProvider() throw();

  /**
    Registers an identifier as a constant.

    @param name The identifier to be registered.
    @param id The desired id of the constant.
  */
  void registerConstant(
    const String& name,
    unsigned int id) throw(AmbiguousRegistration);

  /**
    Registers an identifier as a variable.

    @param name The identifier to be registered.
    @param id The desired id of the variable.
  */
  void registerVariable(
    const String& name,
    unsigned int id) throw(AmbiguousRegistration);

  /**
    Registers an identifier as a function.

    @param name The identifier to be registered.
    @param id The desired id of the function.
    @param arguments The number of arguments taken by the function.
  */
  void registerFunction(
    const String& name,
    unsigned int id,
    unsigned int arguments) throw(AmbiguousRegistration);

  /**
    Returns true if the identifier has been registered.
  */
  inline bool isIdentifier(const String& name) const throw() {
    return identifiers.isKey(name);
  }

  /**
    Returns the name of the constant with the given id.
  */
  inline String getConstant(unsigned int id) const throw() {
    return constants[id];
  }
  
  /**
    Returns the name of the variable with the given id.
  */
  inline String getVariable(unsigned int id) const throw() {
    return variables[id];
  }
  
  /**
    Returns the name of the function with the given id.
  */
  inline String getFunction(unsigned int id) const throw() {
    return functions[id];
  }
  
  /**
    Returns the evaluation node associated with the specified identifier.
    Raises InvalidKey if the identifier has not been registered.

    @param name The identifier to be looked up.
  */
  ExpressionEvaluator::Node getNode(const String& name) const throw(InvalidKey);
  
  /**
    Destroys the expression provider.
  */
  ~ExpressionProvider() throw();
};



/**
  Arithmetic expression parser.
  
  @short Expression parser.
  @see ExpressionProvider
  @see ExpressionEvaluator
  @ingroup mathematics
  @version 1.0
*/

class ExpressionParser : public Object {
protected:
  
  typedef ExpressionEvaluator::Node Node;
  typedef ExpressionEvaluator::Operation Operation;

  /** The auto-register mode. Default is disabled. */
  bool autoRegister = false;
  /** Unknowns. */
  HashTable<String, Node> unknowns;
  /** Lookup of the unknowns by id. */
  HashTable<unsigned int, String> unknownsId;
  /** The expression representation. */
  String expression;
  /** The expression provider. */
  ExpressionProvider& provider;
  /** Stack holding operations. */
  Stack<Operation> stack;
  /** List holding the parsed expression. */
  List<Node> nodes;
  /** Specifies the current number of operands. */
  unsigned int operands = 0;
  /** Specifies that an unary expression is expected. */
  bool unary = false;
  /** The current position in the representation. */
  unsigned int index = 0;
  /** The length of the expression. */
  unsigned int length = 0;
  
  /**
    Pops one operation from the stack.
  */
  void pop() throw(ExpressionException);
  
  /**
    Pushed the specified operation onto the stack.
  */
  void push(Operation operation) throw(ExpressionException);
  
  /**
    Reads an identifier from the representation.
  */
  void readIdentifier() throw(ExpressionException);
  
  /**
    Reads a number from the representation.
  */
  void readValue() throw(ExpressionException);
public:  
  
  /**
    Initializes an expression parser.
    
    @param expression The string representation of the expression to be parsed
    (e.g. "4+5*x/7").
    @param provider The expression provider.
  */
  ExpressionParser(const String& expression, ExpressionProvider& provider) throw();
  
  /**
    Parses the specified arithmetic expression representation.
  */
  void parse() throw(ExpressionException);
  
  /**
    Returns the result of the parser.
  */
  inline List<Node> getExpression() const throw() {
    return nodes;
  }
  
  /**
    Returns the auto-register mode.
  */
  inline bool getAutoRegister() const throw() {
    return autoRegister;
  }
  
  /**
    Sets the auto-register mode.
  */
  inline void setAutoRegister(bool value) throw() {
    autoRegister = value;
  }
  
  /**
    Returns the unknowns.
  */
  inline HashTable<String, Node> getUnknowns() const throw() {
    return unknowns;
  }
  
  /**
    Returns true if the expression contains unknowns.
  */
  inline bool hasUnknowns() const throw() {
    return unknowns.getSize();
  }
  
  /**
    Builds string expression from internal representation.
  */
  String getString() const throw(ExpressionException);
  
  /**
    Destroys the expression parser.
  */
  ~ExpressionParser() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
