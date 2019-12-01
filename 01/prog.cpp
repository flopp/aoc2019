#include <cmath>
#include <iostream>
#include <string>

int fuel_for_mass(int mass) {
    return static_cast<int>(floor(mass / 3.0)) - 2;
}

int total_fuel_for_mass(int mass) {
    const int fuel = fuel_for_mass(mass);
    if (fuel < 0) {
        return 0;
    }

    return fuel + total_fuel_for_mass(fuel);
}

int main(int argc, const char **argv) {
    if (argc != 2) {
        std::cerr << "USAGE: " << argv[0] << " <MODE>" << std::endl;
        return 1;
    }

    const std::string mode = argv[1];
    if (mode == "test1") {
        std::cout << "12: " << fuel_for_mass(12) << " (expected: 2)"
                  << std::endl;
        std::cout << "14: " << fuel_for_mass(14) << " (expected: 2)"
                  << std::endl;
        std::cout << "1969: " << fuel_for_mass(1969) << " (expected: 654)"
                  << std::endl;
        std::cout << "100756: " << fuel_for_mass(100756) << " (expected: 33583)"
                  << std::endl;
    } else if (mode == "test2") {
        std::cout << "14: " << total_fuel_for_mass(14) << " (expected: 2)"
                  << std::endl;
        std::cout << "1969: " << total_fuel_for_mass(1969) << " (expected: 966)"
                  << std::endl;
        std::cout << "100756: " << total_fuel_for_mass(100756)
                  << " (expected: 50346)" << std::endl;
    } else if (mode == "puzzle1") {
        int fuel_sum = 0;
        int mass = 0;
        while (std::cin >> mass) {
            fuel_sum += fuel_for_mass(mass);
        }
        std::cout << fuel_sum << std::endl;
    } else if (mode == "puzzle2") {
        int fuel_sum = 0;
        int mass = 0;
        while (std::cin >> mass) {
            fuel_sum += total_fuel_for_mass(mass);
        }
        std::cout << fuel_sum << std::endl;
    } else {
        std::cerr << "bad <MODE>: " << mode << std::endl;
        return 1;
    }

    return 0;
}