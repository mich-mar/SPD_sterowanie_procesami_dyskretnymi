#include "flowshop.h"
#include <chrono>
#include <iomanip>  // dla std::setw
#include <iostream>

void printResults(const std::string &algorithmName,
                  const std::vector<int> &sequence,
                  int makespan,
                  long long duration) {
    std::cout << "\nWynik algorytmu " << algorithmName << ":\n";
    std::cout << "Kolejność zadań: ";
    for (int jobIdx: sequence) {
        std::cout << (jobIdx + 1) << " ";
    }
    std::cout << "\nCmax: " << std::setw(4) << makespan;
    std::cout << "\nCzas wykonania: " << std::setw(5) << duration << "ms\n";
}

int main() {
    FlowShop flowshop;

    if (!flowshop.loadFromFile("instances/flowshop_n100_m3.csv")) {
        return 1;
    }

    std::cout << "\n" << std::string(50, '=');
    std::cout << "\nZaładowano instancję z " << flowshop.getNumJobs() << " zadaniami i "
              << flowshop.getNumMachines() << " maszynami.";
    std::cout << "\n" << std::string(50, '=');
    std::cout << "\n\n";

    // Johnson (dla pierwszych dwóch maszyn)
    {
        auto start = std::chrono::high_resolution_clock::now();
        auto [perm, makespan] = flowshop.johnson();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now() - start).count();
        printResults("Johnson (2 maszyny)", perm, makespan, duration);
    }

    // NEH
    {
        auto start = std::chrono::high_resolution_clock::now();
        auto [perm, makespan] = flowshop.neh();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now() - start).count();
        printResults("NEH", perm, makespan, duration);
    }

    // FNEH
    {
        auto start = std::chrono::high_resolution_clock::now();
        auto [perm, makespan] = flowshop.fneh();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now() - start).count();
        printResults("FNEH", perm, makespan, duration);
    }

    // Przegląd zupełny (Brute Force)
    {
        auto start = std::chrono::high_resolution_clock::now();
        auto [perm, makespan] = flowshop.bruteForce();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now() - start).count();
        printResults("Przegląd zupełny", perm, makespan, duration);
    }

    return 0;
}