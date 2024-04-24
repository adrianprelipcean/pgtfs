// SPDX-FileCopyrightText: © 2024 Adrian C. Prelipcean
//
// SPDX-License-Identifier: EUPL-1.2

/**
 * @file csa.h
 * @brief Declarations for the CSA (Connection Scan Algorithm) module.
 */

#ifndef CSA_H
#define CSA_H

#include <vector>
#include <ctime>
#include <string>
#include "postgres.h"
#include "src/models/network.h"

/**
 * @struct SolutionCSA
 * @brief Structure representing a solution in the Connection Scan Algorithm.
 */
struct SolutionCSA
{
    std::string stop_id; /**< Stop ID */
    int stop_sequence; /**< Stop sequence */
    time_t arrival_time; /**< Arrival time */
    std::string trip_id; /**< Trip ID */
};

/**
 * @brief Performs the Connection Scan Algorithm.
 * 
 * This function performs the Connection Scan Algorithm (CSA) to find connections
 * between the specified origin and destination at the given departure time.
 * 
 * @param origin The origin stop ID.
 * @param destination The destination stop ID.
 * @param departure_time The departure time.
 * @param network Pointer to the network of stops.
 * @param network_size The size of the network.
 * @return A vector of SolutionCSA structures representing the solutions found.
 */
std::vector<SolutionCSA> perform_CSA(const char *origin, const char *destination, float8 departure_time, NetworkRow *network, int64_t network_size);

#endif /* CSA_H */