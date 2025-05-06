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

std::vector<task> read_tasks_from_file(const std::string& filename);

void print_tasks(const std::vector<task>& tasks);

#endif // TASK_H