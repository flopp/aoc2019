#include <cassert>
#include <cmath>
#include <cctype>
#include <iostream>
#include <map>
#include <string>
#include <vector>

typedef long long Amount;
typedef std::string Chemical;

struct ChemicalAmount {
    Chemical chemical{""};
    Amount amount{0};

    ChemicalAmount() = default;
    ChemicalAmount(const Chemical& c, const Amount& a) : chemical(c), amount(a) {}
};

std::ostream& operator<<(std::ostream& os, const ChemicalAmount& ca) {
    os << ca.amount << " " << ca.chemical;
    return os;
}

struct Reaction {
    std::vector<ChemicalAmount> inputs;
    ChemicalAmount output;

    Reaction() = default;

    void parse(const std::string& s) {
        std::vector<ChemicalAmount> stuff;

        for (auto p = s.begin(); p != s.end(); /**/) {
            auto start = p;
            while (std::isdigit(*p)) {
                ++p;
            }
            const Amount amount = std::atoi(s.substr(start - s.begin(), p - start).c_str());

            ++p;
            start = p;
            while (std::isalpha(*p)) {
                ++p;
            }
            const Chemical chemical = s.substr(start - s.begin(), p - start);
            
            stuff.push_back(ChemicalAmount{chemical, amount});

            while ((p != s.end()) && (!std::isdigit(*p))) {
                ++p;
            }
        }

        assert(stuff.size() >= 2);
        output = stuff.back();
        stuff.pop_back();
        inputs = stuff;
    }
};

std::ostream& operator<<(std::ostream& os, const Reaction& r) {
    for (const auto& ca: r.inputs) {
        os << ca << ", ";
    }
    os << "=> " << r.output;
    return os;
}

int main(int /*argc*/, const char** /*argv*/) {
    std::map<Chemical, Reaction> reactions;
    const std::regex re{"^()=> (\\d+) ([A-Z]+)$"};
    std::string s;
    while (std::getline(std::cin, s)) {
        Reaction reaction;
        reaction.parse(s);
        reactions[reaction.output.chemical] = reaction;
    }

    std::map<Chemical, Amount> chemicals;
    chemicals["FUEL"] = -1;

    while (true) {
        ChemicalAmount needed{"", 0};
        for (auto ca: chemicals) {
            if (ca.first == "ORE") {
                continue;
            }
            if (ca.second < 0) {
                needed.chemical = ca.first;
                needed.amount = -ca.second;
                break;
            }
        }
        if (needed.amount == 0) {
            break;
        }
        
        auto reaction = reactions.find(needed.chemical);
        if (reaction == reactions.end()) {
            break;
        }

        Amount count = ceil(needed.amount / (double)reaction->second.output.amount);
        
        chemicals[needed.chemical] += count * reaction->second.output.amount;
        for (const auto& ca: reaction->second.inputs) {
            chemicals[ca.chemical] -= count * ca.amount;
        }
    }
    std::cout << "part1: " << -chemicals["ORE"] << " ORE for 1 FUEL" << std::endl;

    return 0;
}
