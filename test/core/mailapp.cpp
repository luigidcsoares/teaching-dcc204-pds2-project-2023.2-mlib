#include "doctest.hpp"
#include "core/mailapp.hpp"
#include "core/message.hpp"

#include <filesystem>

using mail_lib::core::Mailapp;
using mail_lib::core::Message;

namespace fs = std::filesystem;

TEST_CASE("Enviando mensagem para outro usuário") {
  fs::remove_all(STORAGE_DIR"/fake-from");
  fs::remove_all(STORAGE_DIR"/fake-to");
  
  Mailapp from_app("fake-from");
  from_app.send_message("fake-to", "subject", "sending message");

  Mailapp to_app("fake-to");
  Message last_sent = from_app.sent().read_all().back();
  Message last_received = to_app.inbox().read_all().back();

  CHECK_EQ(last_sent.from, "fake-from");
  CHECK_EQ(last_sent.to, "fake-to");
  CHECK_EQ(last_sent.subject, "subject");
  CHECK_EQ(last_sent.content, "sending message");
  
  CHECK_EQ(last_received.from, "fake-from");
  CHECK_EQ(last_received.to, "fake-to");
  CHECK_EQ(last_received.subject, "subject");
  CHECK_EQ(last_received.content, "sending message");
}
