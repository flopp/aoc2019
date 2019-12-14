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
    XY() = default;
    XY(int _x, int _y) : x{_x}, y{_y} {};

    bool operator<(const XY& other) const {
        return (x < other.x) || ((x == other.x) && (y < other.y));
    }
    bool operator==(const XY& other) const {
        return (x == other.x) && (y == other.y);
    }
};

void part1(const State& initialState) {
    State state = initialState;
    DATATYPE iptr = 0;

    const bool halted = run(state, iptr);
    assert(halted);
    assert((state.outputs.size() % 3) == 0);

    std::map<XY, int> screen;
    for (auto out = state.outputs.begin(); out != state.outputs.end(); /**/) {
        auto x = *(out++);
        auto y = *(out++);
        auto t = *(out++);
        screen[XY{(int)x, (int)y}] = (int)t;
    }

    int blocks = 0;
    for (auto pixel: screen) {
        if (pixel.second == 2) {
            ++blocks;
        }
    }
    std::cout << "part1: " << blocks << std::endl;
}

struct Screen {
    DATATYPE _max_x{-1};
    DATATYPE _max_y{-1};
    DATATYPE _score{-1};
    DATATYPE _ball{-1};
    DATATYPE _paddle{-1};

    std::map<XY, DATATYPE> _pixels;

    Screen() = default;

    void update(const STREAM& stream) {
        for (auto out = stream.begin(); out != stream.end(); /**/) {
            auto x = *(out++);
            auto y = *(out++);
            auto t = *(out++);
            if ((x == -1) && (y == 0)) {
                _score = t;
                continue;
            }
            _pixels[XY{(int)x, (int)y}] = t;
            _max_x = std::max(_max_x, x);
            _max_y = std::max(_max_y, y);

            switch (t) {
                case 3:
                    _paddle = x;
                    break;
                case 4:
                    _ball = x;
                    break;
                default:
                    break;
            }
        }
    }

    int draw() const {
        XY xy;
        int blocks = 0;
        for (xy.y = 0; xy.y != _max_y; ++xy.y) {
            for (xy.x = 0; xy.x != _max_x; ++xy.x) {
                auto p = _pixels.find(xy);
                if (p == _pixels.end()) {
                    std::cout << "?";
                    continue;
                }

                switch (p->second) {
                    case 0:
                        std::cout << " ";
                        break;
                    case 1:
                        std::cout << "#";
                        break;
                    case 2:
                        std::cout << "X";
                        ++blocks;
                        break;
                    case 3:
                        std::cout << "=";
                        break;
                    case 4:
                        std::cout << "o";
                        break;
                    default:
                        std::cout << "?";
                }
            }
            std::cout << "\n";
        }

        std::cout << "SCORE=" << _score << " BLOCKS=" << blocks << " BALL=" << _ball << " PADDLE=" << _paddle << std::endl;
        return blocks;
    }
};

void part2(const State& initialState) {
    State state = initialState;
    DATATYPE iptr = 0;

    state.memory[0] = 2;
    Screen screen;

    for (;;) {
        const bool halted = run(state, iptr);
        screen.update(state.outputs);
        const int blocks = screen.draw();
        
        if (screen._ball < screen._paddle) {
            state.inputs.push_back(-1);
        } else if (screen._ball == screen._paddle) {
            state.inputs.push_back(0);
        } else {
            state.inputs.push_back(+1);
        }

        if (halted || (blocks == 0)) {
            break;
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

    part1(state);
    part2(state);

    return 0;
}
