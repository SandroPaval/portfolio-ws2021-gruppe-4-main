#pragma once

#include "ParserIf.hpp"
#include "rapidjson/document.h"

using namespace Reminder::Core::Model;

namespace Reminder {
namespace Api {
namespace Parser {

class JsonParser : public ParserIf {
  private:
    static inline std::string const EMPTY_JSON = "{}";

    bool isValidColumn(rapidjson::Document const &document);
    bool isValidItem(rapidjson::Document const &docuement);

    std::string jsonValueToString(rapidjson::Value const &json);

    rapidjson::Value getJsonValueFromModel(Item const &item, rapidjson::Document::AllocatorType &allocator);
    rapidjson::Value getJsonValueFromModel(Column const &column, rapidjson::Document::AllocatorType &allocator);
    rapidjson::Value getJsonValueFromModel(Board &board, rapidjson::Document::AllocatorType &allocator);

  public:
    JsonParser(){};
    virtual ~JsonParser(){};

    virtual std::string convertToApiString(Board &board);

    virtual std::string convertToApiString(Column &column);
    virtual std::string convertToApiString(std::vector<Column> &columns);

    virtual std::string convertToApiString(Item &item);
    virtual std::string convertToApiString(std::vector<Item> &items);

    virtual std::optional<Column> convertColumnToModel(int columnID, std::string &request);
    virtual std::optional<Item> convertItemToModel(int itemID, std::string &request);

    virtual std::string getEmptyResponseString() {
        return JsonParser::EMPTY_JSON;
    }
};

} // namespace Parser
} // namespace Api
} // namespace Reminder
