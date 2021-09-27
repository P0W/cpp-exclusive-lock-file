
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "exclusive-lock-file.h"

namespace {

class NoPolicy {
public:
  NoPolicy([[maybe_unused]] const std::string &fileName) {}
};
template <typename LockPolicy> class FileHandler : public LockPolicy {

public:
  FileHandler(const std::string &fileName)
      : LockPolicy(fileName), m_fileName{fileName}, m_fh{m_fileName} {}

  bool writeVector(const std::vector<int> &vec) {
    if (!m_fh.good()) {
      return false;
    }
    for (const auto &item : vec) {
      m_fh << item << "\n";
    }

    return true;
  }

  ~FileHandler() { m_fh.close(); }

private:
  std::string m_fileName;
  std::ofstream m_fh;
};
} // namespace

auto main() -> int {

  std::vector<int> vec;
  vec.reserve(1e6);

  for (auto x = 1; x < 1e6; ++x) {
    vec.emplace_back(x);
  }

  // Take exclusive lock of file

  try {
    FileHandler<fileutil::exclusive_lock_file> exFile("numbers.txt");
    // FileHandler<NoPolicy> exFile("numbers.txt");
    exFile.writeVector(vec);
    int anyKey = 1;
    std::cout << "Press any key to exit"
              << "\n";
    std::cin >> anyKey;
  } catch (const std::exception &err) {
    std::cout << "Failed :" << err.what() << "\n";
  }
}
