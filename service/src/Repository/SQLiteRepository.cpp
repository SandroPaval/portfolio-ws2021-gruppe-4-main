#include "SQLiteRepository.hpp"
#include <filesystem>
#include <iostream>

using namespace std;
using namespace Reminder::Repository;

#ifdef RELEASE_SERVICE
string const SQLiteRepository::databaseFile = "./data/reminder-app.db";
#else
string const SQLiteRepository::databaseFile = "../data/reminder-app.db";
#endif

SQLiteRepository::SQLiteRepository() : database(nullptr) {

    string databaseDirectory = filesystem::path(databaseFile).parent_path().string();

    if (filesystem::is_directory(databaseDirectory) == false) {
        filesystem::create_directory(databaseDirectory);
    }

    int result = sqlite3_open(databaseFile.c_str(), &database);

    if (SQLITE_OK != result) {
        cout << "Cannot open database: " << sqlite3_errmsg(database) << endl;
    }
    initialize();
}

SQLiteRepository::~SQLiteRepository() {
    sqlite3_close(database);
}

void SQLiteRepository::initialize() {
    int result = 0;
    char *errorMessage = nullptr;
    string sqlCreateTableColumn =
        "create table if not exists column("
        "id integer not null primary key autoincrement,"
        "name text not null,"
        "position integer not null UNIQUE);";

    string sqlCreteTableItem =
        "create table if not exists item("
        "id integer not null primary key autoincrement,"
        "title text not null,"
        "date text not null,"
        "position integer not null,"
        "column_id integer not null,"
        "unique (position, column_id),"
        "foreign key (column_id) references column (id));";

    result = sqlite3_exec(database, sqlCreateTableColumn.c_str(), NULL, 0, &errorMessage);
    handleSQLError(result, errorMessage);
    result = sqlite3_exec(database, sqlCreteTableItem.c_str(), NULL, 0, &errorMessage);
    handleSQLError(result, errorMessage);
}

Board SQLiteRepository::getBoard() {
    Board board = Board(boardTitle);

    vector<Column> columns = getColumns();
    board.setColumns(columns);

    return board;
}

std::vector<Column> SQLiteRepository::getColumns() {
    std::vector<Column> columns;

    string sqlQueryColumns =
        "SELECT column.id, column.name, column.position, item.id, item.title, item.position, item.date from column "
        "left join item on item.column_id = column.id "
        "order by column.position, item.position";

    int result = 0;
    char *errorMessage = nullptr;

    result = sqlite3_exec(database, sqlQueryColumns.c_str(), queryColumnsCallback, &columns, &errorMessage);
    handleSQLError(result, errorMessage);

    return columns;
}

std::optional<Column> SQLiteRepository::getColumn(int id) {
    std::vector<Column> columns;

    string sqlQueryColumns =
        "SELECT column.id, column.name, column.position, item.id, item.title, item.position, item.date from column "
        "left join item on item.column_id = column.id where column.id = " +
        to_string(id) +
        " order by column.position, item.position";

    int result = 0;
    char *errorMessage = nullptr;

    result = sqlite3_exec(database, sqlQueryColumns.c_str(), queryColumnsCallback, &columns, &errorMessage);
    handleSQLError(result, errorMessage);

    if (columns.size() == 1) {
        return columns.front();
    } else {
        return std::nullopt;
    }
}

std::optional<Column> SQLiteRepository::postColumn(std::string name, int position) {

    string sqlPostItem =
        "INSERT INTO column('name', 'position') "
        "VALUES('" +
        name + "', '" + to_string(position) + "')";

    int result = 0;
    char *errorMessage = nullptr;

    result = sqlite3_exec(database, sqlPostItem.c_str(), NULL, 0, &errorMessage);
    handleSQLError(result, errorMessage);

    if (SQLITE_OK == result) {
        int columnID = sqlite3_last_insert_rowid(database);
        return Column(columnID, name, position);
    }

    return std::nullopt;
}

std::optional<Column> SQLiteRepository::putColumn(int id, std::string name, int position) {

    string sqlUpdateColumn =
        "UPDATE column "
        "SET name = '" +
        name + "', position = '" + to_string(position) +
        "' WHERE id = " + to_string(id);

    int result = 0;
    char *errorMessage = nullptr;

    result = sqlite3_exec(database, sqlUpdateColumn.c_str(), NULL, 0, &errorMessage);
    handleSQLError(result, errorMessage);

    return getColumn(id);
}

void SQLiteRepository::deleteColumn(int id) {

    string sqlDeleteColumnItems =
        "DELETE FROM item "
        "WHERE item.column_id = " +
        to_string(id);

    string sqlDeleteColumn =
        "DELETE FROM column "
        "WHERE column.id = " +
        to_string(id);

    int result = 0;
    char *errorMessage = nullptr;

    result = sqlite3_exec(database, sqlDeleteColumnItems.c_str(), NULL, 0, &errorMessage);
    handleSQLError(result, errorMessage);

    result = sqlite3_exec(database, sqlDeleteColumn.c_str(), NULL, 0, &errorMessage);
    handleSQLError(result, errorMessage);
}

std::vector<Item> SQLiteRepository::getItems(int columnID) {

    std::vector<Item> items;

    string sqlQueryItems =
        "SELECT item.id, item.title, item.position, item.date from item "
        "where item.column_id = " +
        std::to_string(columnID) +
        " order by item.position";

    int result = 0;
    char *errorMessage = nullptr;

    result = sqlite3_exec(database, sqlQueryItems.c_str(), queryItemsCallback, &items, &errorMessage);
    handleSQLError(result, errorMessage);

    return items;
}

std::optional<Item> SQLiteRepository::getItem(int columnID, int itemID) {

    std::vector<Item> items;

    string sqlQueryItems =
        "SELECT item.id, item.title, item.position, item.date from item "
        "where item.column_id = " +
        std::to_string(columnID) + " and item.id = " + std::to_string(itemID) +
        " order by item.position";

    int result = 0;
    char *errorMessage = nullptr;

    result = sqlite3_exec(database, sqlQueryItems.c_str(), queryItemsCallback, &items, &errorMessage);
    handleSQLError(result, errorMessage);

    if (items.size() == 1) {
        return items.front();
    } else {
        return std::nullopt;
    }
}

std::optional<Item> SQLiteRepository::postItem(int columnID, std::string title, int position) {

    string sqlPostItem =
        "INSERT INTO item ('title', 'date', 'position', 'column_id')"
        "VALUES ('" +
        title + "', datetime(), " + to_string(position) + ", " + to_string(columnID) + ");";

    int result = 0;
    char *errorMessage = nullptr;

    result = sqlite3_exec(database, sqlPostItem.c_str(), NULL, 0, &errorMessage);
    handleSQLError(result, errorMessage);

    int itemID = INVALID_ID;
    if (SQLITE_OK == result) {
        itemID = sqlite3_last_insert_rowid(database);
    }

    return getItem(columnID, itemID);
}

std::optional<Item> SQLiteRepository::putItem(int columnID, int itemID, std::string title, int position) {

    string sqlUpdateItem =
        "UPDATE item SET title = '" + title + "', position = " + to_string(position) +
        "WHERE item.column_id = " + to_string(columnID) + " AND item.id = " + to_string(itemID);

    int result = 0;
    char *errorMessage = nullptr;

    result = sqlite3_exec(database, sqlUpdateItem.c_str(), NULL, 0, &errorMessage);
    handleSQLError(result, errorMessage);

    return getItem(columnID, itemID);
}

void SQLiteRepository::deleteItem(int columnID, int itemID) {
    string sqlQueryItem =
        "DELETE FROM item "
        "WHERE item.column_id = " +
        to_string(columnID) +
        " AND item.id = " + to_string(itemID);

    int result = 0;
    char *errorMessage = nullptr;

    result = sqlite3_exec(database, sqlQueryItem.c_str(), NULL, 0, &errorMessage);
    handleSQLError(result, errorMessage);
}

Item SQLiteRepository::getItemFromCallback(char **fieldValues, int startIndex) {
    int index = startIndex;

    int itemID = fieldValues[index] ? atoi(fieldValues[index]) : INVALID_ID;
    index++;

    string title = fieldValues[index] ? fieldValues[index] : "";
    index++;

    int position = fieldValues[index] ? atoi(fieldValues[index]) : 0;
    index++;

    string timestamp = fieldValues[index] ? fieldValues[index] : "";

    Item item(itemID, title, position, timestamp);
    return item;
}

Column SQLiteRepository::getColumnFromCallback(char **fieldValues, int startIndex) {
    int index = startIndex;

    int columnID = fieldValues[index] ? atoi(fieldValues[index]) : INVALID_ID;
    index++;

    string name = fieldValues[index] ? fieldValues[index] : "";
    index++;

    int position = fieldValues[index] ? atoi(fieldValues[index]) : 0;

    Column column(columnID, name, position);
    return column;
}

int SQLiteRepository::queryColumnsCallback(void *data, int numberOfColumns, char **fieldValues, char **columnNames) {

    std::vector<Column> *pColumns = static_cast<std::vector<Column> *>(data);
    std::vector<Column> &columns = *pColumns;

    Item item = getItemFromCallback(fieldValues, 3);

    int columnID = fieldValues[0] ? atoi(fieldValues[0]) : INVALID_ID;
    bool wasColumnAleardyAdded = false;

    for (Column &column : columns) {
        if (column.getID() == columnID) {
            wasColumnAleardyAdded = true;
            if (isValid(item.getID())) {
                column.addItem(item);
            }
            break;
        }
    }

    if (false == wasColumnAleardyAdded) {
        Column column = getColumnFromCallback(fieldValues, 0);

        if (isValid(item.getID())) {
            column.addItem(item);
        }

        if (isValid(column.getID())) {
            columns.push_back(column);
        }
    }

    return 0;
}

int SQLiteRepository::queryItemsCallback(void *data, int numberOfColumns, char **fieldValues, char **columnNames) {

    std::vector<Item> *result = static_cast<std::vector<Item> *>(data);

    Item item = getItemFromCallback(fieldValues, 0);
    if (isValid(item.getID())) {
        result->push_back(item);
    }
    return 0;
}

void SQLiteRepository::handleSQLError(int statementResult, char *errorMessage) {

    if (statementResult != SQLITE_OK) {
        cout << "SQL error: " << errorMessage << endl;
        sqlite3_free(errorMessage);
    }
}

void SQLiteRepository::createDummyData() {

    cout << "creatingDummyData ..." << endl;

    int result = 0;
    char *errorMessage;
    string sqlInsertDummyColumns =
        "insert into column (name, position)"
        "VALUES"
        "(\"prepare\", 1),"
        "(\"running\", 2),"
        "(\"finished\", 3);";

    result = sqlite3_exec(database, sqlInsertDummyColumns.c_str(), NULL, 0, &errorMessage);
    handleSQLError(result, errorMessage);

    string sqlInserDummyItems =
        "insert into item (title, date, position, column_id)"
        "VALUES"
        "(\"in plan\", date('now'), 1, 1),"
        "(\"some running task\", date('now'), 1, 2),"
        "(\"finished task 1\", date('now'), 1, 3),"
        "(\"finished task 2\", date('now'), 2, 3);";

    result = sqlite3_exec(database, sqlInserDummyItems.c_str(), NULL, 0, &errorMessage);
    handleSQLError(result, errorMessage);
}
