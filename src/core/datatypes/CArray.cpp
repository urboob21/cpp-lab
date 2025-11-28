#include <iostream>

void arrayExamples() {
  std::cout << "\n--- Array Examples ---\n";

  const int arr[5] = {1, 2, 3, 4, 5};
  for (int i = 0; i < 5; ++i) {
    std::cout << "arr[" << i << "] = " << arr[i] << "\n";
  }

  double matrix[2][3] = {{1.1, 2.2, 3.3}, {4.4, 5.5, 6.6}};
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 3; ++j) {
      std::cout << "matrix[" << i << "][" << j << "] = " << matrix[i][j]
                << "\n";
    }
  }
}

struct CArray {
  CArray() { arrayExamples(); }
};

static CArray autoRunArray;
