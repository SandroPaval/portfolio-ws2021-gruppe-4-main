#pragma once

#include "Core/Model/Board.hpp"
#include <optional>
#include <string>
using namespace Reminder::Core::Model;

namespace Reminder {
namespace Api {
namespace Parser {

class ParserIf {
  public:
    virtual ~ParserIf() {}

    virtual std::string getEmptyResponseString() = 0;

    virtual std::string convertToApiString(Board &board) = 0;
    virtual std::string convertToApiString(Column &column) = 0;
    virtual std::string convertToApiString(std::vector<Column> &columns) = 0;

    virtual std::string convertToApiString(Item &item) = 0;
    virtual std::string convertToApiString(std::vector<Item> &items) = 0;

    virtual std::optional<Column> convertColumnToModel(int columnID, std::string &request) = 0;
    virtual std::optional<Item> convertItemToModel(int itemID, std::string &request) = 0;
};

} // namespace Parser
} // namespace Api
} // namespace Reminder
