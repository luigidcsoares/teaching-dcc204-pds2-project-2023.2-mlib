#include "doctest.hpp"
#include "core/message.hpp"
#include "core/mailbox.hpp"

#include <vector>

using mail_lib::core::Mailbox;
using mail_lib::core::Message;

void check_msg(Message a, Message b) {
  CHECK_EQ(a.from, b.from);
  CHECK_EQ(a.to, b.to);
  CHECK_EQ(a.content, b.content);
}

TEST_CASE("Adicionando mensagens à caixa de entrada vazia") {
  Mailbox mbox("inbox");
  
  Message msg0 = { "from0", "to0", "content0" };
  Message msg1 = { "from1", "to1", "content1" };
  mbox.add(msg0).add(msg1);

  std::vector<Message> msgs = mbox.read_all();

  CHECK_EQ(msgs.size(), 2);
  check_msg(msg0, msgs[0]);
  check_msg(msg1, msgs[1]);
}

TEST_CASE("Adicionando mensagens à caixa já preenchida") {
  std::vector<Message> old_msgs = {{ "from0", "to0", "content0" }};
  
  Mailbox mbox("inbox", old_msgs);
  mbox.add({ "from1", "to1", "content1" });
  
  std::vector<Message> msgs = mbox.read_all();

  CHECK_EQ(msgs.size(), 2);
  check_msg(old_msgs[0], msgs[0]);
  check_msg({ "from1", "to1", "content1" } , msgs[1]);
}
