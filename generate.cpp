#include <vector>
#include <random>
#include <iostream>
#include <map>
#include <set>

std::vector<std::vector<int>> generateWages(int size) {
    std::vector<std::vector<int>> res(size);
    for (auto& row : res) {
        row.resize(size);
    }
    
    std::random_device rd;
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(5, 50);
    // std::cout << "success initialization";
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (i == j) {
                res[i][j] = 1;
            } else if (j < i) {
                res[i][j] = distr(gen);
            }
        }
    }
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (j < i) {
                res[j][i] = res[i][j];
            }
        }
    }
    return res;
}


std::vector<std::vector<double>> generateFeromons(int size) {
    std::vector<std::vector<double>> res(size);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (i == j) {
                res[i].push_back(0);
            } else {
                res[i].push_back(0.2);
            }          
        }
    }
    return res;
}

void printMatrix(const std::vector<std::vector<int>>& matrix) {
    for (const auto& list : matrix) {
        std::cout << '[';
        for (const auto& elem : list) {
            std::cout << elem << ' ';
        }
        std::cout << ']' << '\n';
    }
    
}
