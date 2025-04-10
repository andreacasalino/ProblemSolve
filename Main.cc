#include <InputParser.h>
#include <Scale.h>

#include <iostream>

int main(int argc, const char** argv) {
    if(argc == 1) {
        throw std::runtime_error{"Input file not specfied"};
    }
    std::filesystem::path inputFile{argv[1]};
    auto forest = balancer::importScalesForest(inputFile);

    auto massesDeltas = balancer::balance(forest);
    balancer::print(massesDeltas);

    return EXIT_SUCCESS;
}
