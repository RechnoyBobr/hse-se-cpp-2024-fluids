#pragma once
#include <bits/stdc++.h>
#include <cstdint>
#include <cstdio>
#include <tuple>


#define TO_STRING(x) #x

class Abstract_Number {
};

template<typename T>
concept is_number_class =
        requires(T obj)
        {
            requires(std::convertible_to<decltype(obj.v()), int> || std::convertible_to<decltype(obj.v()), double>);
        };

template<is_number_class First, is_number_class Second>
constexpr static auto align_values(First f, Second s) {
    if constexpr (std::is_same_v<First, Second>) {
        return std::make_tuple<First, First>(First::from_raw(f.v()), First::from_raw(s.v()));
    }
    if constexpr (First::k > Second::k) {
        return std::make_tuple<First, First>(
            First::from_raw(f.v()),
            First::from_raw(static_cast<int64_t>(s.v()) << (First::k - Second::k)));
    }
    if constexpr (First::k <= Second::k) {
        return std::make_tuple<First, First>(
            First::from_raw(f.v()),
            First::from_raw(static_cast<int64_t>(s.v()) >> (Second::k - First::k)));
    }
}

using namespace std;
template<int N, int K>
class Fixed;
template<int N, int K>
class Fast_Fixed;

class Float;
class Double;

template<int N, int K>
class Fast_Fixed : public Abstract_Number {
public:
    constexpr static auto get_name() {
        std::array<char, 17> res{};
        res[0] = 'F';
        res[1] = 'A';
        res[2] = 'S';
        res[3] = 'T';
        res[4] = '_';
        res[5] = 'F';
        res[6] = 'I';
        res[7] = 'X';
        res[8] = 'E';
        res[9] = 'D';
        res[10] = '(';
        int ind = 11;
        if constexpr (N >= 10) {
            res[ind] = '0' + (N / 10);
            ind++;
            res[ind] = '0' + (N % 10);
            ind++;
        } else {
            res[ind] = '0' + (N % 10);
            ind++;
        }
        res[ind] = ',';
        ind++;
        if constexpr (K >= 10) {
            res[ind] = '0' + (K / 10);
            ind++;
            res[ind] = '0' + (K % 10);
            ind++;
        } else {
            res[ind] = '0' + (K % 10);
            ind++;
        }
        res[ind] = ')';
        return res;
    }

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

    template<is_number_class Other>
    constexpr Fast_Fixed(Other other) {
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
            default:
                assert(false);
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

    template<is_number_class Other>
    constexpr Fast_Fixed<N, K> operator+(Other a) {
        auto [first, second] = align_values(*this, a);
        return Fast_Fixed<N, K>::from_raw(first.v() + second.v());
    }

    template<is_number_class Other>
    Fast_Fixed<N, K> constexpr operator-(Other a) {
        auto [first, second] = align_values(*this, a);
        return Fast_Fixed<N, K>::from_raw(first.v() - second.v());
    }

    template<is_number_class Other>
    Fast_Fixed<N, K> constexpr operator*(Other a) {
        auto [first, second] = align_values(*this, a);

        if (second.v() >> first.k != 0) {
            return Fast_Fixed<N, K>::from_raw(static_cast<int_fast64_t>(first.v()) *
                                              (second.v() / (1 << first.k)));
        }
        return Fast_Fixed<N, K>::from_raw((static_cast<int_fast64_t>(first.v()) * second.v()) / (1 << first.k));
    }

    template<is_number_class Other>
    Fast_Fixed<N, K> constexpr operator/(Other a) {
        auto [first, second] = align_values(*this, a);
        if (second.v() / (1 << K) == 0) {
            return Fast_Fixed<N, K>::from_raw((static_cast<int_fast64_t>(first.v()) * (1 << first.k)) /
                                              second.v());
        }
        return Fast_Fixed<N, K>::from_raw(static_cast<int_fast64_t>(first.v()) /
                                          (second.v() / (1 << first.k)));
    }

    template<is_number_class Other>
    auto constexpr &operator+=(Other a) {
        *this = *this + a;
        return *this;
    }

    template<is_number_class Other>
    auto constexpr &operator-=(Other a) {
        *this = *this - a;
        return *this;
    }

    template<is_number_class Other>
    auto constexpr &operator*=(Other a) {
        *this = *this * a;
        return *this;
    }

    template<is_number_class Other>
    auto constexpr &operator/=(Other a) {
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

    template<is_number_class Other>
    constexpr auto operator<=>(const Other &other) const {
        auto [first, second] = align_values(*this, other);
        return first.v() <=> second.v();
    }

    std::ostream constexpr &operator<<(std::ostream &out) const {
        return out << this->v() / static_cast<double>(1 << K);
    }

    int_fast8_t v8;
    int_fast16_t v16;
    int_fast32_t v32;
    int_fast64_t v64;
    int sz;
    friend Fixed<N, K>;
};

template<int N, int K>
class Fixed : public Abstract_Number {
public:
    constexpr static int n = N;
    constexpr static int k = K;

    constexpr static auto get_name() {
        std::array<char, 17> res{};
        res[0] = 'F';
        res[1] = 'I';
        res[2] = 'X';
        res[3] = 'E';
        res[4] = 'D';
        res[5] = '(';
        int ind = 6;
        if constexpr (N >= 10) {
            res[ind] = '0' + (N / 10);
            ind++;
            res[ind] = '0' + (N % 10);
            ind++;
        } else {
            res[ind] = '0' + (N % 10);
            ind++;
        }
        res[ind] = ',';
        ind++;
        if constexpr (K >= 10) {
            res[ind] = static_cast<char>('0' + (K / 10));
            ind++;
            res[ind] = '0' + (K % 10);
            ind++;
        } else {
            res[ind] = '0' + (K % 10);
            ind++;
        }
        res[ind] = ')';
        return res;
    }

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

    template<is_number_class Other>
    constexpr Fixed(Other other) {
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
            default:
                return -1;
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

    template<is_number_class Other>
    constexpr Fixed<N, K> operator+(Other a) {
        auto [first, second] = align_values(*this, a);

        return Fixed<N, K>::from_raw(first.v() + second.v());
    }

    template<is_number_class Other>
    constexpr Fixed<N, K> operator-(Other a) {
        auto [first, second] = align_values(*this, a);
        return Fixed<N, K>::from_raw(first.v() - second.v());
    }

    template<is_number_class Other>
    constexpr Fixed<N, K> operator*(Other a) {
        auto [first, second] = align_values(*this, a);
        if (second.v() / (1 << first.k) != 0) {
            return Fixed<N, K>::from_raw(static_cast<int64_t>(first.v()) *
                                         (second.v() / (1 << first.k)));
        }
        return Fixed<N, K>::from_raw((static_cast<int64_t>(first.v()) * second.v()) / (1 << first.k));
    }

    template<is_number_class Other>
    constexpr Fixed<N, K> operator/(Other a) {
        auto [first, second] = align_values(*this, a);
        if (second.v() / (1 << K) == 0) {
            return Fixed<N, K>::from_raw(((int64_t) first.v() << first.k) /
                                         second.v());
        }
        return Fixed<N, K>::from_raw((int64_t) first.v() /
                                     (second.v() >> first.k));
    }

    template<is_number_class Other>
    Fixed<N, K> constexpr &operator+=(Other a) {
        *this = *this + a;
        return *this;
    }

    template<is_number_class Other>
    Fixed<N, K> constexpr &operator-=(Other a) {
        *this = *this - a;
        return *this;
    }

    template<is_number_class Other>
    Fixed<N, K> constexpr &operator*=(Other a) {
        *this = *this * a;
        return *this;
    }

    template<is_number_class Other>
    Fixed<N, K> constexpr &operator/=(Other a) {
        *this = *this / a;
        return *this;
    }

    std::ostream &operator<<(std::ostream &out) {
        return out << this->v() / (double) (1 << K);
    }

    template<is_number_class Other>
    constexpr bool operator==(const Other &other) const {
        auto [first, second] = align_values(*this, other);
        return first.v() == other.v();
    }

    template<is_number_class Other>
    auto operator<=>(const Other &other) const {
        auto [first, second] = align_values(*this, other);
        return first.v() <=> other.v();
    }

    int8_t v8;
    int16_t v16;
    int32_t v32;
    int64_t v64;
    int sz;
    friend Fast_Fixed<N, K>;
};

class Double : public Abstract_Number {
public:
    static constexpr int n = 64;
    static constexpr int k = 0;

    constexpr static auto get_name() {
        std::array<char, 17> res{};
        res[0] = 'D';
        res[1] = 'O';
        res[2] = 'U';
        res[3] = 'B';
        res[4] = 'L';
        res[5] = 'E';
        return res;
    }

    double v_double;

    constexpr Double(): v_double(0) {
    }

    constexpr Double(double d) : v_double(d) {
    }

    constexpr Double(int i) : v_double(static_cast<double>(i)) {
    }


    static constexpr Double from_raw(double other) {
        Double ret;
        ret.v_double = other;
        return ret;
    }


    [[nodiscard]] constexpr double v() const { return v_double; }


    template<typename Other>
    constexpr Double(Other other) {
        this->v_double =
                static_cast<double>(other.v()) / static_cast<double>(1 << Other::k);
    }

    template<is_number_class Other>
    constexpr auto operator+(Other a) {
        auto [first, second] = align_values(*this, a);
        return Double::from_raw(first.v() + second.v());
    }

    template<is_number_class Other>
    constexpr auto operator-(Other a) {
        auto [first, second] = align_values(*this, a);
        return Double::from_raw(first.v() - second.v());
    }

    template<is_number_class Other>
    constexpr auto operator*(Other a) {
        auto [first, second] = align_values(*this, a);
        if (second.v() / (1 << first.k) != 0) {
            return Double::from_raw((int64_t) first.v() *
                                    (second.v() / (1 << first.k)));
        }
        return Double::from_raw((static_cast<double>(first.v()) * second.v()) /
                                (1 << first.k));
    }

    template<is_number_class Other>
    constexpr Double operator/(Other a) {
        auto [first, second] = align_values(*this, a);
        if (abs(second.v() / (1 << k)) <= 1e-7) {
            return Double::from_raw(((double) first.v() * (1 << first.k)) /
                                    second.v());
        } else {
            return Double::from_raw((double) first.v() /
                                    (second.v() / (1 << first.k)));
        }
    }

    template<is_number_class Other>
    Double constexpr &operator+=(Other a) {
        *this = *this + a;
        return *this;
    }

    template<is_number_class Other>
    Double constexpr &operator-=(Other a) {
        *this = *this - a;
        return *this;
    }

    template<is_number_class Other>
    Double constexpr &operator*=(Other a) {
        *this = *this * a;
        return *this;
    }

    template<is_number_class Other>
    Double constexpr &operator/=(Other a) {
        *this = *this / a;
        return *this;
    }

    std::ostream &operator<<(std::ostream &out) const {
        return out << this->v_double;
    }

    template<is_number_class Other>
    constexpr bool operator==(const Other &other) const {
        auto [first, second] = align_values(*this, other);
        return abs(first.v() - (double) other.v()) < 1e-8;
    }

    template<is_number_class Other>
    auto operator<(const Other &other) const {
        auto [first, second] = align_values(*this, other);
        return first.v() < second.v();
    }

    template<is_number_class Other>
    auto operator>(const Other &other) const {
        auto [first, second] = align_values(*this, other);
        return first.v() > second.v();
    }

    template<is_number_class Other>
    auto operator<=(const Other &other) const {
        return (*this < other || *this == other);
    }
};

class Float : public Abstract_Number {
public:
    static constexpr int n = 32;
    static constexpr int k = 0;
    float v_float;

    constexpr static auto get_name() {
        std::array<char, 17> res{};
        res[0] = 'F';
        res[1] = 'L';
        res[2] = 'O';
        res[3] = 'A';
        res[4] = 'T';
        return res;
    }

    Float(double d) : v_float(static_cast<float>(d)) {
    }


    Float(int i) : v_float(static_cast<float>(i)) {
    }

    static constexpr Float from_raw(float other) {
        Float ret;
        ret.v_float = other;
        return ret;
    }

    [[nodiscard]] constexpr float v() const { return v_float; }

    Float() : v_float(0) {
    }

    template<is_number_class Other>
    constexpr Float(Other other) {
        this->v_float =
                other.v() / static_cast<float>(1 << other.k);
    }

    template<is_number_class Other>
    constexpr auto operator+(Other a) {
        auto [first, second] = align_values(*this, a);
        return Float::from_raw(first.v() + second.v());
    }

    template<is_number_class Other>
    constexpr auto operator-(Other a) {
        auto [first, second] = align_values(*this, a);
        return Float::from_raw(first.v() - second.v());
    }

    template<is_number_class Other>
    constexpr auto operator*(Other a) {
        auto [first, second] = align_values(*this, a);
        if (second.v() / (1 << first.k) != 0) {
            return Float::from_raw(static_cast<float>(first.v()) *
                                   (static_cast<float>(second.v()) / (1 << first.k)));
        }
        return Float::from_raw((static_cast<float>(first.v()) * second.v()) /
                               (1 << first.k));
    }

    template<is_number_class Other>
    constexpr auto operator/(Other a) {
        auto [first, second] = align_values(*this, a);

        if (abs(second.v() / (1 << k)) <= 1e-7) {
            return Float::from_raw((static_cast<float>(first.v()) * (1 << first.k)) /
                                   second.v());
        } else {
            return Float::from_raw((float) first.v() /
                                   (second.v() / (1 << first.k)));
        }
    }

    template<is_number_class Other>
    Float constexpr &operator+=(Other a) {
        *this = *this + a;
        return *this;
    }

    template<is_number_class Other>
    Float constexpr &operator-=(Other a) {
        *this = *this - a;
        return *this;
    }

    template<is_number_class Other>
    Float constexpr &operator*=(Other a) {
        *this = *this * a;
        return *this;
    }

    template<is_number_class Other>
    Float constexpr &operator/=(Other a) {
        *this = *this / a;
        return *this;
    }

    std::ostream &operator<<(std::ostream &out) const {
        return out << this->v_float;
    }

    template<is_number_class Other>
    constexpr bool operator==(const Other &other) const {
        auto [first, second] = align_values(*this, other);
        return abs(first.v() - static_cast<float>(other.v())) < 1e-7;
    }

    template<is_number_class Other>
    auto operator<=>(const Other &other) const {
        auto [first, second] = align_values(*this, other);
        return first.v() <=> other.v();
    }
};
