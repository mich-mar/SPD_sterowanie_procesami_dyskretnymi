import random

def generate_simple_instance(num_jobs, time_range, filename):
    with open(filename, 'w') as f:
        for job_id in range(1, num_jobs + 1):
            p_time = random.randint(*time_range)
            f.write(f"{job_id} {p_time}\n")

# Parametry
num_jobs = 30

# Generacja 3 plików
generate_simple_instance(num_jobs, (1, 5), "data/instance_1_5.txt")
generate_simple_instance(num_jobs, (1, 15), "data/instance_1_15.txt")
generate_simple_instance(num_jobs, (1, 100), "data/instance_1_100.txt")
