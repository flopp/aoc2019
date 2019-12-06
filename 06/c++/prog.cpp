#include <cassert>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

struct SpaceObject;
typedef std::unordered_map<std::string, SpaceObject*> SpaceObjects;

struct SpaceObject {
    std::string name;
    std::string center;
    int depth{0};

    int compute_depth(const SpaceObjects& objects) {
        if ((depth > 0) || (center.empty())) {
            return depth;
        }

        auto other = objects.find(center);
        assert(other != objects.end());

        depth = 1 + other->second->compute_depth(objects);
        return depth;
    }

};

SpaceObjects read_objects(std::istream& is) {
    SpaceObjects objects;
    
    const std::regex re{"^\\s*([^\\s]+)\\)([^\\s]+)\\s*$"};
    std::string s;
    while (std::getline(is, s)) {
        std::smatch match;
        if (std::regex_match(s, match, re)) {
            auto obj = new SpaceObject;
            obj->name = match[2];
            obj->center = match[1];
            objects[obj->name] = obj;
        }
    }

    bool again = true;
    while (again) {
        again = false;
        for (auto obj: objects) {
            if (obj.second->center.empty()) {
                continue;
            }
            if (objects.find(obj.second->center) != objects.end()) {
                continue;
            }

            auto obj2 = new SpaceObject;
            obj2->name = obj.second->center;
            objects[obj2->name] = obj2;
            again = true;
            break;
        }
    }

    return objects;
}

std::vector<SpaceObject*> compute_path(const std::string& obj, const SpaceObjects& objects) {
    std::vector<SpaceObject*> path;
    return path;
}

int transfers(const std::string& obj1, const std::string& obj2, const SpaceObjects& objects) {
    auto path1 = compute_path(obj1, objects);
    auto path2 = compute_path(obj2, objects);
    return 0;
}

int main(int argc, const char** argv) {
    if (argc != 1) {
        std::cerr << "USAGE: " << argv[0] << std::endl;
        return 1;
    }

    auto objects = read_objects(std::cin);

    int sum_depths = 0;
    for (auto obj: objects) {
        sum_depths += obj.second->compute_depth(objects);
    }
    std::cout << "part1 " << sum_depths << std::endl;

    std::cout << "part2 " << transfers("YOU", "SAN", objects) << std::endl;

    for (auto obj: objects) {
        free(obj.second);
    }

    return 0;
}