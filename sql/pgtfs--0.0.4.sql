-- SPDX-FileCopyrightText: © 2024 Adrian C. Prelipcean
--
-- SPDX-License-Identifier: EUPL-1.2

DROP FUNCTION IF EXISTS pgtfs_csa (TEXT, TEXT, DOUBLE PRECISION, TEXT);
DROP FUNCTION IF EXISTS pgtfs_csa (TEXT, TEXT, DOUBLE PRECISION, TEXT, BOOLEAN);

CREATE OR REPLACE FUNCTION pgtfs_csa(
    origin TEXT,
    destination TEXT,
    departure_time DOUBLE PRECISION,
    network TEXT,
    minimize_transfers BOOLEAN DEFAULT FALSE
)
RETURNS TABLE (
    stop_id TEXT,
    stop_sequence INT,
    arrival_time DOUBLE PRECISION,
    trip_id TEXT
)
AS '$libdir/pgtfs'
LANGUAGE c STRICT;

CREATE OR REPLACE FUNCTION pgtfs_version()
RETURNS TEXT
AS '$libdir/pgtfs'
LANGUAGE c STRICT;

CREATE OR REPLACE FUNCTION pgtfs_raptor(
    origin TEXT,
    destination TEXT,
    departure_time DOUBLE PRECISION,
    network TEXT,
    max_rounds INT DEFAULT 5
)
RETURNS TABLE (
    stop_id TEXT,
    stop_sequence INT,
    arrival_time DOUBLE PRECISION,
    trip_id TEXT
)
AS '$libdir/pgtfs'
LANGUAGE c STRICT;
