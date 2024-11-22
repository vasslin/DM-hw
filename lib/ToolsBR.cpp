#include "ToolsBR.h"

// remove in source matrix such edges that can be repesented because of transitivity
// make composition of two relations
bool TransitiveClosureIteration(std::unordered_map<std::string, std::set<std::string>>& matrix_1,
                                std::unordered_map<std::string, std::set<std::string>>& matrix_in_pow,
                                std::unordered_map<std::string, std::set<std::string>>& matrix) {
    std::unordered_map<std::string, std::set<std::string>> composition_matrix;
    bool was_erasing = false;
    for (const auto& [x, from_x] : matrix_1) {
        for (const auto& z : from_x) {
            for (const auto& y : matrix_in_pow[z]) {
                composition_matrix[x].insert(y);
                matrix[x].erase(y);
            }
        }
    }
    matrix_in_pow = composition_matrix;
    return was_erasing;
}

void DeleteLoops(std::unordered_map<std::string, std::set<std::string>>& matrix) {
    for (auto& [x, from_x] : matrix) {
        from_x.erase(x);
    }
}

void MakeCoveringRelation(std::unordered_map<std::string, std::set<std::string>>& matrix) {
    DeleteLoops(matrix);
    std::unordered_map<std::string, std::set<std::string>> matrix_1{matrix};
    std::unordered_map<std::string, std::set<std::string>> matrix_in_pow{matrix};
    bool writed = false;
    int count = matrix.size();
    while (count > 0) {
        TransitiveClosureIteration(matrix_1, matrix_in_pow, matrix);
        --count;
    }
    for (const auto& [x, from_x] : matrix) {
        for (const auto& y : from_x) {
            if (!writed) {
                std::cout << "The covering relation is" << std::endl;
                writed = true;
            }
            std::cout << x << " --> " << y << std::endl;
        }
    }
}

bool dfs(std::unordered_map<std::string, std::set<std::string>>& matrix, std::string a, std::string c) {
    for (const auto& v : matrix[a]) {
        if (v == c) {
            return true;
        }
        if (dfs(matrix, v, c)) {
            return true;
        }
    }
    return false;
}

bool IsLattice(std::unordered_map<std::string, std::set<std::string>>& matrix) {
    // poset is a lattice iff every two elements have both glb and lub
    // (x, y) have lub <=> ∃!z: (x, z) && (y, z)
    // (x, y) have glb <=> ∃!z: (z, x) && (z, y)
    std::string inf;
    std::string sup;

    for (const auto& [x, from_x] : matrix) {
        for (const auto& [y, from_y] : matrix) {
            sup = "";
            inf = "";
            if (x == y) {
                continue;
            }
            for (const auto& [z, from_z] : matrix) {
                // lub
                if ((x == z || dfs(matrix, x, z)) && (y == z || dfs(matrix, y, z))) {
                    if (sup == "") {
                        sup = z;
                    } else {
                        if (dfs(matrix, z, sup)) {
                            sup = z;
                        } else if (dfs(matrix, sup, z)) {
                        } else {  // не сравнимы
                            std::cout << "Pair (" + x + ", " + y + ") has two supremums : " + sup + " and " + z
                                      << std::endl;
                            return false;
                        }
                    }
                }
                // glb
                if ((x == z || dfs(matrix, z, x)) && (y == z || dfs(matrix, z, y))) {
                    if (inf == "") {
                        inf = z;
                    } else {
                        if (dfs(matrix, inf, z)) {
                            inf = z;
                        } else if (dfs(matrix, z, inf)) {
                        } else {  // не сравнимы
                            std::cout << "Pair (" + x + ", " + y + ") has two infimums : " + inf + " and " + z
                                      << std::endl;
                            return false;
                        }
                    }
                }
            }
            if (inf == "") {
                std::cout << "pair ( " + x + ", " + y + " ) hasn't infimum" << std::endl;
                return false;
            }
            if (sup == "") {
                std::cout << "pair ( " + x + ", " + y + " ) hasn't supremum" << std::endl;
                return false;
            }
        }
    }
    return true;
}

void DrawGraph(std::unordered_map<std::string, std::set<std::string>>& matrix) {
    std::ofstream graph;
    graph.open("graph.dot");
    graph << "digraph G {rankdir=BT\nedge [arrowhead=none];\n";
    for (const auto& [from, adj_list] : matrix) {
        for (const auto& to : adj_list) {
            graph << from + " -> " + to + ";\n";
        }
    }
    graph << "}";
    return;
}

bool IsPoset(std::unordered_map<std::string, std::set<std::string>>& matrix) {
    for (const auto& [x, from_x] : matrix) {
        // reflexivity
        if (from_x.find(x) == from_x.end()) {
            std::cout << "Not reflexive: " + x << std::endl;
            return false;
        }

        for (const auto& y : from_x) {
            // antisymmetry
            if (x != y && matrix[y].find(x) != matrix[y].end()) {
                std::cout << "Not antisymmetry: " + x + " " + y << std::endl;
                return false;
            }

            // transitivity
            for (const auto& z : matrix[y]) {
                if (from_x.find(z) == from_x.end()) {
                    std::cout << "Not transitive: " + x + ", " + y + " and " + y + ", " + z << std::endl;
                    return false;
                }
            }
        }
    }
    return true;
}
