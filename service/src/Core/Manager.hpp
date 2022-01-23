#pragma once

#include "Api/Parser/ParserIf.hpp"
#include "Repository/RepositoryIf.hpp"

namespace Reminder {
namespace Core {
class Manager {
  private:
    Reminder::Repository::RepositoryIf &repository;
    Reminder::Api::Parser::ParserIf &parser;

  public:
    Manager(Reminder::Api::Parser::ParserIf &givenParser, Reminder::Repository::RepositoryIf &givenRepository);
    ~Manager();

    std::string getBoard();

    std::string getColumns();
    std::string getColumn(int columnID);
    std::string postColumn(std::string request);
    std::string putColumn(int columnID, std::string request);
    void deleteColumn(int columnID);

    std::string getItems(int columnID);
    std::string getItem(int columnID, int itemID);
    std::string postItem(int columnID, std::string request);
    std::string putItem(int columnID, int itemID, std::string request);
    void deleteItem(int columnID, int itemID);
};

} // namespace Core
} // namespace Reminder
