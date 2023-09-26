#include "storage/yaml.hpp"
#include "core/mailbox.hpp"

#include <filesystem>
#include <iomanip>
#include <sstream>
#include <string>
#include <unordered_map>

namespace fs = std::filesystem;

namespace mail_lib::storage {
YAML::YAML(std::string user) {
  _user_path = STORAGE_DIR"/" + user + "/";
  fs::create_directory(_user_path); // Cria diretório, se não existe
  
  for (auto const &entry : fs::directory_iterator(_user_path)) {
    auto const &file_path = entry.path();
    auto file_mode = std::fstream::in | std::fstream::out | std::fstream::app;
    _user_files[file_path.stem()] = std::fstream(file_path.c_str(), file_mode);
  }
}

std::unordered_map<std::string, core::Mailbox> YAML::load() {
  std::unordered_map<std::string, core::Mailbox> mboxes;
  for (auto &file : _user_files) {
    std::string filename = file.first;
    std::fstream &fs = file.second;
    std::string item;
    
    std::vector<core::Message> msgs;
    fs.seekg(std::ios::beg);
    
    while (std::getline(fs, item)) {
      core::Message msg;

      std::istringstream stream(item.substr(item.find(":") + 2));
      stream >> std::quoted(msg.from);

      std::getline(fs, item);
      stream.str(item.substr(item.find(":") + 2));
      stream >> std::quoted(msg.to);
      
      std::getline(fs, item);
      stream.str(item.substr(item.find(":") + 2));
      stream >> std::quoted(msg.subject);
      
      std::getline(fs, item);
      stream.str(item.substr(item.find(":") + 2));
      stream >> std::quoted(msg.content);

      msgs.push_back(msg);
    }
    mboxes.try_emplace(filename, filename, msgs);
  }
  return mboxes;
}

YAML &YAML::save(std::vector<core::Mailbox> const &mboxes) {
  for (core::Mailbox const &mbox: mboxes) {
    std::string filename = mbox.name();
    
    if (!_user_files.count(filename)) {
      auto file_mode = std::fstream::in | std::fstream::out | std::fstream::app;
      _user_files[filename] = std::fstream(_user_path + filename + ".yaml", file_mode);
    }
    
    std::fstream &fs = _user_files[filename];
    for (core::Message const &msg : mbox.read_all()) {
      fs << "- from: \"" << msg.from << "\"" << std::endl;;
      fs << "  to: \"" << msg.to << "\"" << std::endl;
      fs << "  subject: \"" << msg.subject << "\"" << std::endl;
      fs << "  content: \"" << msg.content << "\"" << std::endl;
    }
  }
  return *this;
}
}
