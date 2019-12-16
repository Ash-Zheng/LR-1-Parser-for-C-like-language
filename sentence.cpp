#include "Sentence.h"

//const Item EMPTY;

Sentence::Sentence(Item senName, const std::vector<Item> &items):
    name{ std::move(senName) }, handleList{ items.begin(),items.end()}{
}

const Item &Sentence::operator[](size_t index) {
    return handleList.at(index);
}

bool Sentence::operator==(const Sentence &other){
    if (this->name!=other.name)
        return false;
    if (handleList.size()!=other.handleList.size())
        return false;
    std::vector<Item>tmp1 = handleList;
    std::vector<Item>tmp2 = other.handleList;
    for (size_t i = 0; i < tmp1.size(); i++)
    {
        if (tmp1[i] != tmp2[i])
            return false;
    }
    return true;
}

bool Sentence::operator!=(Sentence& other)
{
    return !(*this == other);
}

Item Sentence::first() {
    return this->operator[](0);
}

size_t Sentence::size() {
    return handleList.size();
}

Item Sentence::last() {
    return handleList.back();
}

Item Sentence::getItem() {
    return name;
}

Item Sentence::getList(int index) {
    return handleList[index];
}

std::vector<Item>::iterator Sentence::begin() {
    return handleList.begin();
}

std::vector<Item>::iterator Sentence::end() {
    return handleList.end();
}

std::vector<Item>Sentence::getHandleList() {
    return handleList;
}

std::string Sentence::printSentence()//输出sentence
{
    std::string r="";
    r+= getName() + "->";
    for (int i = 0; i < handleList.size(); i++)
    {
        r+= handleList[i].getName() + " ";
    }
    return r;
}
