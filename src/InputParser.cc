#include <InputParser.h>

#include <string_view>
#include <fstream>
#include <unordered_map>
#include <list>
#include <optional>
#include <array>

namespace balancer {
namespace {
struct Dependency {
    bool leftOrRight;
    std::string parentName;
};

struct ScaleData {
    std::string name;
    std::uint64_t leftMass{0};
    std::uint64_t rightMass{0};

    std::optional<Dependency> dep;
};

bool isNumber(char c) {
    return '0' <= c && c <= '9';
}

void parse_line(std::array<std::string_view, 3>& recipient, const std::string& line) {
    auto sepFirst = line.find(',');
    if(sepFirst == std::string::npos) {
        throw std::runtime_error{"Invalid input file: seperator not found"};
    }
    recipient[0] = std::string_view{line.data(), sepFirst};
    auto sepSecond = line.find(',', sepFirst+1);
    if(sepSecond == std::string::npos) {
        throw std::runtime_error{"Invalid input file: seperator not found"};
    }
    recipient[1] = std::string_view{line.data() + sepFirst+1, sepSecond-sepFirst-1};
    recipient[2] = std::string_view{line.data() + sepSecond+1, line.size()-sepSecond-1};
}

std::list<ScaleData> parse(const std::filesystem::path& source) {
    std::list<ScaleData> res;
    std::unordered_map<std::string_view, std::list<ScaleData>::iterator> resTable;
    std::vector<std::pair<std::string, Dependency>> dependencies;
    std::ifstream stream{source};
    if(!stream.is_open()) {
        throw std::runtime_error{"Unable to open input file"};
    }
    std::string buffer;
    std::array<std::string_view, 3> buffer_parser;
    while(!stream.eof()) {
        std::getline(stream, buffer);
        if(buffer.empty() || buffer.front() == '#') {
            continue;
        }
        parse_line(buffer_parser, buffer);
        
        std::string name{buffer_parser[0].data(), buffer_parser[0].size()};
        auto& added = res.emplace_front(ScaleData{name});
        resTable.emplace(std::string_view{added.name.data(), added.name.size()}, res.begin());
        
        const auto& leftData = buffer_parser[1];
        if(isNumber(leftData.front())) {
            added.leftMass = static_cast<std::uint64_t>(std::atoll(leftData.data()));
        }   
        else {
            dependencies.emplace_back(std::make_pair(
                std::string{leftData.data(), leftData.size()},
                Dependency {true, name}
            ));
        }     
        
        const auto& rightData = buffer_parser[2];
        if(isNumber(rightData.front())) {
            added.rightMass = static_cast<std::uint64_t>(std::atoll(rightData.data()));
        }   
        else {
            dependencies.emplace_back(std::make_pair(
                std::string{rightData.data(), rightData.size()},
                Dependency {false, name}
            ));
        }     
    }

    for(const auto& [child, dep] : dependencies) {
        auto it = resTable.at(child);
        it->dep.emplace(dep);
    }
    return res;
}
}

std::vector<std::unique_ptr<Scale>> importScalesForest(const std::filesystem::path& source) {    
    std::vector<std::unique_ptr<Scale>> res;
    auto scalesData = parse(source);
    std::unordered_map<std::string_view, Scale*> parsedCache;
    auto makeScale_ = [&](const ScaleData& data) {
        auto res = std::make_unique<Scale>(data.name);
        res->setPlateMass(data.leftMass, true);
        res->setPlateMass(data.rightMass, false);
        const auto& name = res->name();
        parsedCache.emplace(std::string_view{name.data(), name.size()}, res.get());
        return res;
    };

    // topological sort to build the different trees
    while (!scalesData.empty()) {
        std::size_t sizeSnap = scalesData.size();

        auto it = scalesData.begin();
        while(it != scalesData.end()) {
            if(it->dep.has_value()) {
                auto it_cache = parsedCache.find(std::string_view{it->dep->parentName.data(), it->dep->parentName.size()});
                if(it_cache == parsedCache.end()) {
                    ++it;
                }
                else {
                    it_cache->second->setPlateScale(makeScale_(*it), it->dep->leftOrRight);
                    it = scalesData.erase(it);
                }
            }
            else {
                // is one of the root of the forest
                res.emplace_back(makeScale_(*it));
                it = scalesData.erase(it);
            }
        }

        if(sizeSnap == scalesData.size()) {
            throw std::runtime_error{"Circular dependency detected!!! Input file is invalid"};
        }
    }
    return res;
}
}
