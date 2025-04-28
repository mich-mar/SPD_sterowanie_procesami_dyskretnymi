/**
 * @file scheduler.cpp
 * @brief Implementacja funkcji do szeregowania zadań na maszynach.
 *
 * Ten plik źródłowy zawiera implementacje funkcji deklarowanych w scheduler.h,
 * w tym funkcji do szeregowania zadań na maszynach i wyświetlania harmonogramu.
 */

#include "../inc/scheduler.h"
#include <iostream>
#include <algorithm>

/**
 * @brief Przydziela zadania do maszyn według wybranego algorytmu
 *
 * Implementacja funkcji przydzielającej zadania do maszyn używając albo
 * algorytmu LSA (zadania przydzielane w oryginalnej kolejności), albo algorytmu LPT
 * (zadania sortowane malejąco według czasu przetwarzania).
 *
 * @param tasks Wektor zadań do przydzielenia
 * @param machine_count Liczba dostępnych maszyn
 * @param use_lpt Flaga określająca czy użyć algorytmu LPT (true) czy LSA (false)
 * @return Wektor wektorów zadań przypisanych do poszczególnych maszyn
 */
std::vector<std::vector<task>> schedule_tasks(const std::vector<task>& tasks, int machine_count, bool use_lpt) {
    std::vector<task> sorted_tasks = tasks;

    // Jeśli LPT, to sortujemy zadania malejąco według czasu przetwarzania
    if (use_lpt) {
        std::sort(sorted_tasks.begin(), sorted_tasks.end(), [](const task& a, const task& b) {
            return a.p_time > b.p_time;
        });
    }

    // Kolejki maszyn: każda maszyna ma sumaryczny czas zajęcia
    std::vector<int> machine_loads(machine_count, 0);
    std::vector<std::vector<task>> machine_tasks(machine_count); // maszyny i ich zadania

    for (const auto& task : sorted_tasks) {
        // Znajdź najmniej obciążoną maszynę
        int min_machine = std::min_element(machine_loads.begin(), machine_loads.end()) - machine_loads.begin();

        machine_tasks[min_machine].push_back(task);
        machine_loads[min_machine] += task.p_time;
    }

    return machine_tasks;
}

/**
 * @brief Wyświetla harmonogram zadań na maszynach
 *
 * Implementacja funkcji wypisującej na standardowe wyjście informacje o przydziale
 * zadań do poszczególnych maszyn.
 *
 * @param machine_tasks Wektor wektorów zadań przypisanych do poszczególnych maszyn
 */
void print_schedule(const std::vector<std::vector<task>>& machine_tasks) {
    for (size_t i = 0; i < machine_tasks.size(); ++i) {
        std::cout << "Maszyna " << i+1 << ": ";
        for (const auto& task : machine_tasks[i]) {
            std::cout << "(" << task.id << ", p=" << task.p_time << ") ";
        }
        std::cout << "\n";
    }
}