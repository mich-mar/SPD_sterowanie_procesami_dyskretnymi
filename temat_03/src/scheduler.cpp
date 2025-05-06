#include "../inc/scheduler.h"

/**
 * @brief Implementuje algorytm szeregowania zadań LSA (List Scheduling Algorithm)
 *
 * LSA to prosty algorytm zachłanny, który przydziela zadania do maszyny
 * o najmniejszym obecnym obciążeniu.
 *
 * @param tasks Wektor zadań do zaszeregowania
 * @return Wektor wektorów zadań przydzielonych do poszczególnych maszyn
 */
std::vector<std::vector<task>> schedule_tasks_lsa(const std::vector<task> &tasks) {
    // Inicjalizacja obciążeń wszystkich maszyn na 0
    std::vector<int> machine_loads(MACHINE_NUM, 0);
    // Inicjalizacja wektora przechowującego zadania przypisane do każdej maszyny
    std::vector<std::vector<task>> machine_tasks(MACHINE_NUM);

    // Iteracja przez wszystkie zadania
    for (const auto &task: tasks) {
        // Znajdź maszynę o najmniejszym obciążeniu
        int min_machine = std::min_element(machine_loads.begin(), machine_loads.end()) - machine_loads.begin();
        // Przypisz zadanie do wybranej maszyny
        machine_tasks[min_machine].push_back(task);
        // Zaktualizuj obciążenie maszyny
        machine_loads[min_machine] += task.p_time;
    }

    return machine_tasks;
}

/**
 * @brief Implementuje algorytm szeregowania zadań LPT (Longest Processing Time)
 *
 * LPT najpierw sortuje zadania według malejących czasów wykonania,
 * a następnie używa algorytmu LSA do ich przydziału.
 *
 * @param tasks Wektor zadań do zaszeregowania
 * @return Wektor wektorów zadań przydzielonych do poszczególnych maszyn
 */
std::vector<std::vector<task>> schedule_tasks_lpt(const std::vector<task> &tasks) {
    // Kopiowanie zadań do nowego wektora
    std::vector<task> sorted_tasks = tasks;
    // Sortowanie zadań według malejących czasów wykonania
    std::sort(sorted_tasks.begin(), sorted_tasks.end(), [](const task &a, const task &b) {
        return a.p_time > b.p_time;
    });

    // Użycie algorytmu LSA na posortowanych zadaniach
    return schedule_tasks_lsa(sorted_tasks);
}

/**
 * @brief Wyświetla harmonogram zadań i obliczony makespan
 *
 * Funkcja drukuje przypisanie zadań do maszyn wraz z ich identyfikatorami i czasami wykonania.
 * Dodatkowo oblicza i wyświetla makespan (czas zakończenia wszystkich zadań).
 *
 * @param machine_tasks Wektor wektorów zadań przydzielonych do poszczególnych maszyn
 */
void print_schedule(const std::vector<std::vector<task>> &machine_tasks) {
    int makespan = 0;
    // Iteracja przez wszystkie maszyny
    for (size_t i = 0; i < machine_tasks.size(); ++i) {
        int sum = 0;
        std::cout << "Maszyna " << i + 1 << ": ";
        // Wyświetl wszystkie zadania przypisane do danej maszyny
        for (const auto &task: machine_tasks[i]) {
            std::cout << "(" << task.id << ", p=" << task.p_time << ") ";
            sum += task.p_time;
        }
        // Aktualizacja maksymalnego czasu wykonania (makespan)
        makespan = std::max(makespan, sum);
        std::cout << "\n";
    }
    std::cout << "Makespan: " << makespan << "\n";
}

/**
 * @brief Implementuje algorytm szeregowania zadań metodą siłową (brute force)
 *
 * Algorytm sprawdza wszystkie możliwe przydziały zadań do dwóch maszyn
 * i wybiera ten, który minimalizuje makespan.
 *
 * @param tasks Wektor zadań do zaszeregowania
 * @return Wektor wektorów zadań przydzielonych do poszczególnych maszyn (dokładnie 2 maszyny)
 */
std::vector<std::vector<task>> schedule_tasks_bruteforce(const std::vector<task> &tasks) {
    int n = tasks.size();
    int min_makespan = INT_MAX;
    std::vector<bool> best_assignment;

    // Dla każdego możliwego podziału zadań (2^n możliwości)
    for (int mask = 0; mask < (1 << n); ++mask) {
        int sum0 = 0, sum1 = 0;
        // Oblicz całkowity czas wykonania zadań na każdej z dwóch maszyn
        for (int i = 0; i < n; ++i) {
            if (mask & (1 << i))
                sum1 += tasks[i].p_time;  // Zadanie przydzielone do maszyny 1
            else
                sum0 += tasks[i].p_time;  // Zadanie przydzielone do maszyny 0
        }

        // Oblicz makespan dla obecnego podziału (maksimum z czasów wykonania)
        int makespan = std::max(sum0, sum1);
        // Jeśli znaleziono lepszy podział, zaktualizuj najlepsze rozwiązanie
        if (makespan < min_makespan) {
            min_makespan = makespan;
            // Zapamiętaj przypisanie zadań
            best_assignment.clear();
            for (int i = 0; i < n; ++i)
                best_assignment.push_back((mask & (1 << i)) == 0);
        }
    }

    // Stwórz harmonogram na podstawie najlepszego przypisania
    std::vector<std::vector<task>> machine_tasks(2);
    for (int i = 0; i < n; ++i) {
        machine_tasks[best_assignment[i] ? 0 : 1].push_back(tasks[i]);
    }

    return machine_tasks;
}

/**
 * @brief Implementuje algorytm szeregowania zadań z użyciem programowania dynamicznego
 *
 * Rozwiązuje problem szeregowania zadań na 2 maszyny jako problem plecakowy,
 * starając się znaleźć podzbiór zadań o sumie czasów najbliższej połowie całkowitego czasu.
 *
 * @param tasks Wektor zadań do zaszeregowania
 * @return Wektor wektorów zadań przydzielonych do poszczególnych maszyn (dokładnie 2 maszyny)
 */
std::vector<std::vector<task>> schedule_tasks_dp(const std::vector<task> &tasks) {
    // Oblicz całkowity czas wykonania wszystkich zadań
    int total_time = 0;
    for (const auto &t: tasks) total_time += t.p_time;

    int n = tasks.size();
    int half_time = total_time / 2;  // Idealnie byłoby podzielić zadania na pół

    // dp[i] = true jeśli można osiągnąć sumę i używając podzbioru zadań
    std::vector<bool> dp(half_time + 1, false);
    // Tablica do odtworzenia przypisania zadań
    std::vector<int> parent(half_time + 1, -1);
    dp[0] = true;  // Pusty podzbiór ma sumę 0

    // Rozwiązanie problemu plecakowego
    for (int i = 0; i < n; ++i) {
        for (int j = half_time; j >= tasks[i].p_time; --j) {
            if (dp[j - tasks[i].p_time] && !dp[j]) {
                dp[j] = true;
                parent[j] = i;  // Zapamiętaj, które zadanie dodaliśmy
            }
        }
    }

    // Znajdź największą możliwą sumę <= half_time
    int best_sum = half_time;
    while (!dp[best_sum]) --best_sum;

    // Odtwórz przypisania do maszyny 0
    std::vector<bool> assigned(n, false);
    int curr = best_sum;
    while (curr > 0 && parent[curr] != -1) {
        int idx = parent[curr];
        assigned[idx] = true;  // Zadanie idx przypisane do maszyny 0
        curr -= tasks[idx].p_time;
    }

    // Stwórz harmonogram na podstawie przypisania
    std::vector<std::vector<task>> machine_tasks(2);
    for (int i = 0; i < n; ++i) {
        if (assigned[i])
            machine_tasks[0].push_back(tasks[i]);
        else
            machine_tasks[1].push_back(tasks[i]);
    }

    return machine_tasks;
}

/**
 * @brief Implementuje PTAS (Polynomial-Time Approximation Scheme) dla problemu szeregowania
 *
 * Algorytm rozważa wszystkie możliwe przypisania k największych zadań,
 * a pozostałe zadania przydziela zachłannie.
 *
 * @param tasks Wektor zadań do zaszeregowania
 * @param epsilon Parametr dokładności (większe epsilon = szybsze, ale mniej dokładne)
 * @return Wektor wektorów zadań przydzielonych do poszczególnych maszyn (dokładnie 2 maszyny)
 */
std::vector<std::vector<task>> schedule_tasks_ptas(const std::vector<task> &tasks, double epsilon) {
    // Wybierz k zadań o największych czasach wykonania, gdzie k = 1/epsilon
    int k = static_cast<int>(std::ceil(1.0 / epsilon));
    // Sortuj zadania malejąco według czasów wykonania
    std::vector<task> sorted = tasks;
    std::sort(sorted.begin(), sorted.end(), [](const task &a, const task &b) {
        return a.p_time > b.p_time;
    });

    // Podziel zadania na dwie grupy: k największych i resztę
    std::vector<task> top_k(sorted.begin(), sorted.begin() + std::min(k, (int) sorted.size()));
    std::vector<task> rest(sorted.begin() + std::min(k, (int) sorted.size()), sorted.end());

    int min_makespan = INT_MAX;
    std::vector<std::vector<task>> best_schedule(2);

    // Sprawdź wszystkie możliwe przypisania k największych zadań (2^k)
    int combinations = 1 << top_k.size();
    for (int mask = 0; mask < combinations; ++mask) {
        std::vector<std::vector<task>> current_schedule(2);
        int loads[2] = {0, 0};

        // Przypisz k największych zadań zgodnie z aktualną maską
        for (int i = 0; i < (int) top_k.size(); ++i) {
            int m = (mask & (1 << i)) ? 1 : 0;  // Przypisz do maszyny 0 lub 1
            current_schedule[m].push_back(top_k[i]);
            loads[m] += top_k[i].p_time;
        }

        // Pozostałe zadania przypisz zachłannie
        for (const auto &t: rest) {
            int m = (loads[0] <= loads[1]) ? 0 : 1;  // Wybierz maszynę z mniejszym obciążeniem
            current_schedule[m].push_back(t);
            loads[m] += t.p_time;
        }

        // Sprawdź, czy znaleziono lepszy harmonogram
        int makespan = std::max(loads[0], loads[1]);
        if (makespan < min_makespan) {
            min_makespan = makespan;
            best_schedule = current_schedule;
        }
    }

    return best_schedule;
}

/**
 * @brief Implementuje FPTAS (Fully Polynomial-Time Approximation Scheme) dla problemu szeregowania
 *
 * Algorytm skaluje czasy wykonania zadań, aby zredukować złożoność obliczeniową,
 * a następnie używa programowania dynamicznego do znalezienia optymalnego rozwiązania
 * dla przeskalowanego problemu.
 *
 * @param tasks Wektor zadań do zaszeregowania
 * @param epsilon Parametr dokładności (większe epsilon = szybsze, ale mniej dokładne)
 * @return Wektor wektorów zadań przydzielonych do poszczególnych maszyn (dokładnie 2 maszyny)
 */
std::vector<std::vector<task>> schedule_tasks_fptas(const std::vector<task> &tasks, double epsilon) {
    int n = tasks.size();
    // Znajdź maksymalny czas wykonania zadania
    int max_p = 0;
    for (const auto &t: tasks) max_p = std::max(max_p, t.p_time);

    // Oblicz współczynnik skalowania K
    double K = (epsilon * max_p) / (2 * n);
    if (K < 1.0) K = 1.0;  // Współczynnik skalowania nie może być mniejszy niż 1

    // Przeskaluj czasy wykonania zadań
    std::vector<int> scaled_p(n);
    for (int i = 0; i < n; ++i)
        scaled_p[i] = static_cast<int>(tasks[i].p_time / K);

    // Oblicz sumę przeskalowanych czasów
    int sum = std::accumulate(scaled_p.begin(), scaled_p.end(), 0);
    int half = sum / 2;

    // Użyj programowania dynamicznego dla przeskalowanego problemu
    std::vector<bool> dp(half + 1, false);
    std::vector<int> parent(half + 1, -1);
    dp[0] = true;

    // Rozwiązanie problemu plecakowego dla przeskalowanych czasów
    for (int i = 0; i < n; ++i) {
        for (int j = half; j >= scaled_p[i]; --j) {
            if (dp[j - scaled_p[i]] && !dp[j]) {
                dp[j] = true;
                parent[j] = i;
            }
        }
    }

    // Znajdź największą możliwą sumę <= half
    int best_sum = half;
    while (!dp[best_sum]) --best_sum;

    // Odtwórz przypisania do maszyny 0
    std::vector<bool> assigned(n, false);
    int curr = best_sum;
    while (curr > 0 && parent[curr] != -1) {
        int idx = parent[curr];
        assigned[idx] = true;
        curr -= scaled_p[idx];
    }

    // Stwórz harmonogram na podstawie przypisania
    std::vector<std::vector<task>> machine_tasks(2);
    for (int i = 0; i < n; ++i)
        machine_tasks[assigned[i] ? 0 : 1].push_back(tasks[i]);

    return machine_tasks;
}