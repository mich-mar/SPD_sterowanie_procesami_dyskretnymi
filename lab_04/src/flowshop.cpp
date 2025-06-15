#include "flowshop.h"

/**
 * @brief Wczytuje dane z pliku CSV
 * @details Format pliku:
 * - Pierwszy wiersz zawiera nagłówki
 * - Kolejne wiersze zawierają czasy przetwarzania dla każdej maszyny
 * - Wartości są rozdzielone przecinkami
 * @param filename Ścieżka do pliku wejściowego
 * @return true jeśli wczytywanie się powiodło, false w przeciwnym razie
 */
bool FlowShop::loadFromFile(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Nie można otworzyć pliku: " << filename << std::endl;
        return false;
    }

    jobs.clear();
    std::string line;

    // Pomiń pierwszy wiersz (nagłówki)
    if (!std::getline(file, line)) {
        std::cerr << "Pusty plik" << std::endl;
        return false;
    }

    int jobId = 0;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;

        // Pomiń pierwszą kolumnę (index)
        if (!std::getline(ss, cell, ',')) {
            continue;
        }

        Job job;
        job.id = jobId++;
        job.processingTimes.reserve(3);  // Zakładając 3 maszyny

        // Wczytaj czasy przetwarzania
        while (std::getline(ss, cell, ',')) {
            if (!cell.empty()) {
                try {
                    int time = std::stoi(cell);
                    if (time < 0) {
                        std::cerr << "Ujemny czas przetwarzania w wierszu " << jobId << std::endl;
                        return false;
                    }
                    job.processingTimes.push_back(time);
                } catch (const std::exception &e) {
                    std::cerr << "Błąd konwersji w wierszu " << jobId << ": " << e.what() << std::endl;
                    return false;
                }
            }
        }

        if (numMachines == 0) {
            numMachines = job.processingTimes.size();
        } else if (job.processingTimes.size() != numMachines) {
            std::cerr << "Nieprawidłowa liczba czasów przetwarzania w wierszu " << jobId << std::endl;
            return false;
        }

        jobs.push_back(job);
    }

    if (jobs.empty()) {
        std::cerr << "Nie wczytano żadnych zadań" << std::endl;
        return false;
    }

    return true;
}

/**
 * @brief Implementacja algorytmu NEH (Nawaz, Enscore, Ham)
 * @details Algorytm NEH składa się z następujących kroków:
 * 1. Obliczenie sum czasów przetwarzania dla każdego zadania
 * 2. Sortowanie zadań malejąco według tych sum
 * 3. Konstrukcja rozwiązania przez wstawianie zadań na najlepsze pozycje
 * @return Para {najlepsza permutacja, wartość Cmax}
 * @complexity O(n²m), gdzie n - liczba zadań, m - liczba maszyn
 */
std::pair<std::vector<int>, int> FlowShop::neh() const {
    if (jobs.empty()) {
        return {std::vector<int>(), 0};
    }

    // Oblicz sumy czasów przetwarzania dla każdego zadania
    std::vector<std::pair<int, int>> jobSums;
    jobSums.reserve(jobs.size());  // Rezerwacja pamięci z góry

    for (size_t i = 0; i < jobs.size(); ++i) {
        int sum = 0;
        for (int time: jobs[i].processingTimes) {
            sum += time;
        }
        jobSums.emplace_back(sum, i);
    }

    // Sortuj malejąco według sum czasów przetwarzania
    std::sort(jobSums.begin(), jobSums.end(),
              [](const auto &a, const auto &b) { return a.first > b.first; });

    // Inicjalizacja częściowego harmonogramu
    std::vector<int> partialSchedule;
    partialSchedule.reserve(jobs.size());  // Rezerwacja pamięci z góry
    partialSchedule.push_back(jobSums[0].second);

    // Wstawiaj kolejne zadania na najlepszą pozycję
    for (size_t i = 1; i < jobs.size(); ++i) {
        int currentJob = jobSums[i].second;
        int bestPos = 0;
        int bestMakespan = INT_MAX;
        std::vector<int> bestSchedule = partialSchedule;

        // Sprawdź każdą możliwą pozycję wstawienia
        for (size_t j = 0; j <= partialSchedule.size(); ++j) {
            std::vector<int> testSchedule = partialSchedule;
            testSchedule.insert(testSchedule.begin() + j, currentJob);

            int makespan = calculateMakespan(testSchedule);
            if (makespan < bestMakespan) {
                bestMakespan = makespan;
                bestPos = j;
                bestSchedule = testSchedule;
            }
        }

        // Aktualizuj harmonogram częściowy
        partialSchedule = bestSchedule;
    }

    return {partialSchedule, calculateMakespan(partialSchedule)};
}

/**
 * @brief Oblicza całkowity czas wykonania (Cmax) dla danej permutacji zadań
 * @details Implementuje efektywne obliczanie makespan używając tablicy czasów maszyn
 * @param permutation Permutacja zadań do oceny
 * @return Całkowity czas wykonania (Cmax)
 * @complexity O(nm), gdzie n - liczba zadań, m - liczba maszyn
 */
int FlowShop::calculateMakespan(const std::vector<int> &permutation) const {
    if (permutation.empty() || jobs.empty() || numMachines == 0) {
        return 0;
    }

    std::vector<int> machineTime(numMachines, 0);

    // Dla pierwszego zadania
    for (int m = 0; m < numMachines; ++m) {
        machineTime[m] = (m == 0) ?
                         jobs[permutation[0]].processingTimes[0] :
                         machineTime[m - 1] + jobs[permutation[0]].processingTimes[m];
    }

    // Dla pozostałych zadań
    for (size_t j = 1; j < permutation.size(); ++j) {
        // Pierwsza maszyna
        machineTime[0] += jobs[permutation[j]].processingTimes[0];

        // Pozostałe maszyny
        for (int m = 1; m < numMachines; ++m) {
            machineTime[m] = std::max(machineTime[m - 1], machineTime[m]) +
                             jobs[permutation[j]].processingTimes[m];
        }
    }

    return machineTime[numMachines - 1];
}

/**
 * @brief Oblicza czasy zakończenia dla wszystkich zadań na wszystkich maszynach
 * @param permutation Permutacja zadań do analizy
 * @return Macierz czasów zakończenia [maszyna][zadanie]
 */
std::vector<std::vector<int>> FlowShop::calculateCompletionTimes(const std::vector<int> &permutation) const {
    std::vector<std::vector<int>> C(numMachines, std::vector<int>(jobs.size(), 0));

    // Pierwsza maszyna
    int sum = 0;
    for (size_t j = 0; j < jobs.size(); ++j) {
        sum += jobs[permutation[j]].processingTimes[0];
        C[0][j] = sum;
    }

    // Pozostałe maszyny
    for (int i = 1; i < numMachines; ++i) {
        C[i][0] = C[i - 1][0] + jobs[permutation[0]].processingTimes[i];

        for (size_t j = 1; j < jobs.size(); ++j) {
            C[i][j] = std::max(C[i - 1][j], C[i][j - 1]) +
                      jobs[permutation[j]].processingTimes[i];
        }
    }

    return C;
}

/**
 * @brief Implementacja przeglądu zupełnego (brute force)
 * @details Generuje wszystkie możliwe permutacje zadań i wybiera najlepszą
 * @return Para {najlepsza permutacja, wartość Cmax}
 * @complexity O(n!), gdzie n - liczba zadań
 */
std::pair<std::vector<int>, int> FlowShop::bruteForce() const {
    std::vector<int> bestPermutation(jobs.size());
    std::vector<int> currentPermutation(jobs.size());
    std::iota(currentPermutation.begin(), currentPermutation.end(), 0);

    int bestMakespan = INT_MAX;

    do {
        int currentMakespan = calculateMakespan(currentPermutation);
        if (currentMakespan < bestMakespan) {
            bestMakespan = currentMakespan;
            bestPermutation = currentPermutation;
        }
    } while (std::next_permutation(currentPermutation.begin(), currentPermutation.end()));

    return {bestPermutation, bestMakespan};
}

/**
 * @brief Implementacja algorytmu Johnsona dla problemu z dwoma maszynami
 * @details Algorytm Johnsona:
 * 1. Wybiera zadanie z najkrótszym czasem przetwarzania
 * 2. Jeśli czas jest na M1 - zadanie idzie na początek
 * 3. Jeśli czas jest na M2 - zadanie idzie na koniec
 * @return Para {optymalna permutacja, wartość Cmax}
 * @complexity O(n log n), gdzie n - liczba zadań
 */
std::pair<std::vector<int>, int> FlowShop::johnson() const {
    // Nawet jeśli mamy więcej maszyn, używamy tylko pierwszych dwóch
    const int JOHNSON_MACHINES = 2;

    // Przygotowanie danych do algorytmu Johnsona
    struct JobJohnson {
        int id;
        int time1;  // czas na pierwszej maszynie
        int time2;  // czas na drugiej maszynie (jeśli istnieje)
    };

    std::vector<JobJohnson> jobsData;
    jobsData.reserve(jobs.size());

    for (size_t i = 0; i < jobs.size(); ++i) {
        jobsData.push_back({
                                   static_cast<int>(i),
                                   jobs[i].processingTimes[0],  // pierwsza maszyna
                                   (numMachines > 1) ? jobs[i].processingTimes[1] : 0  // druga maszyna (jeśli istnieje)
                           });
    }

    // Implementacja algorytmu Johnsona
    std::vector<int> sequence(jobs.size());
    int left = 0;                    // indeks od lewej
    int right = jobs.size() - 1;     // indeks od prawej

    while (!jobsData.empty()) {
        // Znajdź zadanie z minimalnym czasem na którejkolwiek maszynie
        auto minTimeJob = std::min_element(jobsData.begin(), jobsData.end(),
                                           [](const JobJohnson &a, const JobJohnson &b) {
                                               return std::min(a.time1, a.time2) < std::min(b.time1, b.time2);
                                           });

        // Jeśli minimalny czas jest na pierwszej maszynie, dodaj zadanie na początek
        // W przeciwnym razie dodaj na koniec
        if (minTimeJob->time1 < minTimeJob->time2) {
            sequence[left++] = minTimeJob->id;
        } else {
            sequence[right--] = minTimeJob->id;
        }

        // Usuń przetworzone zadanie
        jobsData.erase(minTimeJob);
    }

    // Oblicz makespan dla znalezionej sekwencji
    int makespan = calculateMakespan(sequence);

    return {sequence, makespan};
}


/**
 * @brief Aktualizuje tablicę najwcześniejszych czasów zakończenia dla FNEH
 * @param e Referencja do tablicy czasów
 * @param sequence Aktualna sekwencja zadań
 */
void FlowShop::updateEarliestCompletionTimes(std::vector<std::vector<int>> &e,
                                             const std::vector<int> &sequence) const {
    const size_t n = sequence.size();

    // Dla pierwszego zadania
    e[0][0] = jobs[sequence[0]].processingTimes[0];
    for (int m = 1; m < numMachines; ++m) {
        e[0][m] = e[0][m - 1] + jobs[sequence[0]].processingTimes[m];
    }

    // Dla pozostałych zadań
    for (size_t j = 1; j < n; ++j) {
        e[j][0] = e[j - 1][0] + jobs[sequence[j]].processingTimes[0];
        for (int m = 1; m < numMachines; ++m) {
            e[j][m] = std::max(e[j][m - 1], e[j - 1][m]) +
                      jobs[sequence[j]].processingTimes[m];
        }
    }
}

/**
 * @brief Aktualizuje tablicę czasów tail dla FNEH
 * @param q Referencja do tablicy czasów tail
 * @param sequence Aktualna sekwencja zadań
 */
void FlowShop::updateTailTimes(std::vector<std::vector<int>> &q,
                               const std::vector<int> &sequence) const {
    const size_t n = sequence.size();

    // Inicjalizacja ostatniego zadania
    for (int m = 0; m < numMachines; ++m) {
        q[n - 1][m] = 0;
    }

    // Obliczanie czasów tail dla pozostałych zadań
    for (int j = n - 2; j >= 0; --j) {
        q[j][numMachines - 1] = q[j + 1][numMachines - 1] +
                                jobs[sequence[j + 1]].processingTimes[numMachines - 1];

        for (int m = numMachines - 2; m >= 0; --m) {
            q[j][m] = std::max(q[j + 1][m] + jobs[sequence[j + 1]].processingTimes[m],
                               q[j][m + 1]);
        }
    }
}

/**
 * @brief Oblicza częściowy makespan dla FNEH
 * @param data Struktura z danymi pomocniczymi
 * @param sequence Aktualna sekwencja zadań
 * @param insertedJob Zadanie do wstawienia
 * @param position Pozycja wstawienia
 * @return Wartość makespan dla częściowej permutacji
 */
int FlowShop::calculatePartialMakespan(const FNEHData &data,
                                       const std::vector<int> &sequence,
                                       int insertedJob,
                                       size_t position) const {
    std::vector<int> machine_completion(numMachines, 0);

    // Oblicz czasy dla zadań przed pozycją wstawienia
    for (size_t m = 0; m < numMachines; ++m) {
        if (position > 0) {
            machine_completion[m] = data.e[position - 1][m];
        }
    }

    // Dodaj czasy dla wstawianego zadania
    machine_completion[0] += jobs[insertedJob].processingTimes[0];
    for (size_t m = 1; m < numMachines; ++m) {
        machine_completion[m] = std::max(machine_completion[m - 1],
                                         machine_completion[m]) +
                                jobs[insertedJob].processingTimes[m];
    }

    // Uwzględnij tail times
    return machine_completion[numMachines - 1] +
           (position < sequence.size() ? data.q[position][0] : 0);
}

/**
 * @brief Implementacja algorytmu NEH z akceleracją (FNEH)
 * @details FNEH to zmodyfikowana wersja NEH, która:
 * 1. Używa efektywnych struktur danych do przechowywania czasów
 * 2. Wykorzystuje właściwości problemu do redukcji liczby obliczeń
 * 3. Zachowuje tę samą jakość rozwiązań co NEH przy krótszym czasie wykonania
 * @return Para {najlepsza permutacja, wartość Cmax}
 * @complexity O(n²m), gdzie n - liczba zadań, m - liczba maszyn
 */
std::pair<std::vector<int>, int> FlowShop::fneh() const {
    if (jobs.empty()) {
        return {std::vector<int>(), 0};
    }

    // Oblicz sumy czasów przetwarzania dla każdego zadania
    std::vector<std::pair<int, int>> jobSums;
    jobSums.reserve(jobs.size());

    for (size_t i = 0; i < jobs.size(); ++i) {
        int sum = std::accumulate(jobs[i].processingTimes.begin(),
                                  jobs[i].processingTimes.end(), 0);
        jobSums.emplace_back(sum, i);
    }

    // Sortuj malejąco według sum czasów przetwarzania
    std::sort(jobSums.begin(), jobSums.end(),
              [](const auto &a, const auto &b) { return a.first > b.first; });

    // Inicjalizacja struktur pomocniczych
    FNEHData data;
    data.e.resize(jobs.size(), std::vector<int>(numMachines));
    data.q.resize(jobs.size(), std::vector<int>(numMachines));

    // Inicjalizacja sekwencji pierwszym zadaniem
    std::vector<int> sequence;
    sequence.push_back(jobSums[0].second);

    // Główna pętla FNEH
    for (size_t i = 1; i < jobs.size(); ++i) {
        int currentJob = jobSums[i].second;
        int bestPos = 0;
        int bestMakespan = INT_MAX;

        // Aktualizuj tablice pomocnicze
        updateEarliestCompletionTimes(data.e, sequence);
        updateTailTimes(data.q, sequence);

        // Znajdź najlepszą pozycję dla bieżącego zadania
        for (size_t j = 0; j <= sequence.size(); ++j) {
            int makespan = calculatePartialMakespan(data, sequence, currentJob, j);
            if (makespan < bestMakespan) {
                bestMakespan = makespan;
                bestPos = j;
            }
        }

        // Wstaw zadanie na najlepszą pozycję
        sequence.insert(sequence.begin() + bestPos, currentJob);
    }

    return {sequence, calculateMakespan(sequence)};
}