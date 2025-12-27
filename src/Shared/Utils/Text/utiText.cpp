#include "utiText.h"

namespace uti::text {
  void normalizeNewLines(std::string& s) {
    // ... Windows CRLF -> LF
    size_t pos = 0;
    while ((pos = s.find("\r\n", pos)) != std::string::npos) {
      s.replace(pos, 2, "\n");
    }
    // ... Mac CR -> LF
    pos = 0;
    while ((pos = s.find('\r', pos)) != std::string::npos) {
      s.replace(pos, 1, "\n");
    }
    // ... "\n" -> LF
    pos = 0;
    while ((pos = s.find("\\n", pos)) != std::string::npos) {
      s.replace(pos, 2, "\n");
    }
  }

}
