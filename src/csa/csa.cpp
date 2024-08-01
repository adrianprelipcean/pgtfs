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
    double departure_epoch = departure_time;

    int num_rows = network_size;
    std::vector<SolutionCSA> stops;

    std::priority_queue<std::tuple<double, std::string, std::vector<std::tuple<std::string, std::string, double>>>,
                        std::vector<std::tuple<double, std::string, std::vector<std::tuple<std::string, std::string, double>>>>,
                        std::greater<std::tuple<double, std::string, std::vector<std::tuple<std::string, std::string, double>>>>>
        pq;

    std::unordered_map<std::string, double> shortest_times;

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
                stop.trip_id = (stop.stop_id == origin) ? std::get<1>(route[1]) : std::get<1>(step);
                stops.push_back(stop);
                stop_sequence++;
            }
            return stops;
        }

        for (int i = 0; i < num_rows; ++i)
        {
            if (current_node == network[i].from_stop_id)
            {
                if (current_time <= network[i].departure_time)
                {
                    double neighbor_arrival_time = network[i].departure_time + network[i].travel_time;

                    if (shortest_times.find(network[i].to_stop_id) == shortest_times.end() || neighbor_arrival_time < shortest_times[network[i].to_stop_id])
                    {
                        shortest_times[network[i].to_stop_id] = neighbor_arrival_time;
                        std::vector<std::tuple<std::string, std::string, double>> new_route = route;
                        new_route.push_back(std::make_tuple(network[i].to_stop_id, network[i].trip_id, neighbor_arrival_time));
                        pq.push({neighbor_arrival_time, network[i].to_stop_id, new_route});
                    }
                }
            }
        }
    }

    return {};
}

std::vector<SolutionCSA> perform_CSA_Minimize_Transfers(const char *origin, const char *destination, float8 departure_time, NetworkRow *network, int64_t network_size)
{
    double departure_epoch = departure_time;

    int num_rows = network_size;
    std::vector<SolutionCSA> stops;

    std::priority_queue<std::tuple<double, std::string, std::vector<std::tuple<std::string, std::string, double>>>,
                        std::vector<std::tuple<double, std::string, std::vector<std::tuple<std::string, std::string, double>>>>,
                        std::greater<std::tuple<double, std::string, std::vector<std::tuple<std::string, std::string, double>>>>>
        pq;

    std::unordered_map<std::string, double> shortest_times;
    // array of trip_ids relevant for each stop at the earliest arrival time
    std::unordered_map<std::string, std::unordered_map<double, std::vector<std::string>>> stop_arrival_map;

    shortest_times[std::string(origin)] = departure_epoch;
    stop_arrival_map[std::string(origin)][departure_epoch] = {};
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
            std::string prev_trip_id = "";
            for (auto it = route.rbegin(); it != route.rend(); ++it)
            {
                const auto &step = *it;
                SolutionCSA stop;
                stop.stop_id = std::get<0>(step);
                stop.arrival_time = std::get<2>(step);
                stop_sequence++;
                stop.stop_sequence = route.size() - stop_sequence;
                stop.trip_id = (stop.stop_id != origin) ? std::get<1>(step) : prev_trip_id;
                auto stop_it = stop_arrival_map.find(stop.stop_id);

                if (stop_it != stop_arrival_map.end())
                {
                    auto &arrival_map = stop_it->second;
                    auto arrival_it = arrival_map.find(stop.arrival_time);
                    if (arrival_it != arrival_map.end())
                    {
                        auto &trip_ids = arrival_it->second;
                        if (prev_trip_id != "")
                        {
                            if (std::find(trip_ids.begin(), trip_ids.end(), prev_trip_id) != trip_ids.end())
                            {
                                stop.trip_id = prev_trip_id;
                            }
                        }
                        prev_trip_id = stop.trip_id;
                    }
                }
                stops.push_back(stop);
            }
            std::reverse(stops.begin(), stops.end());

            return stops;
        }

        for (int i = 0; i < num_rows; ++i)
        {
            if (current_node == network[i].from_stop_id)
            {
                if (current_time <= network[i].departure_time)
                {
                    double neighbor_arrival_time = network[i].departure_time + network[i].travel_time;

                    if (shortest_times.find(network[i].to_stop_id) == shortest_times.end() || neighbor_arrival_time < shortest_times[network[i].to_stop_id])
                    {
                        shortest_times[network[i].to_stop_id] = neighbor_arrival_time;
                        stop_arrival_map[network[i].to_stop_id][neighbor_arrival_time] = {network[i].trip_id};
                        std::vector<std::tuple<std::string, std::string, double>> new_route = route;
                        new_route.push_back(std::make_tuple(network[i].to_stop_id, network[i].trip_id, neighbor_arrival_time));
                        pq.push({neighbor_arrival_time, network[i].to_stop_id, new_route});
                    }
                    else if (neighbor_arrival_time == shortest_times[network[i].to_stop_id])
                    {
                        stop_arrival_map[network[i].to_stop_id][neighbor_arrival_time].push_back(network[i].trip_id);
                    }
                }
            }
        }
    }

    return {};
}
