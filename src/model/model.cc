#include "model.h"

namespace s21 {

Model::Model(std::string input_string) { SetExpr(input_string); }

void Model::SetExpr(std::string input_string) {
  input_string_ = input_string;
  PrepareString();
  BuildRpnList();
  Calc(0);
}

long double Model::SetExprAndCalc(std::string input_string, long double x) {
  SetExpr(input_string);
  return Calc(x);
}

void Model::FindAndReplace(std::string find_str, std::string replace_str) {
  std::string::size_type n = 0;
  while ((n = input_string_.find(find_str, n)) != std::string::npos) {
    input_string_.replace(n, find_str.size(), replace_str);
    n += replace_str.size();
  }
}

void Model::PrepareString() {
  input_string_.erase(
      std::remove(input_string_.begin(), input_string_.end(), ' '),
      input_string_.end());
  if (input_string_.size() == 0) throw std::logic_error("Empty string");
  FindAndReplace("asin", "i");
  FindAndReplace("sin", "s");
  FindAndReplace("acos", "o");
  FindAndReplace("cos", "c");
  FindAndReplace("atan", "n");
  FindAndReplace("tan", "t");
  FindAndReplace("sqrt", "q");
  FindAndReplace("ln", "l");
  FindAndReplace("log", "g");
  FindAndReplace("mod", "%");
  FindAndReplace(",", ".");
  FindAndReplace("X", "x");
  for (std::string::iterator it = input_string_.begin();
       it != input_string_.end(); ++it) {
    if (it == input_string_.begin() ||
        kOBoSym.find(*(it - 1)) != std::string::npos) {
      if (*it == '-') *it = '~';
      if (*it == '+') *it = '?';
    }
    if (kASym.find(*it) == std::string::npos)
      throw std::logic_error("Incorrect symbol");
    if ((kFSym.find(*it) != std::string::npos) && *(it + 1) != '(')
      throw std::logic_error("No bracket after function");
    if (*it == ')' && it + 1 != input_string_.end() &&
        (kOBcSym.find(*(it + 1)) == std::string::npos))
      throw std::logic_error("Incorrect symbol after bracket");
  }
}

void Model::BuildRpnList() {
  std::stack<char> stack_char;
  rpn_list_.clear();
  auto it = input_string_.cbegin();
  while (it < input_string_.cend()) {
    if (kDSym.find(*it) != std::string::npos) {
      std::size_t sz = 0;
      rpn_list_.push_back(std::stod(&(*it), &sz));
      it += sz - 1;
    }
    if (*it == 'x') rpn_list_.push_back(-(*it));
    if (kFUBoSym.find(*it) != std::string::npos) stack_char.push(*it);
    if (*it == ')') {
      while (!stack_char.empty() && (stack_char.top() != '(')) {
        rpn_list_.push_back(-stack_char.top());
        stack_char.pop();
      }
      if (stack_char.empty()) {
        throw std::logic_error("Wrong expression");
      } else {
        if (stack_char.top() == '(') stack_char.pop();
      }
    }
    if (kOSym.find(*it) != std::string::npos) {
      while (!stack_char.empty() &&
             ((kFSym.find(stack_char.top()) != std::string::npos) ||
              (Priority(stack_char.top()) >= Priority(*it)))) {
        rpn_list_.push_back(-stack_char.top());
        stack_char.pop();
      }
      stack_char.push(*it);
    }
    ++it;
  }
  while (!stack_char.empty()) {
    if (kOFUSym.find(stack_char.top()) != std::string::npos) {
      rpn_list_.push_back(-stack_char.top());
      stack_char.pop();
    } else {
      throw std::logic_error("Wrong expression");
    }
  }
  if (!stack_char.empty()) throw std::logic_error("Wrong expression");
}

long double Model::Calc(long double x) const {
  auto it = rpn_list_.begin();
  std::stack<long double> stack_ldouble;
  long double res{};
  while (it != rpn_list_.end()) {
    if (*it >= 0) {
      stack_ldouble.push(*it);
    } else {
      char c = (int)round(-(*it));
      if (kOSym.find(c) != std::string::npos && stack_ldouble.size() >= 2) {
        long double a = stack_ldouble.top();
        stack_ldouble.pop();
        long double b = stack_ldouble.top();
        stack_ldouble.pop();
        stack_ldouble.push(MathCalculate(a, b, c));
      } else if (c == 'x') {
        stack_ldouble.push(x);
      } else if (kFUSym.find(c) != std::string::npos &&
                 stack_ldouble.size() >= 1) {
        long double a = stack_ldouble.top();
        stack_ldouble.pop();
        stack_ldouble.push(FuncCalculate(a, c));
      } else {
        throw std::logic_error("Wrong expression");
      }
    }
    ++it;
  }
  if (stack_ldouble.size() > 0) {
    res = stack_ldouble.top();
    stack_ldouble.pop();
  }
  if (stack_ldouble.size() > 0) throw std::logic_error("Wrong expression");
  return res;
}

int Model::Priority(char c) const {
  int res = -1;
  switch (c) {
    case '(':
      res = 0;
      break;
    case ')':
      res = 0;
      break;
    case '+':
      res = 1;
      break;
    case '-':
      res = 1;
      break;
    case '*':
      res = 2;
      break;
    case '/':
      res = 2;
      break;
    case '%':
      res = 2;
      break;
    case '~':
      res = 3;
      break;
    case '?':
      res = 3;
      break;
    case '^':
      res = 4;
      break;
  }
  return res;
}

long double Model::FuncCalculate(long double a, char c) const {
  long double r = 0;
  switch (c) {
    case 's':
      r = sinl(a);
      break;
    case 'i':
      r = asinl(a);
      break;
    case 'c':
      r = cosl(a);
      break;
    case 'o':
      r = acosl(a);
      break;
    case 't':
      r = tanl(a);
      break;
    case 'n':
      r = atanl(a);
      break;
    case 'q':
      r = sqrtl(a);
      break;
    case 'l':
      r = logl(a);
      break;
    case 'g':
      r = log10l(a);
      break;
    case '~':
      r = -a;
      break;
    case '?':
      r = a;
      break;
  }
  return r;
}

long double Model::MathCalculate(long double a, long double b, char c) const {
  long double r = 0;
  switch (c) {
    case '+':
      r = b + a;
      break;
    case '-':
      r = b - a;
      break;
    case '*':
      r = b * a;
      break;
    case '^':
      r = powl(b, a);
      break;
    case '/':
      r = b / a;
      break;
    case '%':
      r = fmodl(b, a);
      break;
  }
  return r;
}

}  // namespace s21
