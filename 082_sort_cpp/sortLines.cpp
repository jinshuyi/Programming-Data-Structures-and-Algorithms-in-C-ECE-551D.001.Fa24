#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

void processStream(std::istream & stream) {
  std::string line;
  std::vector<std::string> lines;

  while (std::getline(stream, line)) {
    lines.push_back(line);
  }

  std::sort(lines.begin(), lines.end());

  for (std::vector<std::string>::const_iterator it = lines.begin(); it != lines.end();
       ++it) {
    std::cout << *it << std::endl;
  }
}

int main(int argc, char ** argv) {
  if (argc == 1) {
    processStream(std::cin);
  }
  else {
    for (int i = 1; i < argc; ++i) {
      std::ifstream inputFile(argv[i]);
      if (!inputFile) {
        std::cerr << "Can't open file: " << argv[i] << std::endl;
        exit(EXIT_FAILURE);
      }
      processStream(inputFile);
    }
  }
  return EXIT_SUCCESS;
}
