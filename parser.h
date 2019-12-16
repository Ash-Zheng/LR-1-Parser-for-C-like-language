#ifndef PARSER_H
#define PARSER_H

#include <QString>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <array>
#include <memory>
#include "Sentence.h"
#include "LRitem.h"
#include <sstream>
#include<string>
#include<queue>
#include<stack>
struct TreeNode
{
    std::string sign;
    TreeNode* FirstChild;
    TreeNode* NextSibling;
};
using std::vector;

using std::endl;

using namespace std;

class Parser {
private:
    vector<Sentence> grammarList;
    vector<Item>items;

    map<string, set<string>> firstSet;
    map<Item, set<Item>> followSet;

    vector<Sentence> augGrammarList;
    vector<Item>augItems;


    vector<vector<LRitem>> closureList;

    void tool1(int i, vector<Item>arr);
public:


    map<array<int, 2>, string> ActionTable;
    map<array<int, 2>, int> GotoTable;

    Parser();
    Parser(vector<Sentence>, vector<Item>);

    void aug();//形成增广符号集和语法集

    void first();

    void action();

    void firstClosure();

    void LR0_insert(Sentence);

    void augGrammar();

    QString printClosure();

    void printAugGrammar();

    void findFirstSet(set<string>&FS, vector<Item>arr);

    bool checkInClosure(vector<LRitem>, LRitem);

    bool checkInOwnClosure(vector<LRitem>, LRitem);

    Sentence headInsert(Sentence s);

    void insertClosure(vector<LRitem>&,LRitem);

    vector<Item> nextPoint(vector<LRitem>);

    void closure();

    void nextClosure(vector<LRitem>);

    bool lrNotInList(LRitem);

    vector<LRitem> go_to(vector<LRitem>);//计算goto规则

    void go_to_Table();//计算goto表

    void printFirst();

    QString printResult();

    void printActionTable();

    void printGotoTable();

    void printClosureList();

    void initial();

    QString printTree(TreeNode* root);

    ///////////////////////////
    vector<string> buffer;			//输入串/////////////////////////////////输入串是vector<Item>/////////////////////////////////

    std::stack<int> state_stack;			//状态栈
    std::stack<string> sign_stack;		//符号栈

    TreeNode *Root;
    //void judge_result(vector<Item> itm, vector<Sentence> grammar);

    void Init_Tree(TreeNode*);
    //void MoveIn_Tree(TreeNode *NewTree);
    //void Reduction_Tree(TreeNode* NewTree,int num);
    string Grammar_Tree(vector<string>);

    void init_buffer(vector<string>aaaa);

    string printTree();
    string log();
};
#endif // PARSER_H
