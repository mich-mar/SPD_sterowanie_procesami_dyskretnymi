#include "alg_00_heuristic.h"
#include <vector>
#include <algorithm>
#include "task_struct.h"

/**
 * @brief Sortuje zadania według czasu dostępności (rj) i oblicza maksymalny czas zakończenia (Cmax).
 *
 * @param tasks Wektor zadań do posortowania i zaplanowania.
 * @return int Maksymalny czas zakończenia (Cmax).
 */
int rjSortPlaning(std::vector<task> tasks) {
    // Sortowanie zadań według czasu dostępności (rj)
    std::sort(tasks.begin(), tasks.end(),  [](const task& a, const task& b)  {
        return a.r < b.r;
    });

    // Obliczanie Cmax (maksymalnego czasu zakończenia)
    int currentTime = 0;  // Aktualny czas
    int Cmax = 0;  // Maksymalny czas zakończenia

    for (const auto& t : tasks) {
        // Jeśli zadanie nie jest jeszcze dostępne, czekamy
        if (currentTime < t.r) {
            currentTime = t.r;
        }

        // Wykonujemy zadanie
        currentTime += t.p;

        // Aktualizujemy maksymalny czas zakończenia
        int completionTime = currentTime + t.q;
        Cmax = std::max(Cmax, completionTime);
    }

    return Cmax;
}

/**
 * @brief Sortuje zadania według czasu zakończenia (qj) malejąco i oblicza maksymalny czas zakończenia (Cmax).
 *
 * @param tasks Wektor zadań do posortowania i zaplanowania.
 * @return int Maksymalny czas zakończenia (Cmax).
 */
int qjSortPlaning(std::vector<task> tasks) {
    // Sortowanie zadań według czasu zakończenia (qj) malejąco
    std::sort(tasks.begin(), tasks.end(), [](const task& a, const task& b) {
       return a.q > b.q;
   });

    // Obliczanie Cmax (maksymalnego czasu zakończenia)
    int currentTime = 0;  // Aktualny czas
    int Cmax = 0;  // Maksymalny czas zakończenia

    for (const auto& t : tasks) {
        // Jeśli zadanie nie jest jeszcze dostępne, czekamy
        if (currentTime < t.r) {
            currentTime = t.r;
        }

        // Wykonujemy zadanie
        currentTime += t.p;

        // Aktualizujemy maksymalny czas zakończenia
        int completionTime = currentTime + t.q;
        Cmax = std::max(Cmax, completionTime);
    }

    return Cmax;
}
