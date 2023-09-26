#pragma once

#include <string>

namespace mail_lib::core {
struct Message {
  std::string from;
  std::string to;
  std::string subject;
  std::string content;
};
}
