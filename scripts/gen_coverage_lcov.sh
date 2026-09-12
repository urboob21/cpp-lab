#!/usr/bin/env bash
set -e

# Check lcov installation
if ! command -v lcov >/dev/null 2>&1; then
  echo "Error: lcov is not installed."
  echo "Install with: sudo apt install lcov"
  exit 1
fi

# Configure a separate coverage build (ENABLE_COVERAGE adds --coverage -O0 -g)
cmake -S . -B build-coverage \
  -DCMAKE_BUILD_TYPE=Debug \
  -DENABLE_COVERAGE=ON

# Build project
cmake --build build-coverage -j "$(nproc)"

# Reset previous coverage
lcov --directory build-coverage --zerocounters

# Run unit tests
ctest --test-dir build-coverage --output-on-failure -j "$(nproc)"

# Capture coverage
mkdir -p coverage_lcov

lcov --capture \
     --directory build-coverage \
     --ignore-errors mismatch \
     --rc geninfo_unexecuted_blocks=1 \
     --output-file coverage_lcov/coverage.info

# Remove system headers
lcov --remove coverage_lcov/coverage.info '/usr/*' \
     --output-file coverage_lcov/coverage.info

# Generate HTML report
genhtml coverage_lcov/coverage.info \
        --output-directory coverage_lcov

echo "Report: coverage_lcov/index.html"

# Open it in a browser when there is a desktop session
if command -v xdg-open >/dev/null 2>&1 && [ -n "${DISPLAY:-}" ]; then
  xdg-open coverage_lcov/index.html
fi