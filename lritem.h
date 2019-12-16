#ifndef LRITEM_H
#define LRITEM_H


#include "Sentence.h"
#include <exception>
#include <string>
#include <vector>
#include <map>
#include <set>
#include<QString>
#include<sstream>
#include<string>
class LRitem {
private:
    Sentence sentence;
    Item lookForward;

public:
    LRitem(Sentence p, Item lookTable);

    bool operator==(LRitem lritem) const;

    LRitem getPointMovedRight();//得到将point向右移动一位的LRitem（自身不变）

    Item getItemBehindPoint();//得到在Point右边的item

    std::vector<Item> alpha();

    std::vector<Item> beta();

    Item B();

    Item getLookForward();

    Sentence getSentence();

    std::string printLRitem();

    bool ifEnd();

    Item getPointNext();
};
#endif // LRITEM_H
