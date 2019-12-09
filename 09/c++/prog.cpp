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
#define INSTR_RBO 9
#define INSTR_HLT 99

typedef long long DATATYPE;
typedef std::vector<DATATYPE> MEMORY;
typedef std::deque<DATATYPE> STREAM;

struct State {
    MEMORY memory;
    DATATYPE relative_base{0};
    STREAM inputs;
    STREAM outputs;

    void write(DATATYPE ptr, int mode, DATATYPE value) {
        switch (mode) {
            case 0:
                set(get(ptr), value);
                break;
            case 1:
                assert(false);
                break;
            case 2:
                set(get(ptr) + relative_base, value);
                break;
            default:
                assert(false);
        }
    }

    DATATYPE read(DATATYPE ptr, int mode) const {
        switch (mode) {
            case 0:
                return get(get(ptr));
            case 1:
                return get(ptr);
            case 2:
                return get(get(ptr) + relative_base);
                break;
            default:
                assert(false);
        }
    }

    DATATYPE get(DATATYPE addr) const {
        assert(addr >= 0);
        if (static_cast<std::size_t>(addr) < memory.size()) {
            return memory[addr];
        }
        return 0;
    }

    void set(DATATYPE addr, DATATYPE value) {
        assert(addr >= 0);
        while (static_cast<std::size_t>(addr) >= memory.size()) {
            memory.push_back(0);
        }
        memory[addr] = value;
    }

    bool has_input() const {
        return !inputs.empty();
    }

    DATATYPE input() {
        DATATYPE data = inputs.front();
        inputs.pop_front();
        return data;
    }

    void output(DATATYPE data) {
        std::cout << "OUTPUT: " << data << std::endl;
        outputs.push_back(data);
    }
};

bool run(
    State& state,
    DATATYPE& iptr)
{
    for (;;) {
        const DATATYPE opcode = state.get(iptr);
        const int instruction = opcode % 100;
        const int mode1 = ((opcode / 100) % 10);
        const int mode2 = ((opcode / 1000) % 10);
        const int mode3 = ((opcode / 10000) % 10);

        switch (instruction) {
            case INSTR_ADD: {
                state.write(iptr+3, mode3, state.read(iptr+1, mode1) + state.read(iptr+2, mode2));
                iptr += 4;
                break;
            }
            case INSTR_MUL: {
                state.write(iptr+3, mode3, state.read(iptr+1, mode1) * state.read(iptr+2, mode2));
                iptr += 4;
                break;
            }
            case INSTR_INP: {
                if (state.has_input()) {
                    state.write(iptr+1, mode1, state.input());
                    iptr += 2;
                } else {
                    return false;
                }
                break;
            }
            case INSTR_OUT: {
                state.output(state.read(iptr+1, mode1));
                iptr += 2;
                break;
            }
            case INSTR_JIT: {
                if (state.read(iptr+1, mode1) != 0) {
                    iptr = state.read(iptr+2, mode2);
                } else {
                    iptr += 3;
                }
                break;
            }
            case INSTR_JIF: {
                if (state.read(iptr+1, mode1) == 0) {
                    iptr = state.read(iptr+2, mode2);
                } else {
                    iptr += 3;
                }
                break;
            }
            case INSTR_LSS: {
                state.write(iptr+3, mode3, (state.read(iptr+1, mode1) < state.read(iptr+2, mode2)) ? 1 : 0);
                iptr += 4;
                break;
            }
            case INSTR_EQU: {
                state.write(iptr+3, mode3, (state.read(iptr+1, mode1) == state.read(iptr+2, mode2)) ? 1 : 0);
                iptr += 4;
                break;
            }
            case INSTR_RBO: {
                state.relative_base += state.read(iptr+1, mode1);
                iptr += 2;
                break;
            }
            case INSTR_HLT:
                return true;
            default:
                std::cerr << "Invalid instruction: " << instruction << std::endl;
                exit(1);
        }
    }

    std::cerr << "ran out of instructions :(" << std::endl;
    return true;
}

int main(int argc, const char** argv) {
    State state;

    DATATYPE i;
    while (std::cin >> i) {
        std::cin.ignore();
        state.memory.push_back(i);
    }
    for (int i = 1; i < argc; ++i) {
        state.inputs.push_back(std::stoi(argv[i]));
    }

    DATATYPE iptr = 0;

    
    bool halted = run(state, iptr);
    std::cout << (halted ? "halted" : "stalled; waiting for input!") << std::endl;
    return 0;
}
