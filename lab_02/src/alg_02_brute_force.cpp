#include "alg_02_brute_force.h"
#include <algorithm>
#include <climits>
#include <vector>
#include "task_struct.h"

/**
 * @brief Funkcja bruteForce znajduje optymalny harmonogram zadań metodą brute force.
 *
 * @param tasks Wektor zadań, gdzie każde zadanie ma pola r (czas dostępności), p (czas przetwarzania) i q (czas zakończenia).
 * @return Minimalny czas zakończenia wszystkich zadań (Cmax).
 */
int bruteForce(std::vector<task> tasks) {
    int Cmax = INT_MAX;

    // Sortowanie początkowe wymagane przez "next_permutation"
    std::sort(tasks.begin(), tasks.end(), [](const task &a, const task &b) {
        return a.id < b.id;
    });

    do {
        int currentTime = 0;
        int currentCmax = 0;

        // Obliczanie czasu zakończenia dla bieżącego harmonogramu
        for (const auto &t: tasks) {
            // Jeśli zadanie nie jest jeszcze dostępne, czekamy
            if (currentTime < t.r) {
                currentTime = t.r;
            }

            // Wykonanie zadania
            currentTime += t.p;

            // Aktualizacja maksymalnego czasu zakończenia
            int completionTime = currentTime + t.q;
            currentCmax = std::max(currentCmax, completionTime);
        }

        // Aktualizacja minimalnego Cmax
        if (currentCmax < Cmax) {
            Cmax = currentCmax;
        }
    } while (std::next_permutation(tasks.begin(), tasks.end(), [](const task &a, const task &b) {
        return a.id < b.id;
    }));

    return Cmax;
}