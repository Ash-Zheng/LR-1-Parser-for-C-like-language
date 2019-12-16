#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <Queue>
#include "Parser.h"
#include "Sentence.h"
#include "Item.h"
#include <iostream>
#include <sstream>
#include<lexer.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton1_clicked(bool checked);

    void on_pushButton2_clicked(bool checked);

    void on_pushButton4_clicked(bool checked);

    void on_pushButton3_clicked(bool checked);

private:
    Ui::MainWindow *ui;

    Parser p1;
    Parser p2;

};

#endif // MAINWINDOW_H
