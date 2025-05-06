#include "../inc/task.h"
#include <iostream>
#include <fstream>
/**
 * @brief Wczytuje dane zadań z pliku
 *
 * Implementacja funkcji otwierającej plik i wczytującej z niego dane zadań.
 * Każdy wiersz pliku powinien zawierać dwie liczby: ID zadania i czas przetwarzania.
 *
 * @param filename Nazwa pliku zawierającego dane zadań
 * @return Wektor struktur zadań wczytanych z pliku
 */
std::vector<task> read_tasks_from_file(const std::string& filename) {
    std::vector<task> tasks;
    std::ifstream infile(filename);

    if (!infile) {
        std::cerr << "Blad otwarcia pliku: " << filename << std::endl;
        return tasks;
    }

    int id, p_time;
    while (infile >> id >> p_time) {
        tasks.push_back({id, p_time});
    }

    infile.close();
    return tasks;
}

/**
 * @brief Wyświetla listę zadań
 *
 * Implementacja funkcji wypisującej na standardowe wyjście informacje
 * o wszystkich zadaniach w przekazanym wektorze.
 *
 * @param tasks Wektor zadań do wyświetlenia
 */
void print_tasks(const std::vector<task>& tasks) {
    for (const auto& task : tasks) {
        std::cout << "ID: " << task.id << ", p_time: " << task.p_time << std::endl;
    }
}