#include <vector>
#include <iostream>
#include <map>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>

const int VERTICES = 150;
const int ANTS = 35;
const int ALFA = 2;
const int BETA = 3;
const double RO = 0.4;
int currentBest = 2500.0;

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
}

int main() {
    std::vector<int> a; 
    std::map<int, double> paths = {{1 , 0.004}, {2, 0.004}, {3, 0.002}};
    for (int i = 0; i < 1000; ++i) {
        a.push_back(prob(paths));
    }
    std::cout << std::count(std::begin(a), std::end(a), 1) <<  ' ' << '1' << '\n';
    std::cout << std::count(std::begin(a), std::end(a), 2) <<  ' ' << '2' << '\n'; 
    std::cout << std::count(std::begin(a), std::end(a), 3) <<  ' ' << '3' << '\n'; 
}