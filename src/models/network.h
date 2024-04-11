// SPDX-FileCopyrightText: © 2024 Adrian C. Prelipcean
//
// SPDX-License-Identifier: EUPL-1.2

#ifndef NETWORK_H
#define NETWORK_H

#include <postgres.h>
#include <fmgr.h>
#include <executor/spi.h>
#include <utils/builtins.h>
#include <catalog/pg_type.h>
#include <utils/array.h> 
#include <utils/typcache.h>  
#include <utils/lsyscache.h> 

#define MAX_STRING_LENGTH 256

typedef struct {
    char trip_id[MAX_STRING_LENGTH];
    char from_stop_id[MAX_STRING_LENGTH];
    char to_stop_id[MAX_STRING_LENGTH];
    float8 arrival_time;
    float8 departure_time;
    float8 travel_time;
    bool nulls[6]; 
} NetworkRow;

NetworkRow *create_network(const char *network_query_str, int64_t *network_size);

void print_network_rows(NetworkRow* network_rows, int64_t network_size);
#endif /* NETWORK_H */