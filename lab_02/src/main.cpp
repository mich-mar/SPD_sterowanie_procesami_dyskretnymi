#include "alg_01_heuristic.h"
#include "alg_02_brute_force.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>

/**
 * @brief Funkcja loadTasksFromFile wczytuje dane z pliku i tworzy wektor zadań.
 *
 * @param filename Nazwa pliku, z którego mają być wczytane dane.
 * @return Wektor zadań, gdzie każde zadanie ma pola r (czas dostępności), p (czas przetwarzania) i q (czas zakończenia).
 */
std::vector<task> loadTasksFromFile(const std::string &filename) {
    std::ifstream file("data/" + filename);
    std::vector<task> tasks;

    if (!file) {
        std::cerr << "Nie można otworzyć pliku: " << filename << std::endl;
        exit(EXIT_FAILURE);
    }

    int n;
    file >> n;
    tasks.resize(n);

    for (int i = 0; i < n; i++) {
        file >> tasks[i].r >> tasks[i].p >> tasks[i].q;
        tasks[i].id = i + 1;
    }

    file.close();
    return tasks;
}

/**
 * @brief Funkcja readCorrectOutcome wczytuje poprawny wynik z pliku.
 *
 * @param fileName Nazwa pliku, z którego ma być wczytany wynik.
 * @return Poprawny wynik jako liczba całkowita.
 */
int readCorrectOutcome(const std::string& fileName) {
    std::ifstream inputFile("data/" + fileName);

    if (!inputFile) {
        std::cerr << "Nie można otworzyć pliku: " << fileName << std::endl;
        exit(EXIT_FAILURE);
    }

    int number;
    inputFile >> number;

    inputFile.close();

    return number;
}

/**
 * @brief Funkcja measureExecutionTime mierzy czas wykonania funkcji.
 *
 * @param func Funkcja do wykonania.
 * @param tasks Wektor zadań przekazywany do funkcji.
 * @return Para zawierająca wynik funkcji oraz czas jej wykonania w sekundach.
 */
template <typename Func>
std::pair<int, double> measureExecutionTime(Func func, std::vector<task>& tasks) {
    auto start = std::chrono::high_resolution_clock::now();
    int result = func(tasks);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    return {result, elapsed.count()};
}

int main() {
    std::string name = "SCHRAGE1";
    std::string datFile = name + ".dat";
    std::string outFile = name + ".out";

    std::vector<task> tasks = loadTasksFromFile(datFile);
    int correctAnswer = readCorrectOutcome(outFile);

    auto [bruteCmax, elapsed_brute] = measureExecutionTime(bruteForce, tasks);
    std::cout << "Przegląd zupełny Cmax: " << bruteCmax << std::endl;
    std::cout << "Czas działania algorytmu brute force: " << elapsed_brute << " sekund" << std::endl;

    if (correctAnswer == bruteCmax) {
        std::cout << "Odpowiedz prawidłowa :))" << std::endl;
    }
    else {
        std::cout << "Zły output :((, prawidłowy to: " << correctAnswer << std::endl;
    }

    std::cout << std::endl;

    auto [heurCmax, elapsed_heur] = measureExecutionTime(heuristicPlaning, tasks);
    std::cout << "Heurystyczne Cmax: " << heurCmax << std::endl;
    std::cout << "Czas działania algorytmu heurystycznego: " << elapsed_heur << " sekund" << std::endl;

    if (correctAnswer == heurCmax) {
        std::cout << "Odpowiedz prawidłowa :))" << std::endl;
    }
    else {
        std::cout << "Zły output :((, prawidłowy to: " << correctAnswer << std::endl;
    }

    return 0;
}
