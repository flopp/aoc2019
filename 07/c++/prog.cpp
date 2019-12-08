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

void run(std::vector<int>& memory, const std::vector<int>& inputs, std::vector<int>& outputs) {
    auto input_ptr = inputs.begin();
    auto iptr = memory.begin();
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
                const int op1 = *(iptr++);
                memory[op1] = *(input_ptr++);
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
                return;
            default:
                std::cerr << "Invalid instruction: " << instruction
                          << std::endl;
                exit(1);
        }
    }
}




bool run2(
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
    std::vector<int> inputs;
    std::vector<int> outputs;
    std::vector<int> memory;
    
    memory = asc;
    inputs.clear();
    inputs.push_back(phases[0]);
    inputs.push_back(0);
    outputs.clear();
    run(memory, inputs, outputs);
    assert(outputs.size() == 1);

    memory = asc;
    inputs.clear();
    inputs.push_back(phases[1]);
    inputs.push_back(outputs[0]);
    outputs.clear();
    run(memory, inputs, outputs);
    assert(outputs.size() == 1);

    memory = asc;
    inputs.clear();
    inputs.push_back(phases[2]);
    inputs.push_back(outputs[0]);
    outputs.clear();
    run(memory, inputs, outputs);
    assert(outputs.size() == 1);

    memory = asc;
    inputs.clear();
    inputs.push_back(phases[3]);
    inputs.push_back(outputs[0]);
    outputs.clear();
    run(memory, inputs, outputs);
    assert(outputs.size() == 1);

    memory = asc;
    inputs.clear();
    inputs.push_back(phases[4]);
    inputs.push_back(outputs[0]);
    outputs.clear();
    run(memory, inputs, outputs);
    assert(outputs.size() == 1);

    return outputs[0];
}

int asc_sim2(const std::vector<int>& asc, const std::vector<int>& phases) {
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
        run2(prog[0], iptr[0], inputs[0], inputs[1]);
        run2(prog[1], iptr[1], inputs[1], inputs[2]);
        run2(prog[2], iptr[2], inputs[2], inputs[3]);
        run2(prog[3], iptr[3], inputs[3], inputs[4]);
        if (run2(prog[4], iptr[4], inputs[4], inputs[0])) {
            return inputs[0].back();
        }
    }
    return -1;
}


int main(int argc, const char** argv) {
    if (argc != 1) {
        std::cerr << "USAGE: " << argv[0] << std::endl;
        return 1;
    }

    std::vector<int> asc;
    int i;
    while (std::cin >> i) {
        std::cin.ignore();
        asc.push_back(i);
    }

    std::vector<int> max_phases;
    int max_sim = 0;
    std::vector<int> phases = {0, 1, 2, 3, 4};
    do {
        const int sim = asc_sim(asc, phases);
        if (max_phases.empty() || (sim > max_sim)) {
            max_phases = phases;
            max_sim = sim;
        }
    } while (std::next_permutation(phases.begin(), phases.end()));

    std::cout << "part1 "
        << max_phases[0]
        << max_phases[1]
        << max_phases[2]
        << max_phases[3]
        << max_phases[4]
        << " => "
        << max_sim
        << std::endl;

    max_phases.clear();
    max_sim = 0;
    phases = {5, 6, 7, 8, 9};
    do {
        const int sim = asc_sim2(asc, phases);
        if (max_phases.empty() || (sim > max_sim)) {
            max_phases = phases;
            max_sim = sim;
        }
    } while (std::next_permutation(phases.begin(), phases.end()));

    std::cout << "part2 "
        << max_phases[0]
        << max_phases[1]
        << max_phases[2]
        << max_phases[3]
        << max_phases[4]
        << " => "
        << max_sim
        << std::endl;

    return 0;
}