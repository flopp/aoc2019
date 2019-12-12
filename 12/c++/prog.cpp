#include <cassert>
#include <deque>
#include <iostream>
#include <map>
#include <numeric>
#include <regex>
#include <string>
#include <utility>
#include <vector>

struct XYZ {
    int x{0};
    int y{0};
    int z{0};
    XYZ() = default;
    XYZ(int _x, int _y, int _z) : x{_x}, y{_y}, z{_z} {};

    int manhattan() const {
        return abs(x) + abs(y) + abs(z);
    }

    int& operator[](int i) {
        switch (i) {
            case 0:
                return x;
            case 1:
                return y;
            default:
                return z;
        }
    }
    const int& operator[](int i) const {
        switch (i) {
            case 0:
                return x;
            case 1:
                return y;
            default:
                return z;
        }
    }
};

struct Moon {
    XYZ p;
    XYZ v;

    template <int DIM>
    void apply_gravity(const Moon& other) {
        const int d = p[DIM] - other.p[DIM];
        if (d < 0) {
            v[DIM]++;
        } else if( d > 0) {
            v[DIM]--;
        }
    }

    void apply_gravity_all(const Moon& other) {
        apply_gravity<0>(other);
        apply_gravity<1>(other);
        apply_gravity<2>(other);
    }

    template <int DIM>
    void apply_velocity() {
        p[DIM] += v[DIM];
    }

    void apply_velocity_all() {
        apply_velocity<0>();
        apply_velocity<1>();
        apply_velocity<2>();
    }

    int pot() const {
        return p.manhattan();
    }
    int kin() const {
        return v.manhattan();
    }
    int energy() const {
        return pot() * kin();
    }
};

template <int DIM>
long long simulate_fixpoint(const std::vector<Moon>& initial_moons) {
    std::vector<Moon> moons = initial_moons;
    long long loops = 1;
    for (;;++loops) {
        for (auto& moon: moons) {
            for (const auto& other: moons) {
                moon.apply_gravity<DIM>(other);
            }
        }
        for (auto& moon: moons) {
            moon.apply_velocity<DIM>();
        }

        bool same = true;
        for (std::size_t i = 0; i < initial_moons.size(); ++i) {
            if (moons[i].v[DIM] != 0) {
                same = false;
                break;
            }
            if (moons[i].p[DIM] != initial_moons[i].p[DIM]) {
                same = false;
                break;
            }
        }

        if (same) {
            break;
        }
    }
    return loops;
}

int main(int /*argc*/, const char** argv) {
    const long long loops = std::atoi(argv[1]);

    std::vector<Moon> initial_moons;

    const std::regex re{"^\\s*<x=(-?\\d+), y=(-?\\d+), z=(-?\\d+)>\\s*$"};
    std::string s;
    while (std::getline(std::cin, s)) {
        std::smatch match;
        if (std::regex_match(s, match, re)) {
            Moon m;
            m.p.x = std::atoi(match[1].str().c_str());
            m.p.y = std::atoi(match[2].str().c_str());
            m.p.z = std::atoi(match[3].str().c_str());
            initial_moons.emplace_back(m);
        }
    }


    std::vector<Moon> moons = initial_moons;
    for (long long loop = 0; loop < loops; ++loop) {
        for (auto& moon: moons) {
            for (const auto& other: moons) {
                moon.apply_gravity_all(other);
            }
        }
        for (auto& moon: moons) {
            moon.apply_velocity_all();
        }
    }

    int total_energy = 0;
    for (auto& moon: moons) {
        total_energy += moon.energy();
    }
    std::cout << "part1: " << total_energy << std::endl;


    auto loops_x = simulate_fixpoint<0>(initial_moons);
    auto loops_y = simulate_fixpoint<1>(initial_moons);
    auto loops_z = simulate_fixpoint<2>(initial_moons);

    std::cout << "part2: " << std::lcm(std::lcm(loops_x, loops_y), loops_z) << std::endl;

    return 0;
}
