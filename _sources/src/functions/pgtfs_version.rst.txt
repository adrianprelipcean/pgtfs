.. SPDX-FileCopyrightText: 2024 Adrian C. Prelipcean <adrianprelipceanc@gmail.com>
..
.. SPDX-License-Identifier: CC-BY-NC-SA-4.0

``pgtfs_version``
===============================================================================

.. index from here

``pgtfs_version`` — Query for PGTFS version information.

.. index to here

.. rubric:: Availability

* Version 0.0.1

  * **Official** function


Description
-------------------------------------------------------------------------------

Returns PGTFS version information.

.. index::
    single: version

Signature
-------------------------------------------------------------------------------

.. code:: sql

   pgtfs_csa()
   RETURNS ``TEXT``

:Example: PGTFS Version for this documentation

Result columns
-------------------------------------------------------------------------------

=========== ===============================
 Type       Description
=========== ===============================
``TEXT``    PGTFS version
=========== ===============================
