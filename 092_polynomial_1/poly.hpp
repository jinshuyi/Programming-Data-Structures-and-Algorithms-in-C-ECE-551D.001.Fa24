#include <iostream>
#include <map>
#include <stdexcept>

template<typename NumT>
class Polynomial {
 private:
  std::map<unsigned, NumT> terms;

 public:
  // 默认构造函数，初始化为 0
  Polynomial() : terms{{0, NumT()}} {}

  // 加法运算符重载
  Polynomial operator+(const Polynomial & rhs) const {
    Polynomial result = *this;
    for (const auto & [exp, coef] : rhs.terms) {
      result.addTerm(coef, exp);
    }
    return result;
  }

  // 负号运算符重载
  Polynomial operator-() const {
    Polynomial result;
    for (const auto & [exp, coef] : terms) {
      result.terms[exp] = -coef;
    }
    return result;
  }

  // 减法运算符重载
  Polynomial operator-(const Polynomial & rhs) const { return *this + (-rhs); }

  // 乘以标量的运算符重载
  Polynomial operator*(const NumT & n) const {
    Polynomial result;
    for (const auto & [exp, coef] : terms) {
      result.terms[exp] = coef * n;
    }
    return result;
  }

  // 乘以多项式的运算符重载
  Polynomial operator*(const Polynomial & rhs) const {
    Polynomial result;
    for (const auto & [exp1, coef1] : terms) {
      for (const auto & [exp2, coef2] : rhs.terms) {
        result.addTerm(coef1 * coef2, exp1 + exp2);
      }
    }
    return result;
  }

  // 添加项
  void addTerm(const NumT & c, unsigned p) {
    if (terms.count(p)) {
      terms[p] += c;
    }
    else {
      terms[p] = c;
    }
    if (terms[p] == NumT()) {  // 如果系数为0，则移除该项
      terms.erase(p);
    }
  }

  // 输出运算符重载
  template<typename N>
  friend std::ostream & operator<<(std::ostream & os, const Polynomial<N> & p) {
    bool first = true;
    for (auto it = p.terms.rbegin(); it != p.terms.rend(); ++it) {
      if (!first) {
        os << " + ";
      }
      os << it->second << "*x^" << it->first;
      first = false;
    }
    if (p.terms.empty()) {
      os << "0";
    }
    return os;
  }
};
