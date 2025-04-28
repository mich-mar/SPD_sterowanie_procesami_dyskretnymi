#include "alg_01_schrage.h"
#include <algorithm>
#include <queue>
#include <functional>
#include <vector>
#include "task_struct.h"

/**
 * @brief Funkcja schragePlaning implementuje heurystyczne podejście do problemu harmonogramowania zadań.
 * Zadania są sortowane według czasu dostępności (r), a następnie wybierane jest zadanie z największym czasem zakończenia (q).
 *
 * @param tasks Wektor zadań do przetworzenia
 * @return Maksymalny czas zakończenia (Cmax) wszystkich zadań.
 */
int schragePlaning(std::vector<task> tasks) {
    // Sortowanie zadań według czasu dostępności (r)
    std::sort(tasks.begin(), tasks.end(), [](const task &a, const task &b) { return a.r < b.r; });

    int currentTime = 0, Cmax = 0;
    std::vector<task> available;
    int index = 0;

    // Główna pętla przetwarzająca zadania
    while (index < tasks.size() || !available.empty()) {
        // Dodawanie zadań dostępnych w bieżącym czasie do listy dostępnych zadań
        while (index < tasks.size() && tasks[index].r <= currentTime) {
            available.push_back(tasks[index]);
            index++;
        }

        if (!available.empty()) {
            // Wybór zadania z największym czasem zakończenia (q)
            auto it = std::max_element(available.begin(), available.end(),
                                       [](const task &a, const task &b) { return a.q < b.q; });
            task currentTask = *it;
            available.erase(it);

            // Aktualizacja bieżącego czasu i maksymalnego czasu zakończenia
            currentTime += currentTask.p;
            Cmax = std::max(Cmax, currentTime + currentTask.q);
        } else if (index < tasks.size()) {
            // Jeśli nie ma dostępnych zadań, przesuń bieżący czas do czasu dostępności następnego zadania
            currentTime = tasks[index].r;
        }
    }

    return Cmax;
}

/**
 * @brief Funkcja schragePreemptivePlaning implementuje heurystyczne podejście do problemu harmonogramowania zadań
 * z wywłaszczeniem. Zadania są sortowane według czasu dostępności (r), a następnie wybierane jest zadanie
 * z największym czasem zakończenia (q), z możliwością przerwania.
 *
 * @param tasks Wektor zadań do przetworzenia, gdzie każde zadanie ma pola r (czas dostępności), p (czas przetwarzania)
 * i q (czas zakończenia).
 * @return Maksymalny czas zakończenia (Cmax) wszystkich zadań.
 */
int schragePreemptivePlaning(std::vector<task> tasks) {
    // Komparator do sortowania zadań według r (rosnąco)
    auto cmpR = [](const task &a, const task &b) {
        return a.r > b.r;
    };

    // Kolejka priorytetowa N (zadania niegotowe), sortowana według najmniejszego r
    std::priority_queue<task, std::vector<task>, decltype(cmpR)> N(cmpR);

    // Dodaj wszystkie zadania do kolejki N
    for (auto &t: tasks) {
        N.push(t);
    }

    // Komparator do sortowania zadań według q (malejąco)
    auto cmpQ = [](const task &a, const task &b) {
        return a.q < b.q;
    };

    // Kolejka priorytetowa G (zadania gotowe do wykonania), sortowana po największym q
    std::priority_queue<task, std::vector<task>, decltype(cmpQ)> G(cmpQ);

    int t = 0; // Aktualny czas
    int Cmax = 0; // Wynik: maksymalny czas zakończenia
    task current = {0, 0, INT_MAX}; // Aktualnie wykonywane zadanie (na początku żadne)

    // Główna pętla: wykonuj, dopóki są zadania gotowe lub oczekujące
    while (!G.empty() || !N.empty()) {
        // Przenosimy do G wszystkie zadania, których czas dostępności <= aktualny czas
        while (!N.empty() && N.top().r <= t) {
            task e = N.top();
            N.pop();
            G.push(e);

            // Jeśli nowe zadanie ma większe q niż aktualnie wykonywane, wywłaszczamy
            if (e.q > current.q) {
                current.p = t - e.r; // przelicz pozostały czas aktualnego zadania
                t = e.r; // cofamy czas do momentu nadejścia nowego zadania
                if (current.p > 0) {
                    G.push(current); // dodajemy przerwane zadanie z powrotem do G
                }
            }
        }

        // Jeśli nie ma żadnych zadań gotowych, przejdź do czasu następnego dostępnego zadania
        if (G.empty()) {
            t = N.top().r;
        } else {
            // Wybierz zadanie z największym q do wykonania
            task e = G.top();
            G.pop();
            current = e;

            // Wykonujemy zadanie (czas przetwarzania p)
            t += e.p;

            // Aktualizujemy Cmax
            Cmax = std::max(Cmax, t + e.q);
        }
    }

    return Cmax;
}
