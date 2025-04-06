#include "alg_00_heuristic.h"
#include <vector>
#include <algorithm>

int qjSortPlaning(std::vector<task> tasks) {
    // Sortowanie zadań według q (malejąco)
    sort(tasks.begin(), tasks.end(), [](const task &a, const task &b) {
        return a.q > b.q;
    });

    int currentTime = 0, Cmax = 0;

    for (const auto &t : tasks) {
        currentTime += t.p;  // Przetwarzamy zadanie
        Cmax = std::max(Cmax, currentTime + t.q); // Aktualizacja Cmax
    }

    return Cmax;
}

int rjSortPlaning(std::vector<task> tasks) {
    // Sortowanie zadań według r (rosnąco)
    sort(tasks.begin(), tasks.end(), [](const task &a, const task &b) {
        return a.r < b.r;
    });

    int currentTime = 0, Cmax = 0;

    for (const auto &t : tasks) {
        currentTime = std::max(currentTime, t.r) + t.p;  // Start po dostępności i wykonanie zadania
        Cmax = std::max(Cmax, currentTime + t.q); // Aktualizacja Cmax
    }

    return Cmax;
}
