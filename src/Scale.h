#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace balancer {
class Scale {
public:
    Scale(std::string name)
    : name_{std::move(name)} 
    {}

    const auto& name() const {
        return name_;
    }

    void setPlateMass(std::uint64_t mass, bool leftOrRight) {
        if (leftOrRight) {
            left_.mass = mass;
        }
        else {
            right_.mass = mass;            
        }
    }
    
    void setPlateScale(std::unique_ptr<Scale> child, bool leftOrRight) {
        if (leftOrRight) {
            left_.childScale = std::move(child);
        }
        else {
            right_.childScale = std::move(child);
        }
    }

    template<typename Pred>
    void visitPlatesMasses(Pred pred) {
        // BFS ... actually one order or another is the same ... just need to visit all the nodes
        std::vector<Scale*> level, next;
        level.push_back(this);
        while(!level.empty()) {
            next.clear();
            for(auto* scale : level) {
                pred(std::ref(scale->name_), scale->left_.mass, scale->right_.mass);
                if(scale->left_.childScale) {
                    next.push_back(scale->left_.childScale.get());
                }
                if(scale->right_.childScale) {
                    next.push_back(scale->right_.childScale.get());
                }
            }
            std::swap(level, next);
        }
    }

    // add masses in the proper places to let the tree starting from this scale be fully balanced
    void balance() {
        balance_(*this);
    }

private:
    // returns total mass of the tree departing from the root, after the rebalance
    static std::uint64_t balance_(Scale& root);

    std::string name_;

    struct Plate {
        std::uint64_t mass{0};
        std::unique_ptr<Scale> childScale;
    };
    Plate left_;
    Plate right_;
};

using ScaleForest = std::vector<std::unique_ptr<balancer::Scale>>;

using Masses = std::vector<std::tuple<std::string, std::uint64_t, std::uint64_t>>;

[[nodiscard]]
Masses get_masses(const ScaleForest& forest);

// return mass deltas
[[nodiscard]]
Masses balance(const ScaleForest& forest);

void print(const Masses& masses);
}
