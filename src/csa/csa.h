// SPDX-FileCopyrightText: © 2024 Adrian C. Prelipcean
//
// SPDX-License-Identifier: EUPL-1.2

#ifndef CSA_H
#define CSA_H

#include <vector>
#include <ctime>
#include <string>
#include "postgres.h"
#include "src/models/network.h"

struct SolutionCSA
{
    std::string stop_id;
    int stop_sequence;
    time_t arrival_time;
    std::string trip_id;
};

std::vector<SolutionCSA> perform_CSA(const char *origin, const char *destination, float8 departure_time, NetworkRow *network, int64_t network_size);

#endif /* CSA_H */