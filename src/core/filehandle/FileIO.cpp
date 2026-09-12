// -----------------------------------------------------------------------------
// Text file I/O with <fstream>
//
//   std::ofstream   output file stream (creates/truncates the file by default)
//   std::ifstream   input file stream
//   std::fstream    both directions
//
// Steps: open (constructor or open()), check the stream, read/write with
// << >> getline put get, close (close() or simply let it go out of scope).
//
// Open modes: std::ios::app (append), ate (start at the end), trunc (erase),
// binary, in, out - combine with |.
//
// Reference: https://en.cppreference.com/w/cpp/io/basic_ifstream
// -----------------------------------------------------------------------------

#include <filesystem>
#include <fstream>
#include <string>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

namespace fs = std::filesystem;

void writeFile(const fs::path& path) {
  LOG_SECTION("Writing");
  std::ofstream out(path);  // text mode, truncates an existing file
  if (!out) {
    LOG_S("cannot open " << path << " for writing");
    return;
  }
  out << "time_s,gsr_value\n";
  out << "0.000,45.27\n";
  out << "0.005,41.69\n";
  out.put('#');  // a single character
  out << " end of data\n";
  LOG_S("wrote 4 lines to " << path.filename().string());
}  // `out` is closed by its destructor

void appendToFile(const fs::path& path) {
  LOG_SECTION("Appending");
  std::ofstream out(path, std::ios::app);  // keep the content, write at the end
  out << "0.010,38.13\n";
  LOG("appended one line with std::ios::app");
}

void readWordByWord(const fs::path& path) {
  LOG_SECTION("Reading with >> (skips whitespace)");
  std::ifstream in(path);
  std::string word;
  int words = 0;
  while (in >> word) {  // the stream converts to false at end of file or error
    ++words;
  }
  LOG_S("operator>> read " << words << " whitespace-separated words");
}

void readLineByLine(const fs::path& path) {
  LOG_SECTION("Reading with std::getline (keeps whitespace)");
  std::ifstream in(path);
  std::string line;
  int number = 0;
  while (std::getline(in, line)) {
    LOG_S("  line " << ++number << ": " << line);
  }

  // After reaching the end the stream is in the eof/fail state. To read the
  // file again, clear the flags and rewind (or close and reopen it).
  in.clear();
  in.seekg(0);
  std::getline(in, line);
  LOG_S("after clear() + seekg(0), the first line again: " << line);
}

void run() {
  const fs::path path = fs::temp_directory_path() / "cpplab_fileio_test.csv";
  writeFile(path);
  appendToFile(path);
  readWordByWord(path);
  readLineByLine(path);

  LOG_SECTION("Removing");
  const bool removed =
      fs::remove(path);  // or std::remove(path.c_str()) from <cstdio>
  LOG_S("removed " << path.filename().string() << ": " << std::boolalpha
                   << removed);

  std::ifstream missing(path);
  LOG_S("opening a missing file -> is_open() = " << std::boolalpha
                                                 << missing.is_open());
}

}  // namespace

LAB_EXAMPLE("FileIO", "write, append and read text files with fstream") {
  run();
}
