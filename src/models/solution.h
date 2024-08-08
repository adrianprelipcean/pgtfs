#ifndef SOLUTION_H
#define SOLUTION_H

#include <string>


/**
 * @struct Solution
 * @brief Structure representing a solution for the public transportation algorithms.
 */
struct SolutionAlg
{
    std::string stop_id; /**< Stop ID */
    int stop_sequence;   /**< Stop sequence */
    double arrival_time; /**< Arrival time */
    std::string trip_id; /**< Trip ID */
};

#endif /* SOLUTION_H */