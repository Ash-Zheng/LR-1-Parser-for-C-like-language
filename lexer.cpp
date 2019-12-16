#include "lexer.h"

lexer::lexer()
{}

string lexer::pretreatment()//每次调用都会返回一个处理过的、buffer_length字节长的字符串。
{
    static int p = 0;
    string s0 = "";
    int flag = 0;
    while (s0.length() < buffer_length)
    {
        if (str[p] == '$')
        {
            s0 += '$';
            break;
        }


        switch (flag)
        {
        case 0://初始状态
            if (str[p] == '\"' || str[p] == '\'')
            {
                quota_tag = !quota_tag;
                s0 += str[p++];
            }
            else if (quota_tag || (str[p] != ' ' && str[p] != '\t' && str[p] != '\n' && str[p] != '\r' && str[p] != '/'))
                s0 += str[p++];
            else if (str[p] == '/' && str[p + 1] == '/')
            {
                p += 2;
                flag = 2;
            }
            else if (str[p] == '/' && str[p + 1] == '*')
            {
                p += 2;
                flag = 3;
            }
            else
            {
                s0 += '\n';
                p++;
                flag = 1;
            }
            break;
        case 1://处理空格，\t，\n,\r
            if (str[p] != ' ' && str[p] != '\t' && str[p] != '\n' && str[p] != '\r')
            {
                flag = 0;
            }
            else
                p++;
            break;
        case 2://处理单行注释
            if (str[p] == '\n')
            {
                flag = 0;
            }
            else
                p++;
            break;
        case 3://处理多行注释
            if (str[p] == '*' && str[p + 1] == '/')
            {
                p += 2;
                flag = 0;
            }
            else
                p++;
            break;
        }
        {

        }
    }

    return s0;
}

string lexer::getStringFromBuffer(int p1, int p2)
{
    string s;
    if (p1 < buffer_length && p2 < buffer_length)
        s = buffer1.substr(p1, p2 - p1);
    else if (p1 < buffer_length && p2 >= buffer_length)
        s = buffer1.substr(p1, buffer_length - p1) + buffer2.substr(0, p2 - buffer_length);
    else if (p1 >= buffer_length && p2 >= buffer_length)
        s = buffer2.substr(p1 - buffer_length, p2 - p1);
    else if (p1 >= buffer_length && p2 < buffer_length)
        s = buffer2.substr(p1 - buffer_length, buffer_length * 2 - p1) + buffer1.substr(0, p2);

    return s;
}

void lexer::fresh_buffer(int& p1, int& p2)
{
    static int up_to_data_buffer = 1;
    switch (up_to_data_buffer)
    {
    case 1:
        if (p1 < buffer_length && p2 < buffer_length)
        {
            buffer2 = pretreatment();
            up_to_data_buffer = 2;
        }
        break;
    case 2:
        if (p1 >= buffer_length && p2 >= buffer_length)
        {
            buffer1 = pretreatment();
            up_to_data_buffer = 1;
        }
        break;
    default:
        break;
    }

    if (p1 >= buffer_length * 2)
        p1 -= buffer_length * 2;
    if (p2 >= buffer_length * 2)
        p2 -= buffer_length * 2;

}
string lexer::state_machine()//自动机
{
    int state = 0; //状态
    int quotation_mark = 0;//引号
    typetag = 0;
    string s = "";
    static int p1 = 0, p2 = 0;
    char c;

    while (state >= 0)
    {
        fresh_buffer(p1, p2);

        switch (state) //状态机
        {
        case 0: //初始状态
        {
            p1 = p2;

            if (p1 < buffer_length)
                c = buffer1[p1];
            else
                c = buffer2[p1 - buffer_length];

            typetag = 0;
            if (c - '0' >= 0 && c - '0' <= 9)//是数字
            {
                state = 1;
                p2++;
            }
            else if (c - 'a' >= 0 && c - 'a' <= 25)//是字母
            {
                state = 2;
                p2++;
            }
            else if (c == '_')//是字母
            {
                state = 2;
                p2++;
            }
            else if (c == ' ' || c == '\n' || c == '\r')//读到空格或者换行符或者\r
            {
                state = 0; //继续处于初始状态
                p2++;
            }
            else if (c == '\"')//读到双引号
            {
                state = 4;
                p2++;
            }
            else if (c == '\'')//读到单引号
            {
                state = 5;
                p2++;
            }
            else//读到了特殊符号，进入符号判断状态
            {
                state = 3;
            }
            break;
        }
        case 1://数字
        {
            if (p2 < buffer_length)
                c = buffer1[p2];
            else
                c = buffer2[p2 - buffer_length];

            if (c >= '0' && c <= '9' || c == '.')//是数字
            {
                p2++;
            }
            else//读到其他则结束
            {
                typetag = 1;
                state = -1;
            }
            break;
        }
        case 2://字符
        {
            if (p2 < buffer_length)
                c = buffer1[p2];
            else
                c = buffer2[p2 - buffer_length];


            if (c - 'a' >= 0 && c - 'a' <= 25)//是字母
            {
                p2++;
            }
            else if (c - '0' >= 0 && c - '0' <= 9)//是数字
            {
                p2++;
            }
            else if (c == '_')//是下划线
            {
                p2++;
            }
            else//读到其他则结束
            {
                typetag = 2;//结果是标识符
                state = -1;
            }
            break;
        }
        case 3://特殊符号
        {
            string c;
            c = getStringFromBuffer(p1, p2 + 1);

            //遍历界符的符号表，若符合则加入s，否则输入有误，进入纠错模式
            if (Check_Deli(c[0]))
            {
                p2++;
                state = -1;
                typetag = 3;
                break;
            }

            if (c[0] == '$')//文本结束符号
            {
                endtag = 1;
                state = -1;
                p2++;
                break;
            }

            //遍历操作符的符号表，若符合则加入s，否则输入有误，进入纠错模式
            if (Check_Op1(c))
            {
                fresh_buffer(p1, p2);
                c = getStringFromBuffer(p1, ++p2);

                while (Check_Op1(c.substr(c.length() - 1, 1)))
                {
                    fresh_buffer(p1, p2);
                    c = getStringFromBuffer(p1, ++p2);
                }
                p2--;
                c = getStringFromBuffer(p1, p2);
                if (Check_Op1(c))
                {
                    state = -1;
                    typetag = 4;
                }
                else
                {
                    state = -1;
                    typetag = -1;
                }
                break;
            }





            //if (state != -1)
            //	state = -2;//错误符号,错误处理,暂不考虑
            break;
        }
        ////////未做转义符处理
        case 4://字符串
        {
            if (p2 < buffer_length)
                c = buffer1[p2];
            else
                c = buffer2[p2 - buffer_length];

            //ASCII码可视符号范围为“ ”-“~”
            if (c == '\"')
            {
                quotation_mark++;
                p2++;
            }
            else if (c >= ' ' && c <= '~')
            {
                p2++;
            }

            if (quotation_mark)
            {
                typetag = 7;//结果是字符串
                state = -1;
                //quotation_mark = 0;
            }
            break;
        }
        case 5://char型变量值
        {
            if (p2 < buffer_length)
                c = buffer1[p2];
            else
                c = buffer2[p2 - buffer_length];

            //ASCII码可视符号范围为“ ”-“~”
            if (c == '\'')
            {
                quotation_mark++;
                p2++;
            }
            else if (c >= ' ' && c <= '~')
            {
                p2++;
            }

            if (quotation_mark)
            {
                typetag = 8;//结果是字符串
                state = -1;
                //quotation_mark = 0;
            }
            break;
        }
        }

        if (typetag == 2)//若为标识符，则需判断是否为保留字
        {
            int cnt = 0;

            s = getStringFromBuffer(p1, p2);

            for (int i = 0; i < len_Kw; i++)
            {
                if (Keyword[i] == s) {
                    cnt++;
                    typetag = 5; //是保留字
                }
            }
            if (cnt == 0)
            {
                Insert_Iden(s);
            }
        }
    }

    fresh_buffer(p1, p2);

    s = getStringFromBuffer(p1, p2);

    return s;
}

int lexer::Insert_Iden(string s)//插入标识符列表
{
    for (int i = 0; i < len_Iden; i++)
    {
        if (Iden[i] == s)
        {
            return 1;
        }
    }

    Iden[len_Iden++] = s;
    return 0;
}

int lexer::Check_Op1(string c)
{
    for (int i = 0; i < len_Op1; i++)
    {
        if (Op1[i] == c)
        {
            return 1;
        }
    }
    return 0;
}

int lexer::Check_Deli(char c)

{
    for (int i = 0; i < len_Deli; i++)
    {
        if (Deli[i] == c)
        {
            return 1;
        }
    }
    return 0;
}
vector<string> lexer::getItemsString(QString addr)//得到待规约串
{
    vector<string> vs;//解析好的单词串将会存在这个vector里面。

    initial();

    ifstream in(addr.toStdString());
    getline(in, str, '$');
    str += '$';

    buffer1 = pretreatment();

    while (!endtag)
    {
        string s;
        s = state_machine();
        if (s.size() != 0)
        {
            /*if (s == string(">") || s == string("<"))
                int aaa=1;*/
            switch (typetag)
            {
            case 1://常数
                vs.push_back(string("num"));
                break;
            case 2://标识符
                vs.push_back(string("id"));
                break;
            case 3://界符
                vs.push_back(s);
                break;
            case 4://操作符
                if (s == string(">") || s == string("<")|| s == string(">=")|| s == string("<=")|| s == string("==")|| s == string("!="))
                    vs.push_back(string("relop"));
                else
                    vs.push_back(s);
                break;
            case 5://保留字
                vs.push_back(s);
                break;
            case 6:cout << "< 注释, " << s << " >" << endl; break;
            case 7:
                cout << "< 界符, " << "\"" << " >" << endl;
                cout << "< 字符串, " << s.substr(1, s.length() - 2) << " >" << endl;
                cout << "< 界符, " << "\"" << " >" << endl;
                break;
            case 8:
                cout << "< 界符, " << "\'" << " >" << endl;
                cout << "< 字符串, " << s.substr(1, s.length() - 2) << " >" << endl;
                cout << "< 界符, " << "\'" << " >" << endl;
                break;
            case -1:cout << "< 未知类型, " << s << " >" << endl; break;
            }
        }
    }

    //查看解析好的单词串。
//	for (int i = 0; i < vs.size(); i++)
//	{
//		cout << vs[i] << '\n';
//	}
    //cout << "标识符表为:";
    //for (int i = 0; i < len_Iden; i++)//输出标识符列表，检查标识符是否正确插入标识符表
    //{
    //	cout << Iden[i] << " ";
    //}
    in.close();
    return vs;
}
void lexer::initial()
{
    len_Op1 = 28;//操作符表长度
    len_Kw = 28;//关键字表长度
    len_Deli = 12;//界符表长度
    len_Iden = 0;//标识符列表
    str="";
    for(int i=0;i<99;i++)
        Iden[i]="";
    typetag = 0;  //结果表示符，1为常数，2为标识符，3为操作符，4为界符，5为保留字，6为注释，7为字符串
    //int pls = 0;
    endtag = 0;
    quota_tag = 0;
}
