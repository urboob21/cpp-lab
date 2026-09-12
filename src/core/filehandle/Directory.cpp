// -----------------------------------------------------------------------------
// std::filesystem (C++17)
//
//   std::filesystem::path    portable path manipulation (operator/, filename,
//                            extension, parent_path, ...)
//   create_directory         creates ONE directory, parent must exist
//   create_directories       creates the whole chain, like `mkdir -p`
//   directory_iterator       lists a directory (recursive_directory_iterator
//                            walks the whole tree)
//   remove / remove_all      delete a file or empty dir / a whole tree
//
// Most functions have two forms: one throws std::filesystem::filesystem_error,
// the other takes a std::error_code& and never throws.
//
// Reference: https://en.cppreference.com/w/cpp/filesystem
// -----------------------------------------------------------------------------

#include <filesystem>
#include <fstream>
#include <string>
#include <system_error>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

namespace fs = std::filesystem;

void paths() {
  LOG_SECTION("Working with paths");
  const fs::path file = fs::path("projects") / "cpp-lab" / "src" / "main.cpp";
  LOG_S("path        : " << file.string());
  LOG_S("filename    : " << file.filename().string());
  LOG_S("stem        : " << file.stem().string());
  LOG_S("extension   : " << file.extension().string());
  LOG_S("parent_path : " << file.parent_path().string());
  LOG_S("replace_extension(\".h\") : "
        << fs::path(file).replace_extension(".h").string());
}

void directories(const fs::path& root) {
  LOG_SECTION("Creating directories");
  const fs::path nested = root / "p1" / "p2";

  std::error_code error;
  if (!fs::create_directory(nested, error)) {  // non-throwing overload
    LOG_S("create_directory(p1/p2) failed: "
          << error.message() << " (the parent p1 does not exist)");
  }

  fs::create_directories(nested);  // creates p1 and p2
  LOG_S("create_directories(p1/p2) -> exists: " << std::boolalpha
                                                << fs::exists(nested));

  std::ofstream(nested / "notes.txt") << "hello filesystem\n";
  std::ofstream(root / "p1" / "readme.md") << "# readme\n";

  LOG_SECTION("Listing directories");
  for (const fs::directory_entry& entry :
       fs::recursive_directory_iterator(root)) {
    const std::string relative = fs::relative(entry.path(), root).string();
    if (entry.is_directory()) {
      LOG_S("  [dir]  " << relative);
    } else {
      LOG_S("  [file] " << relative << " (" << entry.file_size() << " bytes)");
    }
  }
}

void run() {
  const fs::path root = fs::temp_directory_path() / "cpplab_directory_demo";
  fs::remove_all(root);  // start from a clean state
  fs::create_directory(root);

  paths();
  try {
    directories(root);
  } catch (const fs::filesystem_error& e) {
    LOG_S("filesystem error: " << e.what());
  }

  LOG_SECTION("Cleaning up");
  const std::uintmax_t removed = fs::remove_all(root);
  LOG_S("remove_all removed " << removed << " files and directories");
}

}  // namespace

LAB_EXAMPLE(
    "Directory",
    "std::filesystem paths, create_directories, iterating and removing") {
  run();
}
