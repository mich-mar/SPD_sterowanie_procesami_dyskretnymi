#ifndef FLOWSHOP_H
#define FLOWSHOP_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <climits>

/**
 * @struct Job
 * @brief Struktura reprezentująca pojedyncze zadanie w problemie flow shop
 */
struct Job {
    int id;                         ///< Identyfikator zadania
    std::vector<int> processingTimes; ///< Czasy przetwarzania na poszczególnych maszynach
};

/**
 * @class FlowShop
 * @brief Klasa implementująca algorytmy szeregowania zadań dla problemu flow shop
 */
class FlowShop {
private:
    std::vector<Job> jobs;        ///< Wektor wszystkich zadań
    int numMachines;              ///< Liczba maszyn w problemie

    /**
     * @brief Oblicza makespan (Cmax) dla danej permutacji zadań
     * @param permutation Permutacja zadań do oceny
     * @return Całkowity czas wykonania wszystkich zadań (Cmax)
     */
    int calculateMakespan(const std::vector<int>& permutation) const;

    /**
     * @brief Oblicza czasy zakończenia zadań na wszystkich maszynach
     * @param permutation Permutacja zadań do analizy
     * @return Macierz czasów zakończenia zadań
     */
    std::vector<std::vector<int>> calculateCompletionTimes(const std::vector<int>& permutation) const;

public:
    /**
     * @brief Konstruktor domyślny
     */
    FlowShop() : numMachines(0) {}

    /**
     * @brief Wczytuje dane z pliku CSV
     * @param filename Ścieżka do pliku z danymi
     * @return true jeśli wczytywanie się powiodło, false w przeciwnym razie
     */
    bool loadFromFile(const std::string& filename);

    /**
     * @brief Implementacja algorytmu przeglądu zupełnego
     * @return Para {najlepsza permutacja, wartość Cmax}
     */
    std::pair<std::vector<int>, int> bruteForce() const;

    /**
     * @brief Implementacja algorytmu NEH z akceleracją (FNEH)
     * @return Para {najlepsza permutacja, wartość Cmax}
     */
    std::pair<std::vector<int>, int> fneh() const;

    /**
     * @brief Implementacja algorytmu NEH
     * @return Para {najlepsza permutacja, wartość Cmax}
     */
    std::pair<std::vector<int>, int> neh() const;

    /**
     * @brief Implementacja algorytmu Johnsona dla dwóch maszyn
     * @return Para {najlepsza permutacja, wartość Cmax}
     */
    std::pair<std::vector<int>, int> johnson() const;

    /**
     * @brief Zwraca liczbę zadań
     * @return Liczba zadań w instancji
     */
    int getNumJobs() const { return jobs.size(); }

    /**
     * @brief Zwraca liczbę maszyn
     * @return Liczba maszyn w instancji
     */
    int getNumMachines() const { return numMachines; }

private:
    /**
     * @struct FNEHData
     * @brief Struktura pomocnicza dla algorytmu FNEH
     */
    struct FNEHData {
        std::vector<std::vector<int>> e;  ///< Najwcześniejsze możliwe czasy zakończenia
        std::vector<std::vector<int>> q;  ///< Czasy tail
        std::vector<int> insertion_sequence; ///< Sekwencja wstawiania
    };

    /**
     * @brief Aktualizuje tablicę najwcześniejszych czasów zakończenia
     * @param e Referencja do tablicy czasów
     * @param sequence Aktualna sekwencja zadań
     */
    void updateEarliestCompletionTimes(std::vector<std::vector<int>>& e,
                                       const std::vector<int>& sequence) const;

    /**
     * @brief Aktualizuje tablicę czasów tail
     * @param q Referencja do tablicy czasów tail
     * @param sequence Aktualna sekwencja zadań
     */
    void updateTailTimes(std::vector<std::vector<int>>& q,
                         const std::vector<int>& sequence) const;

    /**
     * @brief Oblicza częściowy makespan dla FNEH
     * @param data Struktura z danymi pomocniczymi
     * @param sequence Aktualna sekwencja zadań
     * @param insertedJob Zadanie do wstawienia
     * @param position Pozycja wstawienia
     * @return Wartość makespan dla częściowej permutacji
     */
    int calculatePartialMakespan(const FNEHData& data,
                                 const std::vector<int>& sequence,
                                 int insertedJob,
                                 size_t position) const;
};

#endif