#ifndef ALG_01_HEURISTIC_H
#define ALG_01_HEURISTIC_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <climits>
#include <queue>

#include "task_struct.h"

int schragePlaning(std::vector<task> tasks);

int schragePreemptivePlaning(std::vector<task> tasks);

#endif //ALG_01_HEURISTIC_H
