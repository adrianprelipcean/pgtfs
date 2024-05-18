.. SPDX-FileCopyrightText: 2024 Adrian C. Prelipcean <adrianprelipceanc@gmail.com>
..
.. SPDX-License-Identifier: CC-BY-NC-SA-4.0

=====
PGTFS
=====

About 
------------

PGTFS is a `PostgreSQL <https://www.postgresql.org/>`_ extension designed to facilitate routing on top of the GTFS (General Transit Feed Specification) format. 
It provides functionality to query and analyze public transportation data stored in a PostgreSQL database using GTFS feeds.


Why PGTFS?
------------

While there are several projects that support routing using GTFS feeds, I felt the need to build something as simple as `pgRouting <https://pgrouting.org/>`_, but for GTFS feeds. 
The extension capabilities of PostgreSQL made this an easy choice. 

Who is it for? 
--------------

The first version of the PGTFS extension is a proof of concept, the purpose is to add support for the most common used algorithms within public transportation / GTFS feeds. 
As such, it is currently suited for research purposes and products that perform routing at a city-scale.

Licensing
------------
The following linceses are found in PGTFS:

.. list-table::
   :widths: 250 500

   * - **License**
     -
   * - European Union Public License 1.2
     - The PGTFS codebase is available under `European Union Public License 1.2
       <https://spdx.org/licenses/EUPL-1.2.html>`_.
   * - Creative Commons Attribution-NonCommercial-ShareAlike 4.0 License
     - The PGTFS Manual is licensed under a `Creative Commons
       Attribution-NonCommercial-ShareAlike	4.0 License
       <https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en>`_.