// -----------------------------------------------------------------------------
// Binary file I/O
//
//   std::ofstream out(path, std::ios::binary);   write raw bytes with write()
//   std::ifstream in(path, std::ios::binary);    read raw bytes with read()
//   tellp()/tellg() report and seekp()/seekg() move the write/read position
//
// Pitfalls:
//   - Never dump an object with write(this, sizeof(*this)) if it contains
//     pointers or owning members such as std::string: only the pointer value
//     is written, not the characters. Serialize each field instead
//     (strings as length + bytes).
//   - Raw binary data depends on type sizes and endianness, so the file is only
//     portable between identical platforms.
//
// Reference: https://en.cppreference.com/w/cpp/io/basic_fstream
// -----------------------------------------------------------------------------

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

namespace fs = std::filesystem;

class Account {
 public:
  Account() = default;
  Account(std::int32_t code, std::string name, double balance)
      : code_{code}, name_{std::move(name)}, balance_{balance} {}

  std::int32_t code() const { return code_; }

  /// Writes code, balance, name length and name characters.
  void write(std::ostream& out) const {
    const auto length = static_cast<std::uint32_t>(name_.size());
    out.write(reinterpret_cast<const char*>(&code_), sizeof(code_));
    out.write(reinterpret_cast<const char*>(&balance_), sizeof(balance_));
    out.write(reinterpret_cast<const char*>(&length), sizeof(length));
    out.write(name_.data(), static_cast<std::streamsize>(length));
  }

  /// Reads the fields back in the same order. Returns false on failure.
  bool read(std::istream& in) {
    constexpr std::uint32_t kMaxNameLength =
        1024;  // protects against corrupt files
    std::uint32_t length = 0;
    in.read(reinterpret_cast<char*>(&code_), sizeof(code_));
    in.read(reinterpret_cast<char*>(&balance_), sizeof(balance_));
    in.read(reinterpret_cast<char*>(&length), sizeof(length));
    if (!in || length > kMaxNameLength) {
      return false;
    }
    name_.resize(length);
    in.read(name_.data(), static_cast<std::streamsize>(length));
    return static_cast<bool>(in);
  }

  std::string describe() const {
    return "#" + std::to_string(code_) + " " + name_ + ", balance " +
           std::to_string(balance_);
  }

 private:
  std::int32_t code_{0};
  std::string name_;
  double balance_{0.0};
};

void run() {
  const fs::path path = fs::temp_directory_path() / "cpplab_accounts.bin";
  const std::vector<Account> accounts{
      {1, "Whitney Houston", 2500.0},
      {2, "Michael Jackson", 5000.0},
      {3, "Freddie Mercury", 4200.0},
  };

  LOG_SECTION("Writing");
  std::vector<std::streampos> offsets;  // where each record starts
  {
    std::ofstream out(path, std::ios::binary | std::ios::trunc);
    if (!out) {
      LOG_S("cannot open " << path << " for writing");
      return;
    }
    for (const Account& account : accounts) {
      offsets.push_back(out.tellp());
      account.write(out);
    }
  }  // the stream is flushed and closed here (RAII)
  LOG_S("wrote " << accounts.size() << " accounts, " << fs::file_size(path)
                 << " bytes to " << path.string());
  LOG_S("sizeof(Account) = " << sizeof(Account)
                             << " - the object size is NOT what we store");

  LOG_SECTION("Reading sequentially");
  {
    std::ifstream in(path, std::ios::binary);
    Account account;
    while (account.read(in)) {
      LOG_S("  " << account.describe());
    }
  }

  LOG_SECTION("Random access with seekg");
  {
    std::ifstream in(path, std::ios::binary);
    in.seekg(offsets.at(2));  // jump straight to the third record
    Account third;
    if (third.read(in)) {
      LOG_S("  record 3: " << third.describe());
    }
  }

  fs::remove(path);
  LOG_S("removed " << path.filename().string());
}

}  // namespace

LAB_EXAMPLE(
    "BinaryFileHandling",
    "write/read binary records, serialize strings, seekg random access") {
  run();
}
