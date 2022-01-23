
#include "Column.hpp"

using namespace Reminder::Core::Model;
Column::Column(int id, std::string givenName, int givenPosition)
    : id(id), name(name), position(position) {}

int Column::getID() const {
    return id;
}
std::string Column::getName() const {
    return name;
}
int Column::getPosition() const {
    return position;
}
std::vector<Item> Column::getItems() const {
    return items;
}

void Column::setID(int givenID) {
    id = givenID;
}
void Column::setName(std::string givenName) {
    name = givenName;
}
void Column::setPosition(int givenPosition) {
    position = givenPosition;
}
void Column::addItem(Item &givenItem) {
    items.push_back(givenItem);
}
