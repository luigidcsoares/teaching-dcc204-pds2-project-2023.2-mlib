#include "doctest.hpp"
#include "core/message.hpp"
#include "core/mailbox.hpp"
#include "storage/yaml.hpp"

#include <filesystem>
#include <string>
#include <unordered_map>

using mail_lib::core::Mailbox;
using mail_lib::core::Message;

// Apelido para o namespace (para ficar mais curto)
namespace fs = std::filesystem;
namespace storage = mail_lib::storage;

TEST_CASE("YAML") {
  std::unordered_map<std::string, Mailbox> mboxes;

  SUBCASE("Testando o carregamento dos dados: load fake") {
    storage::YAML storage("fake-load");
    mboxes = storage.load();
  }

  SUBCASE("Testando o salvamento dos dados: store + load") {
    fs::remove_all(STORAGE_DIR"/fake-save");
    storage::YAML storage("fake-save");
    mboxes = storage.save({
	Mailbox("inbox")
	.add({ "someone0", "fake-user", "subject0", "received0" })
	.add({ "someone1", "fake-user", "subject1", "received1" }),
	
	Mailbox("sent")
	.add({ "fake-user", "someone0", "subject0", "sent0" })
	.add({ "fake-user", "someone1", "subject1", "sent1" })
      }).load();
  }
  
  Mailbox &sent = mboxes.at("sent");
  std::vector<Message> sent_msgs = sent.read_all();
  
  CHECK_EQ(sent.name(), "sent");
  CHECK_EQ(sent_msgs[0].from, "fake-user");
  CHECK_EQ(sent_msgs[0].to, "someone0");
  CHECK_EQ(sent_msgs[0].subject, "subject0");
  CHECK_EQ(sent_msgs[0].content, "sent0");
  
  CHECK_EQ(sent_msgs[1].from, "fake-user");
  CHECK_EQ(sent_msgs[1].to, "someone1");
  CHECK_EQ(sent_msgs[1].subject, "subject1");
  CHECK_EQ(sent_msgs[1].content, "sent1");

  auto inbox = mboxes.at("inbox");
  auto inbox_msgs = inbox.read_all();
  
  CHECK_EQ(inbox.name(), "inbox");
  CHECK_EQ(inbox_msgs[0].from, "someone0");
  CHECK_EQ(inbox_msgs[0].to, "fake-user");
  CHECK_EQ(inbox_msgs[0].subject, "subject0");
  CHECK_EQ(inbox_msgs[0].content, "received0");
  
  CHECK_EQ(inbox_msgs[1].from, "someone1");
  CHECK_EQ(inbox_msgs[1].to, "fake-user");
  CHECK_EQ(inbox_msgs[1].subject, "subject1");
  CHECK_EQ(inbox_msgs[1].content, "received1");
}
