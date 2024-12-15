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

template <int N, int K> class Fast_Fixed : public Abstract_Number<N, K> {
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
  constexpr auto get_v() {
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
    Fast_Fixed ret;
    ret.v32 = x;
    return ret;
  }

  bool operator==(const Fast_Fixed &) const = default;
  Fast_Fixed<N, K> operator+(Fast_Fixed<N, K> a) {
    return Fast_Fixed<N, K>::from_raw(a.v() + this->v());
  }

  Fast_Fixed<N, K> operator-(Fast_Fixed<N, K> a) {
    return Fast_Fixed<N, K>::from_raw(a.v() - this->v());
  }

  Fast_Fixed<N, K> operator*(Fast_Fixed<N, K> a) {
    return Fast_Fixed<N, K>::from_raw(((int64_t)a.v() * this->v()) >> 16);
  }

  Fast_Fixed<N, K> operator/(Fast_Fixed<N, K> a) {
    return Fast_Fixed<N, K>::from_raw(((int64_t)this->v() << 16) / a.v());
  }

  Fast_Fixed<N, K> &operator+=(Fast_Fixed<N, K> &a) {
    return *this = *this + a;
  }

  Fast_Fixed<N, K> &operator-=(Fast_Fixed<N, K> &a) {
    return *this = *this - a;
  }

  Fast_Fixed<N, K> &operator*=(Fast_Fixed<N, K> &a) {
    return *this = *this * a;
  }

  Fast_Fixed<N, K> &operator/=(Fast_Fixed<N, K> &a) {
    return *this = *this / a;
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

  ostream &operator<<(ostream &out) {
    return out << this->v() / (double)(1 << 16);
  }

private:
  int_fast8_t v8;
  int_fast16_t v16;
  int_fast32_t v32;
  int_fast64_t v64;
  int sz;
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
  auto v() {
    switch (sz) {
    case 8:
      return v8;
    case 16:
      return v16;
    case 32:
      return v;
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

  bool operator==(const Fixed &) const = default;
  Fixed<N, K> operator+(Fixed<N, K> a) {
    return Fixed<N, K>::from_raw(a.v() + this->v());
  }

  Fixed<N, K> operator-(Fixed<N, K> a) {
    return Fixed<N, K>::from_raw(a.v() - this->v());
  }

  Fixed<N, K> operator*(Fixed<N, K> a) {
    return Fixed<N, K>::from_raw(((int64_t)a.v() * this->v()) >> 16);
  }

  Fixed<N, K> operator/(Fixed<N, K> a) {
    return Fixed<N, K>::from_raw(((int64_t)this->v() << 16) / a.v());
  }

  Fixed<N, K> &operator+=(Fixed<N, K> &a) { return *this = *this + a; }

  Fixed<N, K> &operator-=(Fixed<N, K> &a) { return *this = *this - a; }

  Fixed<N, K> &operator*=(Fixed<N, K> &a) { return *this = *this * a; }

  Fixed<N, K> &operator/=(Fixed<N, K> &a) { return *this = *this / a; }

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

private:
  int8_t v8;
  int16_t v16;
  int32_t v32;
  int64_t v64;
  int sz;
};
