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
 * Dodatkowo oblicza i wyświetla Cmax (czas zakończenia wszystkich zadań).
 *
 * @param machine_tasks Wektor wektorów zadań przydzielonych do poszczególnych maszyn
 */
void print_schedule(const std::vector<std::vector<task>> &machine_tasks) {
    int Cmax = 0;
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
        Cmax = std::max(Cmax, sum);
        std::cout << "\n";
    }
    std::cout << "Cmax: " << Cmax << "\n";
}

/**
 * @brief Implementuje algorytm szeregowania zadań metodą siłową (brute force)
 *
 * Algorytm sprawdza wszystkie możliwe przydziały zadań do dwóch maszyn
 * i wybiera ten, który minimalizuje Cmax.
 *
 * @param tasks Wektor zadań do zaszeregowania
 * @return Wektor wektorów zadań przydzielonych do poszczególnych maszyn (dokładnie 2 maszyny)
 */
std::vector<std::vector<task>> schedule_tasks_bruteforce(const std::vector<task> &tasks) {
    int n = tasks.size();
    int min_Cmax = INT_MAX;
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
        int Cmax = std::max(sum0, sum1);

        // Jeśli znaleziono lepszy podział, zaktualizuj najlepsze rozwiązanie
        if (Cmax < min_Cmax) {
            min_Cmax = Cmax;
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
 * @brief Algorytm szeregowania zadań na dwie maszyny przy użyciu programowania dynamicznego
 *
 * Algorytm rozwiązuje problem poprzez:
 * 1. Potraktowanie go jako problemu plecakowego
 * 2. Znalezienie podzbioru zadań o sumie czasów najbliższej połowie całkowitego czasu
 * 3. Przydzielenie znalezionego podzbioru do pierwszej maszyny, a reszty do drugiej
 *
 * @param tasks Wektor zadań do zaszeregowania, każde zadanie zawiera czas wykonania (p_time)
 * @return Wektor dwóch wektorów zadań - przydział dla każdej z maszyn
 */
std::vector<std::vector<task>> schedule_tasks_dp(const std::vector<task> &tasks) {
    // Sumujemy czasy wszystkich zadań, aby poznać całkowity czas do rozdzielenia
    int total_time = 0;
    for (const auto &t: tasks)
        total_time += t.p_time;

    int n = tasks.size();  // liczba wszystkich zadań
    // Obliczamy cel - idealnie byłoby podzielić całkowity czas na pół
    int half_time = total_time / 2;

    // Tablica dp służy do śledzenia możliwych do osiągnięcia sum czasów
    // dp[i] = true oznacza, że można znaleźć podzbiór zadań o sumie czasów równej i
    std::vector<bool> dp(half_time + 1, false);

    // Tablica parent służy do odtworzenia rozwiązania
    // parent[i] = j oznacza, że suma i została osiągnięta przez dodanie zadania j
    std::vector<int> parent(half_time + 1, -1);

    // Pusta suma (0) jest zawsze osiągalna
    dp[0] = true;

    // Główna pętla programowania dynamicznego
    // Dla każdego zadania sprawdzamy, jakie nowe sumy możemy osiągnąć
    for (int i = 0; i < n; ++i) {
        // Iterujemy od największej możliwej sumy w dół
        // Iteracja w dół zapobiega wielokrotnemu użyciu tego samego zadania
        for (int j = half_time; j >= tasks[i].p_time; --j) {
            // Sprawdzamy, czy możemy osiągnąć sumę j przez dodanie obecnego zadania
            // do wcześniej osiągniętej sumy (j - tasks[i].p_time)
            if (dp[j - tasks[i].p_time] && !dp[j]) {
                dp[j] = true;  // Oznaczamy nową osiągalną sumę
                parent[j] = i; // Zapamiętujemy, które zadanie doprowadziło do tej sumy
            }
        }
    }

    // Szukamy największej osiągalnej sumy nie przekraczającej half_time
    // Jest to suma najbliższa idealnemu podziałowi na pół
    int best_sum = half_time;
    while (!dp[best_sum]) --best_sum;

    // Wektor assigned będzie oznaczać, które zadania zostały przydzielone do pierwszej maszyny
    std::vector<bool> assigned(n, false);

    // Odtwarzamy rozwiązanie, idąc wstecz od znalezionej najlepszej sumy
    int curr = best_sum;
    while (curr > 0 && parent[curr] != -1) {
        int idx = parent[curr];        // Indeks zadania, które dodaliśmy
        assigned[idx] = true;          // Oznaczamy to zadanie jako przydzielone do maszyny 0
        curr -= tasks[idx].p_time;     // Odejmujemy czas tego zadania
    }

    // Tworzymy końcowy harmonogram - dwa wektory zadań
    std::vector<std::vector<task>> machine_tasks(2);

    // Przydzielamy zadania do odpowiednich maszyn
    // - assigned[i] = true  -> zadanie idzie do maszyny 0
    // - assigned[i] = false -> zadanie idzie do maszyny 1
    for (int i = 0; i < n; ++i) {
        if (assigned[i])
            machine_tasks[0].push_back(tasks[i]);  // Pierwsza maszyna
        else
            machine_tasks[1].push_back(tasks[i]);  // Druga maszyna
    }

    return machine_tasks;
}

/**
 * @brief Implementuje PTAS (Polynomial-Time Approximation Scheme) dla problemu szeregowania zadań na 2 maszynach
 *
 * Problem szeregowania:
 * - Mamy zbiór zadań, każde z czasem wykonania
 * - Mamy dokładnie 2 maszyny
 * - Cel: Zminimalizować czas zakończenia wszystkich zadań (makespan)
 *
 * Strategia algorytmu:
 * 1. Wybieramy k najdłuższych zadań (k zależy od parametru epsilon)
 * 2. Dla tych k zadań sprawdzamy wszystkie możliwe kombinacje przydziału (2^k kombinacji)
 * 3. Pozostałe (krótsze) zadania przydzielamy zachłannie
 *
 * @param tasks Wektor zadań do zaszeregowania, każde zadanie ma określony czas wykonania (p_time)
 * @param epsilon Parametr dokładności w zakresie (0,1]:
 *               - mniejsze epsilon = większe k = większa dokładność = dłuższy czas wykonania
 *               - większe epsilon = mniejsze k = mniejsza dokładność = krótszy czas wykonania
 * @return Wektor 2 wektorów zadań (przydział zadań do każdej z maszyn)
 */
std::vector<std::vector<task>> schedule_tasks_ptas(const std::vector<task> &tasks, double epsilon) {
    // Obliczamy k jako odwrotność epsilon (zaokrąglone w górę)
    int k = static_cast<int>(std::ceil(1.0 / epsilon));

    // Tworzymy kopię zadań i sortujemy malejąco według czasów wykonania
    std::vector<task> sorted = tasks;
    std::sort(sorted.begin(), sorted.end(), [](const task &a, const task &b) {
        return a.p_time > b.p_time;
    });

    // Dzielimy zadania na dwie grupy:
    // 1. top_k: k największych zadań (analizowane dokładnie)
    // 2. rest: pozostałe zadania (przydzielane zachłannie)
    std::vector<task> top_k(sorted.begin(), sorted.begin() + std::min(k, (int) sorted.size()));
    std::vector<task> rest(sorted.begin() + std::min(k, (int) sorted.size()), sorted.end());

    // Zmienne do śledzenia najlepszego znalezionego rozwiązania
    int min_makespan = INT_MAX;  // Najlepszy znaleziony czas zakończenia
    std::vector<std::vector<task>> best_schedule(2);  // Najlepszy znaleziony przydział zadań

    // Generujemy wszystkie możliwe kombinacje przydziału k największych zadań
    int combinations = 1 << top_k.size();  // 2^k kombinacji
    for (int mask = 0; mask < combinations; ++mask) {
        // Dla każdej kombinacji tworzymy nowy przydział i śledzimy obciążenie maszyn
        std::vector<std::vector<task>> current_schedule(2);
        int loads[2] = {0, 0};  // Aktualne obciążenie każdej maszyny

        // Przydzielamy k największych zadań według aktualnej maski bitowej
        // Każdy bit w masce określa, na którą maszynę idzie odpowiednie zadanie
        for (int i = 0; i < (int) top_k.size(); ++i) {
            // Sprawdzamy i-ty bit maski
            int m = (mask & (1 << i)) ? 1 : 0;
            current_schedule[m].push_back(top_k[i]);
            loads[m] += top_k[i].p_time;
        }

        // Pozostałe zadania przydzielamy zachłannie
        // Zawsze wybieramy maszynę o mniejszym aktualnym obciążeniu
        for (const auto &t: rest) {
            int m = (loads[0] <= loads[1]) ? 0 : 1;
            current_schedule[m].push_back(t);
            loads[m] += t.p_time;
        }

        // Sprawdzamy, czy aktualny przydział jest lepszy od dotychczasowego
        // Cmax to maksymalny czas wykonania na obu maszynach
        int Cmax = std::max(loads[0], loads[1]);
        if (Cmax < min_makespan) {
            min_makespan = Cmax;
            best_schedule = current_schedule;
        }
    }

    return best_schedule;
}

/**
 * @brief Implementuje FPTAS (Fully Polynomial-Time Approximation Scheme) dla problemu szeregowania zadań
 *
 * Problem: Mamy 2 maszyny i N zadań. Każde zadanie ma określony czas wykonania.
 * Cel: Podzielić zadania między maszyny tak, aby zminimalizować różnicę w całkowitym czasie pracy.
 *
 * @param tasks Wektor zadań do zaszeregowania, gdzie każde zadanie ma swój czas wykonania (p_time)
 * @param epsilon Parametr dokładności (0 < epsilon <= 1)
 *               - Mniejsze epsilon = dokładniejsze, ale wolniejsze obliczenia
 *               - Większe epsilon = szybsze, ale mniej dokładne obliczenia
 * @return Wektor 2 wektorów zawierających przydział zadań do maszyn
 */
std::vector<std::vector<task>> schedule_tasks_fptas(const std::vector<task> &tasks, double epsilon) {
    // Pobierz liczbę wszystkich zadań
    int n = tasks.size();

    // Znajdź najdłuższy czas wykonania wśród wszystkich zadań
    // Potrzebne do określenia współczynnika skalowania
    int max_p = 0;
    for (const auto &t: tasks)
        max_p = std::max(max_p, t.p_time);

    // Oblicz współczynnik skalowania K
    // K = (epsilon * max_p) / (2 * n) zapewnia, że błąd aproksymacji nie przekroczy (1 + epsilon)
    double K = (epsilon * max_p) / (2 * n);
    if (K < 1.0)
        K = 1.0;  // Zabezpieczenie przed zbyt małym współczynnikiem

    // Przeskaluj czasy wykonania wszystkich zadań
    // Dzielimy każdy czas przez K, aby uzyskać mniejsze liczby
    std::vector<int> scaled_p(n);
    for (int i = 0; i < n; ++i)
        scaled_p[i] = static_cast<int>(tasks[i].p_time / K);

    // Oblicz sumę wszystkich przeskalowanych czasów i jej połowę
    // half to nasz cel - chcemy znaleźć podzbiór zadań o sumie najbliższej half
    int sum = std::accumulate(scaled_p.begin(), scaled_p.end(), 0);
    int half = sum / 2;

    // Inicjalizacja tablic do programowania dynamicznego
    // dp[j] = true oznacza, że można uzyskać sumę j z podzbioru zadań
    std::vector<bool> dp(half + 1, false);
    // parent[j] przechowuje indeks zadania, które dodano aby uzyskać sumę j
    std::vector<int> parent(half + 1, -1);
    dp[0] = true;  // Suma 0 jest zawsze osiągalna (pusty podzbiór)

    // Główna pętla programowania dynamicznego
    // Implementuje zmodyfikowany algorytm problemu plecakowego
    for (int i = 0; i < n; ++i) {
        // Iterujemy od największej możliwej sumy w dół
        // aby uniknąć wielokrotnego użycia tego samego zadania
        for (int j = half; j >= scaled_p[i]; --j) {
            // Sprawdź czy możemy utworzyć sumę j dodając obecne zadanie
            // do wcześniej obliczonej sumy (j - scaled_p[i])
            if (dp[j - scaled_p[i]] && !dp[j]) {
                dp[j] = true;        // Zaznacz, że suma j jest osiągalna
                parent[j] = i;       // Zapamiętaj, które zadanie dało tę sumę
            }
        }
    }

    // Znajdź największą osiągalną sumę nie większą niż half
    int best_sum = half;
    while (!dp[best_sum]) --best_sum;

    // Odtwórz rozwiązanie - które zadania zostały wybrane
    // assigned[i] = true oznacza, że zadanie i idzie na pierwszą maszynę
    std::vector<bool> assigned(n, false);
    int curr = best_sum;
    while (curr > 0 && parent[curr] != -1) {
        int idx = parent[curr];
        assigned[idx] = true;        // Przypisz zadanie do pierwszej maszyny
        curr -= scaled_p[idx];       // Odejmij jego czas od sumy
    }

    // Utwórz finalny podział zadań między maszyny
    std::vector<std::vector<task>> machine_tasks(2);
    for (int i = 0; i < n; ++i)
        // Zadania zaznaczone jako assigned idą na maszynę 0, pozostałe na maszynę 1
        machine_tasks[assigned[i] ? 0 : 1].push_back(tasks[i]);

    return machine_tasks;
}