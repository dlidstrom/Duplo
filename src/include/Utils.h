#ifndef _UTIL_H_
#define _UTIL_H_

#include "SourceFile.h"
#include <string>

namespace CstyleUtils {
  std::string RemoveSingleLineComments(const std::string& line);
}

namespace StringUtil {
  /**
   * Trim string
   *
   * @param input  string to trim
   * @return returns trimmed string
   */
  std::string Trim(const std::string& input);

  /**
   * Split string
   *
   * @param input  string to split
   * @param delimiter  delimiter string to trim
   * @param results  results vector with substrings
   * @param trim  boolean to indicate trimming or not
   * @return returns number of substrings
   */
  int Split(const std::string& input, const std::string& delimiter, std::vector<std::string>& results, bool trim);

  std::string Substitute(char s, char d, const std::string& str);

  void StrSub(std::string& cp, const std::string& sub_this, const std::string& for_this, const int& num_times);

  std::string ToLower(const std::string& s);

  std::string GetFileExtension(const std::string& filename);

  std::string GetFilenamePart(const std::string& fullpath);

  bool IsSameFilename(const SourceFile& left, const SourceFile& right);
}

namespace FileSystem {
  std::vector<std::string> LoadFileList(const std::string& listFilename);
}

#endif
