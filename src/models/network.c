// SPDX-FileCopyrightText: © 2024 Adrian C. Prelipcean
//
// SPDX-License-Identifier: EUPL-1.2

#include "network.h"
#include <postgres.h>
#include <fmgr.h>
#include <executor/spi.h>
#include <utils/builtins.h>
#include <catalog/pg_type.h>
#include <utils/array.h> 
#include <utils/typcache.h>
#include <utils/lsyscache.h> 

NetworkRow *create_network(const char *network_query_str, int64_t *network_size)
{
    int ret = SPI_connect();
    if (ret != SPI_OK_CONNECT)
    {
        elog(ERROR, "Failed to connect to SPI: %d", ret);
        return NULL;
    }

    ret = SPI_execute(network_query_str, true, 0);

    if (ret != SPI_OK_SELECT)
    {
        elog(ERROR, "Query execution failed: %d", ret);
        SPI_finish();
        return NULL;
    }

    int num_rows = SPI_processed;
    if (num_rows <= 0)
    {
        elog(INFO, "No data found in network query result.");
        SPI_finish();
        return NULL;
    }

    // Allocate memory for the array of NetworkRow structs
    NetworkRow *network_rows = (NetworkRow *)SPI_palloc(num_rows * sizeof(NetworkRow));
    if (network_rows == NULL)
    {
        elog(ERROR, "Failed to allocate memory for network_rows.");
        SPI_finish();
        return NULL;
    }

    int prev_stop_sequence = -1;

    // Retrieve data from SPI results
    for (int i = 0; i < num_rows; i++)
    {
        HeapTuple tuple = SPI_tuptable->vals[i];
        TupleDesc tupdesc = SPI_tuptable->tupdesc;
        memset(network_rows[i].nulls, false, sizeof(network_rows[i].nulls));

        text *trip_id_text = DatumGetTextP(SPI_getbinval(tuple, tupdesc, 1, &network_rows[i].nulls[0]));
        text *stop_id_text = DatumGetTextP(SPI_getbinval(tuple, tupdesc, 2, &network_rows[i].nulls[1]));
        double arrival_time = DatumGetFloat8(SPI_getbinval(tuple, tupdesc, 3, &network_rows[i].nulls[2]));
        double departure_time = DatumGetFloat8(SPI_getbinval(tuple, tupdesc, 4, &network_rows[i].nulls[3]));
        int stop_sequence = DatumGetInt32(SPI_getbinval(tuple, tupdesc, 5, &network_rows[i].nulls[4]));
        // Copy trip_id directly
        strncpy(network_rows[i].trip_id, text_to_cstring(trip_id_text), MAX_STRING_LENGTH - 1);
        network_rows[i].trip_id[MAX_STRING_LENGTH - 1] = '\0'; // Ensure null-terminated

        strncpy(network_rows[i].from_stop_id, text_to_cstring(stop_id_text), MAX_STRING_LENGTH - 1);
        network_rows[i].from_stop_id[MAX_STRING_LENGTH - 1] = '\0'; // Ensure null-terminated
        network_rows[i].arrival_time = arrival_time;
        network_rows[i].departure_time = departure_time;
        network_rows[i].travel_time = 0; // Initialize travel_time
        // Assign stop_id based on stop_sequence within the same trip
        if (i > 0 && strcmp(network_rows[i].trip_id, network_rows[i - 1].trip_id) == 0)
        {
            // Within the same trip

            if (stop_sequence == prev_stop_sequence + 1)
            {
                // This is the next stop in sequence
                strncpy(network_rows[i - 1].to_stop_id, text_to_cstring(stop_id_text), MAX_STRING_LENGTH - 1);
                network_rows[i - 1].to_stop_id[MAX_STRING_LENGTH - 1] = '\0'; // Ensure null-terminated
                network_rows[i - 1].travel_time = arrival_time - network_rows[i - 1].departure_time;
            }
            else
            {
                // This is the new start of a sequence
                network_rows[i].travel_time = 0; // Initialize travel_time
            }
        }
        prev_stop_sequence = stop_sequence;
    }

    SPI_finish();

    *network_size = num_rows;

    return network_rows;
}

void print_network_rows(NetworkRow *network_rows, int64_t network_size)
{
    elog(INFO, "Printing network rows:");
    for (int i = 0; i < network_size; i++)
    {
        elog(INFO, "Row %d:", i + 1);
        elog(INFO, "  Trip ID: %s", network_rows[i].trip_id);
        elog(INFO, "  From Stop ID: %s", network_rows[i].from_stop_id);
        elog(INFO, "  To Stop ID: %s", network_rows[i].to_stop_id);
        elog(INFO, "  Arrival Time: %.2f", network_rows[i].arrival_time);
        elog(INFO, "  Departure Time: %.2f", network_rows[i].departure_time);
        elog(INFO, "  Travel Time: %.2f", network_rows[i].travel_time);
    }
}
