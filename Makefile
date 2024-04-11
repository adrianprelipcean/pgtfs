# SPDX-FileCopyrightText: © 2024 Adrian C. Prelipcean
#
# SPDX-License-Identifier: CC0-1.0

EXTENSION = pgtfs
DATA = pgtfs--0.0.1.sql
MODULE_big = pgtfs
OBJS = pgtfs.o src/models/network.o src/csa/csa.o src/raptor/raptor.o
REGRESS=pgtfs_test
REGRESS_OPTS= --user=postgres
PG_CONFIG = /usr/lib/postgresql/12/bin/pg_config 
PGXS := $(shell $(PG_CONFIG) --pgxs)

include $(PGXS)