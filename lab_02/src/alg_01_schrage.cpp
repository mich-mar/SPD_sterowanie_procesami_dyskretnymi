#include "alg_01_schrage.h"

/**
 * @brief Funkcja heuristic implementuje heurystyczne podejście do problemu harmonogramowania zadań.
 * Zadania są sortowane według czasu dostępności (r), a następnie wybierane jest zadanie z największym czasem zakończenia (q).
 *
 * @param tasks Wektor zadań do przetworzenia, gdzie każde zadanie ma pola r (czas dostępności), p (czas przetwarzania) i q (czas zakończenia).
 * @return Maksymalny czas zakończenia (Cmax) wszystkich zadań.
 */
int heuristicPlaning(std::vector<task> tasks) {
    // Sortowanie zadań według czasu dostępności (r)
    sort(tasks.begin(), tasks.end(), [](const task &a, const task &b) { return a.r < b.r; });


    int currentTime = 0, Cmax = 0;
    std::vector<task> available;
    size_t index = 0;

    // Główna pętla przetwarzająca zadania
    while (index < tasks.size() || !available.empty()) {
        // Dodawanie zadań dostępnych w bieżącym czasie do listy dostępnych zadań
        while (index < tasks.size() && tasks[index].r <= currentTime) {
            available.push_back(tasks[index]);
            index++;
        }

        if (!available.empty()) {
            // Wybór zadania z największym czasem zakończenia (q_j)
            auto it = max_element(available.begin(), available.end(),
                                  [](const task &a, const task &b) { return a.q < b.q; });
            task task = *it;
            available.erase(it);

            // Aktualizacja bieżącego czasu i maksymalnego czasu zakończenia
            currentTime += task.p;
            Cmax = std::max(Cmax, currentTime + task.q);
        } else {
            // Jeśli nie ma dostępnych zadań, przesuń bieżący czas do czasu dostępności następnego zadania
            currentTime = tasks[index].r;
        }
    }

    return Cmax;
}
