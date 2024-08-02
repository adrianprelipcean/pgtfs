# SPDX-FileCopyrightText: 2024 Adrian C. Prelipcean <adrianprelipceanc@gmail.com>
#
# SPDX-License-Identifier: CC-BY-NC-SA-4.0

EXTENSION = pgtfs
DATA = sql/pgtfs--*.sql
MODULE_big = pgtfs
OBJS = pgtfs.o src/models/network.o src/csa/csa.o src/raptor/raptor.o
REGRESS= pgtfs_csa pgtfs_raptor network_edge_cases_test
REGRESS_OPTS= --inputdir=tests --outputdir=tests --user=postgres
PG_CONFIG = pg_config 
PGXS := $(shell $(PG_CONFIG) --pgxs)

include $(PGXS)