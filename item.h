#ifndef ITEM_H
#define ITEM_H


#include<string>
#include<QString>
#include<sstream>
class Item {
public:
    Item(std::string itemName, int itemType);

    bool isTerminal() const;

    bool operator==(const Item &other) const;

    bool operator!=(Item other);

    std::string getName() const {
        return name;
    }

private:
    std::string name;
    int type;
};
#endif // ITEM_H
