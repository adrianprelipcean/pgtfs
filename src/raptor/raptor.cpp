// SPDX-FileCopyrightText: © 2024 Adrian C. Prelipcean
//
// SPDX-License-Identifier: EUPL-1.2

#include "raptor.h"
#include <queue>
#include <unordered_map>
#include <tuple>
#include <algorithm>
#include <cstring>

std::vector<SolutionRAPTOR> perform_RAPTOR(const char *origin, const char *destination, float8 departure_time, NetworkRow *network, int64_t network_size)
{
    time_t departure_epoch = (time_t)departure_time;

    int num_rows = network_size;
    std::vector<SolutionRAPTOR> stops;

    std::unordered_map<std::string, std::vector<NetworkRow>> departures;
    std::priority_queue<std::tuple<time_t, std::string, std::string, int>, std::vector<std::tuple<time_t, std::string, std::string, int>>, std::greater<std::tuple<time_t, std::string, std::string, int>>> pq;
    std::unordered_map<std::string, std::tuple<time_t, std::string, std::string>> previous_stop;

    preprocess_timetable(network, num_rows, departures);

    previous_stop[origin] = {departure_epoch, "", ""};
    pq.push({departure_epoch, origin, "", 0});

    while (!pq.empty())
    {
        auto [current_time, current_node, prev_trip_id, current_stop_sequence] = pq.top();
        pq.pop();

        if (current_node == destination)
        {
            int num_stops = 0;
            std::string node = destination;

            while (node != origin)
            {
                ++num_stops;
                auto [arrival_time, prev_node, trip_id] = previous_stop[node];
                node = prev_node;
            }

            std::vector<SolutionRAPTOR> route;
            node = destination;
            int stop_sequence = num_stops;
            while (node != origin)
            {
                auto [arrival_time, prev_node, trip_id] = previous_stop[node];
                route.push_back({node, stop_sequence--, arrival_time, trip_id});
                node = prev_node;
            }
            route.push_back({origin, 0, departure_epoch, ""});
            std::reverse(route.begin(), route.end());
            return route;
        }

        for (const auto &departure : departures[current_node])
        {
            if (departure.departure_time < current_time)
                continue;

            time_t arrival_time = (time_t)departure.departure_time + (time_t)departure.travel_time;

            if (previous_stop.find(departure.to_stop_id) == previous_stop.end() || arrival_time < std::get<0>(previous_stop[departure.to_stop_id]))
            {
                previous_stop[departure.to_stop_id] = {arrival_time, current_node, departure.trip_id};
                pq.push({arrival_time, departure.to_stop_id, departure.trip_id, current_stop_sequence});
            }
        }
    }

    return {};
}

void preprocess_timetable(NetworkRow *network, int64_t network_size, std::unordered_map<std::string, std::vector<NetworkRow>> &departures)
{
    for (int i = 0; i < network_size; ++i)
    {
        std::string from_stop_id = network[i].from_stop_id;
        std::string trip_id = network[i].trip_id;
        time_t departure_time = (time_t)network[i].departure_time;
        time_t arrival_time = departure_time + (time_t)network[i].travel_time;

        departures[from_stop_id].push_back(network[i]);
    }

    for (auto &[from_stop_id, dep_list] : departures)
    {
        std::sort(dep_list.begin(), dep_list.end(), [](const NetworkRow &a, const NetworkRow &b)
                  { return a.departure_time < b.departure_time; });
    }
}