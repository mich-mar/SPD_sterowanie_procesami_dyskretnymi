#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "task.h"
#include <vector>
#include <numeric>
#include <bitset>
#include <iostream>
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <climits>
#include <cmath>

#define MACHINE_NUM 2

void print_schedule(const std::vector<std::vector<task>>& machine_tasks);

std::vector<std::vector<task>> schedule_tasks_lsa(const std::vector<task>& tasks);
std::vector<std::vector<task>> schedule_tasks_lpt(const std::vector<task>& tasks);
std::vector<std::vector<task>> schedule_tasks_bruteforce(const std::vector<task> &tasks);
std::vector<std::vector<task>> schedule_tasks_dp(const std::vector<task> &tasks);
std::vector<std::vector<task>> schedule_tasks_fptas(const std::vector<task> &tasks, double epsilon);
std::vector<std::vector<task>> schedule_tasks_ptas(const std::vector<task> &tasks, double epsilon);

#endif