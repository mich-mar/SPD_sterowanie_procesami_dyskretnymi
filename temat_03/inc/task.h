/**
 * @file task.h
 * @brief Definiuje strukturę zadania i funkcje związane z zarządzaniem zadaniami.
 *
 * Ten plik nagłówkowy zawiera definicję struktury zadania, która reprezentuje
 * zadanie przetwarzania z identyfikatorem i czasem przetwarzania.
 * Zawiera również deklaracje funkcji do wczytywania zadań z plików i ich wyświetlania.
 */

#ifndef TASK_H
#define TASK_H

#include <vector>
#include <string>

/**
 * @brief Struktura reprezentująca zadanie przetwarzania
 *
 * Ta struktura przechowuje informacje o zadaniu, które ma zostać zaplanowane
 * do przetworzenia na maszynie.
 */
struct task {
    int id;      /**< Unikalny identyfikator zadania */
    int p_time;  /**< Czas przetwarzania wymagany dla zadania */
};

/**
 * @brief Wczytuje dane zadań z pliku
 *
 * Ta funkcja otwiera podany plik i wczytuje z niego dane zadań, tworząc
 * wektor struktur zadań. Każdy wiersz pliku powinien zawierać ID zadania i czas przetwarzania.
 *
 * @param filename Nazwa pliku zawierającego dane zadań
 * @return Wektor struktur zadań wczytanych z pliku
 */
std::vector<task> read_tasks_from_file(const std::string& filename);

/**
 * @brief Wyświetla listę zadań
 *
 * Ta funkcja wypisuje na standardowe wyjście informacje o wszystkich zadaniach
 * w przekazanym wektorze. Używana głównie do celów diagnostycznych.
 *
 * @param tasks Wektor zadań do wyświetlenia
 */
void print_tasks(const std::vector<task>& tasks);

#endif // TASK_H