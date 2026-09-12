#pragma once

#include <iosfwd>

#include "lab/Registry.h"

namespace lab {

/// Interactive menu that mirrors the folder structure of the examples.
///
/// Type a number to open a folder or run an example, `0` to go back, `q` to
/// quit, or any other text to search. Returns when the user quits or the
/// input stream ends.
void runMenu(const Registry& registry, std::istream& in, std::ostream& out);

}  // namespace lab
