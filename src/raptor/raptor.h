// SPDX-FileCopyrightText: © 2024 Adrian C. Prelipcean
//
// SPDX-License-Identifier: EUPL-1.2

#ifndef RAPTOR_H
#define RAPTOR_H

#include <postgres.h>
#include <fmgr.h>
#include <utils/builtins.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include "../models/network.h"

struct SolutionRAPTOR {
    std::string stop_id;
    int stop_sequence;
    time_t arrival_time;
    std::string trip_id;
};

// Function declarations
std::vector<SolutionRAPTOR> perform_RAPTOR(const char *origin, const char *destination, float8 departure_time, NetworkRow *network, int64_t network_size);
void preprocess_timetable(NetworkRow *network, int64_t network_size, std::unordered_map<std::string, std::vector<NetworkRow>>& departures);

#endif // RAPTOR_H