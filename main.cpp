#include "fixed.hpp"
#include "simulation.hpp"
#include "sim_gen.hpp"
#include "getopt.h"

constexpr void run_asserts() {
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


auto &get_sim(vector<string_view> &m) {
    for (size_t i = 0; i < sims.size(); i++) {
        int flag = 1;
        for (size_t j = 0; j < 3; j++) {
            for (size_t k = 0; k < m[j].size(); k++) {
                if (m[j][k] != sims_names[i][j][k]) {
                    flag = 0;
                    break;
                }
            }
            if (!flag) {
                break;
            }
        }
        if (flag) {
            return sims[i];
        }
    }
    cout << "No type in the DTYPES\n";
    assert(false);
}


int main(int argc, char *argv[]) {
    run_asserts();

    string_view p_type_raw_str;
    string_view v_type_raw_str;
    string_view v_flow_type_raw_str;
    int c;
    struct option long_options[] = {
        {"p_type", required_argument, 0, 'p'},
        {"v_type", required_argument, 0, 'v'},
        {"v_flow_type", required_argument, 0, 'f'}
    };
    int opt_idx = 0;
    while ((c = getopt_long(argc, argv, "pvf:", long_options, &opt_idx)) != -1) {
        switch (c) {
            case 'p': {
                p_type_raw_str = optarg;
                break;
            }
            case 'v': {
                v_type_raw_str = optarg;
                break;
            }
            case 'f': {
                v_flow_type_raw_str = optarg;
                break;
            }
            default: {
                std::cout << "Can't recognize the flag\n";
                return -1;
            }
        }
    }
    vector<string_view> vec{p_type_raw_str, v_type_raw_str, v_flow_type_raw_str};
    auto sim = get_sim(vec);
    visit([](auto &sim) {
        ifstream in = ifstream("./input.txt");
        double empty_rho, fluid_rho, g_double;
        in >> empty_rho >> fluid_rho >> g_double;
        int n = 36, m = 84;
        sim.rho[' '] = empty_rho;
        sim.rho['.'] = fluid_rho;
        in.get();
        sim.g = g_double;
        for (int i = 0; i < n; i++) {
            in.getline(sim.field[i], m + 1);
        }
        in.close();
        sim.run();
    }, sim);

    return 0;
}
