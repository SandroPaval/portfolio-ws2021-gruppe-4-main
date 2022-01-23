

#include "Item.hpp"
using namespace Reminder::Core::Model;

Item::Item(int id, std::string givenTitle, int givenPosition, std::string givenTimestamp)
    : id(id), title(givenTitle), position(givenPosition), timestamp(givenTimestamp) {}

int Item::getID() const {
    return id;
}
std::string Item::getTitle() const {
    return title;
}
int Item::getPosition() const {
    return position;
}
std::string Item::getTimestamp() const {
    return timestamp;
}

void Item::setId(int givenID) {
    id = givenID;
}
void Item::setTitle(std::string givenTitle) {
    title = givenTitle;
}
void Item::setPosition(int givenPosition) {
    position = givenPosition;
}
void Item::setTimestamp(std::string givenTimestamp) {
    timestamp = givenTimestamp;
}
