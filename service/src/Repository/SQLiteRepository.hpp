#pragma once

#include "Repository/RepositoryIf.hpp"
#include "sqlite3.h"
#include <string>

using namespace Reminder::Core::Model;
namespace Reminder {
namespace Repository {

class SQLiteRepository : public RepositoryIf {
  private:
    sqlite3 *database;
    void initialize();
    void createDummyData();
    void handleSQLError(int statementResult, char *errorMessage);

    static bool isValid(int id) {
        return id != INVALID_ID;
    }

    static Item getItemFromCallback(char **fieldVaues, int startIndex);
    static Column getColumnFromCallback(char **fieldValues, int startIndexx);

    static int queryColumnsCallback(void *data, int numberofColumns, char **fieldValues, char **columnNames);
    static int queryItemsCallback(void *data, int numberOfColumns, char **fieldValues, char **columnNames);

  public:
    SQLiteRepository();
    virtual ~SQLiteRepository();

    virtual Board getBoard();
    virtual std::vector<Column> getColumns();
    virtual std::optional<Column> getColumn(int id);
    virtual std::optional<Column> postColumn(std::string name, int position);
    virtual std::optional<Column> putColumn(int id, std::string name, int position);
    virtual void deleteColumn(int id);
    virtual std::vector<Item> getItems(int columnID);
    virtual std::optional<Item> getItem(int columnID, int itemID);
    virtual std::optional<Item> postItem(int columnID, std::string title, int position);
    virtual std::optional<Item> putItem(int columnID, int itemID, std::string title, int position);
    virtual void deleteItem(int columnID, int itemID);

    static inline std::string const boardTitle = "Reminder App";
    static inline int const INVALID_ID = -1;

    static std::string const databaseFile;
};

} // namespace Repository
} // namespace Reminder
