#include "fixed.hpp"
#include "simulation.hpp"


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
    // run_asserts();
    double empty_rho, fluid_rho, g_double;
    int n, m;
    // Process_Types(types{});
    // ifstream in = ifstream("./input.txt");
    // in >> empty_rho >> fluid_rho >> g_double;
    Simulation<Fixed<32,16>,Fixed<32,16>,Fixed<32,16>,36,84> d;
    d.run();
    // d.rho[' '] = Fixed<16, 8>(empty_rho);
    // d.rho['.'] = Fixed<16, 8>(fluid_rho);
    // Double x();
    // in.get();
    // d.g = Fixed<16, 8>(g_double);
    // for (int i = 0; i < n; i++) {
    //     in.getline(field[i], m + 1);
    // }
    // in.close();
    // Simulation<Fixed<64, 10>, Fast_Fixed<32, 12>, Fast_Fixed<32, 10>, 36, 84> sim;
    // sim.run();
    return 0;
}
