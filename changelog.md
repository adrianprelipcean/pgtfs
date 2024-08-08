<!--
SPDX-FileCopyrightText: 2024 Adrian C. Prelipcean <adrianprelipceanc@gmail.com>

SPDX-License-Identifier: CC-BY-NC-SA-4.0
-->

# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to
[Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.0.4](https://github.com/adrianprelipcean/pgtfs/releases/tag/v0.0.4)
### Added
- Added Round Based Public Transit Routing (RAPTOR) implementation

### Changed
- Centralized solutions used by different algorithms into on `SolutionAlg` available in `src/models/solution.h`
- Moved algorithms to one folder `src/algorithms`
- Renamed `pg_tests` to `pg_csa` to allow for multiple algorithms to be tested

## [0.0.3](https://github.com/adrianprelipcean/pgtfs/releases/tag/v0.0.3)
### Added
- Added version of Connection Scan Algorithms that prioritize searches on the same route (`perform_CSA_Minimize_Transfers`) invoked by setting the `minimize_transfers` boolean flag to `true`
- Added regression test for minimizing route transfers 

### Changed
- Removed `pfree` for `PG_GETARG_XXX` calls, which caused `pfree called with invalid pointer` crashes


## [0.0.2](https://github.com/adrianprelipcean/pgtfs/releases/tag/v0.0.2)
### Changed
- use `double` instead of `float8` and `time_t` to maintain data type consistency between PostgreSQL and PGTFS
- `csa.cpp` correctly imports `postgres.h` for calling `elog` type functionality  
- folder structure change: tests and relevant data is in the `tests/` folder and the (mostly versioning) extension specific sql files are in the `sql/` folder
 
## [0.0.1](https://github.com/adrianprelipcean/pgtfs/releases/tag/v0.0.1)
### Added
- Added Connection Scan Algorithm implementation 
- Added SQL extension implementation