#include "Utils.h"
#include "TextFile.h"
#include <vector>
#include <iostream>

std::vector<std::string> FileSystem::LoadFileList(const std::string& listFilename) {
  if (listFilename == "-") {
      std::vector<std::string> lines;
      std::string line;
      while (std::getline(std::cin, line)) {
          lines.push_back(line);
      }

      return lines;
  } else {
      TextFile textFile(listFilename);
      auto lines = textFile.ReadLines(true);
      return lines;
  }
}
