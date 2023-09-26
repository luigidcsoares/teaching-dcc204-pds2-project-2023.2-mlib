#include "core/mailbox.hpp"
#include "core/message.hpp"

#include <vector>
#include <iostream>

namespace mail_lib::core {
Mailbox::Mailbox(std::string name, std::vector<Message> const &msgs) {
  std::cout << "Criando uma mailbox " << std::endl;
  _name = name;
  _msgs = msgs;
}

Mailbox::Mailbox(std::string name) : Mailbox(name, {}) {}


Mailbox &Mailbox::add(Message msg) {
  _msgs.push_back(msg);
  return *this;
}

std::vector<Message> Mailbox::read_all() const {
  return _msgs;
}

std::string Mailbox::name() const {
  return _name;
}
}
