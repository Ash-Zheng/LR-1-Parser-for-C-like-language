#include "Parser.h"
#include <algorithm>
#include <array>
#include <iostream>
#include <utility>
#include <cstdio>
#include <cstring>
#include <string>

extern Item EMPTY{ "-", 1};
extern Item Eof{ "$", 1 };
extern Item Point{ "@",1 };
extern Item Start{ "S_",0 };

using namespace std;
using std::string;

Parser::Parser(vector<Sentence> grammar, vector<Item>items) :grammarList(move(grammar)),items(move(items))
//, firstSet{},followSet{}
{


}
Parser::Parser()
{

}

void Parser::aug()
{
    augGrammarList.clear();
    augItems.clear();

    augGrammarList.insert(augGrammarList.begin(), grammarList.begin(), grammarList.end());
    augItems.insert(augItems.end(), this->items.begin(), this->items.end());
    augItems.insert(augItems.end(), Eof);
    //生成增广
    augItems.insert(augItems.begin(), Start);
    augGrammarList.insert(augGrammarList.begin(), Sentence(Start, vector<Item>{grammarList[0].getItem()}));
}

void Parser::action()
{
    //array<int, 2> arr = { 1,2 };
        //string ItemName;
    for (int t1 = 0; t1 < closureList.size(); t1++)
    {
        for (int t2 = 0; t2 < closureList[t1].size(); t2++)
        {
            if (closureList[t1][t2].ifEnd() == 0)
            {
                auto tmp = closureList[t1][t2].getPointMovedRight();

                Item ia = closureList[t1][t2].getItemBehindPoint();
                if (ia.isTerminal() == 1)
                {
                    int int_a = 0;
                    while (augItems[int_a].getName() != ia.getName())
                        int_a++;
                    array<int, 2 >arr = { t1,int_a };
                    string s;
                    //string a = GotoTable[array<int, 2>{t1, int_a}];
                    s = "s" + to_string(GotoTable[array<int, 2>{t1, int_a}]);

                    ActionTable[arr] = s;

                }
                continue;
            }

            if (closureList[t1][t2].ifEnd() == 1)
            {
                int int_a = 0;
                while (augItems[int_a].getName() != closureList[t1][t2].getLookForward().getName())
                    int_a++;
                array<int, 2>arr{ t1,int_a };
                string s;
                vector<Item> tmp1;
                tmp1 = closureList[t1][t2].getSentence().getHandleList();
                tmp1.pop_back();
                Sentence sen1(closureList[t1][t2].getSentence().getItem(), tmp1);
                int k1 = 0;
                while (augGrammarList[k1] != sen1)
                    k1++;
                s = "r" + to_string(k1);
                ActionTable[arr] = s;
            }

            if (closureList[t1][t2].ifEnd() == 1)
            {
                Sentence tmp(closureList[t1][t2].getSentence());
                vector<Item>tmp2(tmp.begin(), tmp.end() - 1);
                Sentence tmp3(tmp.getItem(), tmp2);

                if (tmp3 == augGrammarList[0] && closureList[t1][t2].getLookForward().getName() == "$")
                {
                    int tmp = 0;
                    while (augItems[tmp].getName() != "$")
                        tmp++;
                    array<int, 2>arr{ t1,tmp };
                    ActionTable[arr] = string("acc");
                }
                continue;
            }
        }
    }
}

vector<LRitem> Parser::go_to(vector<LRitem> I)
{
    vector<LRitem> GT;
    for (size_t i = 0; i < I.size(); i++)
    {
        GT.insert(GT.end(), I[i].getPointMovedRight());
    }
    return GT;
}

void Parser::go_to_Table()
{
    for (size_t t1 = 0; t1 < closureList.size(); t1++)
    {
        for (size_t t2 = 0; t2 < closureList[t1].size(); t2++)
        {
            if (closureList[t1][t2].ifEnd() == 0)//未结束
            {
                Item tmp = closureList[t1][t2].getItemBehindPoint();//点后的符号
                //if (tmp.isTerminal() == 0)//非终结符
                {
                    LRitem tmp2 = closureList[t1][t2].getPointMovedRight();
                    for (size_t t3 = 0; t3 < closureList.size(); t3++)
                    {
                        for (size_t t4 = 0; t4 < closureList[t3].size(); t4++)
                        {
                            if (closureList[t3][t4] == tmp2)
                            {
                                int tmp3 = 0;
                                while (augItems[tmp3] != tmp)
                                    tmp3++;
                                array<int, 2>arr{ t1, tmp3 };
                                GotoTable[arr] = t3;
                            }
                        }
                    }
                }
            }
        }
    }
}

void Parser::findFirstSet(set<string>&FS, vector<Item>arr)
{
    if (arr.size() == 0)
    {
        if (FS.count("empty") == 0)
            FS.insert("empty");
    }
    else
    {
        if (arr[0].isTerminal() == 1)
        {
            if (FS.count(arr[0].getName()) == 0)
                FS.insert(arr[0].getName());
        }
        else
            for (size_t cnt = 0; cnt < grammarList.size(); cnt++)
            {
                if (grammarList[cnt].getItem() == arr[0])
                {
                    vector<Item>tmp(arr);
                    tmp.erase(tmp.begin());
                    tmp.insert(tmp.begin(), grammarList[cnt].begin(), grammarList[cnt].end());
                    findFirstSet(FS, tmp);
                }
            }
    }
    return;
}

void Parser::LR0_insert(Sentence S)
{
    int length = S.size();
    for (int i = 0; i < length; i++)
    {
        vector<Item> tempList;
        if (S[0] == EMPTY)
        {
            tempList.push_back(EMPTY);
            augGrammarList.push_back(Sentence{ S.getItem(),tempList });
            return;
        }
        for (int j = 0; j < i; j++)
        {
            tempList.push_back(S[j]);
        }
        tempList.push_back(Point);
        for (int j = i; j < length; j++)
        {
            tempList.push_back(S[j]);
        }
        augGrammarList.push_back(Sentence{ S.getItem(),tempList});
    }
}

void Parser::augGrammar()
{
    int length = grammarList.size();
    Item begin = grammarList[0].getItem();
    auto temp = Sentence{ Start,vector<Item>{Point,begin} };
    augGrammarList.push_back(temp);
    temp = Sentence{ Start,vector<Item>{begin,Point} };
    augGrammarList.push_back(temp);


    for (int i = 0; i < length; i++)
    {
        Sentence temp = grammarList[i];
        LR0_insert(temp);
    }
}

void Parser::printAugGrammar()
{
    for (auto p : augGrammarList) {
        cout << p.getItem().getName() << " -> ";
        for (int i = 0; i < static_cast<int>(p.size()); i++) {
            cout << p[i].getName();
        }
        cout << endl;
    }
}

void Parser::first()
{
    for (size_t i = 0; i < grammarList.size(); i++)
    {
        vector<Item>tmp;
        tmp.insert(tmp.begin(), grammarList[i].begin(), grammarList[i].end());
        tool1(i, tmp);
    }
    for (size_t i = 0; i < items.size(); i++)
    {
        if (items[i].isTerminal() == 1)
        {
            firstSet[items[i].getName()].insert(items[i].getName());
        }
    }

    return;
}

void Parser::tool1(int i, vector<Item>arr)
{
    if (arr.size() == 0)
    {
        if (firstSet[grammarList[i].getItem().getName()].count("empty") == 0)
            firstSet[grammarList[i].getItem().getName()].insert("empty");
    }
    else
    {
        if (arr[0].isTerminal() == 1)
        {
            if (firstSet[grammarList[i].getItem().getName()].count(arr[0].getName()) == 0)
                firstSet[grammarList[i].getItem().getName()].insert(arr[0].getName());
        }
        else
            for (size_t cnt = 0; cnt < grammarList.size(); cnt++)
            {
                if (grammarList[cnt].getItem() == arr[0])
                {
                    vector<Item>tmp(arr);
                    tmp.erase(tmp.begin());
                    tmp.insert(tmp.begin(), grammarList[cnt].begin(), grammarList[cnt].end());
                    tool1(i, tmp);
                }
            }
    }
    return;
}

void Parser::printFirst()
{
    for (size_t cnt = 0; cnt < items.size(); cnt++)
    {

        string A;
        A = items[cnt].getName();
        cout << A << '\t';
        for (auto i = firstSet[A].begin(); i != firstSet[A].end(); i++)
        {
            cout << *i << '\t';
        }
        cout << endl;

    }
}

bool Parser::checkInOwnClosure(vector<LRitem> List, LRitem temp)
{
    int tag = 0;
    for (size_t i = 0; i < List.size(); i++)
    {
        if (List[i] == temp)
            tag++;
    }
    if (tag > 0)
        return true;
    else
        return false;
}

bool Parser::checkInClosure(vector<LRitem> List, LRitem temp)
{
    int tag=0;
    for(size_t i=0;i<List.size();i++)
    {
        if(List[i]==temp)
            tag++;
    }
    for (size_t i = 0; i < closureList.size(); i++)
    {
        vector<LRitem> T = closureList[i];
        for (size_t j = 0; j < T.size(); j++)
        {
            if (T[j] == temp)
                tag++;
        }
    }
    if(tag>0)
        return true;
    else
        return false;
}

Sentence Parser::headInsert(Sentence S)//头插Point
{
    Item I=S.getItem();
    vector<Item> L;
    L.push_back(Point);
    for(size_t i=0;i<S.size();i++)
    {
        L.push_back(S[i]);
    }
    Sentence s=Sentence{I,L};
    return s;
}

void Parser::insertClosure(vector<LRitem>&closure,LRitem L)
{
    auto alpha=L.alpha();
    auto beta=L.beta();
    auto b=L.B();
    auto a=L.getLookForward();
    if (b == EMPTY)
        return;
    for(size_t i=0;i<grammarList.size();i++) ///search for sentence
    {
        if(b==grammarList[i].getItem())
        {
            set<string> FS;
            beta.push_back(a);
            findFirstSet(FS, beta);
            for (auto f = FS.begin(); f != FS.end(); f++)
            {
                Item b = Item{ *f, 1 };
                Sentence s = headInsert(grammarList[i]);
                LRitem l = { s,b };
                if (!checkInOwnClosure(closure, l))
                {
                    closure.push_back(l);
                    insertClosure(closure, l);
                }
            }
        }
    }
}

void Parser::firstClosure()
{
    Item begin = grammarList[0].getItem();
    auto StartSentence = Sentence{ Start,vector<Item>{Point,begin} };
    auto StartLRitem = LRitem{StartSentence,Eof};
    vector<LRitem> closure;
    closure.push_back(StartLRitem);
    auto alpha=StartLRitem.alpha();
    auto beta=StartLRitem.beta();
    auto b=StartLRitem.B();
    auto a=StartLRitem.getLookForward();
    for(size_t i=0;i<grammarList.size();i++) ///search for sentence
    {
        if(b==grammarList[i].getItem())
        {
            set<string> FS;
            beta.push_back(a);
            findFirstSet(FS, beta);
            for(auto f=FS.begin();f!=FS.end();f++)
            {
                Item kr=Item {*f, 1};
                Sentence s = headInsert(grammarList[i]);
                LRitem l = { s,kr };
                if(!checkInClosure(closure,l))
                {
                    closure.push_back(l);
                    insertClosure(closure,l);
                }
            }
        }
    }
    closureList.push_back(closure);
}

bool Parser::lrNotInList(LRitem L)
{
    int tag = 0;
    for (size_t i = 0; i < closureList.size(); i++)
    {
        LRitem T = closureList[i][0];
        if (T == L)
            tag++;
    }
    if (tag == 0)
        return true;
    else
        return false;
}

vector<Item> Parser::nextPoint(vector<LRitem> L)
{
    size_t i = 0;
    vector<Item>next;
    for (i; i < L.size(); i++)
    {
        Item temp=L[i].getPointNext();
        if (temp == EMPTY)
            continue;
        else
        {
            int tag = 0;
            for (size_t j = 0; j < next.size(); j++)
            {
                if (next[j] == temp)
                    tag++;
            }
            if(tag==0)
                next.push_back(temp);
        }

    }
    return next;
}

void Parser::nextClosure(vector<LRitem> I_0)
{
    vector<Item>next = nextPoint(I_0);
    for (size_t i = 0; i < next.size(); i++)
    {
        Item temp = next[i];
        vector<LRitem> nextclosure;
        for (size_t j = 0; j < I_0.size(); j++)
        {
            if (I_0[j].getPointNext() == temp)
            {
                Item lookforward = I_0[j].getLookForward();
                Sentence S = I_0[j].getSentence();
                Item t = S.getItem();
                vector<Item> vec;
                size_t p = 0;
                while (p < S.size())
                {
                    if (S[p] == Point)
                        break;
                    p++;
                }
                for (size_t k = 0; k < p; k++)
                {
                    vec.push_back(S[k]);
                }
                vec.push_back(S[p + 1]);
                vec.push_back(Point);
                for (size_t k = p + 2; k < S.size(); k++)
                {
                    vec.push_back(S[k]);
                }
                Sentence temp_s = { t,vec };
                LRitem LR = { temp_s,lookforward };
                if (!checkInClosure(nextclosure,LR))
                {
                    nextclosure.push_back(LR);
                    insertClosure(nextclosure, LR);	//Ii内扩展
                }
            }
        }
        if (nextclosure.size() > 0)
        {
            closureList.push_back(nextclosure);
            nextClosure(nextclosure);
        }
    }
}

void Parser::closure()//计算closure集
{
    firstClosure();
    vector<LRitem> I_0=closureList[0];
    vector<Item>next = nextPoint(I_0);
    for (size_t i = 0; i < next.size(); i++)
    {
        Item temp = next[i];
        vector<LRitem> nextclosure;
        for (size_t j = 0; j < I_0.size(); j++)
        {
            if (I_0[j].getPointNext() == temp)
            {
                Item lookforward = I_0[j].getLookForward();
                Sentence S = I_0[j].getSentence();
                Item t = S.getItem();
                vector<Item> vec;
                size_t p = 0;
                while (p < S.size())
                {
                    if (S[p] == Point)
                        break;
                    p++;
                }
                for (size_t k = 0; k < p; k++)
                {
                    vec.push_back(S[k]);
                }
                vec.push_back(S[p + 1]);
                vec.push_back(Point);
                for (size_t k = p + 2; k < S.size(); k++)
                {
                    vec.push_back(S[k]);
                }
                Sentence temp_s = { t,vec };
                LRitem LR = { temp_s,lookforward };
                if (!checkInClosure(nextclosure, LR))
                {
                    nextclosure.push_back(LR);
                    insertClosure(nextclosure, LR);	//Ii内扩展
                }
            }
        }
        closureList.push_back(nextclosure);
        nextClosure(nextclosure);
    }
    if(closureList[closureList.size()-1].empty())
        closureList.pop_back();
}
#include<sstream>
QString Parser::printClosure()
{
    stringstream ss;
    for (size_t i = 0; i < closureList.size(); i++)
    {
        ss << 'I' << i << ':' << endl;
        for (size_t j = 0; j < closureList[i].size(); j++)
        {
            ss<<closureList[i][j].printLRitem();
        }
        ss << endl;
    }
    QString r;
    string ts;
    getline(ss,ts,'!');
    r =QString::fromStdString(ts);
    return r;
}

QString Parser::printResult()
{
    QString r;
    r += QString("Action Table & GoTo Table:\n\n\t");
    for (size_t tmp2 = 0; tmp2 < augItems.size(); tmp2++)
    {
        if (augItems[tmp2] == Start)
            continue;
        r += QString::fromStdString(augItems[tmp2].getName()) + "\t";
    }
    r += "\n";
    for (int tmp1 = 0; tmp1 < closureList.size(); tmp1++)
    {
        r += QString::number(tmp1) + "\t|";
        for (int tmp2 = 0; tmp2 < augItems.size(); tmp2++)
        {
            if (augItems[tmp2] == Start)
                continue;
            if (augItems[tmp2].isTerminal())
            {
                r += QString::fromStdString(ActionTable[array<int, 2>{tmp1, tmp2}]) + "\t";
            }
            else
            {
                if (GotoTable[array<int, 2>{tmp1, tmp2}] != 0)
                {
                    r += QString::number(GotoTable[array<int, 2>{tmp1, tmp2}]) + "\t";
                }
                else
                    r += "\t";
            }
            if(augItems[tmp2].getName().length()>8)
                r+='\t';
            r+='|';
        }
        r += "\n";
    }
    return r;
}

void Parser::printActionTable()
{
    cout << "\t";
    for (size_t tmp2 = 0; tmp2 < augItems.size(); tmp2++)
    {
        cout << augItems[tmp2].getName() << "\t";
    }
    cout << endl;
    for (int tmp1 = 0; tmp1 < closureList.size(); tmp1++)
    {
        cout << tmp1 << "\t";
        for (int tmp2 = 0; tmp2 < augItems.size(); tmp2++)
        {
            cout << ActionTable[array<int, 2>{tmp1, tmp2}] << "\t";
        }
        cout << endl;
    }

}

void Parser::printGotoTable()
{
    cout << "\t";
    for (size_t tmp2 = 0; tmp2 < augItems.size(); tmp2++)
    {
        cout << augItems[tmp2].getName() << "\t";
    }
    cout << endl;
    for (int tmp1 = 0; tmp1 < closureList.size(); tmp1++)
    {
        cout << tmp1 << "\t";
        for (int tmp2 = 0; tmp2 < augItems.size(); tmp2++)
        {
            cout << GotoTable[array<int, 2>{tmp1, tmp2}] << "\t";
        }
        cout << endl;
    }
}

void Parser::printClosureList()
{
    for (size_t t1 = 0; t1 < closureList.size(); t1++)
    {
        for (size_t t2 = 0; t2 < closureList[t1].size(); t2++)
        {
            cout << closureList[t1][t2].getSentence().getItem().getName() << "->";
            //for (auto t3 = closureList[t1][t2].getSentence().begin(); t3 != closureList[t1][t2].getSentence().end(); t3++)
            //	cout << (*t3).getName() << '\t';

            for (size_t t3 = 0; t3 < closureList[t1][t2].getSentence().size(); t3++)
                cout << closureList[t1][t2].getSentence()[t3].getName();

            cout << "\t\t" << closureList[t1][t2].getLookForward().getName();

            cout << endl;
        }

        cout << endl;
        cout << endl;
    }
}

QString Parser::printTree(TreeNode* root)
{
    map<string, int>m;
    queue<TreeNode*>q;
    string str = "";
    for (int i = 0; i < items.size(); i++)
    {
        m[items[i].getName()] = 0;
    }
    m[root->sign]++;
    root->sign += to_string(m[root->sign]);
    q.push(root);
    while (q.empty() == 0)
    {
        TreeNode* thisNode = q.front();
        q.pop();
        TreeNode* child;
        child = thisNode->FirstChild;
        while (child != NULL)
        {
            m[child->sign]++;
            child->sign += to_string(m[child->sign]);

            q.push(child);
            str += thisNode->sign + "->" + child->sign+"\n";

            child = child->NextSibling;
        }
    }
    return QString::fromStdString(str);
}

void Parser::initial()
{
    closureList.clear();

    auto& S_ = augItems[0];
    auto& S = augItems[1];
    auto& C = augItems[2];
    auto& c = augItems[3];
    auto& d = augItems[4];

    vector<LRitem>tmp0
    {
        LRitem{ Sentence{S_,vector<Item>{Point,S}},Eof },
        LRitem{ Sentence{S,vector<Item>{Point,C,C}},Eof },
        LRitem{ Sentence{C,vector<Item>{Point,c,C}},c },
        LRitem{ Sentence{C,vector<Item>{Point,c,C}},d },
        LRitem{ Sentence{C,vector<Item>{Point,d}},c },
        LRitem{ Sentence{C,vector<Item>{Point,d}},d }
    };

    vector<LRitem>tmp1{
        LRitem{ Sentence{S_,vector<Item>{S,Point}},Eof }
    };

    vector<LRitem>tmp2{
        LRitem{ Sentence{S,vector<Item>{C,Point,C}},Eof },
        LRitem{ Sentence{C,vector<Item>{Point,c ,C}},Eof },
        LRitem{ Sentence{C,vector<Item>{Point,d}},Eof }
    };

    vector<LRitem>tmp7{
        LRitem{ Sentence{C,vector<Item>{c,Point,C}},c },
        LRitem{ Sentence{C,vector<Item>{c,Point,C}},d },
        LRitem{ Sentence{C,vector<Item>{Point,c,C}},c },
        LRitem{ Sentence{C,vector<Item>{Point,c,C}},d },
        LRitem{ Sentence{C,vector<Item>{Point,d}},c },
        LRitem{ Sentence{C,vector<Item>{Point,d}},d },
    };

    vector<LRitem>tmp9{
        LRitem{ Sentence{C,vector<Item>{d,Point}},c },
        LRitem{ Sentence{C,vector<Item>{d,Point}},d }
    };

    vector<LRitem>tmp3{
        LRitem{ Sentence{S,vector<Item>{C,C,Point}},Eof }
    };

    vector<LRitem>tmp4{
        LRitem{ Sentence{C,vector<Item>{c,Point,C}},Eof },
        LRitem{ Sentence{C,vector<Item>{Point,c,C}},Eof },
        LRitem{ Sentence{C,vector<Item>{Point,d}},Eof }
    };

    vector<LRitem>tmp6{
        LRitem{ Sentence{C,vector<Item>{d,Point}},Eof }
    };

    vector<LRitem>tmp8{
        LRitem{ Sentence{C,vector<Item>{c,C,Point}},c },
        LRitem{ Sentence{C,vector<Item>{c,C,Point}},d }
    };

    vector<LRitem>tmp5{
        LRitem{ Sentence{C,vector<Item>{c,C,Point}},Eof }
    };

    closureList.push_back(tmp0);
    closureList.push_back(tmp1);
    closureList.push_back(tmp2);
    closureList.push_back(tmp3);
    closureList.push_back(tmp4);
    closureList.push_back(tmp5);
    closureList.push_back(tmp6);
    closureList.push_back(tmp7);
    closureList.push_back(tmp8);
    closureList.push_back(tmp9);

    return;

}
//获得Action表的列号
int get_item_num(vector<Item>itms,const string ch)
{
    int i;			//需要减去终结符的数量
//	int j = 0;


    for (i = 0; i < itms.size(); i++)
    {
//		if (!itm[i].isTerminal())
//			j++;

        if (ch == itms[i].getName())
            return i;
    }

    return -1;		//找不到该符号返回-1，否则返回对应列号
}

//获得Goto表的列号
int get_goto_num(vector<Item> itm, const string ch)
{
    int i;

    for (i = 0; i < itm.size(); i++)
    {
        if (ch == itm[i].getName())
            return i;
    }

    return -1;		//找不到该符号返回-1，否则返回对应列号
}

void Parser::Init_Tree(TreeNode* a)
{
    a->sign = "\0";

    a->FirstChild = NULL;


    a->NextSibling = NULL;
}


//移进时插入新节点
//void Parser::MoveIn_Tree(TreeNode *NewTree)
//{
//	TreeNode *p;

//	if (Root->FirstChild == NULL)		//如果是第一个插入的结点，插入在根节点下一层
//	{
//		Root->FirstChild = NewTree;
//		NewTree->FatherNode = Root;
//		NewTree->FirstChild = NULL;
//		NewTree->LastSibling = NULL;
//		NewTree->NextSibling = NULL;
//	}
//	else
//	{									//如果不是第一个插入的结点，则插入在第二层（根节点下一层）的最右侧
//		p = Root->FirstChild;
//		while (p->NextSibling)
//		{
//			p = p->NextSibling;
//		}
//		p->NextSibling = NewTree;
//		NewTree->FatherNode = Root;
//		NewTree->LastSibling = p;
//		NewTree->FirstChild = NULL;
//		NewTree->NextSibling = NULL;
//	}
//}

//归约时插入新节点
//void Parser::Reduction_Tree(TreeNode* NewTree,int num)		//num为归约产生式的右侧符号数量
//{
//	TreeNode* p;

//	int i;

//	//查找需要归约的结点---一定在第二层的右侧
//	p = Root->FirstChild;
//	while (p->NextSibling)
//	{
//		p = p->NextSibling;
//	}

//	for (i = 0; i < num; i++)
//	{
//		p = p->LastSibling;		//查找归约产生式的第一个符号
//	}

//	NewTree->FatherNode = p->FatherNode;
//	NewTree->LastSibling = p->LastSibling;
//	NewTree->FirstChild = p;
//	p->FatherNode = NewTree;
//	p->LastSibling->NextSibling = NewTree;
//	p->LastSibling = NULL;

//	while (p->NextSibling)
//	{
//		p->FatherNode = NewTree;
//		p = p->NextSibling;
//	}

//}
string Parser::log()
{
    string l;
    static int cnt=0;
    l +="\n"+to_string(cnt)+"\t";

    //栈
    stack<int>copyState=state_stack;
    string tmp="";
    while(copyState.empty()==0)
    {
        tmp.insert(0,to_string(copyState.top()));
        tmp+=" ";
        copyState.pop();
    }
    l+=tmp+"\t";

    //符号
    stack<string>copySign=sign_stack;
    string tmp2="";
    while(copySign.empty()==0)
    {
        tmp2.insert(0,copySign.top());
        tmp2+=" ";
        copySign.pop();
    }
    l+=tmp2+"\t";

    for(int i=0;i<buffer.size();i++)
    {
        l+=buffer[i];
    }
    l+="\t";

    cnt++;
    return l;
}


string Parser::Grammar_Tree(vector<string>a)
{
    string guocheng="";
    //Init_Tree();

    while (!state_stack.empty())
        state_stack.pop();
    while (!sign_stack.empty())
        sign_stack.pop();


    state_stack.push(0);
    sign_stack.push("$");

    //TreeNode *NewTree;

    int cur_state;		//当前状态
    int cur_sign_num;	//当前符号是第几个符号(列号)
    int new_state;		//下一个状态
    int new_sign_num;
    int sent_lenth;		//产生式长度
    string target;		//目标状态
    string cur_sign;	//当前符号
    array<int, 2> arr = { 0,0 };		//for actiontable
    array<int, 2> arr_goto = { 0,0 };	//for gototable

    int i = 0, j;
    int count = 0;

    stack<TreeNode*>treeStk;
//
    init_buffer(a);
    while (1)
    {
        cur_state = state_stack.top();
        cur_sign = buffer[0];

        cur_sign_num = get_item_num(augItems, cur_sign);

        if (cur_sign_num != -1)
        {
            arr[0] = cur_state;
            arr[1] = cur_sign_num;
            if (ActionTable[arr] == "acc")							//接受态
            {
                Root = treeStk.top();
                guocheng+=log();//日志
                guocheng+="接受";
                return guocheng;
            }
            if (ActionTable[arr][0] == 's')							//移进项
            {
                guocheng+=log();//日志

                new_sign_num = get_item_num(augItems,buffer[0]);
                arr[0] = cur_state;
                arr[1] = new_sign_num;
                target = ActionTable[arr].substr(1);
                new_state = stoi(target);

                state_stack.push(new_state);						//新状态入栈
                guocheng+="移入到"+to_string(new_state);//动作日志
                sign_stack.push(buffer[0]);

                buffer.erase(buffer.begin());

                TreeNode* newTree = new TreeNode;
                Init_Tree(newTree);
                newTree->sign = sign_stack.top();
                treeStk.push(newTree);
            }
            else if (ActionTable[arr][0] == 'r')						//归约项
            {
                guocheng+=log();//日志

                target = ActionTable[arr].substr(1);

                sent_lenth = augGrammarList[stoi(target)].size();			//sent_lenth=产生式右边长度(符号个数)
                for (j = 0; j < sent_lenth; j++)						//归约出栈
                {
                    state_stack.pop();
                    sign_stack.pop();
                }
                guocheng+="按照"+augGrammarList[stoi(target)].printSentence()+"规约";//动作日志
                sign_stack.push(augGrammarList[stoi(target)].getName());	//新入栈---归约后的符号

                cur_state = GotoTable[array<int, 2>{state_stack.top(), get_item_num(augItems,sign_stack.top())}];
                state_stack.push(cur_state);						//goto表中状态

                TreeNode* newTree = new TreeNode;
                Init_Tree(newTree);
                newTree->sign = sign_stack.top();

                TreeNode* lastTree = NULL;
                TreeNode* childTree = NULL;
                for (j = 0; j < sent_lenth; j++)
                {
                    childTree = treeStk.top();
                    treeStk.pop();
                    childTree->NextSibling = lastTree;
                    lastTree = childTree;
                }
                newTree->FirstChild = childTree;

                treeStk.push(newTree);

            }
            else
            {
                string a("不是该文法的句子。");
                return a;
            }
            //cout << endl;
        }
        else
        {															//出现不在符号表中的符号
            string a("不是该文法的句子。");
            return a;
        }
    }
}
void Parser::init_buffer(vector<string>abc)
{
    buffer=abc;
}
string Parser::printTree()
{
    map<string, int>m;
    queue<TreeNode*>q;
    string str = "";
    for (int i = 0; i < augItems.size(); i++)
    {
        m[augItems[i].getName()] = 0;
    }
    m[Root->sign]++;
    Root->sign += to_string(m[Root->sign]);
    q.push(Root);
    while (q.empty() == 0)
    {
        TreeNode* thisNode = q.front();
        q.pop();
        TreeNode* child;
        child = thisNode->FirstChild;
        while (child != NULL)
        {
            m[child->sign]++;
            child->sign += to_string(m[child->sign]);

            q.push(child);
            str += "s_"+thisNode->sign + "->" + "s_"+child->sign+";\n";

            child = child->NextSibling;
        }
    }
    string str2="";
    for (int i = 0; i < augItems.size(); i++)
    {
        for(int j=1;j<=m[augItems[i].getName()];j++)
        {
            str2+="s_"+augItems[i].getName()+to_string(j)+"[label=\""+augItems[i].getName()+"\"];\n";
        }

    }
    str=str2+"\n"+str;


    return str;
}
