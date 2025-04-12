#include "alg_00_heuristic.h"
#include <vector>
#include <algorithm>

#include <vector>
#include <queue>
#include <algorithm>
#include "task_struct.h"  // Zakładam, że ten plik zawiera definicję struct task

// Funkcja do obliczenia Cmax
int calculateCmax(std::vector<task>& tasks) {
    int currentTime = 0;
    int cmax = 0;

    for (auto& t : tasks) {
        currentTime = std::max(currentTime, t.r) + t.p;  // Zadanie zaczyna się po jego dostępności
        cmax = std::max(cmax, currentTime);              // Aktualizuj maksymalny czas zakończenia
    }

    return cmax;
}

// Heurystyka: sortowanie po rj (czas dostępności) i obliczanie Cmax
int rjSortPlaning(std::vector<task> &tasks) {
    sort(tasks.begin(), tasks.end(), [](const task& t1, const task& t2) {
        return t1.r < t2.r;  // Sortowanie rosnąco po rj
    });
    return calculateCmax(tasks);  // Zwracamy Cmax po sortowaniu
}

// Heurystyka: sortowanie po qj (czas zakończenia) i obliczanie Cmax
int qjSortPlaning(std::vector<task>& tasks) {
    sort(tasks.begin(), tasks.end(), [](const task& t1, const task& t2) {
        return t1.q > t2.q;  // Sortowanie malejąco po qj
    });
    return calculateCmax(tasks);  // Zwracamy Cmax po sortowaniu
}
