# SPDX-FileCopyrightText: 2024 Adrian C. Prelipcean <adrianprelipceanc@gmail.com>
#
# SPDX-License-Identifier: CC-BY-NC-SA-4.0

EXTENSION = pgtfs
DATA = pgtfs--0.0.1.sql
MODULE_big = pgtfs
OBJS = pgtfs.o src/models/network.o src/csa/csa.o
REGRESS= pgtfs_test network_edge_cases_test
REGRESS_OPTS= --user=postgres
PG_CONFIG = pg_config 
PGXS := $(shell $(PG_CONFIG) --pgxs)

include $(PGXS)