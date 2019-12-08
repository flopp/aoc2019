#include <algorithm>
#include <cassert>
#include <deque>
#include <iostream>
#include <string>
#include <vector>

#define INSTR_ADD 1
#define INSTR_MUL 2
#define INSTR_INP 3
#define INSTR_OUT 4
#define INSTR_JIT 5
#define INSTR_JIF 6
#define INSTR_LSS 7
#define INSTR_EQU 8
#define INSTR_HLT 99

int value(const std::vector<int>& memory, int position, bool immediate) {
    if (immediate) {
        return position;
    }
    return memory[position];
}

bool run(
    std::vector<int>& memory,
    std::vector<int>::iterator& iptr,
    std::deque<int>& inputs,
    std::deque<int>& outputs)
{
    while (iptr != memory.end()) {
        const int opcode = *(iptr++);
        const int instruction = opcode % 100;
        const bool immediate1 = ((opcode / 100) % 10) == 1;
        const bool immediate2 = ((opcode / 1000) % 10) == 1;

        switch (instruction) {
            case INSTR_ADD: {
                const int op1 = value(memory, *(iptr++), immediate1);
                const int op2 = value(memory, *(iptr++), immediate2);
                const int op3 = *(iptr++);
                memory[op3] = op1 + op2;
                break;
            }
            case INSTR_MUL: {
                const int op1 = value(memory, *(iptr++), immediate1);
                const int op2 = value(memory, *(iptr++), immediate2);
                const int op3 = *(iptr++);
                memory[op3] = op1 * op2;
                break;
            }
            case INSTR_INP: {
                if (inputs.empty()) {
                    iptr--;
                    return false;
                }
                const int op1 = *(iptr++);
                memory[op1] = inputs.front();
                inputs.pop_front();
                break;
            }
            case INSTR_OUT: {
                const int op1 = value(memory, *(iptr++), immediate1);
                outputs.push_back(op1);
                break;
            }
            case INSTR_JIT: {
                const int op1 = value(memory, *(iptr++), immediate1);
                const int op2 = value(memory, *(iptr++), immediate2);
                if (op1 != 0) {
                    iptr = memory.begin() + op2;
                }
                break;
            }
            case INSTR_JIF: {
                const int op1 = value(memory, *(iptr++), immediate1);
                const int op2 = value(memory, *(iptr++), immediate2);
                if (op1 == 0) {
                    iptr = memory.begin() + op2;
                }
                break;
            }
            case INSTR_LSS: {
                const int op1 = value(memory, *(iptr++), immediate1);
                const int op2 = value(memory, *(iptr++), immediate2);
                const int op3 = *(iptr++);
                if (op1 < op2) {
                    memory[op3] = 1;
                } else {
                    memory[op3] = 0;
                }
                break;
            }
            case INSTR_EQU: {
                const int op1 = value(memory, *(iptr++), immediate1);
                const int op2 = value(memory, *(iptr++), immediate2);
                const int op3 = *(iptr++);
                if (op1 == op2) {
                    memory[op3] = 1;
                } else {
                    memory[op3] = 0;
                }
                break;
            }
            case INSTR_HLT:
                return true;
            default:
                std::cerr << "Invalid instruction: " << instruction
                          << std::endl;
                exit(1);
        }
    }

    std::cerr << "ran out of instructions :(" << std::endl;
    return true;
}

int asc_sim(const std::vector<int>& asc, const std::vector<int>& phases) {
    std::vector<int> prog[5];
    std::vector<int>::iterator iptr[5];
    std::deque<int> inputs[5];
    for (int i = 0; i < 5; ++i) {
        prog[i] = asc;
        iptr[i] = prog[i].begin();
        inputs[i].push_back(phases[i]);
    }
    inputs[0].push_back(0);

    for (;;) {
        run(prog[0], iptr[0], inputs[0], inputs[1]);
        run(prog[1], iptr[1], inputs[1], inputs[2]);
        run(prog[2], iptr[2], inputs[2], inputs[3]);
        run(prog[3], iptr[3], inputs[3], inputs[4]);
        if (run(prog[4], iptr[4], inputs[4], inputs[0])) {
            return inputs[0].back();
        }
    }
    return -1;
}

int find_max_value(const std::vector<int>& asc, const std::vector<int>& phases) {
    std::vector<int> max_phases;
    int max_sim = 0;
    auto permuted_phases = phases;
    do {
        const int sim = asc_sim(asc, permuted_phases);
        if (max_phases.empty() || (sim > max_sim)) {
            max_phases = permuted_phases;
            max_sim = sim;
        }
    } while (std::next_permutation(permuted_phases.begin(), permuted_phases.end()));

    return max_sim;
}

int main(int argc, const char** argv) {
    assert(argc == 1);

    std::vector<int> asc;
    int i;
    while (std::cin >> i) {
        std::cin.ignore();
        asc.push_back(i);
    }

    std::cout << "part1 " << find_max_value(asc, std::vector<int>({0, 1, 2, 3, 4})) << std::endl;
    std::cout << "part2 " << find_max_value(asc, std::vector<int>({5, 6, 7, 8, 9})) << std::endl;

    return 0;
}
