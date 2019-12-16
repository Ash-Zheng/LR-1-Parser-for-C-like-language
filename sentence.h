#ifndef SENTENCE_H
#define SENTENCE_H


#include "Item.h"
#include <string>
#include <vector>
#include <algorithm>
#include<sstream>
class Sentence {
public:

    Sentence(Item name, const std::vector<Item> &handleList);

    bool operator==(const Sentence &other);

    bool operator!=(Sentence& other);

    const Item &operator[](size_t index);

    std::string getName() const {
        return name.getName();
    }

    std::vector<Item>::iterator begin();

    std::vector<Item>::iterator end();

    Item getItem();

    Item getList(int);

    size_t size();

    Item first();

    Item last();

    std::vector<Item>getHandleList();

    std::string printSentence();//输出sentence

private:
    Item name;
    std::vector<Item> handleList;
};

#endif // SENTENCE_H
