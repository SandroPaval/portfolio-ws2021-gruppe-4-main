#include "Manager.hpp"
#include <iostream>

using namespace Reminder::Core::Model;
using namespace Reminder::Core;
using namespace Reminder::Api::Parser;
using namespace Reminder::Repository;
using namespace std;

Manager::Manager(ParserIf &givenParser, RepositoryIf &givenRepository)
    : parser(givenParser), repository(givenRepository) {
}

Manager::~Manager() {
}

std::string Manager::getBoard() {
    Board board = repository.getBoard();

    return parser.convertToApiString(board);
}

std::string Manager::getColumns() {
    std::vector<Column> columns = repository.getColumns();

    return parser.convertToApiString(columns);
}

std::string Manager::getColumn(int columnID) {
    std::optional<Column> column = repository.getColumn(columnID);
    if (column) {
        return parser.convertToApiString(column.value());
    } else {
        return parser.getEmptyResponseString();
    }
}

std::string Manager::postColumn(std::string request) {

    int const dummyID = -1;
    std::optional<Column> parsedColumnOptional = parser.convertColumnToModel(dummyID, request);
    if (false == parsedColumnOptional.has_value()) {
        return parser.getEmptyResponseString();
    }

    Column parsedColumn = parsedColumnOptional.value();

    std::optional<Column> postedColumn = repository.postColumn(parsedColumn.getName(), parsedColumn.getPosition());

    if (postedColumn) {
        return parser.convertToApiString(postedColumn.value());
    } else {
        return parser.getEmptyResponseString();
    }
}

std::string Manager::putColumn(int columnID, std::string request) {

    std::optional<Column> parsedColumnOptional = parser.convertColumnToModel(columnID, request);

    if (false == parsedColumnOptional.has_value()) {
        return parser.getEmptyResponseString();
    }
    Column column = parsedColumnOptional.value();
    std::optional<Column> putColumn = repository.putColumn(columnID, column.getName(), column.getPosition());

    if (putColumn) {
        return parser.convertToApiString(putColumn.value());
    } else {
        return parser.getEmptyResponseString();
    }
}

void Manager::deleteColumn(int columnID) {
    repository.deleteColumn(columnID);
}

std::string Manager::getItems(int columnID) {
    std::vector<Item> items = repository.getItems(columnID);

    return parser.convertToApiString(items);
}

std::string Manager::getItem(int columnID, int itemID) {

    std::optional<Item> item = repository.getItem(columnID, itemID);

    if (item) {
        return parser.convertToApiString(item.value());
    } else {
        return parser.getEmptyResponseString();
    }
}

std::string Manager::postItem(int columnID, std::string request) {

    int const dummyID = -1;
    std::optional parsedItemOptional = parser.convertItemToModel(dummyID, request);
    if (false == parsedItemOptional.has_value()) {
        return parser.getEmptyResponseString();
    }

    Item item = parsedItemOptional.value();
    std::optional<Item> postedItem = repository.postItem(columnID, item.getTitle(), item.getPosition());
    if (postedItem) {
        return parser.convertToApiString(postedItem.value());
    } else {
        return parser.getEmptyResponseString();
    }
}

std::string Manager::putItem(int columnID, int itemID, std::string request) {

    std::optional parsedItemOptional = parser.convertItemToModel(itemID, request);
    if (false == parsedItemOptional.has_value()) {
        return parser.getEmptyResponseString();
    }

    Item item = parsedItemOptional.value();
    std::optional<Item> putItem = repository.putItem(columnID, itemID, item.getTitle(), item.getPosition());

    if (putItem) {
        return parser.convertToApiString(putItem.value());
    } else {
        return parser.getEmptyResponseString();
    }
}

void Manager::deleteItem(int columnID, int itemID) {
    repository.deleteItem(columnID, itemID);
}
