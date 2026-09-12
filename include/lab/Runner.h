#pragma once

#include <iosfwd>
#include <string_view>

#include "lab/Registry.h"

namespace lab {

/// Runs one example between a header and a footer. Exceptions that escape the
/// example are caught and reported. Returns true if the example succeeded.
bool runExample(const Example& example, std::ostream& out);

/// Runs every non-interactive example that matches `filter` and prints a
/// summary. Returns the number of failed examples.
int runAll(const Registry& registry, std::string_view filter,
           std::ostream& out);

}  // namespace lab
