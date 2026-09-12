#!/usr/bin/env bash
set -e

# Check gcovr installation
if ! command -v gcovr >/dev/null 2>&1; then
  echo "Error: gcovr is not installed."
  echo "Install with: pip install gcovr  or  sudo apt install gcovr or sudo apt install python3-gcovr"
  exit 1
fi

# Configure a separate coverage build (ENABLE_COVERAGE adds --coverage -O0 -g)
cmake -S . -B build-coverage \
  -DCMAKE_BUILD_TYPE=Debug \
  -DENABLE_COVERAGE=ON

# Build project
cmake --build build-coverage -j "$(nproc)"

# Run unit tests
ctest --test-dir build-coverage --output-on-failure -j "$(nproc)"

# Generate coverage report
mkdir -p coverage_gcovr

gcovr -r . build-coverage \
  --branches \
  --html \
  --html-details \
  -o coverage_gcovr/index.html

echo "Report: coverage_gcovr/index.html"

# Open it in a browser when there is a desktop session
if command -v xdg-open >/dev/null 2>&1 && [ -n "${DISPLAY:-}" ]; then
  xdg-open coverage_gcovr/index.html >/dev/null 2>&1 || true
fi