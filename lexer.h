#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include<vector>
#include<QString>

//此处更改缓冲区大小
#define buffer_length 20

using namespace std;

class lexer
{
private:
    string str;

    int len_Op1 = 28;//操作符表长度
    int len_Kw = 28;//关键字表长度
    int len_Deli = 12;//界符表长度
    int len_Iden = 0;//标识符列表

    const char* Keyword[30] = { "int","main","if","else","return",
                              "string","double","float","char","void",
                              "bool","short","long int","signed","unsigned",
                              "struct","class","define","const","for",
                              "while","continue","break","switch","goto",
                              "case","else if","include"
    };//关键字
    //const char Op1[] = { '+','-','=','>','<' };//操作符

    const string Op1[30] = { "+","-","=","++","--",
                           "*","/","%","==","!=",
                           ">","<",">=","<=","&","&&",
                           "|","||","<<",">>","^","~",
                           "+=","-=","*=","/=","#",
                           ",",".","->"
    };//操作符



    const char Deli[12] = { ',',';','{','}','(',')',
                          '\"','\'','[',']'
    };//界符

    string Iden[99];//标识符列表

    int typetag = 0;  //结果表示符，1为常数，2为标识符，3为操作符，4为界符，5为保留字，6为注释，7为字符串
    //int pls = 0;
    int endtag = 0;

    int Insert_Iden(string);//插入标识符列表
    int Check_Op1(string);	//检测是否为操作符
    int Check_Deli(char);	//检测是否为界符

    bool quota_tag = 0;

    string pretreatment();//每次调用都会返回一个处理过的、buffer_length字节长的字符串。
    string buffer1, buffer2;

    string getStringFromBuffer(int p1, int p2);

    void fresh_buffer(int& p1, int& p2);

    string state_machine();//自动机

public:
    lexer();
    void initial();

    vector<string>getItemsString(QString addr);//得到待规约串
};

#endif // LEXER_H
