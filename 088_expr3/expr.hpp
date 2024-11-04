#ifndef _EXPR_H_
#define _EXPR_H_

#include <sstream>
#include <string>

class Expression {
 public:
  virtual ~Expression() {}
  virtual std::string toString() const = 0;
  virtual long evaluate() const = 0;
};

class NumExpression : public Expression {
 private:
  long number;

 public:
  NumExpression(long n) : number(n) {}
  std::string toString() const {
    std::stringstream result;
    result << number;
    return result.str();
  }
  long evaluate() const { return number; }
};

class BinaryExpression : public Expression {
 protected:
  Expression * lhs;
  Expression * rhs;

 public:
  BinaryExpression(Expression * left, Expression * right) : lhs(left), rhs(right) {}
  virtual ~BinaryExpression() {
    delete lhs;
    delete rhs;
  }
};

class PlusExpression : public BinaryExpression {
 public:
  PlusExpression(Expression * left, Expression * right) : BinaryExpression(left, right) {}
  std::string toString() const {
    std::stringstream result;
    result << "(" << lhs->toString() << " + " << rhs->toString() << ")";
    return result.str();
  }
  long evaluate() const { return lhs->evaluate() + rhs->evaluate(); }
};

class MinusExpression : public BinaryExpression {
 public:
  MinusExpression(Expression * left, Expression * right) :
      BinaryExpression(left, right) {}
  std::string toString() const {
    std::stringstream result;
    result << "(" << lhs->toString() << " - " << rhs->toString() << ")";
    return result.str();
  }
  long evaluate() const { return lhs->evaluate() - rhs->evaluate(); }
};

class TimesExpression : public BinaryExpression {
 public:
  TimesExpression(Expression * left, Expression * right) :
      BinaryExpression(left, right) {}
  std::string toString() const {
    std::stringstream result;
    result << "(" << lhs->toString() << " * " << rhs->toString() << ")";
    return result.str();
  }
  long evaluate() const { return lhs->evaluate() * rhs->evaluate(); }
};

class DivExpression : public BinaryExpression {
 public:
  DivExpression(Expression * left, Expression * right) : BinaryExpression(left, right) {}
  std::string toString() const {
    std::stringstream result;
    result << "(" << lhs->toString() << " / " << rhs->toString() << ")";
    return result.str();
  }
  long evaluate() const { return lhs->evaluate() / rhs->evaluate(); }
};

#endif
