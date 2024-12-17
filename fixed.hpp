#pragma once
#include <bits/stdc++.h>
#include <cstdint>
#include <cstdio>
#include <tuple>

template <typename T>
concept is_number_class =
    requires(T obj) { requires(std::convertible_to<decltype(obj.v()), int>); };

template <is_number_class First, is_number_class Second>
constexpr static auto align_values(First f, Second s) {
  if constexpr (First::n > Second::n) {
    if constexpr (First::k > Second::k) {

      return std::make_tuple<First, First>(
          First::from_raw(f.v()),
          First::from_raw((int64_t)s.v() << (First::k - Second::k)));
    } else {
      return std::make_tuple<First, First>(
          First::from_raw(f.v()),
          First::from_raw((int64_t)s.v() >> (Second::k - First::k)));
    }
  } else {
    if constexpr (First::k > Second::k) {
      return std::make_tuple<Second, Second>(
          Second::from_raw((int64_t)f.v() >> (First::k - Second::k)),
          Second::from_raw(s.v()));
    } else {
      return std::make_tuple<Second, Second>(
          Second::from_raw((int64_t)f.v() << (Second::k - First::k)),
          Second::from_raw(s.v()));
    }
  }
}

template <int N, int K> class Fixed;
template <int N, int K> class Fast_Fixed;

class Float;
class Double;

using namespace std;
template <int N, int K> class Fast_Fixed {

public:
  constexpr static int n = N;
  constexpr static int k = K;
  constexpr Fast_Fixed(int v) {
    if constexpr (N <= 8) {
      v8 = v * (1 << K);
      sz = 8;
    } else if constexpr (N <= 16) {
      v16 = v * (1 << K);
      sz = 16;
    } else if constexpr (N <= 32) {
      v32 = v * (1 << K);
      sz = 32;
    } else if constexpr (N <= 64) {
      v64 = v * (1 << K);
      sz = 64;
    } else {
      static_assert(false, "Size of Fast Fixed class is wrong");
    }
  }
  constexpr Fast_Fixed(float f) {
    if constexpr (N <= 8) {
      v8 = f * (1 << K);
      sz = 8;
    } else if constexpr (N <= 16) {
      v16 = f * (1 << K);
      sz = 16;
    } else if constexpr (N <= 32) {
      v32 = f * (1 << K);
      sz = 32;
    } else if constexpr (N <= 64) {
      v64 = f * (1 << K);
      sz = 64;
    } else {
      static_assert(false, "Size of Fast Fixed class is wrong");
    }
  }
  constexpr Fast_Fixed(double d) {
    if constexpr (N <= 8) {
      v8 = d * (1 << K);
      sz = 8;
    } else if constexpr (N <= 16) {
      v16 = d * (1 << K);
      sz = 16;
    } else if constexpr (N <= 32) {
      v32 = d * (1 << K);
      sz = 32;
    } else if constexpr (N <= 64) {
      v64 = d * (1 << K);
      sz = 64;
    } else {
      static_assert(false, "Size of Fast Fixed class is wrong");
    }
  }
  template <is_number_class Other> constexpr Fast_Fixed(Other other) {
    if constexpr (N <= 8) {
      v8 = other.v();
      sz = 8;
    } else if constexpr (N <= 16) {
      v16 = other.v();
      sz = 16;
    } else if constexpr (N <= 32) {
      v32 = other.v();
      sz = 32;
    } else if constexpr (N <= 64) {
      v64 = other.v();
      sz = 64;
    } else {
      static_assert(false, "Size of Fast Fixed class is wrong");
    }
  }
  constexpr Fast_Fixed() {
    if constexpr (N <= 8) {
      v8 = 0;
      sz = 8;
    } else if constexpr (N <= 16) {
      v16 = 0;
      sz = 16;
    } else if constexpr (N <= 32) {
      v32 = 0;
      sz = 32;
    } else if constexpr (N <= 64) {
      v64 = 0;
      sz = 64;
    } else {
      static_assert(false, "Size of Fixed class is wrong");
    }
  }
  [[nodiscard]] constexpr int_fast64_t v() const {
    switch (sz) {
    case 8:
      return v8;
    case 16:
      return v16;
    case 32:
      return v32;
    case 64:
      return v64;
    }
  }
  static constexpr Fast_Fixed<N, K> from_raw(int_fast64_t x) {
    Fast_Fixed<N, K> ret;
    if constexpr (N <= 8) {
      ret.v8 = static_cast<int_fast8_t>(x);
      ret.sz = 8;
    } else if constexpr (N <= 16) {
      ret.v16 = x;
      ret.sz = 16;
    } else if constexpr (N <= 32) {
      ret.v32 = x;
      ret.sz = 32;
    } else if constexpr (N <= 64) {
      ret.v64 = x;
      ret.sz = 64;
    } else {
      static_assert(false, "Size of Fast Fixed class is wrong");
    }
    return ret;
  }

  Fast_Fixed<N, K> constexpr abs() {
    Fast_Fixed<N, K> cpy = Fast_Fixed<N, K>::from_raw(this->v());
    if (this->v() < 0) {
      cpy = Fast_Fixed<N, K>::from_raw(-this->v());
    }
    return cpy;
  }

  template <is_number_class Other> auto constexpr operator+(Other a) {
    auto [first, second] = align_values(*this, a);
    if (N > Other::n) {
      return Fast_Fixed<N, K>::from_raw(first.v() + second.v());
    } else {
      return Other::from_raw(first.v() + second.v());
    }
  }

  template <is_number_class Other>
  Fast_Fixed<N, K> constexpr operator-(Other a) {
    auto [first, second] = align_values(*this, a);
    if (N > Other::n) {
      return Fast_Fixed<N, K>::from_raw(first.v() - second.v());
    } else {
      return Other::from_raw(first.v() - second.v());
    }
  }

  template <is_number_class Other>
  Fast_Fixed<N, K> constexpr operator*(Other a) {
    auto [first, second] = align_values(*this, a);
    if (N > Other::n) {
      return Fast_Fixed<N, K>::from_raw(((int64_t)first.v() * second.v()) >> K);
    } else {
      return Other::from_raw(((int64_t)first.v() * second.v()) >> Other::k);
    }
  }

  template <is_number_class Other>
  Fast_Fixed<N, K> constexpr operator/(Other a) {
    auto [first, second] = align_values(*this, a);
    if (N > Other::n) {
      return Fast_Fixed<N, K>::from_raw(((int64_t)first.v() / second.v()) << K);
    } else {
      return Other::from_raw(((int64_t)first.v() * second.v()) << Other::k);
    }
  }

  template <is_number_class Other> auto constexpr &operator+=(Other a) {
    *this = *this + a;
    return *this;
  }

  template <is_number_class Other> auto constexpr &operator-=(Other a) {
    *this = *this - a;
    return *this;
  }

  template <is_number_class Other> auto constexpr &operator*=(Other a) {
    *this = *this * a;
    return *this;
  }

  template <is_number_class Other> auto constexpr &operator/=(Other a) {
    *this = *this / a;
    return this;
  }

  constexpr Fast_Fixed<N, K> operator-() {
    return Fast_Fixed<N, K>::from_raw(-this->v());
  }

  constexpr bool operator==(const Fast_Fixed &other) const {
    auto [first, second] = align_values(*this, other);
    return first.v() == second.v();
  }
  template <is_number_class Other>
  constexpr auto operator<=>(const Other &other) const {
    auto [first, second] = align_values(*this, other);
    return first.v() <=> second.v();
  }
  std::ostream constexpr &operator<<(std::ostream &out) {
    return out << this->v() / (double)(1 << K);
  }

private:
  int_fast8_t v8;
  int_fast16_t v16;
  int_fast32_t v32;
  int_fast64_t v64;
  int sz;
  friend Fixed<N, K>;
};

template <int N, int K> class Fixed {

public:
  constexpr static int n = N;
  constexpr static int k = K;
  constexpr Fixed(int v) {
    if constexpr (N == 8) {
      v8 = v * (1 << K);
      sz = 8;
    } else if constexpr (N == 16) {
      v16 = v * (1 << K);
      sz = 16;
    } else if constexpr (N == 32) {
      v32 = v * (1 << K);
      sz = 32;
    } else if constexpr (N == 64) {
      v64 = v * (1 << K);
      sz = 64;
    } else {
      static_assert(false, "Size of Fixed class is wrong");
    }
  }
  template <is_number_class Other> constexpr Fixed(Other other) {
    if constexpr (N == 8) {
      sz = 8;
    } else if constexpr (N == 16) {
      sz = 16;
    } else if constexpr (N == 32) {
      sz = 32;
    } else if constexpr (N == 64) {
      sz = 64;
    } else {
      static_assert(false, "Size of Fixed class is wrong");
    }
    switch (sz) {
    case 8: {
      v8 = other.v();
      break;
    }
    case 16: {
      v16 = other.v();
      break;
    }
    case 32: {
      v32 = other.v();
      break;
    }
    case 64: {
      v64 = other.v();
      break;
    }
    }
  }
  constexpr Fixed(float f) {
    if constexpr (N == 8) {
      v8 = f * (1 << K);
      sz = 8;
    } else if constexpr (N == 16) {
      v16 = f * (1 << K);
      sz = 16;
    } else if constexpr (N == 32) {
      v32 = f * (1 << K);
      sz = 32;
    } else if constexpr (N == 64) {
      v64 = f * (1 << K);
      sz = 64;
    } else {
      static_assert(false, "Size of Fixed class is wrong");
    }
  }

  constexpr Fixed(double f) {
    if constexpr (N == 8) {
      v8 = f * (1 << K);
      sz = 8;
    } else if constexpr (N == 16) {
      v16 = f * (1 << K);
      sz = 16;
    } else if constexpr (N == 32) {
      v32 = f * (1 << K);
      sz = 32;
    } else if constexpr (N == 64) {
      v64 = f * (1 << K);
      sz = 64;
    } else {
      static_assert(false, "Size of Fixed class is wrong");
    }
  }
  [[nodiscard]] constexpr int64_t v() const {
    switch (sz) {
    case 8:
      return v8;
    case 16:
      return v16;
    case 32:
      return v32;
    case 64:
      return v64;
    }
  }
  constexpr Fixed() {
    if constexpr (N == 8) {
      v8 = 0;
      sz = 8;
    } else if constexpr (N == 16) {
      v16 = 0;
      sz = 16;
    } else if constexpr (N == 32) {
      v32 = 0;
      sz = 32;
    } else if constexpr (N == 64) {
      v64 = 0;
      sz = 64;
    } else {
      static_assert(false, "Size of Fixed class is wrong");
    }
  }

  static constexpr Fixed from_raw(int64_t x) {
    Fixed ret;
    if (N == 8) {
      ret.sz = 8;
      ret.v8 = static_cast<int8_t>(x);
    } else if (N == 16) {
      ret.v16 = static_cast<int16_t>(x);
      ret.sz = 16;
    } else if (N == 32) {
      ret.v32 = static_cast<int32_t>(x);
      ret.sz = 32;
    } else if (N == 64) {
      ret.v64 = x;
      ret.sz = 64;
    }
    return ret;
  }
  constexpr Fixed<N, K> operator-() {
    return Fixed<N, K>::from_raw(-this->v());
  }

  Fixed<N, K> abs() {
    Fixed<N, K> cpy = *this;
    if (cpy.v() < 0) {
      cpy = Fixed<N, K>::from_raw(-this->v());
    }
    return cpy;
  }

  template <is_number_class Other> constexpr Fixed<N, K> operator+(Other a) {
    auto [first, second] = align_values(*this, a);
    if (N > Other::n) {
      return Fixed<N, K>::from_raw(first.v() + second.v());
    } else {
      return Other::from_raw(first.v() + second.v());
    }
  }

  template <is_number_class Other> constexpr Fixed<N, K> operator-(Other a) {
    auto [first, second] = align_values(*this, a);
    if (N > Other::n) {
      return Fixed<N, K>::from_raw(first.v() - second.v());
    } else {
      return Other::from_raw(first.v() - second.v());
    }
  }

  template <is_number_class Other> constexpr Fixed<N, K> operator*(Other a) {
    auto [first, second] = align_values(*this, a);
    if (N > Other::n) {
      return Fixed<N, K>::from_raw(((int64_t)first.v() * second.v()) >> K);
    } else {
      return Other::from_raw(((int64_t)first.v() * second.v()) >> Other::k);
    }
  }

  template <is_number_class Other> constexpr Fixed<N, K> operator/(Other a) {
    auto [first, second] = align_values(*this, a);
    if (N > Other::n) {
      return Fixed<N, K>::from_raw(((int64_t)first.v() / second.v()) << K);
    } else {
      return Other::from_raw(((int64_t)first.v() * second.v()) << Other::k);
    }
  }

  template <is_number_class Other> Fixed<N, K> constexpr &operator+=(Other a) {
    *this = *this + a;
    return *this;
  }

  template <is_number_class Other> Fixed<N, K> constexpr &operator-=(Other a) {
    *this = *this - a;
    return *this;
  }

  template <is_number_class Other> Fixed<N, K> constexpr &operator*=(Other a) {
    *this = *this * a;
    return *this;
  }

  template <is_number_class Other> Fixed<N, K> constexpr &operator/=(Other a) {
    *this = *this / a;
    return *this;
  }

  std::ostream &operator<<(std::ostream &out) {
    return out << this->v() / (double)(1 << K);
  }
  template <is_number_class Other>
  constexpr bool operator==(const Other &other) const {
    auto [first, second] = align_values(*this, other);
    return first.v() == other.v();
  }
  //  bool operator==(const Double &other) const {
  //   return this->v() == other.v();
  // }
  //  bool operator==(const Float &other) const {
  //   return this->v() == other.v();
  // }
  template <is_number_class Other> auto operator<=>(const Other &other) const {
    auto [first, second] = align_values(*this, other);
    return first.v() <=> other.v();
  }

private:
  int8_t v8;
  int16_t v16;
  int32_t v32;
  int64_t v64;
  int sz;
  friend Fast_Fixed<N, K>;
};

class Double {
public:
  static constexpr int n = 64;
  static constexpr int k = 0;
  double v;
  Double(double d) : v(d) {}
  Double(int i) : v(static_cast<double>(i)) {}
  Double() : v(0) {}
};
