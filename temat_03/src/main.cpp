/**
 * @file main.cpp
 * @brief Główny plik programu do szeregowania zadań.
 *
 * Ten plik zawiera funkcję main, która wczytuje zadania z pliku,
 * wykonuje szeregowanie przy użyciu różnych algorytmów i wyświetla
 * wyniki wraz z czasem wykonania.
 */

#include "../inc/task.h"
#include "../inc/scheduler.h"
#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>

/**
 * @brief Funkcja pomocnicza do pomiaru i wyświetlania czasu wykonania
 * @param duration Czas wykonania w mikrosekundach
 */
void print_execution_time(const std::chrono::microseconds& duration) {
    auto us = duration.count();
    if (us < 1000) {
        std::cout << "Czas wykonania: " << us << " µs\n";
    } else if (us < 1000000) {
        std::cout << "Czas wykonania: " << std::fixed << std::setprecision(3)
                  << (us / 1000.0) << " ms\n";
    } else {
        std::cout << "Czas wykonania: " << std::fixed << std::setprecision(3)
                  << (us / 1000000.0) << " s\n";
    }
}

/**
 * @brief Funkcja główna programu
 *
 * Ta funkcja wczytuje zadania z pliku, wykonuje szeregowanie przy użyciu
 * algorytmów, a następnie wyświetla wyniki wraz z czasem wykonania.
 *
 * @return Kod zakończenia programu (0 oznacza poprawne zakończenie)
 */
int main() {
    // Ścieżka do pliku z danymi
    std::string filename = "data/instance_1_5.txt";

    std::cout << "====== Plik: " << filename << " ========== \n\n";

    // Wczytaj zadania z pliku
    std::vector<task> tasks = read_tasks_from_file(filename);

    // Szeregowanie LSA (w kolejności z pliku)
    std::cout << "=== LSA (kolejność z pliku) ===\n";
    auto start = std::chrono::high_resolution_clock::now();
    auto lsa_schedule = schedule_tasks_lsa(tasks);
    auto end = std::chrono::high_resolution_clock::now();
    print_schedule(lsa_schedule);
    print_execution_time(std::chrono::duration_cast<std::chrono::microseconds>(end - start));

    // Szeregowanie LPT (posortowane według czasu przetwarzania)
    std::cout << "\n=== LPT (sortowanie malejąco wg p_j) ===\n";
    start = std::chrono::high_resolution_clock::now();
    auto lpt_schedule = schedule_tasks_lpt(tasks);
    end = std::chrono::high_resolution_clock::now();
    print_schedule(lpt_schedule);
    print_execution_time(std::chrono::duration_cast<std::chrono::microseconds>(end - start));

    // Szeregowanie DP (programowanie dynamiczne)
    std::cout << "\n=== Programowanie dynamiczne (Subset Sum) ===\n";
    start = std::chrono::high_resolution_clock::now();
    auto dp_schedule = schedule_tasks_dp(tasks);
    end = std::chrono::high_resolution_clock::now();
    print_schedule(dp_schedule);
    print_execution_time(std::chrono::duration_cast<std::chrono::microseconds>(end - start));

    // Szeregowanie PTAS
    double epsilon = 0.1;  // Przykładowa wartość epsilon
    std::cout << "\n=== PTAS (przybliżenie z epsilon=" << epsilon << ") ===\n";
    start = std::chrono::high_resolution_clock::now();
    auto ptas_schedule = schedule_tasks_ptas(tasks, epsilon);
    end = std::chrono::high_resolution_clock::now();
    print_schedule(ptas_schedule);
    print_execution_time(std::chrono::duration_cast<std::chrono::microseconds>(end - start));

    // Szeregowanie FPTAS
    std::cout << "\n=== FPTAS (przybliżenie z epsilon=" << epsilon << ") ===\n";
    start = std::chrono::high_resolution_clock::now();
    auto fptas_schedule = schedule_tasks_fptas(tasks, epsilon);
    end = std::chrono::high_resolution_clock::now();
    print_schedule(fptas_schedule);
    print_execution_time(std::chrono::duration_cast<std::chrono::microseconds>(end - start));

    // Szeregowanie brute-force
    std::cout << "\n=== Brute-force (wszystkie możliwe podziały) ===\n";
    start = std::chrono::high_resolution_clock::now();
    auto brute_schedule = schedule_tasks_bruteforce(tasks);
    end = std::chrono::high_resolution_clock::now();
    print_schedule(brute_schedule);
    print_execution_time(std::chrono::duration_cast<std::chrono::microseconds>(end - start));

    return 0;
}