#pragma once

#include <bits/stdc++.h>
#include "fixed.hpp"

using namespace std;

template<typename T, typename T1>
T amin(T &a, T1 b) {
    if (b < a)a = b;
    return a;
}


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

    v_t &add(int x, int y, int dx, int dy, v_t dv) {
        if (x == 21 && y == 64) {
            // std::cout << "Bug located\n";
        }
        return get(x, y, dx, dy) += dv;
    }

    v_t &get(int x, int y, int dx, int dy) {
        size_t i = (((dx & 1) ^ ((dx & 2) >> 1)) | (((dy & 1) ^ ((dy & 2) >> 1)) | ((dy & 1) << 1)));
        assert(i < 4 && i >= 0);
        return v[x][y][i];
    }

    static tuple<v_t, bool, pair<int, int> > propagate_flow(int x, int y,
                                                            v_t lim, auto &last_use, int &UT, auto &velocity,
                                                            auto &velocity_flow, auto &deltas, auto &field) {
        last_use[x][y] = UT - 1;
        v_t ret = 0;

        for (auto [dx, dy]: deltas) {
            int nx = x + dx, ny = y + dy;
            if (field[nx][ny] != '#' && last_use[nx][ny] < UT) {
                auto cap = velocity.get(x, y, dx, dy);
                auto flow = velocity_flow.get(x, y, dx, dy);
                if (flow == cap) {
                    continue;
                }
                // assert(v >= velocity_flow.get(x, y, dx, dy));
                v_t vp;
                if (lim < cap - flow) {
                    vp = lim;
                } else {
                    vp = cap - flow;
                }
                if (last_use[nx][ny] == UT - 1) {
                    velocity_flow.add(x, y, dx, dy, vp);
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
                    velocity_flow.add(x, y, dx, dy, t);
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
