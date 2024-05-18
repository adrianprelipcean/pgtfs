.. SPDX-FileCopyrightText: 2024 Adrian C. Prelipcean <adrianprelipceanc@gmail.com>
..
.. SPDX-License-Identifier: CC-BY-NC-SA-4.0

Concepts
==========

These are some of the relevant concepts you should be aware of to successfully utilize PGTFS.

GTFS 
-----

First, it is important to be acquinted with the GTFS data structure, which is well documented on their `website <https://gtfs.org/>`_ 

GTFS Network 
------------

This is the network on top of which the routing is done. 
The entries here are obtained as a combination from different GTFS core entities like: `stop_times <https://gtfs.org/schedule/reference/#stop_timestxt>`_ / `frequencies <https://gtfs.org/schedule/reference/#frequenciestxt>`_, and  `calendar <https://gtfs.org/schedule/reference/#calendartxt>`_ / `calendar_dates <https://gtfs.org/schedule/reference/#calendar_datestxt>`_.

In the table below, all of the data is available within `stop_times`, but without information from `calendar`, the routes will likely be wrong as they will contain overlapping data (in form of trips) between weekends / weekdays / holidays / etc. 
It is recommended you understand the GTFS structure before attempting to use PGTFS. 


GTFS Network SQL format
-----------------------

.. basic_network_sql_start

=============== ==================== ===============================
Name            Type                 Description
=============== ==================== ===============================
trip_id         ``TEXT``             Id of the trip
stop_id         ``TEXT``             Id of the stop
arrival_time    ``DOUBLE PRECISION`` Arrival time at the stop
departure_time  ``DOUBLE PRECISION`` Departure time from the stop
stop_sequence   ``INT``              Stop sequence within trip
=============== ==================== ===============================


.. basic_network_sql_end

The following SQL snippet produces a valid GTFS network

.. code:: sql

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
    as data (trip_id, stop_id, arrival_time, departure_time, stop_sequence);
