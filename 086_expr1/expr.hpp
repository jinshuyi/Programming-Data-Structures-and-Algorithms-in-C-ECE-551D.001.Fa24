#ifndef _EXPR_H_
#define _EXPR_H_

#include <sstream>
#include <string>

class Expression {
 public:
  virtual std::string toString() const = 0;
  virtual ~Expression() {}
};

class NumExpression : public Expression {
 private:
  long number;

 public:
  NumExpression(long n) : number(n) {}
  virtual std::string toString() const {
    std::ostringstream result;
    result << number;
    return result.str();
  }
  virtual ~NumExpression() {}
};

class PlusExpression : public Expression {
 private:
  Expression * lhs;
  Expression * rhs;

 public:
  PlusExpression(Expression * l, Expression * r) : lhs(l), rhs(r) {}
  virtual std::string toString() const {
    std::ostringstream result;
    result << "(" << lhs->toString() << " + " << rhs->toString() << ")";
    return result.str();
  }
  virtual ~PlusExpression() {
    delete lhs;
    delete rhs;
  }
};

#endif
