<!--
SPDX-FileCopyrightText: 2024 Adrian C. Prelipcean <adrianprelipceanc@gmail.com>

SPDX-License-Identifier: CC-BY-NC-SA-4.0
-->

# PGTFS

PGTFS is a PostgreSQL extension designed to facilitate routing on top of the GTFS (General Transit Feed Specification) format. It provides functionality to query and analyze public transportation data stored in a PostgreSQL database using GTFS feeds.

## Features

- Perform routing and journey planning using the Connection Scan Algorithm (CSA).
- Easily integrate public transportation data into your PostgreSQL-based applications.
- Similar interface to pgRouting. 

> Note: This is still a work in progress, and is not ready for production at this stage.

## Installation

### On your local environment

1. Clone the PGTFS repository:

    ```sh
    git clone https://github.com/adrianprelipcean/pgtfs.git
    ```

2. Install the server development packages (e.g., for Ubuntu):

    ```sh
    apt-get install postgresql-server-dev-all
    ```

3. Build and install the extension:

    ```sh
    cd pgtfs
    make && make install
    ```

4. Enable the extension in your PostgreSQL database:

    ```sql
    CREATE EXTENSION pgtfs;
    ```

### Using Docker

1. Build the Docker container

    ```sh
    docker build -t gtfs/db:1.0 . 
    ```

2. Run the image 

    ```sh
    docker run --name gtfs-container -d -p 15432:5432 --env-file=.env -v `pwd`/docs:/docs gtfs/db:1.0
    ```

3. Enable the extension 
    ```sh
    psql --host localhost --port 15432 --user postgres -c "CREATE EXTENSION pgtfs;"
    ```

4. Verify that the extension is enabled 
    ```sh
    psql --host localhost --port 15432 --user postgres -c "SELECT * FROM pgtfs_version();"
    ```


## Usage

1. Make sure you have imported GTFS data (i used [this GTFS feed](https://www.transit.land/feeds/f-u8-romania~interregionalcalatori~regiotrans~cfrc%C4%83l%C4%83tori~softrans~trans) for this example) in the database you want to test this extension in. At a minimum, you would need data for the following tables.

    ```sql
    CREATE TABLE IF NOT EXISTS stops (
        stop_id TEXT PRIMARY KEY,
        stop_name TEXT,
        stop_lat DOUBLE PRECISION,
        stop_lon DOUBLE PRECISION
    );

    CREATE TABLE IF NOT EXISTS trips (
        route_id TEXT,
        service_id TEXT,
        trip_id TEXT PRIMARY KEY,
        trip_short_name TEXT
    );

    CREATE TABLE IF NOT EXISTS stop_times (
        trip_id TEXT,
        arrival_time INTERVAL,
        departure_time INTERVAL,
        stop_id TEXT,
        stop_sequence INTEGER,
        PRIMARY KEY (trip_id, stop_sequence),
        FOREIGN KEY (trip_id) REFERENCES trips (trip_id),
        FOREIGN KEY (stop_id) REFERENCES stops (stop_id)
    );
    ```
2. Create the PGTFS extension 

    ```sql
    CREATE EXTENSION pgtfs;
    ```

3. Call the Connection Scan Algorithm function for the route between an origin ('10017') and destination ('55770), given a departure time (this day at 12:00:00 AM) and the GTFS trips. 

    ```sql
    SELECT *, to_timestamp(arrival_time) FROM pgtfs_csa('10017', '55770', EXTRACT(EPOCH from current_date), 
    '
        SELECT 
            trip_id, 
            stop_id, 
            EXTRACT(EPOCH FROM current_date+arrival_time) AS arrival_time, 
            EXTRACT(EPOCH FROM current_date+departure_time) AS departure_time,
            stop_sequence
        FROM stop_times 
        ORDER BY trip_id, stop_sequence
    ');
    ```

    > Note: This example is for demonstrative purposes only, dealing with multi-day trips, timezones and non-gtfs routing is not in scope.

## Generating the documentation
Easiest way to generate the documentation is to use Docker

```sh
docker exec -it gtfs-container bash -c "doxygen Doxygen && sphinx-build docs/ docs/_build/ -a"
```

## Important notes 

There are a couple drawbacks for the CSA implementation, in particular: 
- routing is performed exclusively on trips, so information such as footpaths is not considered 
- the SQL query that retrieves the information from `stop_times` **needs to be ordered**
- CSA is an earliest arrival time algorithm, which means it does not prioritize same trip routes  
- it is still a work in progress, and not production ready

## License 

License information is found in the [LICENSES](./LICENSES/) folder. 