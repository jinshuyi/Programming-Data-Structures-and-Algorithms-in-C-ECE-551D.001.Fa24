// poly.hpp

#include <map>      // 包含 map 以使用 terms
#include <ostream>  // 包含 ostream 以支持输出操作符

template<typename NumT>
class Polynomial {
 private:
  std::map<unsigned, NumT> terms;

 public:
  // 默认构造函数
  Polynomial() : terms() {}

  // 添加一个项
  void addTerm(NumT coefficient, unsigned exponent) {
    terms[exponent] += coefficient;
    if (terms[exponent] == 0) {
      terms.erase(exponent);
    }
  }

  // 操作符重载：加法
  Polynomial operator+(const Polynomial & rhs) const {
    Polynomial result = *this;
    for (const std::pair<const unsigned, NumT> & term : rhs.terms) {
      result.addTerm(term.second, term.first);
    }
    return result;
  }

  // 操作符重载：取负
  Polynomial operator-() const {
    Polynomial result;
    for (const std::pair<const unsigned, NumT> & term : terms) {
      result.terms[term.first] = -term.second;
    }
    return result;
  }

  // 操作符重载：乘以标量
  Polynomial operator*(const NumT & n) const {
    Polynomial result;
    for (const std::pair<const unsigned, NumT> & term : terms) {
      result.terms[term.first] = term.second * n;
    }
    return result;
  }

  // 操作符重载：多项式乘法
  Polynomial operator*(const Polynomial & rhs) const {
    Polynomial result;
    for (const std::pair<const unsigned, NumT> & term1 : terms) {
      for (const std::pair<const unsigned, NumT> & term2 : rhs.terms) {
        result.addTerm(term1.second * term2.second, term1.first + term2.first);
      }
    }
    return result;
  }

  // 操作符重载：累乘赋值
  Polynomial & operator*=(const NumT & n) {
    for (std::pair<const unsigned, NumT> & term : terms) {
      term.second *= n;
    }
    return *this;
  }

  // 输出操作符重载
  friend std::ostream & operator<<(std::ostream & os, const Polynomial & p) {
    bool first = true;
    for (auto it = p.terms.rbegin(); it != p.terms.rend(); ++it) {
      if (!first && it->second > 0) {
        os << " + ";
      }
      if (it->second < 0) {
        os << " - ";
      }
      if (std::abs(it->second) != 1 || it->first == 0) {
        os << std::abs(it->second);
      }
      if (it->first > 0) {
        os << "x";
        if (it->first > 1) {
          os << "^" << it->first;
        }
      }
      first = false;
    }
    return os;
  }
};
