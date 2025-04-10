This is a **CMake** project, with the following targets:
- **Balancer-lib** is a library with the minimum functionalities needed to parse the scales structures, rebalance them and print the results.
- **Balancer-lib-tests** links to **Balancer-lib** and to [**gtest**](https://github.com/google/googletest) and contains the unit tests used to validate the implementation.
- **Balancer** is the main application required to develop. It expects the path of the input file to be passed and prints the masses addition needed for the balancing.

In addition to the requirements, these were my assumptions:
- the proposed implementation does not actually expect the input to be alphabetically sorted according to the scales names. Anyway, printed results adhere to such ordering.
- the scales structure could be actually a forest, i.e. an ensemble of trees. Each tree is imported and independently rebalanced.
