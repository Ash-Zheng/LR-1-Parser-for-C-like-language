#include "Item.h"

using std::string;

Item::Item(string itemName, int itemType)
{
    name = itemName;
    type = itemType;
}

bool Item::operator!=(Item other)
{
    return !(*this == other);
}


bool Item::operator==(const Item &other) const {
    return this->type == other.type && this->name == other.name;
}


bool Item::isTerminal() const
{
    if (type == 1)
        return true;
    else
        return false;
}
