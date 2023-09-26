#include "core/mailapp.hpp"
#include "core/mailbox.hpp"
#include "storage/yaml.hpp"

#include <iostream>

namespace mail_lib::core {
Mailapp::Mailapp(std::string user) : _user(user), _user_storage(user) {
  std::cout << "Construíndo mailapp para o usuário " << user << std::endl;
  auto mboxes = _user_storage.load();

  auto inbox_it = mboxes.find("inbox");
  if (inbox_it != mboxes.end()) {
    _inbox = inbox_it->second;
    _start_new_inbox = _inbox.read_all().size();
  }
  
  auto sent_it = mboxes.find("sent");
  if (sent_it != mboxes.end()) {
    _sent = sent_it->second;
    _start_new_sent = _sent.read_all().size();
  }
}

Mailapp::~Mailapp() {
  Mailbox new_inbox("inbox");
  auto inbox_msgs = _inbox.read_all();
  for (unsigned i = _start_new_inbox; i < inbox_msgs.size(); i++) {
    new_inbox.add(inbox_msgs[i]);
  }
  
  Mailbox new_sent("sent");
  auto sent_msgs = _sent.read_all();
  for (unsigned i = _start_new_sent; i < sent_msgs.size(); i++) {
    new_sent.add(sent_msgs[i]);
  }
  
  _user_storage.save({new_inbox, new_sent});
}

Mailbox Mailapp::inbox() const {
  return _inbox;
}

Mailbox Mailapp::sent() const {
  return _sent;
}

Mailapp &Mailapp::send_message(std::string to, std::string subject, std::string content) {
  Message m = {_user, to, subject, content};
  _sent.add(m);
  
  // Aqui estamos acessando a pasta de um usuário não autenticado,
  // possível fonte de problemas/ataques...
  // Mas é só um exemplo para as aulas, então vamos simplificar;
  storage::YAML(to).save({ Mailbox("inbox").add(m) });

  return *this;
}
}
