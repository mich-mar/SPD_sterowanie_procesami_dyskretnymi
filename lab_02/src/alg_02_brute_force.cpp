#include "alg_02_brute_force.h"

/**
 * @brief Funkcja bruteForce znajduje optymalny harmonogram zadań metodą brute force.
 *
 * @param tasks Wektor zadań, gdzie każde zadanie ma pola r (czas dostępności), p (czas przetwarzania) i q (czas zakończenia).
 * @return Minimalny czas zakończenia wszystkich zadań (Cmax).
 */
int bruteForce(std::vector<task> tasks) {
    int min_cmax = INT_MAX;

    // Sortowanie początkowe wymagane przez next_permutation
    sort(tasks.begin(), tasks.end(), [](const task &a, const task &b) {
        return a.id < b.id;
    });

    do {
        int current_time = 0;
        int current_cmax = 0;

        // Obliczanie czasu zakończenia dla bieżącego harmonogramu
        for (const auto &t: tasks) {
            current_time = std::max(current_time, t.r) + t.p;
            current_cmax = std::max(current_cmax, current_time + t.q);
        }

        // Aktualizacja minimalnego Cmax i najlepszego harmonogramu
        if (current_cmax < min_cmax) {
            min_cmax = current_cmax;
        }
    } while (next_permutation(tasks.begin(), tasks.end(), [](const task &a, const task &b) {
        return a.id < b.id;
    }));


    return min_cmax;
}
