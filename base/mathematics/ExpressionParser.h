/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MATHEMATICS__EVALUATE_H
#define _DK_SDU_MIP__BASE_MATHEMATICS__EVALUATE_H

#include "base/Object.h"
#include "base/string/String.h"
#include "base/string/FormatOutputStream.h"
#include "base/collection/Stack.h"
#include "base/collection/List.h"
#include "base/collection/Map.h"
#include "ExpressionException.h"
#include "AmbiguousRegistration.h"

namespace eval {

  /** The valid types of a node in an expression. */
  typedef enum {VALUE, VARIABLE, CONSTANT, BUILTIN, FUNCTION, UNKNOWN} NodeType;

  /** Node of expression. */
  struct EvaluationNode {
    NodeType type;
    union {
      double value; // type should be determined by template argument
      unsigned int variable;
      unsigned int constant;
      unsigned int builtin;
      struct {
        unsigned int function;
        unsigned int arguments;
      };
      unsigned int unknown;
    };
  };

  typedef EvaluationNode Node;

  typedef enum {LEFT, RIGHT} Glue; // determines order when operators have same precedence

  class Operation {
  private:
    unsigned int id;
    unsigned int arguments;
    unsigned int precedence;
    Glue glue;
    bool builtin;
    bool popable;
  public:
    Operation(unsigned int i, unsigned int a, unsigned int p, Glue g, bool b, bool po) throw() : id(i), arguments(a), precedence(p), glue(g), builtin(b), popable(po) {}
    unsigned int getArguments() const throw() {return arguments;}
    unsigned int getPrecedence() const throw() {return precedence;}
    Glue getGlue() const throw() {return glue;}
    bool isBuiltin() const throw() {return builtin;}
    bool isPopable() const throw() {return popable;}
    unsigned int getId() const throw() {return id;}
  };

};



/**
  Arithmetic expression evaluator.

  @see ExpressionParser
  @author René Møller Fonseca
  @version 1.0
*/

class ExpressionEvaluator : public Object {
private:

  /** Container for the names of the variables in the formula. */
  List<String<> > variableNames;
  /** Container for the parsed formula. */
  eval::EvaluationNode* nodes;
public:

  /**
    Initializes expression evaluator.
  */
  ExpressionEvaluator();

  /**
    Returns the names of variables of the formula in the order expected by the evaluate methods.
  */
  const List<String<> > getVariables() const throw();

  /**
    Evaluates the formula once.

    @param variables The array containing the values of the variables (e.g. x0, y0, z0).
    @return The result of the evaluation.
  */
  double evaluate(const double* variables) const throw();

  /**
    Evaluates the formula several times.

    @param variable The values of the variables (e.g. x0, y0, z0, x1, y1, z1, x2, ...).
    @param results The array to store the results in. (e.g. res0, res1, res3, ...).
    @param count The desired number of evaluations.
  */
  void evaluate(const double* variables, double* results, unsigned int count) const throw();

  /**
    Destroys the expression evaluator.
  */
  ~ExpressionEvaluator() throw();
};



/**
  Expression provider responsible for mapping identifiers into constants, variables, and functions.

  @author René Møller Fonseca
  @version 1.0
*/

class ExpressionProvider : public Object {
protected:

  /** Collection holding the registrated identifiers. */
  Map<const char*, eval::EvaluationNode> identifiers;
public:

  /**
    Initializes an expression provider.
  */
  ExpressionProvider() throw();

  /**
    Registers an identifier as a constant.

    @param name The identifier.
    @param id The desired id.
  */
  void registerConstant(const char* name, unsigned int id) throw(AmbiguousRegistration);

  /**
    Registers an identifier as a variable.

    @param name The identifier.
    @param id The desired id.
  */
  void registerVariable(const char* name, unsigned int id) throw(AmbiguousRegistration);

  /**
    Registers an identifier as a function.

    @param name The identifier.
    @param id The desired id of the.
    @param arguments The number of arguments.
  */
  void registerFunction(const char* name, unsigned int id, unsigned int arguments) throw(AmbiguousRegistration);

  /**
    Returns the evaluation node associated with the specified identifier.
  */
  eval::EvaluationNode getNode(const char* name) const throw(InvalidKey);

  /**
    Destroys the expression provider.
  */
  ~ExpressionProvider();
};



/**
  Arithmetic expression parser.

  @see ExpressionProvider
  @see ExpressionEvaluator
  @author René Møller Fonseca
  @version 1.0
*/

class ExpressionParser : public Object {
protected:

  /** The expression representation. */
  String<> expression;
  /** The expression provider. */
  ExpressionProvider& provider;
  /** Stack holding operations. */
  Stack<eval::Operation> stack;
  /** List holding the parsed expression. */
  List<eval::EvaluationNode> nodes;
  /** Specifies the current number of operands. */
  unsigned int operands;
  /** Specifies that an unary expression is expected. */
  bool unary;
  /** The current position in the representation. */
  unsigned int index;
  /** The last character in the expression. */
  unsigned int last;

  /**
    Pops one operation from the stack.
  */
  void pop() throw(ExpressionException);

  /**
    Pushed the specified operation onto the stack.
  */
  void push(eval::Operation opr) throw(ExpressionException);

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
  */
  ExpressionParser(const String<>& expression, ExpressionProvider& provider) throw();

  /**
    Parses the specified arithmetic expression representation.

    @param expression The string representation of the expression to be parsed (e.g. "4+5*x/7").
  */
  void parse() throw(ExpressionException);

  /**
    Returns the result of the parser.
  */
// Nodes getNodes const throw(); - enumeration???

  /**
    Destroys the expression parser.
  */
//  ~ExpressionParser();
};

#endif
