#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <unordered_map>

#include "ToolsBR.h"

int main() {
    std::unordered_map<std::string, std::set<std::string>> matrix;
    std::string key, value;
    std::cout << "Write a poset pairs in format: x y\nWhite 'done' if  you finish." << std::endl;
    while (true) {
        std::cin >> key;
        if (key == "done") {
            break;
        }

        std::cin >> value;
        matrix[key].insert(value);

        if (matrix.find(value) == matrix.end()) {
            matrix[value];
        }
    }

    if (!IsPoset(matrix)) {
        std::cout << "This is not a partially ordered set.\nPlease write a poset.";
        return 0;
    }

    MakeCoveringRelation(matrix);

    if (IsLattice(matrix)) {
        std::cout << "The poset is a lattice." << std::endl;
    } else {
        std::cout << "The poset is not a lattice." << std::endl;
    }

    DrawGraph(matrix);
    system("dot graph.dot -Tpng -o graph1.png");
    return 0;
}
