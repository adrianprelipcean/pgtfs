.. SPDX-FileCopyrightText: 2024 Adrian C. Prelipcean <adrianprelipceanc@gmail.com>
..
.. SPDX-License-Identifier: CC-BY-NC-SA-4.0

Installation
=============

Currently, the only option for installing PGTFS is by building the source code.

Build from the repository 
--------------------------

To use PGTFS, first clone the repository, and then build it:

.. code-block:: bash

   git clone https://github.com/adrianprelipcean/pgtfs.git
   cd gtfs
   make install

This should build the binaries for the extension, and copy them to your PostgreSQL extension folder.

Verify build 
------------

To verify that the extension is successfully installed, we can enable the extesion according to the `PostgreSQL Docs <https://www.postgresql.org/docs/current/sql-createextension.html>`_ 

.. code-block:: sql

   CREATE EXTENSION IF NOT EXISTS pgtfs;

We can also do a sanity check by verifying the pgtfs version 

.. code-block:: sql 

   SELECT * FROM pgtfs_version();

If the above steps did not throw any errors, and you have access to the version of PGTFS, then it means you were successfull and your database is ready for using PGTFS.
