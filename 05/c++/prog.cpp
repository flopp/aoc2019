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

void run(std::vector<int>& memory, const std::vector<int>& inputs) {
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
                std::cout << op1 << std::endl;
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

int main(int argc, const char** argv) {
    if (argc < 2) {
        std::cerr << "USAGE: " << argv[0] << "[values...]" << std::endl;
        return 1;
    }

    std::vector<int> inputs;
    for (int i = 1; i < argc; ++i) {
        inputs.push_back(std::stoi(argv[i]));
    }

    std::vector<int> memory;
    int i;
    while (std::cin >> i) {
        std::cin.ignore();
        memory.push_back(i);
    }

    run(memory, inputs);

    return 0;
}