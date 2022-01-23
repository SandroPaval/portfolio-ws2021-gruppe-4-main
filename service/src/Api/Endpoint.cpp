#include "Endpoint.hpp"

#include <iostream>

using namespace Reminder::Api;
using namespace Reminder::Core;
using namespace crow;
using namespace std;

Endpoint::Endpoint(SimpleApp &givenApp, Manager &givenManager) : app(givenApp),
                                                                 manager(givenManager) {
    registerRoutes();
}

Endpoint::~Endpoint() {
}

void Endpoint::registerRoutes() {
    CROW_ROUTE(app, "/api/board")
    ([this](const request &req, response &res) {
        std::string jsonBoards = manager.getBoard();
        res.write(jsonBoards);
        res.end();
    });

    CROW_ROUTE(app, "/api/board/columns")
        .methods("GET"_method, "POST"_method)([this](const request &req, response &res) {
            std::string jsonColumns;

            switch (req.method) {
            case HTTPMethod::Get: {
                jsonColumns = manager.getColumns();
                break;
            }
            case HTTPMethod::Post: {
                jsonColumns = manager.postColumn(req.body);
                res.code = 201;
                break;
            }
            default: {
                break;
            }
            }

            res.write(jsonColumns);
            res.end();
        });

    CROW_ROUTE(app, "/api/board/columns/<int>")
        .methods("GET"_method, "PUT"_method, "DELETE"_method)([this](const request &req, response &res, int columnID) {
            std::string jsonColumn = "{}";

            switch (req.method) {
            case HTTPMethod::Get: {
                jsonColumn = manager.getColumn(columnID);
                break;
            }
            case HTTPMethod::Put: {
                jsonColumn = manager.putColumn(columnID, req.body);
                break;
            }
            case HTTPMethod::Delete: {
                manager.deleteColumn(columnID);
                break;
            }
            default: {
                break;
            }
            }

            res.write(jsonColumn);
            res.end();
        });

    CROW_ROUTE(app, "/api/board/columns/<int>/items")
        .methods("GET"_method, "POST"_method)([this](const request &req, response &res, int columnID) {
            std::string jsonItem;

            switch (req.method) {
            case HTTPMethod::Get: {
                jsonItem = manager.getItems(columnID);
                break;
            }
            case HTTPMethod::Post: {
                jsonItem = manager.postItem(columnID, req.body);
                break;
            }
            default: {
                break;
            }
            }
            res.write(jsonItem);
            res.end();
        });

    CROW_ROUTE(app, "/api/board/columns/<int>/items/<int>")
        .methods("GET"_method, "PUT"_method, "DELETE"_method)([this](const request &req, response &res, int columnID, int itemID) {
            std::string jsonItem;

            switch (req.method) {
            case HTTPMethod::Get: {
                jsonItem = manager.getItem(columnID, itemID);
                break;
            }
            case HTTPMethod::Put: {
                jsonItem = manager.putItem(columnID, itemID, req.body);
                break;
            }
            case HTTPMethod::Delete: {
                manager.deleteItem(columnID, itemID);
                break;
            }
            default: {
                break;
            }
            }
            res.write(jsonItem);
            res.end();
        });
}
