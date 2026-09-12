#!/usr/bin/env bash
# Build pipeline: configure + build, static analysis, tests, then start the menu.
#
#   ./scripts/run.sh             full pipeline
#   ./scripts/run.sh --no-tests  skip ctest
set -e  # Exit immediately if a command fails

BUILD_DIR="./build"
PROJECT_EXEC="$BUILD_DIR/bin/cpp_lab_project"
RUN_TESTS=1
if [[ "${1:-}" == "--no-tests" ]]; then
    RUN_TESTS=0
fi

clear
echo "=============================="
echo "   Starting build pipeline... "
echo "=============================="

# Check required tools
for tool in cmake cppcheck; do
    if ! command -v "$tool" &> /dev/null; then
        echo "[ERR]: $tool is not installed."
        exit 1
    fi
done

echo ""
echo "===========>> Building project..."
cmake -S . -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Debug
cmake --build "$BUILD_DIR" -j "$(nproc)"

echo ""
echo "===========>> Running cppcheck..."
cppcheck \
    --enable=warning,style,performance,portability \
    --inconclusive \
    --inline-suppr \
    --quiet \
    --error-exitcode=1 \
    -I include \
    --suppressions-list=.cppcheck-suppressions \
    ./src ./include \
    -isrc/embedded/

echo "[OK] Static analysis passed"

if [[ "$RUN_TESTS" == 1 ]]; then
    echo ""
    echo "===========>> Running tests (unit tests + one smoke test per example)..."
    ctest --test-dir "$BUILD_DIR" --output-on-failure -j "$(nproc)"
fi

# Optional local step: private/ is not part of the repository.
if [[ -f ./private/genid.py ]] && command -v python3 &> /dev/null; then
    echo ""
    echo "===========>> Generating commit id..."
    python3 ./private/genid.py
fi

echo ""
echo "===========>> Running program..."
if [ -f "$PROJECT_EXEC" ]; then
    "$PROJECT_EXEC"
else
    echo "[ERR] Executable not found: $PROJECT_EXEC"
    exit 1
fi

echo ""
echo "=============================="
echo "Pipeline finished successfully!"
echo "=============================="
