
#include "Board.hpp"

using namespace Reminder::Core::Model;

Board::Board(std::string title) : title(title) {}

std::string Board::getTitle() const {
    return title;
}
std::vector<Column> &Board::getColumns() {
    return columns;
}

void Board::setColumns(std::vector<Column> const &columns) {
    this->columns = columns;
}
