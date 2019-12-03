
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Segment {
    int x1, y1, x2, y2;

    bool intersects(const Segment& other, int& x, int& y) const {
        if ((x1 == x2) &&
            (other.y1 == other.y2) &&
            (other.x1 <= x1) && (x1 <= other.x2) &&
            (y1 <= other.y1) && (other.y1 <= y2))
        {
            x = x1;
            y = other.y1;
            return true;
        }
    
        if ((y1 == y2) &&
            (other.x1 == other.x2) &&
            (other.y1 <= y1) && (y1 <= other.y2) &&
            (x1 <= other.x1) && (other.x1 <= x2))
        {
            x = other.x1;
            y = y1;
            return true;
        }

        return false;
    }
};

std::vector<Segment> createWire(const std::string& line) {
    std::vector<Segment> segments;
    std::istringstream iss(line.c_str());
    int x = 0;
    int y = 0;

    char direction;
    int distance;
    while (iss >> direction >> distance) {
        switch (direction) {
            case 'U':
                segments.push_back({x, y, x, y + distance});
                y += distance;
                break;
            case 'D':
                segments.push_back({x, y - distance, x, y});
                y -= distance;
                break;
            case 'L':
                segments.push_back({x - distance, y, x, y});
                x -= distance;
                break;
            case 'R':
                segments.push_back({x, y, x + distance, y});
                x += distance;
                break;
            default:
                std::cerr << "BAD DIRECTION: " << direction << std::endl;
                exit(1);
        }
        if (!(iss >> direction)) {
            break;
        }
    }
    
    return segments;
}


int nearest_crossing(const std::vector<Segment>& wire1, const std::vector<Segment>& wire2) {
    int min_dist = 0;
    for (const auto& s1: wire1) {
        for (const auto& s2: wire2) {
            int x, y;
            if (s1.intersects(s2, x, y)) {
                if (x < 0) {
                    x = -x;
                }
                if (y < 0) {
                    y = -y;
                }
                int dist = x + y;
                if ((min_dist == 0) || (dist < min_dist)) {
                    min_dist = dist;
                }
            }
        }
    }
    return min_dist;
}

int main(int argc, const char **argv) {
    if (argc < 2) {
        std::cerr << "USAGE: " << argv[0] << std::endl;
        return 1;
    }

    std::string line;
    std::getline(std::cin, line);
    const std::vector<Segment> wire1 = createWire(line);

    std::getline(std::cin, line);
    const std::vector<Segment> wire2 = createWire(line);

    int distance = nearest_crossing(wire1, wire2);
    std::cout << distance << std::endl;

    return 0;
}