#include "fixed.hpp"
#include "simulation.hpp"

// #define FLOAT float

void run_asserts() {
  auto y = Fast_Fixed<16, 7>(14);
  auto x = Fixed<32, 14>(120);
  auto res2 = x * y;
  auto res3 = x / y;
  assert((res3.v() >> 14) == 8);
  assert((res2.v() >> 14) == 1680);
  auto res = x + y;
  auto res1 = x - y;
  assert((res1.v() >> 14) == 106);
  assert((res.v() >> 14) == 134);
  Fast_Fixed<32, 14> a(10);
  Fixed<64, 10> b(16);
  assert((a + b).v() >> 14 == 26);
}

#define ERROR_MACRO                                                            \
  static_assert(                                                               \
      false,                                                                   \
      "You should specify available types through compile flag -DTYPES");
#ifndef TYPES
ERROR_MACRO
#endif
template <TYPES> constexpr void Binder() {}
//     constexpr std::array<char[20], 1000> split_types() {
//   std::array<char[20], 1000> result = {};
//   bool inside_brackets = false;
//   int ind_tmp = 0;
//   int i = 0;
//   int ind = 0;
//   for (; i < 10000; i++) {
//     if (res[i] == '\0') {
//       break;
//     }
//     char ch = res[i];
//     if (res[i] == ',' && !inside_brackets) {
//       ind++;
//       ind_tmp = 0;
//     } else if (res[i] != ' ' && res[i] != '\"') {
//       result[ind][ind_tmp] = res[i];
//       ind_tmp++;
//       if (res[i] == '(') {
//         inside_brackets = true;
//       } else if (res[i] == ')') {
//         inside_brackets = false;
//       }
//     }
//   }
//   return result;
// }

int main(int argc, char *argv[]) {
  // Fixed<32, 0> x(5);
  // Fast_Fixed<10, 0> f(4);
  // auto result = x - f;
  // std::cout << result.v();
  run_asserts();
  Simulation<Fast_Fixed<32, 10>, Fast_Fixed<32, 10>, Fast_Fixed<32, 10>> sim;
  sim.run();
  return 0;
}
