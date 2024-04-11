// SPDX-FileCopyrightText: © 2024 Adrian C. Prelipcean
//
// SPDX-License-Identifier: EUPL-1.2

#include "csa.h"

#include <queue>
#include <unordered_map>
#include <tuple>
#include <algorithm>
#include <cstring>

std::vector<SolutionCSA> perform_CSA(const char *origin, const char *destination, float8 departure_time, NetworkRow *network, int64_t network_size)
{
    time_t departure_epoch = (time_t)departure_time;

    int num_rows = network_size;
    std::vector<SolutionCSA> stops;

    std::priority_queue<std::tuple<time_t, std::string, std::vector<std::tuple<std::string, std::string, time_t>>>,
                        std::vector<std::tuple<time_t, std::string, std::vector<std::tuple<std::string, std::string, time_t>>>>,
                        std::greater<std::tuple<time_t, std::string, std::vector<std::tuple<std::string, std::string, time_t>>>>>
        pq;

    std::unordered_map<std::string, time_t> shortest_times;

    shortest_times[std::string(origin)] = departure_epoch;
    pq.push({departure_epoch, std::string(origin), {{std::string(origin), "", departure_epoch}}});

    while (!pq.empty())
    {
        auto tuple = pq.top();
        auto current_time = std::get<0>(tuple);
        auto current_node = std::get<1>(tuple);
        auto route = std::get<2>(tuple);
        pq.pop();

        if (current_node == destination)
        {
            int stop_sequence = 0;
            for (const auto &step : route)
            {
                SolutionCSA stop;
                stop.stop_id = std::get<0>(step);
                stop.stop_sequence = stop_sequence;
                stop.arrival_time = std::get<2>(step);
                stop.trip_id = std::get<1>(step);
                stops.push_back(stop);
                stop_sequence++;
            }
            return stops;
        }

        for (int i = 0; i < num_rows; ++i)
        {
            if (current_node == network[i].from_stop_id)
            {
                if (current_time <= (time_t)network[i].departure_time)
                {
                    time_t neighbor_arrival_time = (time_t)network[i].departure_time + (time_t)network[i].travel_time;

                    if (shortest_times.find(network[i].to_stop_id) == shortest_times.end() || neighbor_arrival_time < shortest_times[network[i].to_stop_id])
                    {
                        shortest_times[network[i].to_stop_id] = neighbor_arrival_time;
                        std::vector<std::tuple<std::string, std::string, time_t>> new_route = route;
                        new_route.push_back(std::make_tuple(network[i].to_stop_id, network[i].trip_id, neighbor_arrival_time));
                        pq.push({neighbor_arrival_time, network[i].to_stop_id, new_route});
                    }
                }
            }
        }
    }

    return {};
}