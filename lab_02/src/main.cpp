#include "alg_01_schrage.h"
#include "alg_02_brute_force.h"
#include "alg_00_heuristic.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include "alg_03_wspt.h"

/**
 * @brief Funkcja loadTasksFromFile wczytuje dane z pliku i tworzy wektor zadań.
 *
 * @param filename Nazwa pliku, z którego mają być wczytane dane.
 * @return Wektor zadań, gdzie każde zadanie ma pola r (czas dostępności), p (czas przetwarzania) i q (czas zakończenia).
 */
std::vector<task> loadTasksFromFile(const std::string &filename)
{
    std::ifstream file("data/" + filename);
    std::vector<task> tasks;

    if (!file)
    {
        std::cerr << "Nie można otworzyć pliku: " << filename << std::endl;
        exit(EXIT_FAILURE);
    }

    int n;
    file >> n;
    tasks.resize(n);

    for (int i = 0; i < n; i++)
    {
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
int readCorrectOutcome(const std::string &fileName)
{
    std::ifstream inputFile("data/" + fileName);

    if (!inputFile)
    {
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
std::pair<int, double> measureExecutionTime(Func func, std::vector<task> &tasks)
{
    auto start = std::chrono::high_resolution_clock::now();
    int result = func(tasks);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    return {result, elapsed.count()};
}

// ctr; + shift + i <--- code format
int main()
{
    std::string name = "SCHRAGE";

    for (int i = 1; i < 10; i++)
    {

        std::string datFile = name + std::to_string(i) + ".dat";
        std::string outFile = name + std::to_string(i) + ".out";

        std::vector<task> tasks = loadTasksFromFile(datFile);
        int correctAnswer = readCorrectOutcome(outFile);

        std::cout << "==================================================" << std::endl;
        std::cout << "Wyniki dla pliku: " << datFile << std::endl;
        std::cout << "Dane w pliku: " << correctAnswer << std::endl;
        std::cout << "==================================================" << std::endl;
        std::cout << std::endl;

        auto [qjMax, elapsed_gj] = measureExecutionTime(rjSortPlaning, tasks);
        std::cout << "QjSort Cmax: " << qjMax << std::endl;
        std::cout << "Czas działania algorytmu QjSort: " << elapsed_gj << " sekund" << std::endl;

        std::cout << std::endl;

        auto [rjMax, elapsed_rj] = measureExecutionTime(qjSortPlaning, tasks);
        std::cout << "RjSort Cmax: " << rjMax << std::endl;
        std::cout << "Czas działania algorytmu RjSort: " << elapsed_rj << " sekund" << std::endl;

        std::cout << std::endl;

        if (i <= 2)
        {
            auto [bruteCmax, elapsed_brute] = measureExecutionTime(bruteForce, tasks);
            std::cout << "Brute force Cmax: " << bruteCmax << std::endl;
            std::cout << "Czas działania algorytmu brute force: " << elapsed_brute << " sekund" << std::endl;

            std::cout << std::endl;
        }

        auto [schargeCmax, elapsed_schrage] = measureExecutionTime(schragePlaning, tasks);
        std::cout << "Scharge Cmax: " << schargeCmax << std::endl;
        std::cout << "Czas działania algorytmu Scharge: " << elapsed_schrage << " sekund" << std::endl;

        std::cout << std::endl;

        auto [schargePreemptiveCmax, elapsed_schragePreemptive] = measureExecutionTime(schragePreemptivePlaning, tasks);
        std::cout << "SchargePreemptive Cmax: " << schargePreemptiveCmax << std::endl;
        std::cout << "Czas działania algorytmu Scharge z wywłaszczeniem: " << elapsed_schragePreemptive << " sekund" << std::endl;

        std::cout << std::endl;

        auto [WSPTCmax, elapsed_WSPT] = measureExecutionTime(weightedSPTPlaning, tasks);
        std::cout << "WSPT Cmax: " << WSPTCmax << std::endl;
        std::cout << "Czas działania algorytmu WSPT: " << elapsed_WSPT << " sekund" << std::endl;

        // if (correctAnswer == schargeCmax)
        // {
        //     std::cout << "Odpowiedz prawidłowa :))" << std::endl;
        // }
        // else
        // {
        //     std::cout << "Zły output :((, prawidłowy to: " << correctAnswer << std::endl;
        // }

        std::cout << std::endl;
    }
    return 0;
}
