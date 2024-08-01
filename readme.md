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


## Documentation and tutorials

Official documentation is available at [https://adrianprelipcean.github.io/pgtfs/](https://adrianprelipcean.github.io/pgtfs/)

Tutorials are available at [https://github.com/adrianprelipcean/pgtfs-tutorials](https://github.com/adrianprelipcean/pgtfs-tutorials)

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