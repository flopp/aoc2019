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

    std::vector<std::string> missing;
    for (auto obj: objects) {
        if (obj.second->center.empty()) {
            continue;
        }
        if (objects.find(obj.second->center) != objects.end()) {
            continue;
        }
        missing.push_back(obj.second->center);
    }
    for (auto name: missing) {
        auto obj = new SpaceObject;
        obj->name = name;
        objects[name] = obj;
    }

    return objects;
}

int transfers(const std::string& obj1, const std::string& obj2, const SpaceObjects& objects) {
    std::unordered_map<SpaceObject*, int> path1, path2;

    int depth = 0;
    for (auto current = obj1; !current.empty(); /**/) {
        auto obj = objects.find(current);
        assert(obj != objects.end());
        path1[obj->second] = depth++;
        current = obj->second->center;
    }

    depth = 0;
    for (auto current = obj2; !current.empty(); /**/) {
        auto obj = objects.find(current);
        assert(obj != objects.end());
        path2[obj->second] = depth++;
        current = obj->second->center;
    }

    for (auto current = obj1; !current.empty(); /**/) {
        auto obj = objects.find(current);
        assert(obj != objects.end());

        auto steps2 = path2.find(obj->second);
        if (steps2 != path2.end()) {
            auto steps1 = path1.find(obj->second);
            assert(steps1 != path1.end());

            return steps1->second + steps2->second - 2;
        }

        current = obj->second->center;
    }

    return -1;
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