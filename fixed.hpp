#pragma once
#include <bits/stdc++.h>
#include <cstdint>
#include <cstdio>

using namespace std;

template <int N, int K> class Abstract_Number {
public:
  static constexpr int n = N;
  static constexpr int k = K;
};
template <int N, int K> class Fixed;
template <int N, int K>

class Fast_Fixed : public Abstract_Number<N, K> {
public:
  constexpr Fast_Fixed(int v) {
    if (N <= 8) {
      v8 = v * (1 << K);
      sz = 8;
    } else if (N <= 16) {
      v16 = v * (1 << K);
      sz = 16;
    } else if (N <= 32) {
      v32 = v * (1 << K);
      sz = 32;
    } else if (N <= 64) {
      v64 = v * (1 << K);
      sz = 64;
    }
  }
  constexpr Fast_Fixed(Fixed<N, K> &other) {
    this->sz = other.sz;
    switch (sz) {
    case 8: {
      v8 = other.v8;
      break;
    }
    case 16: {
      v16 = other.v16;
      break;
    }
    case 32: {
      v32 = other.v32;
      break;
    }
    case 64: {
      v64 = other.v64;
      break;
    }
    }
  }
  constexpr Fast_Fixed(float f) {
    if (N <= 8) {
      v8 = f * (1 << K);
      sz = 8;
    } else if (N <= 16) {
      v16 = f * (1 << K);
      sz = 16;
    } else if (N <= 32) {
      v32 = f * (1 << K);
      sz = 32;
    } else if (N <= 64) {
      v64 = f * (1 << K);
      sz = 64;
    }
  }
  constexpr Fast_Fixed(double d) {
    if (N <= 8) {
      v8 = d * (1 << K);
      sz = 8;
    } else if (N <= 16) {
      v16 = d * (1 << K);
      sz = 16;
    } else if (N <= 32) {
      v32 = d * (1 << K);
      sz = 32;
    } else if (N <= 64) {
      v64 = d * (1 << K);
      sz = 64;
    }
  }
  constexpr Fast_Fixed() {
    if (N <= 8) {
      v8 = 0;
      sz = 8;
    } else if (N <= 16) {
      v16 = 0;
      sz = 16;
    } else if (N <= 32) {
      v32 = 0;
      sz = 32;
    } else if (N <= 64) {
      v64 = 0;
      sz = 64;
    }
  }
  constexpr int_fast64_t v() const {
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
  static constexpr Fast_Fixed<N, K> from_raw(int_fast32_t x) {
    Fast_Fixed<N, K> ret;
    ret.v32 = x;
    ret.sz = 32;
    return ret;
  }
  template <int N2, int K2> Fast_Fixed<N, K> operator+(Fast_Fixed<N2, K2> a) {
    return Fast_Fixed<N, K>::from_raw(a.v() + this->v());
  }

  template <int N2, int K2> Fast_Fixed<N, K> operator-(Fast_Fixed<N2, K2> a) {
    return Fast_Fixed<N, K>::from_raw(this->v() - a.v());
  }

  template <int N2, int K2> Fast_Fixed<N, K> operator*(Fast_Fixed<N2, K2> a) {
    return Fast_Fixed<N, K>::from_raw(((int64_t)a.v() * this->v()) >> 16);
  }

  template <int N2, int K2> Fast_Fixed<N, K> operator/(Fast_Fixed<N2, K2> a) {
    return Fast_Fixed<N, K>::from_raw(((int64_t)this->v() << 16) / a.v());
  }

  template <int N2, int K2>
  Fast_Fixed<N, K> &operator+=(Fast_Fixed<N2, K2> &a) {
    *this = *this + a;
    return *this;
  }

  template <int N2, int K2>
  Fast_Fixed<N, K> &operator-=(Fast_Fixed<N2, K2> &a) {
    *this = *this - a;
    return *this;
  }

  template <int N2, int K2>
  Fast_Fixed<N, K> &operator*=(Fast_Fixed<N2, K2> &a) {
    *this = *this * a;
    return *this;
  }

  template <int N2, int K2>
  Fast_Fixed<N, K> &operator/=(Fast_Fixed<N2, K2> &a) {
    *this = *this / a;
    return this;
  }

  Fast_Fixed<N, K> operator-() {
    return Fast_Fixed<N, K>::from_raw(-this->v());
  }

  Fast_Fixed<N, K> abs() {
    Fast_Fixed<N, K> cpy = Fast_Fixed<N, K>::from_raw(this->v());
    if (this->v() < 0) {
      cpy = Fast_Fixed<N, K>::from_raw(-this->v());
    }
    return cpy;
  }

  bool operator==(const Fast_Fixed &) const = default;
  template <int N2, int K2>
  auto operator<=>(const Fast_Fixed<N2, K2> &other) const {
    return this->v() <=> other.v();
  }
  ostream &operator<<(ostream &out) {
    return out << this->v() / (double)(1 << 16);
  }

private:
  int_fast8_t v8;
  int_fast16_t v16;
  int_fast32_t v32;
  int_fast64_t v64;
  int sz;
  friend Fixed<N, K>;
};

template <int N, int K> class Fixed : public Abstract_Number<N, K> {
public:
  constexpr Fixed(int v) {
    if (N <= 8) {
      v8 = v * (1 << K);
      sz = 8;
    } else if (N <= 16) {
      v16 = v * (1 << K);
      sz = 16;
    } else if (N <= 32) {
      v32 = v * (1 << K);
      sz = 32;
    } else if (N <= 64) {
      v64 = v * (1 << K);
      sz = 64;
    }
  }
  constexpr Fixed(Fast_Fixed<N, K> &other) {
    this->sz = other.sz;
    switch (sz) {
    case 8: {
      v8 = other.v8;
      break;
    }
    case 16: {
      v16 = other.v16;
      break;
    }
    case 32: {
      v32 = other.v32;
      break;
    }
    case 64: {
      v64 = other.v64;
      break;
    }
    }
  }
  constexpr Fixed(float f) {
    if (N <= 8) {
      v8 = f * (1 << K);
      sz = 8;
    } else if (N <= 16) {
      v16 = f * (1 << K);
      sz = 16;
    } else if (N <= 32) {
      v32 = f * (1 << K);
      sz = 32;
    } else if (N <= 64) {
      v64 = f * (1 << K);
      sz = 64;
    }
  }

  constexpr Fixed(double f) {
    if (N <= 8) {
      v8 = f * (1 << K);
      sz = 8;
    } else if (N <= 16) {
      v16 = f * (1 << K);
      sz = 16;
    } else if (N <= 32) {
      v32 = f * (1 << K);
      sz = 32;
    } else if (N <= 64) {
      v64 = f * (1 << K);
      sz = 64;
    }
  }
  constexpr int64_t v() const {
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
    if (N <= 8) {
      v8 = 0;
      sz = 8;
    } else if (N <= 16) {
      v16 = 0;
      sz = 16;
    } else if (N <= 32) {
      v32 = 0;
      sz = 32;
    } else if (N <= 64) {
      v64 = 0;
      sz = 64;
    }
  }

  static constexpr Fixed from_raw(int32_t x) {
    Fixed ret;
    ret.v32 = x;
    return ret;
  }
  template <int N2, int K2> Fixed<N, K> operator+(Fixed<N2, K2> a) {
    return Fixed<N, K>::from_raw(a.v() + this->v());
  }

  template <int N2, int K2> Fixed<N, K> operator-(Fixed<N2, K2> a) {
    return Fixed<N, K>::from_raw(this->v() - a.v());
  }

  template <int N2, int K2> Fixed<N, K> operator*(Fixed<N2, K2> a) {
    return Fixed<N, K>::from_raw(((int64_t)a.v() * this->v()) >> 16);
  }

  template <int N2, int K2> Fixed<N, K> operator/(Fixed<N2, K2> a) {
    return Fixed<N, K>::from_raw(((int64_t)this->v() << 16) / a.v());
  }

  template <int N2, int K2> Fixed<N, K> &operator+=(Fixed<N2, K2> a) {
    *this = *this + a;
    return *this;
  }

  template <int N2, int K2> Fixed<N, K> &operator-=(Fixed<N2, K2> a) {
    *this = *this - a;
    return *this;
  }

  template <int N2, int K2> Fixed<N, K> &operator*=(Fixed<N2, K2> a) {
    *this = *this * a;
    return *this;
  }

  template <int N2, int K2> Fixed<N, K> &operator/=(Fixed<N2, K2> a) {
    *this = *this / a;
    return *this;
  }

  Fixed<N, K> operator-() { return Fixed<N, K>::from_raw(-this->v()); }

  Fixed<N, K> abs() {
    Fixed<N, K> cpy = *this;
    if (cpy.v() < 0) {
      cpy = Fixed<N, K>::from_raw(-this->v());
    }
    return cpy;
  }

  ostream &operator<<(ostream &out) {
    return out << this->v() / (double)(1 << 16);
  }
  bool operator==(const Fixed &other) const { return this->v() == other.v(); }
  template <int N2, int K2> auto operator<=>(const Fixed<N2, K2> &other) const {
    return this->v() <=> other.v();
  }

private:
  int8_t v8;
  int16_t v16;
  int32_t v32;
  int64_t v64;
  int sz;
  friend Fast_Fixed<N, K>;
};
