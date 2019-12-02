
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

int run(std::vector<int>& program, int noun, int verb) {
    program[1] = noun;
    program[2] = verb;

    auto iptr = program.begin();
    while (iptr != program.end()) {
        const int opcode = *(iptr++);
        const int position1 = *(iptr++);
        const int position2 = *(iptr++);
        const int position3 = *(iptr++);

        if (opcode == 1) {
            program[position3] = program[position1] + program[position2];
        } else if (opcode == 2) {
            program[position3] = program[position1] * program[position2];
        } else if (opcode == 99) {
            break;
        } else {
            std::cerr << "Invalid opcode: " << opcode << std::endl;
            return 1;
        }
    }

    return program[0];
}

int main(int argc, const char **argv) {
    if (argc < 2) {
        std::cerr << "USAGE: " << argv[0] << "[values...]" << std::endl;
        return 1;
    }

    std::vector<int> program;
    int i;
    while (std::cin >> i) {
        std::cin.ignore();
        program.push_back(i);
    }

    const std::string mode = argv[1];
    if (mode == "puzzle1") {
        const int noun = std::stoi(argv[2]);
        const int verb = std::stoi(argv[3]);
        std::cout << run(program, noun, verb) << std::endl;
    } else if (mode == "puzzle2") {
        const int output = std::stoi(argv[2]);
        for (int noun = 0; noun <= 99; ++noun) {
            for (int verb = 0; verb <= 99; ++verb) {
                auto program2 = program;
                if (run(program2, noun, verb) == output) {
                    std::cout << (100 * noun + verb) << std::endl;
                    return 0;
                }
            }
        }

    }

    return 0;
}