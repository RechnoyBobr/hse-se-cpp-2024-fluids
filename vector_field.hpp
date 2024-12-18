#pragma once

#include <bits/stdc++.h>

using namespace std;

template<typename v_t, int P, int K, int N, int M>
class VectorField {
public:
    array<v_t, 4> v[N][M];

    constexpr VectorField() {
        for (auto &i: v) {
            for (auto &j: i) {
                for (auto &k: j) {
                    k = 0;
                }
            }
        }
    }

    v_t &add(int x, int y, int dx, int dy, v_t dv, auto &deltas) {
        if (x == 21 && y == 64) {
            // std::cout << "Bug located\n";
        }
        return get(x, y, dx, dy, deltas) += dv;
    }

    v_t &get(int x, int y, int dx, int dy, auto deltas) {
        size_t i = ranges::find(deltas, pair(dx, dy)) - deltas.begin();

        assert(i < deltas.size());
        return v[x][y][i];
    }

    static tuple<v_t, bool, pair<int, int> > propagate_flow(int x, int y,
                                                            v_t lim, auto &last_use, int UT, auto &velocity,
                                                            auto &velocity_flow, auto &deltas, auto &field) {
        last_use[x][y] = UT - 1;
        v_t ret = 0;
        for (auto [dx, dy]: deltas) {
            int nx = x + dx, ny = y + dy;
            if (field[nx][ny] != '#' && last_use[nx][ny] < UT) {
                auto cap = velocity.get(x, y, dx, dy, deltas);
                auto flow = velocity_flow.get(x, y, dx, dy, deltas);
                if (flow == cap) {
                    continue;
                }
                // assert(v >= velocity_flow.get(x, y, dx, dy));
                auto vp = min(lim, cap - flow);
                if (last_use[nx][ny] == UT - 1) {
                    if (x == 24 && y == 63 && dx == -1 && dy == 0) {
                        // std::cout << "There is a bug!\n";
                    }
                    velocity_flow.add(x, y, dx, dy, vp, deltas);
                    last_use[x][y] = UT;
                    // cerr << x << " " << y << " -> " << nx << " " << ny << " " << vp
                    // << / " << lim << "\n";
                    return {vp, 1, {nx, ny}};
                }
                auto [t, prop, end] = propagate_flow(nx, ny, vp, last_use, UT, velocity, velocity_flow, deltas, field);
                ret += t;
                if (prop) {
                    if (x == 24 && y == 63 && dx == -1 && dy == 0) {
                        // std::cout << "There is a bug!\n";
                    }
                    velocity_flow.add(x, y, dx, dy, t, deltas);
                    last_use[x][y] = UT;
                    // cerr << x << " " << y << " -> " << nx << " " << ny << " " << t <<
                    // " << lim << "\n";
                    return {t, prop && end != pair(x, y), end};
                }
            }
        }
        last_use[x][y] = UT;
        return {ret, 0, {0, 0}};
    }
};
