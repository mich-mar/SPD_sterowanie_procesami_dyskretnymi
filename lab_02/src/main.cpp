#include "alg_01_heuristic.h"
#include "alg_02_brute_force.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// @todo: SCHRAGE2.dat ma takie same dane jak SCHRAGE1.dat, SCHRAGE1.out i SCHRAGE1.out są różne

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

int main() {
    std::string name = "SCHRAGE1";
    std::string datFile = name + ".dat";
    std::string outFile = name + ".out";

    std::vector<task> tasks = loadTasksFromFile(datFile);
    int correctAnswer = readCorrectOutcome(outFile);

    // int heurCmax = heuristic(tasks);
    // std::cout << "Heurystyczne Cmax: " << heurCmax << std::endl;

    int bruteCmax = bruteForce(tasks);
    std::cout << "Przegld zupełny Cmax: " << bruteCmax << std::endl;

    if (correctAnswer == bruteCmax) {
        std::cout << "Odpowiedz prawidłowa :))" << std::endl;
    }
    else {
        std::cout << "Zły output :((, prawidłowy to: " << correctAnswer << std::endl;
    }

    return 0;
}
