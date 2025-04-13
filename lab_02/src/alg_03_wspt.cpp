#include "alg_03_wspt.h"
#include <algorithm>
#include <vector>
#include "task_struct.h"

/**
 * @brief Algorytm Ważonego Najkrótszego Czasu Przetwarzania (WSPT)
 *
 * Jest to wariant algorytmu SPT, który uwzględnia zarówno czas przetwarzania,
 * jak i czas dostarczenia do ustalenia priorytetów zadań. Oblicza wartość priorytetu
 * na podstawie ważonej kombinacji p i q.
 *
 * @param tasks Wektor zadań do zaplanowania
 * @return Maksymalny czas zakończenia (Cmax)
 */
int weightedSPTPlaning(std::vector<task> tasks) {
    if (tasks.empty()) {
        return 0;
    }

    // Sortowanie zadań według wzoru ważonego priorytetu
    std::sort(tasks.begin(), tasks.end(), [](const task& a, const task& b) {
        // Niższa wartość = wyższy priorytet
        // Ta formuła priorytetyzuje zadania z niskim p i wysokim q
        return (a.p - 0.5 * a.q) < (b.p - 0.5 * b.q);
    });

    int currentTime = 0;
    int Cmax = 0;

    // Planowanie zadań zgodnie z priorytetem
    for (const auto& t : tasks) {
        // Jeśli zadanie nie jest jeszcze dostępne, czekamy
        if (currentTime < t.r) {
            currentTime = t.r;
        }

        // Wykonanie zadania
        currentTime += t.p;

        // Obliczenie czasu zakończenia i aktualizacja Cmax
        int completionTime = currentTime + t.q;
        Cmax = std::max(Cmax, completionTime);
    }

    return Cmax;
}