#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <unordered_map>

bool TransitiveClosureIteration(std::unordered_map<std::string, std::set<std::string>>& matrix_1,
                                std::unordered_map<std::string, std::set<std::string>>& matrix_in_pow,
                                std::unordered_map<std::string, std::set<std::string>>& matrix);

void MakeCoveringRelation(std::unordered_map<std::string, std::set<std::string>>& matrix);

bool dfs(std::unordered_map<std::string, std::set<std::string>>& matrix, std::string a, std::string c);
bool IsLattice(std::unordered_map<std::string, std::set<std::string>>& matrix);

void DrawGraph(std::unordered_map<std::string, std::set<std::string>>& matrix);

bool IsPoset(std::unordered_map<std::string, std::set<std::string>>& matrix);
