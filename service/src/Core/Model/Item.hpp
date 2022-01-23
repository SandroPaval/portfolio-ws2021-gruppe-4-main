#pragma once
#include <string>

namespace Reminder {
namespace Core {
namespace Model {

class Item {

  public:
    Item(int id, std::string givenTitle, int givenPosition, std::string givenTimestamp);
    ~Item(){};

    int getID() const;
    std::string getTitle() const;
    int getPosition() const;
    std::string getTimestamp() const;

    void setId(int givenID);
    void setTitle(std::string givenTitle);
    void setPosition(int givenPosition);
    void setTimestamp(std::string givenTimestamp);

  private:
    int id;
    std::string title;
    int position;
    std::string timestamp;
};

} // namespace Model
} // namespace Core
} // namespace Reminder
