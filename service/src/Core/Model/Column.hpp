#pragma once

#include "Item.hpp"
#include <iostream>
#include <string>
#include <vector>

namespace Reminder {
namespace Core {
namespace Model {
class Column {
  public:
    Column(int id, std::string givenName, int givenPosition);
    ~Column(){};

    int getID() const;
    std::string getName() const;
    int getPosition() const;
    std::vector<Item> getItems() const;

    void setID(int givenID);
    void setName(std::string givenName);
    void setPosition(int givenPosition);
    void addItem(Item &givenItem);

  private:
    int id;
    std::string name;
    int position;
    std::vector<Item> items;
};
} // namespace Model
} // namespace Core
} // namespace Reminder
