#!/usr/bin/env bash
# Creates a new example from a template.
#
#   scripts/new_example.sh <folder below src> <Name> ["one-line description"]
#   scripts/new_example.sh core/utils Span "std::span: a view over contiguous memory"
#
# The file src/<folder>/<Name>.cpp is picked up by CMake on the next build and
# shows up in the menu as <folder>/<Name>. See docs/adding-examples.md.
set -euo pipefail

usage() {
  sed -n '2,8p' "$0" | sed 's/^# \{0,1\}//'
  exit 1
}

if [[ $# -lt 2 || $# -gt 3 ]]; then
  usage
fi

folder="${1%/}"
folder="${folder#src/}"
name="$2"
description="${3:-TODO: one-line description}"

if [[ ! "$name" =~ ^[A-Za-z][A-Za-z0-9_]*$ ]]; then
  echo "error: the name must look like an identifier, e.g. MyExample" >&2
  exit 1
fi
if [[ ! "$folder" =~ ^[a-z0-9_]+(/[a-z0-9_]+)*$ ]]; then
  echo "error: the folder must be lower_case segments separated by '/', e.g. core/utils" >&2
  exit 1
fi

root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
module="${folder%%/*}"
if ! grep -q "cpplab_add_example_module(${module})" "$root/src/CMakeLists.txt"; then
  echo "error: '${module}' is not an example module." >&2
  echo "       Add 'cpplab_add_example_module(${module})' to src/CMakeLists.txt first." >&2
  exit 1
fi

file="$root/src/$folder/$name.cpp"
if [[ -e "$file" ]]; then
  echo "error: src/$folder/$name.cpp already exists" >&2
  exit 1
fi
mkdir -p "$(dirname "$file")"

# Escape backslashes and quotes so the description is a valid string literal.
description="${description//\\/\\\\}"
description="${description//\"/\\\"}"

cat > "$file" <<EOF
// -----------------------------------------------------------------------------
// ${name}
//
// Explain in two or three sentences what this example teaches.
//
// Key points:
//   - ...
//   - ...
//
// Reference: https://en.cppreference.com/w/cpp
// -----------------------------------------------------------------------------

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

void basics() {
  LOG_SECTION("Basics");
  LOG("Hello from ${name}");
  LOG_S("values can be streamed: " << 42);
}

}  // namespace

LAB_EXAMPLE("${name}", "${description}") {
  basics();
}
EOF

echo "created src/$folder/$name.cpp"
echo
echo "Build and run it:"
echo "  cmake --build build && ./build/bin/cpp_lab_project --run $folder/$name"
