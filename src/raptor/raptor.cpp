// SPDX-FileCopyrightText: © 2024 Adrian C. Prelipcean
//
// SPDX-License-Identifier: EUPL-1.2

#include "raptor.h"
#include <queue>
#include <unordered_map>
#include <tuple>
#include <algorithm>
#include <cstring>

const double INF = std::numeric_limits<double>::infinity();

std::vector<SolutionRAPTOR> perform_RAPTOR(const char *origin, const char *destination, float8 departure_time, NetworkRow *network, int64_t network_size, int max_rounds)
{
    std::unordered_map<std::string, std::vector<NetworkRow>> departures;
    preprocess_timetable(network, network_size, departures);

    std::vector<RoundData> rounds(max_rounds);

    rounds[0].arrival_times[origin] = departure_time;
    rounds[0].stops[origin] = {departure_time, "", "", 0};

    for (int round = 1; round < max_rounds; ++round)
    {
        RoundData &current_round = rounds[round];
        RoundData &previous_round = rounds[round - 1];

        current_round.stops = previous_round.stops;
        current_round.arrival_times = previous_round.arrival_times;

        // For each neighbor of the nodes analyzed in the previous round 
        // evaluate the effect of the next transfers
        for (const auto &entry : previous_round.stops)
        {
            const std::string &current_node = entry.first;
            const auto &current_info = entry.second;
            double current_time = std::get<0>(current_info);
            int current_transfers = std::get<3>(current_info);
            std::string current_trip_id = std::get<2>(current_info);

            if (departures.find(current_node) != departures.end())
            {
                for (const auto &departure : departures[current_node])
                {
                    if (departure.departure_time >= current_time)
                    {
                        double arrival_time = departure.departure_time + departure.travel_time;
                        std::string to_stop_id = departure.to_stop_id;
                        std::string trip_id = departure.trip_id;

                        int new_transfers = (departure.trip_id == current_trip_id) ? current_transfers : current_transfers + 1;

                        // Check for improvements in either arrival time, or number of transfers
                        if (current_round.stops.find(to_stop_id) == current_round.stops.end() ||
                            (arrival_time < std::get<0>(current_round.stops[to_stop_id]) ||
                             (arrival_time == std::get<0>(current_round.stops[to_stop_id]) && new_transfers < std::get<3>(current_round.stops[to_stop_id]))))
                        {
                            current_round.stops[to_stop_id] = {arrival_time, current_node, trip_id, new_transfers};
                            current_round.arrival_times[to_stop_id] = arrival_time;

                            auto trip_departures = departures.find(to_stop_id);
                            std::string prev_neighbor = to_stop_id;
                            // Traverse the entire trip
                            while (trip_departures != departures.end() && !trip_departures->second.empty() && trip_departures->second[0].trip_id == trip_id)
                            {
                                double stop_departure_time = trip_departures->second[0].departure_time;
                                arrival_time = stop_departure_time + trip_departures->second[0].travel_time;
                                std::string next_stop_id = trip_departures->second[0].to_stop_id;
                                trip_departures->second.erase(trip_departures->second.begin());

                                // Check for improvements in either arrival time, or number of transfers
                                if (stop_departure_time > 0 && (current_round.stops.find(next_stop_id) == current_round.stops.end() ||
                                                                (arrival_time < std::get<0>(current_round.stops[next_stop_id]) ||
                                                                 (arrival_time == std::get<0>(current_round.stops[next_stop_id]) && new_transfers < std::get<3>(current_round.stops[next_stop_id])))))
                                {
                                    current_round.stops[next_stop_id] = {arrival_time, prev_neighbor, trip_id, new_transfers};
                                    current_round.arrival_times[next_stop_id] = arrival_time;
                                }

                                trip_departures = departures.find(next_stop_id);
                                prev_neighbor = next_stop_id;
                            }
                        }
                    }
                }
            }
        }

        // Check if the destination is reached in the current round
        if (current_round.stops.find(destination) != current_round.stops.end())
        {
            break;
        }
    }

    std::vector<SolutionRAPTOR> route;
    double best_arrival_time = INF;
    int best_round = -1;
    std::unordered_map<std::string, std::tuple<double, std::string, std::string, int>> best_stops;

    // Find the best round with the optimal arrival time and fewest transfers
    for (int round = 0; round < max_rounds; ++round)
    {
        if (rounds[round].stops.find(destination) != rounds[round].stops.end())
        {
            double arrival_time = std::get<0>(rounds[round].stops[destination]);
            int transfers = std::get<3>(rounds[round].stops[destination]);
            if (arrival_time < best_arrival_time ||
                (arrival_time == best_arrival_time && transfers < std::get<3>(best_stops[destination])))
            {
                best_arrival_time = arrival_time;
                best_round = round;
                best_stops = rounds[round].stops;
            }
        }
    }

    if (best_round != -1)
    {
        std::string node = destination;
        std::string last_trip_id = "";
        // Backtrack to build the sequence of stops from destination to origin
        while (node != origin)
        {
            const auto &stop_info = best_stops[node];
            double arrival_time = std::get<0>(stop_info);
            std::string prev_node = std::get<1>(stop_info);
            std::string trip_id = std::get<2>(stop_info);
            route.push_back({node, 0, arrival_time, trip_id});
            node = prev_node;
            last_trip_id = trip_id;
        }
        route.push_back({origin, 0, departure_time, last_trip_id});
        std::reverse(route.begin(), route.end());
        for (size_t i = 0; i < route.size(); ++i)
        {
            route[i].stop_sequence = static_cast<int>(i);
        }
        return route;
    }

    return {};
}

void preprocess_timetable(NetworkRow *network, int64_t network_size, std::unordered_map<std::string, std::vector<NetworkRow>> &departures)
{
    for (int i = 0; i < network_size; ++i)
    {
        std::string from_stop_id = network[i].from_stop_id;
        std::string trip_id = network[i].trip_id;

        departures[from_stop_id].push_back(network[i]);
    }

    for (auto it = departures.begin(); it != departures.end(); ++it)
    {
        std::vector<NetworkRow> &dep_list = it->second;
        std::sort(dep_list.begin(), dep_list.end(), [](const NetworkRow &a, const NetworkRow &b)
                  { return a.departure_time < b.departure_time; });
    }
}
