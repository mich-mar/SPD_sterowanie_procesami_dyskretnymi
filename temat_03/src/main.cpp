/**
 * @file main.cpp
 * @brief Główny plik programu do szeregowania zadań.
 *
 * Ten plik zawiera funkcję main, która wczytuje zadania z pliku,
 * wykonuje szeregowanie przy użyciu różnych algorytmów i wyświetla
 * wyniki.
 */

#include "../inc/task.h"
#include "../inc/scheduler.h"
#include <iostream>
#include <string>

/**
 * @brief Funkcja główna programu
 *
 * Ta funkcja wczytuje zadania z pliku, wykonuje szeregowanie przy użyciu
 * algorytmów, a następnie wyświetla wyniki.
 *
 * @return Kod zakończenia programu (0 oznacza poprawne zakończenie)
 */
int main() {
    // Ścieżka do pliku z danymi
    std::string filename = "data/instance_1_100.txt";

    // Wczytaj zadania z pliku
    std::vector<task> tasks = read_tasks_from_file(filename);

    // Szeregowanie LSA (w kolejności z pliku)
    std::cout << "=== LSA (kolejność z pliku) ===\n";
    auto lsa_schedule = schedule_tasks_lsa(tasks);
    print_schedule(lsa_schedule);

    // Szeregowanie LPT (posortowane według czasu przetwarzania)
    std::cout << "\n=== LPT (sortowanie malejąco wg p_j) ===\n";
    auto lpt_schedule = schedule_tasks_lpt(tasks);
    print_schedule(lpt_schedule);

    // Szeregowanie DP (programowanie dynamiczne)
    std::cout << "\n=== Programowanie dynamiczne (Subset Sum) ===\n";
    auto dp_schedule = schedule_tasks_dp(tasks);
    print_schedule(dp_schedule);

    // Szeregowanie PTAS
    double epsilon = 0.1;  // Przykładowa wartość epsilon
    std::cout << "\n=== PTAS (przybliżenie z epsilon=" << epsilon << ") ===\n";
    auto ptas_schedule = schedule_tasks_ptas(tasks, epsilon);
    print_schedule(ptas_schedule);

    // Szeregowanie FPTAS
    std::cout << "\n=== FPTAS (przybliżenie z epsilon=" << epsilon << ") ===\n";
    auto fptas_schedule = schedule_tasks_fptas(tasks, epsilon);
    print_schedule(fptas_schedule);

    // Szeregowanie brute-force
    std::cout << "\n=== Brute-force (wszystkie możliwe podziały) ===\n";
    auto brute_schedule = schedule_tasks_bruteforce(tasks);
    print_schedule(brute_schedule);

    return 0;
}