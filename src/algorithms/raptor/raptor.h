// SPDX-FileCopyrightText: © 2024 Adrian C. Prelipcean
//
// SPDX-License-Identifier: EUPL-1.2

#ifndef RAPTOR_H
#define RAPTOR_H

extern "C"
{
#include "postgres.h"
#include <fmgr.h>
}
#include <utils/builtins.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include "src/models/network.h"
#include "src/models/solution.h"

/**
 * @struct RoundData
 * @brief Contains data related to stops and arrival times for a round.
 *
 * This struct holds information about stops and their associated data,
 * including arrival times, previous stops, trip IDs, and the number of transfers.
 */
struct RoundData
{
    /**
     * @var stops
     * @brief Maps stop identifiers to detailed stop information.
     *
     * This map associates each stop identifier (as a string) with a tuple containing:
     * - Arrival time (double)
     * - Previous stop identifier (string)
     * - Trip ID (string)
     * - Number of transfers (int)
     */
    std::unordered_map<std::string, std::tuple<double, std::string, std::string, int>> stops;

    /**
     * @var arrival_times
     * @brief Maps stop identifiers to their arrival times.
     *
     * This map associates each stop identifier (as a string) with its corresponding
     * arrival time (double).
     */
    std::unordered_map<std::string, double> arrival_times;
};

/**
 * @brief Performs the RAPTOR algorithm to find the best route.
 *
 * This function executes the RAPTOR algorithm to find connections
 * between the specified origin and destination at the given departure time.
 *
 * @param origin The origin stop ID.
 * @param destination The destination stop ID.
 * @param departure_time The departure time.
 * @param network Pointer to the network of stops.
 * @param network_size The size of the network.
 * @param max_rounds The limit on the number of rounds to be performed by RAPTOR.
 * @return A vector of SolutionRAPTOR structures representing the solutions found.
 *
 */
std::vector<SolutionAlg> perform_RAPTOR(const char *origin, const char *destination, float8 departure_time, NetworkRow *network, int64_t network_size, int max_rounds);

/**
 * @brief Preprocesses the timetable for the RAPTOR algorithm.
 *
 * This function preprocesses the timetable data to optimize the performance of the RAPTOR algorithm.
 *
 * @param network The network data.
 * @param network_size The size of the network data.
 * @param departures A reference to an unordered map that will hold the preprocessed departure data.
 */
void preprocess_timetable(NetworkRow *network, int64_t network_size, std::unordered_map<std::string, std::vector<NetworkRow>> &departures);

#endif /* RAPTOR_H */