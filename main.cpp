#include <array>
#include <codecvt>
#include <iostream>
#include <type_traits>

#include "fixed.hpp"

#define FIXED(N, K) Fixed<N, K>
#define FAST_FIXED(N, K) Fast_Fixed<N, K>
#define DOUBLE double
#define FLOAT float

// constexpr std::array<char[20], 1000> split_types() {
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
//
//

int main(int argc, char* argv[]) {
#ifdef TYPES
  TYPES res();
#endif
  return 0;
}
