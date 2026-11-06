#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
cc -std=c11 -Wall -Wextra -Werror -pedantic -o /tmp/frdm_mcxa156_core_test tests/test_core.c
/tmp/frdm_mcxa156_core_test
