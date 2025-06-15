import numpy as np
import pandas as pd
import os

def generate_flowshop_instance(n_jobs, n_machines, min_time=1, max_time=99):
    # Generowanie czasów wykonywania dla każdego zadania na każdej maszynie
    processing_times = np.random.randint(min_time, max_time + 1, size=(n_jobs, n_machines))

    # Tworzenie DataFrame
    df = pd.DataFrame(processing_times,
                      columns=[f'Machine_{i+1}' for i in range(n_machines)],
                      index=[f'Job_{i+1}' for i in range(n_jobs)])
    return df

def save_instances(sizes=[10, 25, 100], n_machines=3, output_dir="instances"):
    # Tworzenie katalogu jeśli nie istnieje
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    # Generowanie instancji dla każdego rozmiaru
    for size in sizes:
        instance = generate_flowshop_instance(size, n_machines)

        # Zapisywanie do pliku CSV
        filename = os.path.join(output_dir, f'flowshop_n{size}_m{n_machines}.csv')
        instance.to_csv(filename)
        print(f"Wygenerowano plik: {filename}")

        # Wyświetlenie pierwszych kilku wierszy dla sprawdzenia
        print(f"\nPierwsze 5 wierszy dla instancji z {size} zadaniami:")
        print(instance.head())
        print("\n" + "="*50 + "\n")

if __name__ == "__main__":
    # Ustawienie ziarna dla powtarzalności wyników
    np.random.seed(42)

    # Generowanie instancji
    save_instances(sizes=[10, 25, 100], n_machines=3)