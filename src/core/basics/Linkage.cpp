// cppcheck-suppress-file [unreadVariable]
#include <iostream>
using namespace std;

namespace {
namespace Internal {
void run() {}
}  // namespace Internal

namespace External {
void run() {}
}  // namespace External

namespace SharingGlobalConstantsAcrossMultipleFiles {
void run() {}
}  // namespace SharingGlobalConstantsAcrossMultipleFiles

}  // namespace

struct LinkageAutoRunner {
  LinkageAutoRunner() {
    cout << "\n"
         << "\n"
         << "Linkage\n";
  }
};

static LinkageAutoRunner autoRunInstance;
