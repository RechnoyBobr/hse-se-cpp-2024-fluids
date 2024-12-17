#pragma once
#include <bits/stdc++.h>

#include <ctime>
#include <random>

#include "fixed.hpp"

template <class p_type, class v_type, class v_flow> class Simulation {
  constexpr static size_t N = 36, M = 84;
  // constexpr size_t N = 14, M = 5;
  constexpr static size_t T = 1'000'000;
  constexpr static std::array<pair<int, int>, 4> deltas{
      {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}};
  inline static int dirs[N][M];
  mt19937 rnd{1337};
  inline static int last_use[N][M];
  inline static int UT;
  inline static char field[N][M + 1];

  void propagate_stop(int x, int y, bool force = false) {
    if (!force) {

      bool stop = true;
      for (auto [dx, dy] : deltas) {
        int nx = x + dx, ny = y + dy;
        if (field[nx][ny] != '#' && last_use[nx][ny] < UT - 1 &&
            velocity.get(x, y, dx, dy) > v_type::from_raw(0)) {
          stop = false;
          break;
        }
      }
      if (!stop) {
        return;
      }
    }
    last_use[x][y] = UT;
    for (auto [dx, dy] : deltas) {
      int nx = x + dx, ny = y + dy;
      if (field[nx][ny] == '#' || last_use[nx][ny] == UT ||
          velocity.get(x, y, dx, dy) > v_type::from_raw(0)) {
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
      auto v = velocity.get(x, y, dx, dy);
      if (v < v_type(0)) {
        continue;
      }
      sum += v;
    }
    return sum;
  }

  class ParticleParams {
  public:
    char type = '1';
    p_type cur_p;
    array<v_type, deltas.size()> v;

    void swap_with(int x, int y) {
      swap(field[x][y], type);
      swap(p[x][y], cur_p);
      swap(velocity.v[x][y], v);
    }
  };

  bool propagate_move(int x, int y, bool is_first) {
    last_use[x][y] = UT - is_first;
    bool ret = false;
    int nx = -1, ny = -1;
    do {
      std::array<v_type, deltas.size()> tres;
      v_type sum = 0;
      for (size_t i = 0; i < deltas.size(); ++i) {
        auto [dx, dy] = deltas[i];
        int nx = x + dx, ny = y + dy;
        if (field[nx][ny] == '#' || last_use[nx][ny] == UT) {
          tres[i] = sum;
          continue;
        }
        auto v = velocity.get(x, y, dx, dy);
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

      p_type p = random01() * sum;
      size_t d = std::upper_bound(tres.begin(), tres.end(), p) - tres.begin();

      auto [dx, dy] = deltas[d];
      nx = x + dx;
      ny = y + dy;
      assert(velocity.get(x, y, dx, dy) > v_type(0) && field[nx][ny] != '#' &&
             last_use[nx][ny] < UT);

      ret = (last_use[nx][ny] == UT - 1 || propagate_move(nx, ny, false));
    } while (!ret);
    last_use[x][y] = UT;
    for (auto [dx, dy] : deltas) {
      int nx = x + dx, ny = y + dy;
      if (field[nx][ny] != '#' && last_use[nx][ny] < UT - 1 &&
          velocity.get(x, y, dx, dy) < v_type::from_raw(0)) {
        propagate_stop(nx, ny);
      }
    }
    if (ret) {
      if (!is_first) {
        ParticleParams pp{};
        pp.swap_with(x, y);
        pp.swap_with(nx, ny);
        pp.swap_with(x, y);
      }
    }
    return ret;
  }

  p_type rho[256];

  inline static p_type p[N][M], old_p[N][M];

  template <typename v_t, int P, int K> class VectorField {
  public:
    array<v_t, deltas.size()> v[N][M];
    VectorField() {
      for (auto &i : v) {
        for (auto &j : i) {
          for (auto &k : j) {
            k = 0;
          }
        }
      }
    }
    v_t &add(int x, int y, int dx, int dy, v_type dv) {
      return get(x, y, dx, dy) += dv;
    }

    v_t &get(int x, int y, int dx, int dy) {
      size_t i = ranges::find(deltas, pair(dx, dy)) - deltas.begin();
      if (i >= deltas.size()) {
        cout << dx << " " << dy << "\n";
      }
      assert(i < deltas.size());
      return v[x][y][i];
    }

    static tuple<v_t, bool, pair<int, int>> propagate_flow(int x, int y,
                                                           v_type lim) {
      last_use[x][y] = UT - 1;
      v_t ret = 0;
      for (auto [dx, dy] : deltas) {
        int nx = x + dx, ny = y + dy;
        if (field[nx][ny] != '#' && last_use[nx][ny] < UT) {
          auto cap = velocity.get(x, y, dx, dy);
          auto flow = velocity_flow.get(x, y, dx, dy);
          if (flow == cap) {
            continue;
          }
          // assert(v >= velocity_flow.get(x, y, dx, dy));
          auto vp = min(lim, cap - flow);
          if (last_use[nx][ny] == UT - 1) {
            velocity_flow.add(x, y, dx, dy, vp);
            last_use[x][y] = UT;
            // cerr << x << " " << y << " -> " << nx << " " << ny << " " << vp
            // << / " << lim << "\n";
            return {vp, 1, {nx, ny}};
          }
          auto [t, prop, end] = propagate_flow(nx, ny, vp);
          ret += t;
          if (prop) {
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

  inline static VectorField<v_type, v_type::n, v_type::k> velocity;
  inline static VectorField<v_flow, v_flow::n, v_flow::k> velocity_flow;
  // TODO: find a way to use sizes directly from compile options or runtime

  p_type random01() { return p_type::from_raw((rnd() & ((1 << v_type::k) - 1))); }

public:
  constexpr Simulation() {
    char field_c[N][M + 1] = {
        "######################################################################"
        "##"
        "##"
        "##########",
        "#                                                                     "
        "  "
        "  "
        "         #",
        "#                                                                     "
        "  "
        "  "
        "         #",
        "#                                                                     "
        "  "
        "  "
        "         #",
        "#                                                                     "
        "  "
        "  "
        "         #",
        "#                                                                     "
        "  "
        "  "
        "         #",
        "#                                       .........                     "
        "  "
        "  "
        "         #",
        "#..............#            #           .........                     "
        "  "
        "  "
        "         #",
        "#..............#            #           .........                     "
        "  "
        "  "
        "         #",
        "#..............#            #           .........                     "
        "  "
        "  "
        "         #",
        "#..............#            #                                         "
        "  "
        "  "
        "         #",
        "#..............#            #                                         "
        "  "
        "  "
        "         #",
        "#..............#            #                                         "
        "  "
        "  "
        "         #",
        "#..............#            #                                         "
        "  "
        "  "
        "         #",
        "#..............#............#                                         "
        "  "
        "  "
        "         #",
        "#..............#............#                                         "
        "  "
        "  "
        "         #",
        "#..............#............#                                         "
        "  "
        "  "
        "         #",
        "#..............#............#                                         "
        "  "
        "  "
        "         #",
        "#..............#............#                                         "
        "  "
        "  "
        "         #",
        "#..............#............#                                         "
        "  "
        "  "
        "         #",
        "#..............#............#                                         "
        "  "
        "  "
        "         #",
        "#..............#............#                                         "
        "  "
        "  "
        "         #",
        "#..............#............################                     #    "
        "  "
        "  "
        "         #",
        "#...........................#....................................#    "
        "  "
        "  "
        "         #",
        "#...........................#....................................#    "
        "  "
        "  "
        "         #",
        "#...........................#....................................#    "
        "  "
        "  "
        "         #",
        "##################################################################    "
        "  "
        "  "
        "         #",
        "#                                                                     "
        "  "
        "  "
        "         #",
        "#                                                                     "
        "  "
        "  "
        "         #",
        "#                                                                     "
        "  "
        "  "
        "         #",
        "#                                                                     "
        "  "
        "  "
        "         #",
        "#                                                                     "
        "  "
        "  "
        "         #",
        "#                                                                     "
        "  "
        "  "
        "         #",
        "#                                                                     "
        "  "
        "  "
        "         #",
        "#                                                                     "
        "  "
        "  "
        "         #",
        "######################################################################"
        "##"
        "##"
        "##########",
    };
    memcpy(field, field_c, sizeof(field_c));
    velocity = VectorField<v_type, v_type::n, v_type::k>();
    velocity_flow = VectorField<v_flow, v_flow::n, v_flow::k>();
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
    rho[' '] = p_type(0.01);
    rho['.'] = p_type(1000);
    p_type g = p_type(0.1);

    for (size_t x = 0; x < N; ++x) {
      for (size_t y = 0; y < M; ++y) {
        if (field[x][y] == '#')
          continue;
        for (auto [dx, dy] : deltas) {
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
          if (field[x + 1][y] != '#')
            velocity.add(x, y, 1, 0, g);
        }
      }

      // Apply forces from p
      memcpy(old_p, p, sizeof(p));
      for (size_t x = 0; x < N; ++x) {
        for (size_t y = 0; y < M; ++y) {
          if (field[x][y] == '#')
            continue;
          for (auto [dx, dy] : deltas) {
            int nx = static_cast<int>(x) + dx, ny = static_cast<int>(y) + dy;
            if (field[nx][ny] != '#' && old_p[nx][ny] < old_p[x][y]) {
              auto delta_p = old_p[x][y] - old_p[nx][ny];
              auto force = delta_p;
              auto &contr = velocity.get(nx, ny, -dx, -dy);
              if (contr * rho[(int)field[nx][ny]] >= force) {
                contr -= force / rho[(int)field[nx][ny]];
                continue;
              }
              force -= contr * rho[(int)field[nx][ny]];
              contr = 0;
              velocity.add(x, y, dx, dy, force / rho[(int)field[x][y]]);
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
              if (i == 3 && x == 23 && y == 63) {
                std::cout << "Hell break loose\n";
              }
              auto [t, local_prop, _] =
                  VectorField<v_flow, v_flow::n, v_flow::k>::propagate_flow(
                      x, y, 1);
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
          for (auto [dx, dy] : deltas) {
            auto old_v = velocity.get(x, y, dx, dy);
            auto new_v = velocity_flow.get(x, y, dx, dy);
            if (new_v.v() > 1000000) {
              std::cout << i << " " << x << " " << y << " " << new_v.v() << "\n";
            }
            if (old_v > v_type::from_raw(0)) {
              if (new_v > old_v) {
                cout << "Error";
              }
              assert(new_v <= old_v);
              velocity.get(x, y, dx, dy) = new_v;
              auto force = (old_v - new_v) * rho[(int)field[x][y]];
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
            if (random01() < move_prob(x, y)) {
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
        for (auto &x : field) {
          cout << x << "\n";
        }
      }
    }
  }
};
