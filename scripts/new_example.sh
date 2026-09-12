#!/usr/bin/env bash
# Creates a new example from a template.
#
#   scripts/new_example.sh [--draft] <folder below src> <Name> ["description"]
#   scripts/new_example.sh core/utils Span "std::span: a view over contiguous memory"
#   scripts/new_example.sh --draft core/utils Mdspan "std::mdspan: multidimensional views"
#
# --draft scaffolds a topic to write later: it registers with lab::kDraft, so
# the menu marks it [draft] and running it prints its own outline.
#
# The file src/<folder>/<Name>.cpp is picked up by CMake on the next build and
# shows up in the menu as <folder>/<Name>. See docs/adding-examples.md.
set -euo pipefail

usage() {
  sed -n '2,12p' "$0" | sed 's/^# \{0,1\}//'
  exit 1
}

draft=0
if [[ "${1:-}" == "--draft" ]]; then
  draft=1
  shift
fi

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

if [[ $draft -eq 1 ]]; then
cat > "$file" <<EOF
// -----------------------------------------------------------------------------
// ${name}
//
// DRAFT - a scaffold, not a finished example: it says what this topic should
// show, but the code is not written yet. Replace outline() with real
// demonstrations and drop \`lab::kDraft\` from LAB_EXAMPLE at the bottom.
//
// Planned content:
//   - ...
//   - ...
//
// Reference: https://en.cppreference.com/w/cpp
// -----------------------------------------------------------------------------

#include <array>
#include <string_view>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

// The same list as in the header comment, so that running the draft shows what
// is still missing.
constexpr std::array<std::string_view, 2> kPlanned{
    "...",
    "...",
};

void outline() {
  LOG_SECTION("${name} - planned content");
  for (const std::string_view point : kPlanned) {
    LOG_S("  - " << point);
  }
  LOG("");
  LOG("This example is still a draft. Write it, then remove lab::kDraft.");
}

}  // namespace

LAB_EXAMPLE("${name}", "${description}", lab::kDraft) {
  outline();
}
EOF
else
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

fi

echo "created src/$folder/$name.cpp"
echo
echo "Build and run it:"
echo "  cmake --build build && ./build/bin/cpp_lab_project --run $folder/$name"
