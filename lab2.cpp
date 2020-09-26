#include <vector>
#include "gen.hpp"
#include <cmath>
#include <iostream>
#include <set>
#include <random>
#include <map>
#include <algorithm>

const int VERTICES = 150;
const int ANTS = 35;
const int ALFA = 2;
const int BETA = 3;
const double RO = 0.4;
int currentBest = 2500.0;

const std::vector<std::vector<int>> distances = {{1}};// generateWages(VERTICES);
std::vector<std::vector<double>> feromons = {{0.5}};//  generateFeromons(VERTICES);

double koefs(double tau, double distance, double alfa = ALFA, double beta = BETA) {
    return std::pow(tau, alfa) * std::pow(1 / distance, beta);
}

bool contains(const std::vector<int>& vect, int val) {
    for (const auto& item : vect) {
        if (val == item) {
            return true;
        }   
    }
    return false;
}

double summarize(const std::map<int, double>& m) {
    double acc = 0;
    for (const auto& [key, value] : m) {
        acc += value;
    }
    return acc;
}

int prob(std::map<int, double> paths) {
    double sum = summarize(paths);

    for(auto& [key, value] : paths) {
        value /= sum;
    }

    std::random_device rd;
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, 1000);

    double acc = 0;
    double genered = distr(gen);

    for (const auto& [key, value] : paths) {
        acc += value;
        if (genered <= acc*1000) {
            return key;
        }
    }
    throw std::exception();
}

class Ant {
private:
    std::vector<int> memory;
    int location;
    int length = 0;
    std::map<int, int> crossings;

    void moveTo(int vertex) {
        length += distances[location][vertex];
        memory.push_back(vertex);
        crossings.insert({location, vertex});
        location = vertex;
    }

    
    int probability() {
        std::map<int, double> probabilities;
        for (int i = 0; i < VERTICES; ++i) {
            if (contains(memory, i)) {
                continue;
            }
            
            double feromon = feromons[location][i];
            double distance = distances[location][i];

            probabilities[i] = koefs(feromon, distance);
        }
        return prob(probabilities);
    }

    void choosePath() {
        moveTo(probability());
    }

public:
    Ant(int vertex) {
        location = vertex;
        memory.push_back(vertex);
    }

    std::map<int, int> getCrossings() const {
        return crossings;
    }

    int getLength() const {
        return length;
    }

    int cycle() {
        for (int i = 0; i < VERTICES - 1; ++i) {
            choosePath();
        }
        moveTo(memory[0]);
        return length;
    }
};

std::vector<Ant> spawnAnts() {
    std::set<int> antsNumbers;
    // random
    std::random_device rd;
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, VERTICES - 1);

    while (antsNumbers.size() < ANTS) {
        antsNumbers.insert(distr(gen));
    }
    
    std::vector<Ant> res;
    for (const auto& number : antsNumbers) {
        res.push_back({number});
    }
    return res;
}

double sumFeromons(const std::vector<Ant>& antes, int row, int column) {
    double res = 0;
    for (const auto& anet : antes) {
        for (const auto& [key, value] : anet.getCrossings()) {
            if (key == row && value == column) {
                res += (double) currentBest / (double) anet.getLength();
            } else if (key == column && value == key) {
                res += (double) currentBest / (double) anet.getLength();
            }
        }
    }
    return res;
}

void feromonUpdate(const std::vector<Ant>& antes) {
    for (int i = 0; i < VERTICES; ++i) {
        for (int j = 0; j < VERTICES; ++j) {
            feromons[i][j] = (1 - RO) * feromons[i][j] + sumFeromons(antes, i, j);
        }
        
    }
    
}

int main() {
    for (int i = 0; i < 1000; ++i) {
        std::vector<Ant> ants = spawnAnts();
        int len = currentBest;
        for (auto& ant: ants) {
            len = std::min(ant.cycle(), len);
        }
        currentBest = std::min(len, currentBest);
    }
    std::cout << currentBest << '\n';
}
