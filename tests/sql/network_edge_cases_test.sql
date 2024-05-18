-- SPDX-FileCopyrightText: © 2024 Adrian C. Prelipcean
--
-- SPDX-License-Identifier: EUPL-1.2

create extension if not exists pgtfs;

-- wrong trip id type
select
    stop_id,
    stop_sequence,
    arrival_time,
    trip_id
from pgtfs_csa(
    '1', '2', 1712707200.0,
    $$
select 
    trip_id::int, 
    stop_id, 
    arrival_time::double precision, 
    departure_time::double precision, 
    stop_sequence::int from (
    values
     ('1753','1',NULL,1712757660,1),
     ('1753','2',1712759880,1712759940,2),
     ('1753','3',1712762040,1712762100,3),
     ('1753','4',1712762100,NULL,4)
    )
    as data (trip_id, stop_id, arrival_time, departure_time, stop_sequence);
$$
);

-- wrong stop id type
select
    stop_id,
    stop_sequence,
    arrival_time,
    trip_id
from pgtfs_csa(
    '1', '2', 1712707200.0,
    $$
select 
    trip_id, 
    stop_id::int, 
    arrival_time::double precision, 
    departure_time::double precision, 
    stop_sequence::int from (
    values
     ('1753','1',NULL,1712757660,1),
     ('1753','2',1712759880,1712759940,2),
     ('1753','3',1712762040,1712762100,3),
     ('1753','4',1712762100,NULL,4)
    )
    as data (trip_id, stop_id, arrival_time, departure_time, stop_sequence);
$$
);

-- wrong arrival time type
select
    stop_id,
    stop_sequence,
    arrival_time,
    trip_id
from pgtfs_csa(
    '1', '2', 1712707200.0,
    $$
select 
    trip_id, 
    stop_id, 
    arrival_time::numeric, 
    departure_time::double precision, 
    stop_sequence::int from (
    values
     ('1753','1',NULL,1712757660,1),
     ('1753','2',1712759880,1712759940,2),
     ('1753','3',1712762040,1712762100,3),
     ('1753','4',1712762100,NULL,4)
    )
    as data (trip_id, stop_id, arrival_time, departure_time, stop_sequence);
$$
);

-- wrong departure time type
select
    stop_id,
    stop_sequence,
    arrival_time,
    trip_id
from pgtfs_csa(
    '1', '2', 1712707200.0,
    $$
select 
    trip_id, 
    stop_id, 
    arrival_time::double precision, 
    departure_time::numeric, 
    stop_sequence::int from (
    values
     ('1753','1',NULL,1712757660,1),
     ('1753','2',1712759880,1712759940,2),
     ('1753','3',1712762040,1712762100,3),
     ('1753','4',1712762100,NULL,4)
    )
    as data (trip_id, stop_id, arrival_time, departure_time, stop_sequence);
$$
);

-- wrong stop sequence type
select
    stop_id,
    stop_sequence,
    arrival_time,
    trip_id
from pgtfs_csa(
    '1', '2', 1712707200.0,
    $$
select 
    trip_id, 
    stop_id, 
    arrival_time::double precision, 
    departure_time::double precision, 
    stop_sequence::text from (
    values
     ('1753','1',NULL,1712757660,1),
     ('1753','2',1712759880,1712759940,2),
     ('1753','3',1712762040,1712762100,3),
     ('1753','4',1712762100,NULL,4)
    )
    as data (trip_id, stop_id, arrival_time, departure_time, stop_sequence);
$$
);
