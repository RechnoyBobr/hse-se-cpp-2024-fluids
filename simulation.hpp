#pragma once
#include <bits/stdc++.h>

#include <ctime>
#include <random>

#include "fixed.hpp"
#include "vector_field.hpp"


inline mt19937 rnd{1337};
inline uniform_real_distribution<> r{0.0, 1.0};
inline int cnt = 0;
inline int subcnt = 0;

template<class p_type, class v_type, class v_flow, int N, int M>
class Simulation {
    // constexpr size_t N = 14, M = 5;
    int last_use[N][M] = {};
    int UT;


    int dirs[36][84] = {};
    constexpr static size_t T = 1'000'000;

    constexpr static std::array<pair<int, int>, 4> deltas{
        {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}
    };


    void propagate_stop(int x, int y, bool force = false) {
        if (!force) {
            bool stop = true;
            for (auto [dx, dy]: deltas) {
                int nx = x + dx, ny = y + dy;
                if (field[nx][ny] != '#' && last_use[nx][ny] < UT - 1 &&
                    velocity.get(x, y, dx, dy, deltas) > v_type::from_raw(0)) {
                    stop = false;
                    break;
                }
            }
            if (!stop) {
                return;
            }
        }
        last_use[x][y] = UT;
        for (auto [dx, dy]: deltas) {
            int nx = x + dx, ny = y + dy;
            if (field[nx][ny] == '#' || last_use[nx][ny] == UT ||
                velocity.get(x, y, dx, dy, deltas) > v_type::from_raw(0)) {
                continue;
            }
            propagate_stop(nx, ny);
        }
    }

    v_type move_prob(int x, int y) {
        v_type sum = 0;
        for (size_t i = 0; i < deltas.size(); ++i) {
            auto [dx, dy] = deltas[i];
            int nx = x + dx, ny = y + dy;
            if (field[nx][ny] == '#' || last_use[nx][ny] == UT) {
                continue;
            }
            auto v = velocity.get(x, y, dx, dy, deltas);
            if (v < v_type(0)) {
                continue;
            }
            sum += v;
        }
        return sum;
    }

    char type = '1';
    p_type cur_p;
    array<v_type, deltas.size()> v;

    void swap_with(int x, int y) {
        swap(field[x][y], type);
        swap(p[x][y], cur_p);
        swap(velocity.v[x][y], v);
    }


    bool propagate_move(int x, int y, bool is_first) {
        last_use[x][y] = UT - is_first;
        bool ret = false;
        int nx = -1, ny = -1;
        do {
            std::array<v_type, deltas.size()> tres;
            v_type sum = 0;
            for (size_t i = 0; i < deltas.size(); ++i) {
                auto [dx, dy] = deltas[i];
                nx = x + dx, ny = y + dy;
                if (field[nx][ny] == '#' || last_use[nx][ny] == UT) {
                    tres[i] = sum;
                    continue;
                }
                auto v = velocity.get(x, y, dx, dy, deltas);
                if (v < v_type::from_raw(0)) {
                    tres[i] = sum;
                    continue;
                }
                sum += v;
                tres[i] = sum;
            }

            if (sum == v_type(0)) {
                break;
            }

            v_type p = random01() * sum;
            size_t d = std::upper_bound(tres.begin(), tres.end(), p) - tres.begin();

            auto [dx, dy] = deltas[d];
            nx = x + dx;
            ny = y + dy;
            assert(velocity.get(x, y, dx, dy, deltas) > v_type(0) && field[nx][ny] != '#' &&
                last_use[nx][ny] < UT);

            ret = (last_use[nx][ny] == UT - 1 || propagate_move(nx, ny, false));
        } while (!ret);
        last_use[x][y] = UT;
        for (auto [dx, dy]: deltas) {
            nx = x + dx, ny = y + dy;
            if (field[nx][ny] != '#' && last_use[nx][ny] < UT - 1 &&
                velocity.get(x, y, dx, dy, deltas) < v_type::from_raw(0)) {
                propagate_stop(nx, ny);
            }
        }
        if (ret) {
            if (!is_first) {
                v[0] = 0;
                v[1] = 0;
                v[2] = 0;
                v[3] = 0;
                swap_with(x, y);
                swap_with(nx, ny);
                swap_with(x, y);
            }
        }
        return ret;
    }


    p_type p[N][M], old_p[N][M];
    VectorField<v_type, v_type::n, v_type::k, N, M> velocity;
    VectorField<v_flow, v_flow::n, v_flow::k, N, M> velocity_flow;

    v_type random01() {
        if constexpr (is_same_v<v_type, Double> || is_same_v<v_type, Float>) {
            return v_type::from_raw(static_cast<double>(rnd()) / INT_MAX);
        }
        return v_type::from_raw((rnd() & ((1 << v_type::k) - 1)));
    }

public:
    char field[N][M + 1]{};
    p_type rho[256];
    p_type g;

    Simulation() {
        velocity = VectorField<v_type, v_type::n, v_type::k, N, M>();
        velocity_flow = VectorField<v_flow, v_flow::n, v_flow::k, N, M>();
        UT = 0;
        for (size_t i = 0; i < N; i++) {
            for (size_t j = 0; j < M; j++) {
                dirs[i][j] = 0;
                last_use[i][j] = 0;
                p[i][j] = p_type();
            }
        }
    }

    void run() {
        for (size_t x = 0; x < N; ++x) {
            for (size_t y = 0; y < M; ++y) {
                if (field[x][y] == '#')
                    continue;
                for (auto [dx, dy]: deltas) {
                    dirs[x][y] += (field[x + dx][y + dy] != '#');
                }
            }
        }

        for (size_t i = 0; i < T; ++i) {
            p_type total_delta_p = 0;
            // Apply external forces
            for (size_t x = 0; x < N; ++x) {
                for (size_t y = 0; y < M; ++y) {
                    if (field[x][y] == '#')
                        continue;
                    if (field[x + 1][y] != '#') {
                        velocity.add(x, y, 1, 0, g, deltas);
                    }
                }
            }

            // Apply forces from p
            for (size_t x = 0; x < N; ++x) {
                for (size_t y = 0; y < M; ++y) {
                    old_p[x][y] = p[x][y];
                }
            }
            for (size_t x = 0; x < N; ++x) {
                for (size_t y = 0; y < M; ++y) {
                    if (field[x][y] == '#')
                        continue;
                    for (auto [dx, dy]: deltas) {
                        int nx = static_cast<int>(x) + dx, ny = static_cast<int>(y) + dy;
                        if (field[nx][ny] != '#' && old_p[nx][ny] < old_p[x][y]) {
                            auto delta_p = old_p[x][y] - old_p[nx][ny];
                            auto force = delta_p;
                            auto &contr = velocity.get(nx, ny, -dx, -dy, deltas);
                            if (contr * rho[static_cast<int>(field[nx][ny])] >= force) {
                                contr -= force / rho[static_cast<int>(field[nx][ny])];
                                continue;
                            }
                            force -= contr * rho[static_cast<int>(field[nx][ny])];
                            contr = 0;
                            velocity.add(x, y, dx, dy, force / rho[static_cast<int>(field[x][y])], deltas);
                            p[x][y] -= force / p_type(dirs[x][y]);
                            total_delta_p -= force / p_type(dirs[x][y]);
                        }
                    }
                }
            }

            // Make flow from velocities
            velocity_flow = {};
            bool prop = false;
            do {
                UT += 2;
                prop = false;
                for (size_t x = 0; x < N; ++x) {
                    for (size_t y = 0; y < M; ++y) {
                        if (field[x][y] != '#' && last_use[x][y] != UT) {
                            auto [t, local_prop, _] =
                                    VectorField<v_flow, v_flow::n, v_flow::k, N, M>::propagate_flow(
                                        x, y, v_flow(1), last_use, UT, velocity, velocity_flow, deltas, field);
                            if (t > v_flow(0)) {
                                prop = true;
                            }
                        }
                    }
                }
            } while (prop);

            // Recalculate p with kinetic energy
            for (size_t x = 0; x < N; ++x) {
                for (size_t y = 0; y < M; ++y) {
                    if (field[x][y] == '#')
                        continue;
                    for (auto [dx, dy]: deltas) {
                        auto old_v = velocity.get(x, y, dx, dy, deltas);
                        auto new_v = velocity_flow.get(x, y, dx, dy, deltas);
                        if (old_v > v_type::from_raw(0)) {
                            assert(new_v <= old_v);
                            velocity.get(x, y, dx, dy, deltas) = new_v;
                            auto force = (old_v - new_v) * rho[static_cast<int>(field[x][y])];
                            if (field[x][y] == '.')
                                force *= p_type(0.8);
                            if (field[x + dx][y + dy] == '#') {
                                p[x][y] += force / p_type(dirs[x][y]);
                                total_delta_p += force / p_type(dirs[x][y]);
                            } else {
                                p[x + dx][y + dy] += force / p_type(dirs[x + dx][y + dy]);
                                total_delta_p += force / p_type(dirs[x + dx][y + dy]);
                            }
                        }
                    }
                }
            }

            UT += 2;

            prop = false;
            for (size_t x = 0; x < N; ++x) {
                for (size_t y = 0; y < M; ++y) {
                    if (field[x][y] != '#' && last_use[x][y] != UT) {
                        auto f = random01();
                        if (f < move_prob(x, y)) {
                            prop = true;
                            propagate_move(x, y, true);
                        } else {
                            propagate_stop(x, y, true);
                        }
                    }
                }
            }

            if (prop) {
                cout << "Tick " << i << ":\n";
                for (auto &x: field) {
                    cout << x << "\n";
                }
            }
        }
    }
};
