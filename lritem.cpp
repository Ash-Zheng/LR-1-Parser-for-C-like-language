#include "LRitem.h"
#include <iostream>
#include <utility>

using std::move;
using std::vector;
using std::next;
using std::exception;
using std::set;
using std::cout;
using std::endl;
using std::distance;
using std::equal;
using std::lexicographical_compare;
using std::runtime_error;

extern Item Eof;
extern Item Point;
extern Item EMPTY;

using namespace std;
using std::string;

LRitem::LRitem(Sentence p,Item lookTable) :
        sentence{move(p)},
        lookForward(move(lookTable)){
}

std::vector<Item> LRitem::alpha()
{
    vector<Item> vec;
    size_t i=0;
    while(i<sentence.size())
    {
        if(sentence[i]==Point)
            break;
        i++;
    }
    for(size_t j=0;j<i;j++)
    {
        vec.push_back(sentence[j]);
    }
    return vec;
}

std::vector<Item> LRitem::beta()
{
    vector<Item> vec;
    size_t i=0;
    while(i<sentence.size())
    {
        if(sentence[i]==Point)
            break;
        i++;
    }
    for(size_t j=i+2;j<sentence.size();j++)
    {
        vec.push_back(sentence[j]);
    }
    return vec;
}

Item LRitem::B()
{
    vector<Item> vec;
    size_t i=0;
    while(i<sentence.size())
    {
        if(sentence[i]==Point)
            break;
        i++;
    }
    i++;
    if(i>=sentence.size())
        return EMPTY;
    else
        return sentence[i];
}

Item LRitem::getLookForward()
{
    return lookForward;
}

bool LRitem::ifEnd()
{
    if(sentence[sentence.size()-1]==Point)
        return true;
    else
        return false;
}

Sentence LRitem::getSentence()
{
    return sentence;
}

bool LRitem::operator==(LRitem LR) const {
    Sentence LRsentence = LR.getSentence();
    Sentence S = sentence;
    if (S.size() != LRsentence.size()) {
        return false;
    }
    int tag = 0;
    for (size_t i = 0; i < S.size(); i++)
    {
        if (S[i] == LRsentence[i])
            tag++;
    }
    bool match;
    if (tag == S.size())
        match = true;
    else
        match = false;
    auto lookMatch = (lookForward==LR.getLookForward());
    return match && lookMatch;
}

LRitem LRitem::getPointMovedRight()
{
    if (this->ifEnd() == 0)
    {
        vector<Item>A(sentence.begin(), sentence.end());
        for (auto i = A.begin(); i != A.end(); i++)
        {
            if ((*i).getName() == Point.getName())
            {
                *i = *(i + 1);
                *(i + 1) = Point;
                break;
            }
        }
        Sentence S(sentence.getItem(), A);
        LRitem L(S, this->getLookForward());
        return L;
    }
    else
        exit(99);

}

Item LRitem::getItemBehindPoint()
{
    for (size_t i = 0; i < sentence.size(); i++)
    {
        if (sentence[i].getName() == Point.getName())
            return sentence[i + 1];
    }
    return EMPTY;
}

string LRitem::printLRitem()
{
    stringstream ss;
    ss << "[" << sentence.getName() << "->";
    for (size_t i = 0; i < sentence.size(); i++)
    {
        if (sentence[i] == Point)
            ss << '*';
        else
            ss << sentence[i].getName();
    }
    ss << ',' << lookForward.getName();
    ss << ']' << endl;



    string ts;
    getline(ss,ts);
    return ts;
}

Item LRitem::getPointNext()
{
    size_t i = 0;//Point的位置
    while (i < sentence.size())
    {
        if (sentence[i] == Point)
            break;
        i++;
    }
    i++;
    if (i == sentence.size())
        return EMPTY;
    else
        return sentence[i];
}
