#include <Scale.h>

#include <algorithm>
#include <iostream>

namespace balancer {
std::uint64_t Scale::balance_(Scale& root) {
    std::uint64_t leftPlateMass = root.left_.mass;
    if(root.left_.childScale) {
        leftPlateMass += balance_(*root.left_.childScale);
    }
    std::uint64_t rightPlateMass = root.right_.mass;
    if(root.right_.childScale) {
        rightPlateMass += balance_(*root.right_.childScale);
    }
    std::uint64_t diff = 0;
    if(leftPlateMass < rightPlateMass) {
        diff = rightPlateMass - leftPlateMass;
        root.left_.mass += diff;
    }
    else if(rightPlateMass < leftPlateMass) {
        diff = leftPlateMass - rightPlateMass;
        root.right_.mass += diff;
    }
    return leftPlateMass + rightPlateMass + diff + 1;
}

Masses get_masses(const ScaleForest& forest) {
    Masses res;
    for(const auto& root : forest) {
        root->visitPlatesMasses([&res](const std::string& name, std::uint64_t left, std::uint64_t right) {
            res.emplace_back(std::make_tuple(name, left, right));
        });
    }
    std::sort(res.begin(), res.end(), [](const auto& a, const auto& b) {
        return std::get<0>(a) < std::get<0>(b);
    });
    return res;
}

Masses balance(const ScaleForest& forest) {
    Masses massesBeforeBalance = get_masses(forest);

    // balance all roots
    for(auto& root : forest) {
        root->balance();
    }

    // compute deltas
    Masses massesDeltas = get_masses(forest);
    for(std::size_t k=0; k<massesDeltas.size(); ++k) {
        std::get<1>(massesDeltas[k]) -= std::get<1>(massesBeforeBalance[k]);
        std::get<2>(massesDeltas[k]) -= std::get<2>(massesBeforeBalance[k]);
    }

    return massesDeltas;
}

void print(const Masses& masses) {
    for(const auto& [name, left, right] : masses) {
        std::cout << name << ',' << left << ',' << right << std::endl;
    }
}
}
