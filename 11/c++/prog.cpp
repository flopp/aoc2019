#include <cassert>
#include <deque>
#include <iostream>
#include <map>
#include <string>
#include <utility>
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

struct XY {
    int x{0};
    int y{0};
    XY(int _x, int _y) : x{_x}, y{_y} {};

    bool operator<(const XY& other) const {
        return (x < other.x) || ((x == other.x) && (y < other.y));
    }
    bool operator==(const XY& other) const {
        return (x == other.x) && (y == other.y);
    }
};

void part(int partNumber, const State& initialState) {
    State state = initialState;
    DATATYPE iptr = 0;

    std::map<XY, bool> hull;
    XY xy{0, 0};
    int dir = 0;

    if (partNumber != 1) {
        hull[xy] = 1;
    }

    for (;;) {
        assert(state.inputs.size() == 0);
        auto p = hull.find(xy);
        if ((p == hull.end()) || (!p->second)) {
            state.inputs.push_back(0);
        } else {
            state.inputs.push_back(1);
        }

        const bool halted = run(state, iptr);
        if (halted) {
            break;
        }

        assert(state.outputs.size() == 2);
        assert(state.outputs[0] == 0 || state.outputs[0] == 1);
        assert(state.outputs[1] == 0 || state.outputs[1] == 1);
        hull[xy] = state.outputs[0];
        if (state.outputs[1] == 0) {
            --dir;
        } else {
            ++dir;
        }
        dir = (dir + 4) % 4;

        switch (dir) {
            case 0:
                xy.y--;
                break;
            case 1:
                xy.x++;
                break;
            case 2:
                xy.y++;
                break;
            case 3:
                xy.x--;
                break;
        }

        state.outputs.clear();
    }

    if (partNumber == 1) {
        std::cout << "part1: " << hull.size() << std::endl;
    } else {
        std::cout << "part2:" << std::endl;

        int min_x = 0;
        int max_x = 0;
        int min_y = 0;
        int max_y = 0;

        for (auto p: hull) {
            min_x = std::min(min_x, p.first.x);
            max_x = std::max(max_x, p.first.x);
            min_y = std::min(min_y, p.first.y);
            max_y = std::max(max_y, p.first.y);
        }
        for (xy.x = min_y; xy.y <= max_y; ++xy.y) {
            for (xy.x = min_x; xy.x <= max_x; ++xy.x) {
                auto p = hull.find(xy);
                if ((p == hull.end()) || (!p->second)) {
                    std::cout << " ";
                } else {
                    std::cout << "#";
                }
            }
            std::cout << std::endl;
        }
    }
}

int main(int argc, const char** argv) {
    State state;

    DATATYPE i;
    while (std::cin >> i) {
        std::cin.ignore();
        state.memory.push_back(i);
    }

    part(1, state);
    part(2, state);

    return 0;
}
