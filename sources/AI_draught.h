#pragma once

#include <QtWidgets/QMainWindow>
#include<QDebug>
#include <QCloseEvent>
#include "ui_AI_draught.h"
#include "AI_engine.h"

extern char INPUT[50];
extern struct Command OUTPUT;

class AI_draught : public QMainWindow
{
    Q_OBJECT

public:
    QPushButton* flag[8][8];
    QPushButton* draw;
    QPushButton* surrender;
    QPushButton* help;
    QPushButton* finshMove;
    QAction* start_white;
    QAction* start_black;

    char tag[10] = { 0 };
public:
    AI_draught(QWidget *parent = Q_NULLPTR);
    void get_flag();//绑定棋子
    void draw_board();//画棋盘
    void gameLoop();//主循环
private:
    Ui::AI_draughtClass ui;
    struct Command playerCmd;
    char playerCmdString[100];
    bool isclose;
private:
    void lockButton();
    void unlockButton();
    void gameOver(int winner);
    void lockcmd(int sw);

protected slots:    //使用slots关键字 槽函数
    void sWhite();
    void sBlack();
    void getButtonLoc();
    void finishCMD();
    void Surrender();
    void Help();
    void Draw();
protected:
    void closeEvent(QCloseEvent *event);
};
