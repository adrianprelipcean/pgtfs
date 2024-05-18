-- SPDX-FileCopyrightText: © 2024 Adrian C. Prelipcean
--
-- SPDX-License-Identifier: EUPL-1.2

CREATE OR REPLACE FUNCTION pgtfs_csa(
    origin TEXT,
    destination TEXT,
    departure_time DOUBLE PRECISION,
    network TEXT
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
