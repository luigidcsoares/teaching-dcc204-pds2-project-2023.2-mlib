#pragma once

#include "core/mailbox.hpp"
#include "core/message.hpp"
#include "storage/yaml.hpp"

namespace mail_lib::core {
class Mailapp {
public:
  /// @brief Inicialização das caixas de entrada e saída
  ///
  /// Busca as mensagens lidas e enviadas pelo usuário,
  /// para inicializar as caixas de mensagens.
  ///
  /// FIXME: obviamente, aqui (ou em algum passo antes)
  ///        deveria ter alguma etapa de autenticação...
  ///
  /// @param user Usuário (email) que está logado
  Mailapp(std::string user);

  /// @brief Realiza o flush das caixas de mensagem
  ~Mailapp();

  /// @return A caixa de entrada, para leitura de emails recebidos
  Mailbox inbox() const;

  /// @return A caixa de saída, para leitura de emails enviados
  Mailbox sent() const;

  /// @brief Adiciona nova mensagem à caixa de saída
  Mailapp &send_message(std::string to, std::string subject, std::string content);

private:
  std::string _user;
  
  Mailbox _inbox = Mailbox("inbox");
  Mailbox _sent = Mailbox("sent");

  unsigned _start_new_inbox = 0;
  unsigned _start_new_sent = 0;
  
  storage::YAML _user_storage;
};
}
