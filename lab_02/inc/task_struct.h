#ifndef TASK_STRUCT_H
#define TASK_STRUCT_H

/**
 * @brief Struktura reprezentująca zadanie w problemie harmonogramowania.
 *
 * Struktura zawiera cztery pola:
 * - id: Unikalny identyfikator zadania.
 * - r: Czas dostępności zadania (czas, od którego zadanie może być przetwarzane).
 * - p: Czas przetwarzania zadania (czas, jaki jest potrzebny na wykonanie zadania).
 * - q: Czas zakończenia zadania (czas, jaki upływa od zakończenia przetwarzania do momentu, gdy zadanie jest uznane za zakończone).
 */
struct task {
    int id; ///< Unikalny identyfikator zadania
    int r;  ///< Czas dostępności zadania
    int p;  ///< Czas przetwarzania zadania
    int q;  ///< Czas zakończenia zadania
};

#endif //TASK_STRUCT_H
