#include "alg_03_wspt.h"

/**
 * @brief Weighted Shortest Processing Time (WSPT) algorithm
 * 
 * This is a variation of SPT that takes into account both processing time
 * and delivery time to prioritize tasks. It calculates a priority value
 * based on a weighted combination of p and q.
 * 
 * @param tasks Vector of tasks to be scheduled
 * @return Maximum completion time (Cmax)
 */
int weightedSPTPlaning(std::vector<task>& tasks) {
    if (tasks.empty()) return 0;
    
    // Sort tasks by a weighted priority formula
    std::sort(tasks.begin(), tasks.end(), [](const task& a, const task& b) {
        // Lower value = higher priority
        // This formula prioritizes tasks with low p and high q
        return (a.p - 0.5 * a.q) < (b.p - 0.5 * b.q);
    });
    
    int currentTime = 0;
    int cmax = 0;
    
    // Schedule tasks according to priority
    for (const auto& t : tasks) {
        currentTime = std::max(currentTime, t.r);
        currentTime += t.p;
        int completionTime = currentTime + t.q;
        cmax = std::max(cmax, completionTime);
    }
    
    return cmax;
}
