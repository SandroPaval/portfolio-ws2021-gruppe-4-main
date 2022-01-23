#pragma once
#include "Core/Model/Board.hpp"
#include "optional"

using namespace Reminder::Core::Model;

namespace Reminder {
namespace Repository {
class RepositoryIf {
  public:
    virtual ~RepositoryIf() {}

    virtual Board getBoard() = 0;
    virtual std::vector<Column> getColumns() = 0;
    virtual std::optional<Column> getColumn(int id) = 0;
    virtual std::optional<Column> postColumn(std::string name, int position) = 0;
    virtual std::optional<Column> putColumn(int id, std::string name, int position) = 0;
    virtual void deleteColumn(int id) = 0;
    virtual std::vector<Item> getItems(int columnID) = 0;
    virtual std::optional<Item> getItem(int columndID, int itemID) = 0;
    virtual std::optional<Item> postItem(int columnID, std::string title, int position) = 0;
    virtual std::optional<Item> putItem(int columnID, int itemID, std::string title, int position) = 0;
    virtual void deleteItem(int columndID, int itemID) = 0;
};

} // namespace Repository
} // namespace Reminder
