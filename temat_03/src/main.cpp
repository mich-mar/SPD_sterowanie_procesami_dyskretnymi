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

#define MACHINE_NUM 2

/**
 * @brief Funkcja główna programu
 *
 * Ta funkcja wczytuje zadania z pliku, wykonuje szeregowanie przy użyciu
 * algorytmów LSA i LPT, a następnie wyświetla wyniki.
 *
 * @return Kod zakończenia programu (0 oznacza poprawne zakończenie)
 */
int main() {
    // Ścieżka do pliku z danymi
    std::string filename = "data/instance_1_5.txt";

    // Wczytaj zadania z pliku
    std::vector<task> tasks = read_tasks_from_file(filename);


    // Szeregowanie LSA (w kolejności z pliku)
    std::cout << "=== LSA (kolejność z pliku) ===\n";
    auto lsa_schedule = schedule_tasks(tasks, MACHINE_NUM, false);
    print_schedule(lsa_schedule);

    // Szeregowanie LPT (posortowane według czasu przetwarzania)
    std::cout << "\n=== LPT (sortowanie malejąco wg p_j) ===\n";
    auto lpt_schedule = schedule_tasks(tasks, MACHINE_NUM, true);
    print_schedule(lpt_schedule);

    return 0;
}