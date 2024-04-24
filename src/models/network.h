// SPDX-FileCopyrightText: © 2024 Adrian C. Prelipcean
//
// SPDX-License-Identifier: EUPL-1.2

/**
 * @file network.h
 * @brief Declarations for functions related to network operations.
 */


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

/**
 * @struct NetworkRow
 * @brief Structure representing a row in a network.
 */
typedef struct {
    char trip_id[MAX_STRING_LENGTH];  /**< Trip ID */
    char from_stop_id[MAX_STRING_LENGTH]; /**< From stop ID */
    char to_stop_id[MAX_STRING_LENGTH]; /**< To stop ID */
    float8 arrival_time; /**< Arrival time as epoch*/
    float8 departure_time; /**< Departure time as epoch*/
    float8 travel_time; /**< Travel time */
    bool nulls[6];  /**< Array of boolean flags indicating null values */
} NetworkRow;

/**
 * @brief Creates a network from the given query string.
 * 
 * This function creates a network based on the provided query string
 * and returns a pointer to the network.
 * 
 * @param network_query_str The query string specifying the network.
 * @param network_size Pointer to store the size of the network.
 * @return Pointer to the created network.
 */
NetworkRow *create_network(const char *network_query_str, int64_t *network_size);

/**
 * @brief Prints the network rows.
 * 
 * This function prints the given network rows along with their details.
 * 
 * @param network_rows Pointer to the network rows array.
 * @param network_size The size of the network.
 */
void print_network_rows(NetworkRow* network_rows, int64_t network_size);
#endif /* NETWORK_H */