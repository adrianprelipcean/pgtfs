.. SPDX-FileCopyrightText: 2024 Adrian C. Prelipcean <adrianprelipceanc@gmail.com>
..
.. SPDX-License-Identifier: CC-BY-NC-SA-4.0

``pgtfs_raptor``
===============================================================================

.. index from here

``pgtfs_raptor`` — Perform the Real-Time Algorithm for Public Transit Routing (RAPTOR) algorithm to identify the shortest arrival time between an origin and a destination, given a departure time and a GTFS network.

.. index to here

.. rubric:: Availability

* Version 0.0.4

  * **Official** function was introduced


Description
-------------------------------------------------------------------------------


The RAPTOR algorithm efficiently computes the optimal routes from a given origin to a destination by exploring a sequence of transit stops and connections.

Beginning at the origin, RAPTOR iteratively scans through the available transit stops and connections to identify and expand potential routes. It evaluates each connection and updates the shortest path to the destination based on real-time data, allowing for dynamic adjustments. The algorithm continues this process, considering multiple rounds of exploration to ensure the most accurate and efficient route is found.

The exploration is performed iteratively for a predefined number of rounds or until all feasible routes are evaluated, providing the best possible connections while considering factors such as travel time and transfer points.

.. index::
    single: version

Signature
-------------------------------------------------------------------------------

.. code:: sql

   pgtfs_raptor(
    origin TEXT,
    destination TEXT,
    departure_time DOUBLE PRECISION,
    network TEXT,
    max_rounds INTEGER
    )

   RETURNS TABLE (
    stop_id TEXT,
    stop_sequence INT,
    arrival_time DOUBLE PRECISION,
    trip_id TEXT
    )


Input columns
-------------------------------------------------------------------------------

=================== ==================== ===============================
Name                Type                 Description
=================== ==================== ===============================
origin              ``TEXT``             Id of the origin
destination         ``TEXT``             Id of the destination
departure_time      ``DOUBLE PRECISION`` Epoch for departure time
network             `Network SQL`_       SQL query to select the GTFS network (see below for definition)
max_rounds          ``int``              The number of rounds RAPTOR is limited to (defaults to 5)
=================== ==================== ===============================


Result columns
-------------------------------------------------------------------------------

=============== ==================== ===============================
Name            Type                 Description
=============== ==================== ===============================
stop_id         ``TEXT``             Id of the stop
stop_sequence   ``TEXT``             Position of the stop within the route
arrival_time    ``DOUBLE PRECISION`` Epoch for arrival time
trip_id         ``TEXT``             Id of the trip for the stop sequence
=============== ==================== ===============================


Network SQL
...............................................................................
The network query should return the following data

.. include:: ../pgtfs_concepts.rst
    :start-after: basic_network_sql_start
    :end-before: basic_network_sql_end


Example query 
...............................................................................

.. code::  
    
    select stop_id, stop_sequence, to_timestamp(arrival_time), trip_id from pgtfs_raptor(
        '1', -- origin
        '6', -- destination 
        extract (epoch from '2024-01-01 11:55:00+00'::timestamptz)::double precision, -- expected departure time
        $bd$
        select 
            trip_id, 
            stop_id, 
            extract (epoch from arrival_time)::double precision, 
            extract (epoch from departure_time)::double precision, 
            stop_sequence::int 
            from (
                values
                ('trip-1','1',NULL::timestamptz, '2024-01-01 12:00:00+00'::timestamptz,1),
                ('trip-1','2','2024-01-01 12:10:00+00','2024-01-01 12:15:00+00',2),
                ('trip-1','3','2024-01-01 12:20:00+00','2024-01-01 12:25:00+00',3),
                ('trip-1','4','2024-01-01 12:30:00+00',NULL,4),
                ('trip-2','3',NULL, '2024-01-01 12:45:00+00',1),
                ('trip-2','5','2024-01-01 12:50:00+00','2024-01-01 12:55:00+00',2),
                ('trip-2','6','2024-01-01 13:00:00+00',NULL,3)
            )
            as data (trip_id, stop_id, arrival_time, departure_time, stop_sequence)
        $bd$
    );

Which produces a result similar to 

.. code::

    stop_id | stop_sequence |      to_timestamp      | trip_id 
    --------+---------------+------------------------+---------
    1       |             0 | 2024-01-01 11:55:00+00 | trip-1
    2       |             1 | 2024-01-01 12:10:00+00 | trip-1
    3       |             2 | 2024-01-01 12:20:00+00 | trip-1
    5       |             3 | 2024-01-01 12:50:00+00 | trip-2
    6       |             4 | 2024-01-01 13:00:00+00 | trip-2

Minimizing transfers on route
...............................................................................
This is done by default by the RAPTOR algorithm and does not require special consideration. 

.. code::  
    
    select stop_id, stop_sequence, to_timestamp(arrival_time), trip_id from pgtfs_raptor(
        '1', -- origin
        '4', -- destination 
        extract (epoch from '2024-01-01 11:55:00+00'::timestamptz)::double precision, -- expected departure time
        $bd$
        select 
            trip_id, 
            stop_id, 
            extract (epoch from arrival_time)::double precision, 
            extract (epoch from departure_time)::double precision, 
            stop_sequence::int 
            from (
                values
                ('trip-1','1',NULL::timestamptz, '2024-01-01 12:00:00+00'::timestamptz,1),
                ('trip-1','2','2024-01-01 12:10:00+00','2024-01-01 12:15:00+00',2),
                ('trip-1','3','2024-01-01 12:20:00+00','2024-01-01 12:25:00+00',3),
                ('trip-1','4','2024-01-01 12:30:00+00',NULL,4),
                ('trip-2','1',NULL, '2024-01-01 12:00:00+00',1),
                ('trip-2','2','2024-01-01 12:10:00+00','2024-01-01 12:15:00+00',2),
                ('trip-2','4','2024-01-01 12:30:00+00',NULL,3)
            )
            as data (trip_id, stop_id, arrival_time, departure_time, stop_sequence)
        $bd$
    );

Produces the following result 

.. code::

    stop_id | stop_sequence |      to_timestamp      | trip_id 
    --------+---------------+------------------------+---------
    1       |             0 | 2024-01-01 12:55:00+01 | trip-1
    2       |             1 | 2024-01-01 13:10:00+01 | trip-1
    3       |             2 | 2024-01-01 13:20:00+01 | trip-1
    4       |             3 | 2024-01-01 13:30:00+01 | trip-1


See also 
...............................................................................
- `Original RAPTOR paper <https://www.microsoft.com/en-us/research/wp-content/uploads/2012/01/raptor_alenex.pdf>`_