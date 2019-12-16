#include "mainwindow.h"
#include "ui_mainwindow.h"





MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    vector<Item> items1{
//            Item{"S", 0},
//            Item{"C", 0},
//            Item{"c", 1},
//            Item{"d", 1}
//    };
//    auto &S = items1[0];
//    auto &C = items1[1];
//    auto &c = items1[2];
//    auto &d = items1[3];
//    vector<Sentence> grammar1{
//        Sentence{S, vector<Item>{C, C}},
//        Sentence{C, vector<Item>{c,C}},
//        Sentence{C, vector<Item>{d}}
//    };
    vector<Item> items1{
            Item{"S", 0},
            Item{"B", 0},
            Item{"a", 1},
            Item{"b", 1}
    };
    auto &S = items1[0];
    auto &B = items1[1];
    auto &a = items1[2];
    auto &b = items1[3];
    vector<Sentence> grammar1{
        Sentence{S, vector<Item>{B, B}},
        Sentence{B, vector<Item>{a,B}},
        Sentence{B, vector<Item>{b}}
    };

    p1=Parser(grammar1, items1);
    p1.augGrammar();
    p1.closure();
    p1.aug();
    p1.go_to_Table();
    p1.action();

    vector<Item> items2{
        Item{"program", 0},
        Item{"type", 0},
        Item{"block", 0},
        Item{"declare", 0},
        Item{"sequence", 0},
        Item{"varDecl", 0},
        Item{"sentence", 0},
        Item{"assigns", 0},
        Item{"returns", 0},
        Item{"returnValue", 0},
        Item{"whiles", 0},
        Item{"ifs", 0},
        Item{"elses", 0},
        Item{"expr", 0},
        Item{"unused1", 0},
        Item{"unused2", 0},
        Item{"unused3", 0},
        Item{"unused4", 0},
        Item{"unused5", 0},
        Item{"unused6", 0},
        Item{"unused7", 0},
        Item{"unused8", 0},
        Item{"compute_operator", 0},
        Item{"compute", 0},
        Item{"compare", 0},

        Item{"id", 1},
        Item{"(", 1},
        Item{")", 1},
        Item{"int", 1},
        Item{"void", 1},
        Item{"{", 1},
        Item{"}", 1},
        Item{"=", 1},
        Item{"return", 1},
        Item{"while", 1},
        Item{"if", 1},
        Item{"else", 1},
        Item{"relop", 1},
        Item{"*", 1},
        Item{"/", 1},
        Item{"+", 1},
        Item{"-", 1},
        Item{"num", 1},
        Item{";", 1},
        Item{",", 1},
    };
    auto& program = items2[0];
    auto& type = items2[1];
    auto& block = items2[2];
    auto& declare = items2[3];
    auto& sequence = items2[4];
    auto& varDecl = items2[5];
    auto& sentence = items2[6];
    auto& assigns = items2[7];
    auto& returns = items2[8];
    auto& returnValue = items2[9];
    auto& whiles = items2[10];
    auto& ifs = items2[11];
    auto& elses = items2[12];
    auto& expr = items2[13];
    auto& restExpr = items2[14];
    auto& addExpr = items2[15];
    auto& restAddExpr = items2[16];
    auto& term = items2[17];
    auto& restTerm = items2[18];
    auto& factor = items2[19];
    auto& funcType = items2[20];
    auto& call = items2[21];
    auto& compute_operator = items2[22];
    auto& compute = items2[23];
    auto& compare = items2[24];

    auto& id = items2[25];
    auto& left1 = items2[26];
    auto& right1 = items2[27];
    auto& int1 = items2[28];
    auto& void1 = items2[29];
    auto& left2 = items2[30];
    auto& right2 = items2[31];
    auto& equ = items2[32];
    auto& return1 = items2[33];
    auto& while1 = items2[34];
    auto& if1 = items2[35];
    auto& else1 = items2[36];
    auto& relop = items2[37];
    auto& mult = items2[38];
    auto& div = items2[39];
    auto& add = items2[40];
    auto& minus = items2[41];
    auto& num = items2[42];
    auto& sem = items2[43];
    auto& com = items2[44];

    vector<Sentence> grammar2{
            Sentence{program, vector<Item>{type,id,left1,right1,block}},
            Sentence{type, vector<Item>{int1}},
            Sentence{type, vector<Item>{void1}},
            Sentence{block, vector<Item>{left2,declare,sequence,right2}},////
            Sentence{declare, vector<Item>{}},
            Sentence{declare, vector<Item>{varDecl,sem,declare}},
            Sentence{varDecl, vector<Item>{int1,id}},

            Sentence{sequence, vector<Item>{}},
            Sentence{sequence, vector<Item>{sentence,sequence}},

            Sentence{sentence, vector<Item>{ifs}},
            Sentence{sentence, vector<Item>{whiles}},
            Sentence{sentence, vector<Item>{returns}},
            Sentence{sentence, vector<Item>{assigns}},

            Sentence{assigns, vector<Item>{id,equ,expr,sem}},////这里加了sem
            Sentence{returns, vector<Item>{return1,returnValue,sem}},
            Sentence{returnValue, vector<Item>{}},
            Sentence{returnValue, vector<Item>{expr}},
            Sentence{whiles, vector<Item>{while1,left1,expr,right1,block}},
            Sentence{ifs, vector<Item>{if1,left1,expr,right1,block,elses}},
            Sentence{elses, vector<Item>{}},
            Sentence{elses, vector<Item>{else1,block}},
//           Sentence{expr, vector<Item>{addExpr,restExpr}},


            Sentence{expr, vector<Item>{num}},
            Sentence{expr, vector<Item>{compare}},
            Sentence{expr, vector<Item>{compute}},

            Sentence{compare, vector<Item>{id,relop,id}},
            Sentence{compare, vector<Item>{id,relop,num}},
            Sentence{compare, vector<Item>{num,relop,id}},
            Sentence{compare, vector<Item>{num,relop,num}},

            Sentence{compute, vector<Item>{id,compute_operator,id}},
            Sentence{compute, vector<Item>{id,compute_operator,num}},
            Sentence{compute, vector<Item>{num,compute_operator,id}},
            Sentence{compute, vector<Item>{num,compute_operator,num}},

            Sentence{compute_operator, vector<Item>{mult}},
            Sentence{compute_operator, vector<Item>{div}},
            Sentence{compute_operator, vector<Item>{add}},
            Sentence{compute_operator, vector<Item>{minus}}

//            Sentence{restExpr, vector<Item>{}},
//            Sentence{restExpr, vector<Item>{relop,   addExpr,restExpr}},
//            Sentence{addExpr, vector<Item>{term,restAddExpr}},
//            Sentence{restAddExpr, vector<Item>{}},
//            Sentence{restAddExpr, vector<Item>{add,term,restAddExpr}},
//            Sentence{restAddExpr, vector<Item>{minus,term,restAddExpr}},
//            Sentence{term, vector<Item>{factor,restTerm}},
//            Sentence{restTerm, vector<Item>{}},
//            Sentence{restTerm, vector<Item>{mult,factor,restTerm}},
//            Sentence{restTerm, vector<Item>{div,factor,restTerm}},
//            Sentence{factor, vector<Item>{num}},
//            Sentence{factor, vector<Item>{left1,expr,right1}},
//            Sentence{factor, vector<Item>{}},
//            Sentence{factor, vector<Item>{id}}
//            Sentence{factor, vector<Item>{id,funcType}},
//            Sentence{funcType, vector<Item>{call}},
//            Sentence{funcType, vector<Item>{}},
//            Sentence{call, vector<Item>{left1,argList,right1}},
//            Sentence{args, vector<Item>{argList}},
//            Sentence{args, vector<Item>{}},
//            Sentence{argList, vector<Item>{expr,restArgList}},
//            Sentence{restArgList, vector<Item>{}},
//            Sentence{restArgList, vector<Item>{com,expr,restArgList}}
        };
    p2=Parser(grammar2, items2);
    p2.augGrammar();
    p2.closure();
    p2.aug();
    p2.go_to_Table();
    p2.action();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton1_clicked(bool checked)
{

    QString r;
    if(ui->radioButton->isChecked())
    {
        r=p1.printResult();
    }
    else if(ui->radioButton_2->isChecked())
    {
        r=p2.printResult();
    }
    ui->textBrowser->setText(r);
}

void MainWindow::on_pushButton2_clicked(bool checked)
{
    QString r;
    if(ui->radioButton->isChecked())
    {
        r=p1.printClosure();
    }
    else if(ui->radioButton_2->isChecked())
    {
        r=p2.printClosure();
    }
    ui->textBrowser->setText(r);

}

void MainWindow::on_pushButton4_clicked(bool checked)
{
    QString r;
    if(ui->radioButton->isChecked())
    {
        r=QString::fromStdString(p1.printTree());
    }
    else if(ui->radioButton_2->isChecked())
    {
        r=QString::fromStdString(p2.printTree());
    }
    ui->textBrowser->setText(r);
}

void MainWindow::on_pushButton3_clicked(bool checked)
{
    vector<string>vs;
    QString r;
    vs.clear();
    if(ui->radioButton_2->isChecked())
    {
        QString curPath=QDir::currentPath();//获取系统当前目录
        //获取应用程序的路径
        QString dlgTitle="选择一个文件"; //对话框标题
        QString filter="文本文件(*.txt);;所有文件(*.*)"; //文件过滤器
        QString aFileName=QFileDialog::getOpenFileName(this,dlgTitle,curPath,filter);
        lexer lex;

        vs=lex.getItemsString(aFileName);
        vs.push_back("$");

//        vs.clear();
//        vs.push_back("int");
//        vs.push_back("id");
//        vs.push_back("(");
//        vs.push_back(")");
//        vs.push_back("{");
//        vs.push_back("}");
//        vs.push_back("$");

        string log=p2.Grammar_Tree(vs);
        string err("不是该文法的句子。");
        if(log==err)
            r=QString::fromStdString(err);
        else
        {
            r=QString::fromStdString(log)+"\n";
            //r+=QString::fromStdString(p2.printTree());
        }
    }
    else if(ui->radioButton->isChecked())
    {
        vs.clear();
        vs.push_back("a");
        vs.push_back("a");
        vs.push_back("b");
        vs.push_back("b");
        vs.push_back("$");

        string log=p1.Grammar_Tree(vs);
        string err("不是该文法的句子。");
        if(log==err)
            r=QString::fromStdString(err);
        else
        {
            r=QString::fromStdString(log)+"\n";
            //r+=QString::fromStdString(p1.printTree());
        }

    }

    ui->textBrowser->setText(r);

}
