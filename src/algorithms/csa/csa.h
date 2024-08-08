// SPDX-FileCopyrightText: © 2024 Adrian C. Prelipcean
//
// SPDX-License-Identifier: EUPL-1.2

/**
 * @file csa.h
 * @brief Declarations for the CSA (Connection Scan Algorithm) module.
 */

#ifndef CSA_H
#define CSA_H

extern "C" {
#include "postgres.h"
}
#include <vector>
#include <ctime>
#include <string>
#include "src/models/network.h"
#include "src/models/solution.h"

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
std::vector<SolutionAlg> perform_CSA(const char *origin, const char *destination, float8 departure_time, NetworkRow *network, int64_t network_size);

/**
 * @brief Performs the Connection Scan Algorithm and minimizes transfers when creating route from solution.
 * 
 * This function performs the Connection Scan Algorithm (CSA) to find connections
 * between the specified origin and destination at the given departure time.
 * It keeps an array of trip_ids that are relevant for each stop and arrival time pair
 * that is then used to stay on the same trip_id as the previous stop when multiple trip_ids are relevant. 
 * This does not minimize number of transfers as a tradeoff with arrival time, it only 
 * ensures that the final route does not randomly change between trip_ids when maintaining 
 * the same trip_id is possible.
 * 
 * @param origin The origin stop ID.
 * @param destination The destination stop ID.
 * @param departure_time The departure time.
 * @param network Pointer to the network of stops.
 * @param network_size The size of the network.
 * @return A vector of SolutionCSA structures representing the solutions found.
 */
std::vector<SolutionAlg> perform_CSA_Minimize_Transfers(const char *origin, const char *destination, float8 departure_time, NetworkRow *network, int64_t network_size);

#endif /* CSA_H */