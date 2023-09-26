#pragma once

#include "message.hpp"

#include <string>
#include <vector>

namespace mail_lib::core {
class Mailbox {
public:
  /// @brief Inicializa a caixa com um determinado nome, mas vazia
  ///
  /// Detalhes...
  /// @param name Nome da caixa de mensagens (e.g., inbox)
  Mailbox(std::string name);

  /// @brief Inicializa a caixa com mensagens previamente lidas
  /// @param name Nome da caixa de mensagens (e.g., inbox)
  /// @param msgs Vetor de mensagens, onde a **última** deve ser
  ///             a mais recente (idealmente teria um timestamp...)
  Mailbox(std::string name, std::vector<Message> const &msgs);

  /// @brief Adiciona uma mensagem ao final da lista
  /// @param msg Mensagem a ser adicionada
  /// @return A própria caixa, para chamadas consecutivas
  Mailbox &add(Message msg);

  /// @return A lista de mensagens, em que o último elemento é
  ///         a mensagem adicionada mais recentemente
  std::vector<Message> read_all() const;

  /// @return Nome da caixa de entrada
  std::string name() const;
  
private:
  std::string _name;
  std::vector<Message> _msgs;
};
};
