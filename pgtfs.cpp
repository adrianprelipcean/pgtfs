// SPDX-FileCopyrightText: © 2024 Adrian C. Prelipcean
//
// SPDX-License-Identifier: EUPL-1.2

/**
 * @file pgtfs.cpp
 * @brief Implementation of PostgreSQL extension functions.
 */

extern "C"
{
#include <postgres.h>
#include <fmgr.h>
#include <utils/builtins.h>
#include <funcapi.h>
#include "src/models/network.h"
}

#include "src/csa/csa.h"
#include "src/raptor/raptor.h"

extern "C"
{
    PG_MODULE_MAGIC;
    static const char *EXTENSION_VERSION = "0.0.3";

    PG_FUNCTION_INFO_V1(pgtfs_csa);
    /**
     * @brief Implements the pgtfs_csa PostgreSQL extension function.
     *
     * This function performs the Connection Scan Algorithm (CSA) to find connections
     * between the specified origin and destination at the given departure time.
     *
     * @param fcinfo Function call information.
     * @return A set of rows representing the solutions found.
     */
    Datum pgtfs_csa(PG_FUNCTION_ARGS)
    {
        FuncCallContext *funcctx;
        MemoryContext oldcontext;

        if (SRF_IS_FIRSTCALL())
        {
            funcctx = SRF_FIRSTCALL_INIT();

            // Switch to multi-call memory context
            oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

            // Create a tuple descriptor for the returned rows
            TupleDesc tupdesc = CreateTemplateTupleDesc(4);
            TupleDescInitEntry(tupdesc, (AttrNumber)1, "stop_id", TEXTOID, -1, 0);
            TupleDescInitEntry(tupdesc, (AttrNumber)2, "stop_sequence", INT4OID, -1, 0);
            TupleDescInitEntry(tupdesc, (AttrNumber)3, "arrival_time", FLOAT8OID, -1, 0);
            TupleDescInitEntry(tupdesc, (AttrNumber)4, "trip_id", TEXTOID, -1, 0);

            // Set the tuple descriptor in the function call context
            funcctx->tuple_desc = BlessTupleDesc(tupdesc);

            text *origin = PG_GETARG_TEXT_PP(0);
            text *destination = PG_GETARG_TEXT_PP(1);
            float8 departure_time = PG_GETARG_FLOAT8(2);
            text *network_query_text = PG_GETARG_TEXT_PP(3);
            bool minimize_transfers = PG_GETARG_BOOL(4);

            if (departure_time < 0)
                ereport(ERROR,
                        (
                            errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE),
                            errmsg("departure_time needs to be valid")));

            char *network_query_str = text_to_cstring(network_query_text);
            int64_t network_size;

            NetworkRow *network = create_network(network_query_str, &network_size);
            if (network == NULL)
            {
                elog(ERROR, "Failed to create the network.");
                PG_RETURN_NULL();
            }

            std::vector<SolutionCSA> solution;

            if (minimize_transfers)
                solution = perform_CSA_Minimize_Transfers(text_to_cstring(origin), text_to_cstring(destination), departure_time, network, network_size);
            else
                solution = perform_CSA(text_to_cstring(origin), text_to_cstring(destination), departure_time, network, network_size);

            SPI_pfree(network);
            pfree(destination);
            pfree(network_query_text);
            pfree(origin);

            if (solution.empty())
            {
                PG_RETURN_NULL();
            }

            funcctx->max_calls = solution.size();
            std::vector<SolutionCSA> *solution_vector = new std::vector<SolutionCSA>(solution);
            funcctx->user_fctx = reinterpret_cast<void *>(solution_vector);
            funcctx->call_cntr = 0;

            // Switch back to original memory context
            MemoryContextSwitchTo(oldcontext);
        }

        // Subsequent calls reuse the existing function call context
        funcctx = SRF_PERCALL_SETUP();
        int call_cntr = funcctx->call_cntr;
        int max_calls = funcctx->max_calls;

        if (call_cntr < max_calls)
        {
            Datum values[4];

            std::vector<SolutionCSA> *solution_vector = reinterpret_cast<std::vector<SolutionCSA> *>(funcctx->user_fctx);

            SolutionCSA stop = (*solution_vector)[call_cntr];

            values[0] = CStringGetTextDatum(stop.stop_id.c_str());
            values[1] = Int32GetDatum(stop.stop_sequence);
            values[2] = Float8GetDatum(stop.arrival_time);
            values[3] = CStringGetTextDatum(stop.trip_id.c_str());

            bool nulls[4];
            nulls[0] = stop.stop_id.empty();
            nulls[1] = false;
            nulls[2] = false;
            nulls[3] = stop.trip_id.empty();

            HeapTuple tuple = heap_form_tuple(funcctx->tuple_desc, values, nulls);
            SRF_RETURN_NEXT(funcctx, HeapTupleGetDatum(tuple));
        }
        else
        {
            SRF_RETURN_DONE(funcctx);
        }
    }

    PG_FUNCTION_INFO_V1(pgtfs_version);
    /**
     * @brief Returns version information about the extension.
     *
     * This function returns a textual representation of version information
     * including the extension version, PostgreSQL version, and compiler version.
     *
     * @param fcinfo Function call information.
     * @return A text representation of the version information.
     */
    Datum pgtfs_version(PG_FUNCTION_ARGS)
    {
        const char *COMPILER_VERSION = __VERSION__;
        char *metadata = psprintf("Extension version: %s, PostgreSQL version: %s, Compiler: %s", EXTENSION_VERSION, PG_VERSION_STR, COMPILER_VERSION);
        text *result = cstring_to_text(metadata);
        pfree(metadata);
        PG_RETURN_TEXT_P(result);
    }

    PG_FUNCTION_INFO_V1(pgtfs_raptor);
    Datum pgtfs_raptor(PG_FUNCTION_ARGS)
    {
        FuncCallContext *funcctx;
        MemoryContext oldcontext;

        if (SRF_IS_FIRSTCALL())
        {
            funcctx = SRF_FIRSTCALL_INIT();

            // Switch to multi-call memory context
            oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

            // Create a tuple descriptor for the returned rows
            TupleDesc tupdesc = CreateTemplateTupleDesc(4);
            TupleDescInitEntry(tupdesc, (AttrNumber)1, "stop_id", TEXTOID, -1, 0);
            TupleDescInitEntry(tupdesc, (AttrNumber)2, "stop_sequence", INT4OID, -1, 0);
            TupleDescInitEntry(tupdesc, (AttrNumber)3, "arrival_time", FLOAT8OID, -1, 0);
            TupleDescInitEntry(tupdesc, (AttrNumber)4, "trip_id", TEXTOID, -1, 0);

            // Set the tuple descriptor in the function call context
            funcctx->tuple_desc = BlessTupleDesc(tupdesc);

            text *origin = PG_GETARG_TEXT_PP(0);
            text *destination = PG_GETARG_TEXT_PP(1);
            float8 departure_time = PG_GETARG_FLOAT8(2);
            text *network_query_text = PG_GETARG_TEXT_PP(3);

            if (departure_time < 0)
                ereport(ERROR,
                        (
                            errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE),
                            errmsg("departure_time needs to be valid")));

            char *network_query_str = text_to_cstring(network_query_text);
            int64_t network_size;

            NetworkRow *network = create_network(network_query_str, &network_size);
            if (network == NULL)
            {
                elog(ERROR, "Failed to create the network.");
                PG_RETURN_NULL();
            }

            std::vector<SolutionRAPTOR> solution = perform_RAPTOR(text_to_cstring(origin), text_to_cstring(destination), departure_time, network, network_size);

            SPI_pfree(network);
            pfree(destination);
            pfree(network_query_text);
            pfree(origin);

            if (solution.empty())
            {
                PG_RETURN_NULL();
            }

            funcctx->max_calls = solution.size();
            std::vector<SolutionRAPTOR> *solution_vector = new std::vector<SolutionRAPTOR>(solution);
            funcctx->user_fctx = reinterpret_cast<void *>(solution_vector);
            funcctx->call_cntr = 0;

            // Switch back to original memory context
            MemoryContextSwitchTo(oldcontext);
        }

        // Subsequent calls reuse the existing function call context
        funcctx = SRF_PERCALL_SETUP();
        int call_cntr = funcctx->call_cntr;
        int max_calls = funcctx->max_calls;

        if (call_cntr < max_calls)
        {
            Datum values[4];

            std::vector<SolutionRAPTOR> *solution_vector = reinterpret_cast<std::vector<SolutionRAPTOR> *>(funcctx->user_fctx);

            SolutionRAPTOR stop = (*solution_vector)[call_cntr];

            values[0] = CStringGetTextDatum(stop.stop_id.c_str());
            values[1] = Int32GetDatum(stop.stop_sequence);
            values[2] = Float8GetDatum((float)stop.arrival_time);
            values[3] = CStringGetTextDatum(stop.trip_id.c_str());

            bool nulls[4];
            nulls[0] = stop.stop_id.empty(); 
            nulls[1] = false;                
            nulls[2] = false;                
            nulls[3] = stop.trip_id.empty(); 

            HeapTuple tuple = heap_form_tuple(funcctx->tuple_desc, values, nulls);
            SRF_RETURN_NEXT(funcctx, HeapTupleGetDatum(tuple));
        }
        else
        {
            SRF_RETURN_DONE(funcctx);
        }
    }
}