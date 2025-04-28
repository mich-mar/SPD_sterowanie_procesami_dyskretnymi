/**
 * @file scheduler.h
 * @brief Definiuje funkcje do szeregowania zadań na maszynach.
 *
 * Ten plik nagłówkowy zawiera deklaracje funkcji do szeregowania zadań
 * na maszynach przy użyciu algorytmów LSA (List Scheduling Algorithm)
 * i LPT (Longest Processing Time).
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "task.h"
#include <vector>

/**
 * @brief Przydziela zadania do maszyn według wybranego algorytmu
 *
 * Ta funkcja przydziela zadania do maszyn używając albo algorytmu LSA
 * (zadania przydzielane w oryginalnej kolejności), albo algorytmu LPT
 * (zadania sortowane malejąco według czasu przetwarzania).
 *
 * @param tasks Wektor zadań do przydzielenia
 * @param machine_count Liczba dostępnych maszyn
 * @param use_lpt Flaga określająca czy użyć algorytmu LPT (true) czy LSA (false)
 * @return Wektor wektorów zadań przypisanych do poszczególnych maszyn
 */
std::vector<std::vector<task>> schedule_tasks(const std::vector<task>& tasks, int machine_count, bool use_lpt = false);

/**
 * @brief Wyświetla harmonogram zadań na maszynach
 *
 * Ta funkcja wypisuje na standardowe wyjście informacje o przydziale
 * zadań do poszczególnych maszyn.
 *
 * @param machine_tasks Wektor wektorów zadań przypisanych do poszczególnych maszyn
 */
void print_schedule(const std::vector<std::vector<task>>& machine_tasks);

#endif // SCHEDULER_H