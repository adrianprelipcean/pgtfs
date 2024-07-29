# SPDX-FileCopyrightText: 2024 Adrian C. Prelipcean <adrianprelipceanc@gmail.com>
#
# SPDX-License-Identifier: CC-BY-NC-SA-4.0

EXTENSION = pgtfs
DATA = sql/pgtfs--0.0.1.sql sql/pgtfs--0.0.2.sql sql/pgtfs--0.0.3.sql
MODULE_big = pgtfs
OBJS = pgtfs.o src/models/network.o src/csa/csa.o
REGRESS= pgtfs_test network_edge_cases_test
REGRESS_OPTS= --inputdir=tests --outputdir=tests --user=postgres
PG_CONFIG = pg_config 
PGXS := $(shell $(PG_CONFIG) --pgxs)

include $(PGXS)