#pragma once

#include "core/mailbox.hpp"

#include <fstream>
#include <unordered_map>
#include <string>

namespace mail_lib::storage {
class YAML {
public:
  /// @brief Inicializa o gerenciador YAML das caixas do usuário
  ///
  /// O diretório do usuário é criado vazio, caso não exista
  YAML(std::string user);

  /// TODO: poderia verificar se os arquivos não foram alterados,
  ///       para evitar loads em sequência...
  /// @return Lista de mailboxes do usuário
  std::unordered_map<std::string, core::Mailbox> load();

  /// @brief Persiste as mailboxes nos arquivos do usuário.
  YAML &save(std::vector<core::Mailbox> const &mboxes);

private:
  std::string _user_path;
  std::unordered_map<std::string, std::fstream> _user_files;
};
}
