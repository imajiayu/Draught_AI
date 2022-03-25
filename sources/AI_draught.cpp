#include "AI_draught.h"
#include "AI_engine.h"
#include <QtWidgets/QMessageBox>
#include <cstring>
extern char trueBoard[BOARD_SIZE][BOARD_SIZE];
extern int myFlag;
extern int numFlag[2];
extern int roundNum;

AI_draught::AI_draught(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setFixedSize(400, 464);

    setWindowTitle(QString::fromUtf16(u"国际跳棋小程序"));
    setWindowIcon(QIcon("./vivado_logo.ico"));

    start_white = QWidget::findChild<QAction*>("start_white");
    connect(start_white, SIGNAL(triggered()), this, SLOT(sWhite()));

    start_black = QWidget::findChild<QAction*>("start_black");
    connect(start_black, SIGNAL(triggered()), this, SLOT(sBlack()));

    draw = QWidget::findChild<QPushButton*>("Draw");
    connect(draw, SIGNAL(clicked()), this, SLOT(Draw()));

    surrender = QWidget::findChild<QPushButton*>("Surrender");
    connect(surrender, SIGNAL(clicked()), this, SLOT(Surrender()));

    help = QWidget::findChild<QPushButton*>("Help");
    connect(help, SIGNAL(clicked()), this, SLOT(Help()));

    finshMove = QWidget::findChild<QPushButton*>("Finish_move");
    connect(finshMove, SIGNAL(clicked()), this, SLOT(finishCMD()));

    isclose = false;
    
    get_flag();
    draw_board();

    lockButton();
    lockcmd(1);

    strcpy(tag,IDLE);

}

void AI_draught::get_flag() {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++) {
            if ((i + j) % 2)
            {
                QString objectName;
                objectName[0] = '_';
                objectName[1] = i + '0';
                objectName[2] = '_';
                objectName[3] = j + '0';
                flag[i][j] = QWidget::findChild<QPushButton*>(objectName);
                connect(flag[i][j], SIGNAL(clicked()), this, SLOT(getButtonLoc()));
            }
        }
}

void AI_draught::draw_board()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            switch (trueBoard[i][j])
            {
            case EMPTY:
                if ((i + j) % 2)
                    flag[i][j]->setStyleSheet(tr("background-image: url(./green_empty.png);"));
                    break;
            case ENEMY_FLAG:
                flag[i][j]->setStyleSheet(tr("background-image: url(./black_flag.png);"));
                break;
            case MY_FLAG:
                flag[i][j]->setStyleSheet(tr("background-image: url(./white_flag.png);"));
                break;
            case ENEMY_KING:
                flag[i][j]->setStyleSheet(tr("background-image: url(./black_king.png);"));
                break;
            case MY_KING:
                flag[i][j]->setStyleSheet(tr("background-image: url(./white_king.png);"));
                break;
            default:
                break;
            }
        }
    }
}

void AI_draught::sWhite()  //槽函数与信号函数一直，没有参数
{
    lockcmd(0);
    memset(playerCmdString, 0, sizeof(playerCmdString));
    strcpy(INPUT, "1");
    loop();
    unlockButton();
    draw_board();
    strcpy(tag, PLACE);
}

void AI_draught::sBlack()
{
    lockcmd(0);
    memset(playerCmdString, 0, sizeof(playerCmdString));
    strcpy(INPUT, "0");
    loop();
    draw_board();
    strcpy(INPUT, "TURN");
    loop();
    unlockButton();
    draw_board();
    strcpy(tag, PLACE);
}

void AI_draught::gameLoop()
{
    while (true)
    {
        if (isclose)
            break;
        if (numFlag[0] == 0&&strcmp(tag,IDLE))
            gameOver(0);
        else if (numFlag[1] == 0&&strcmp(tag, IDLE))
            gameOver(1);

        QCoreApplication::processEvents();
        if (strcmp(tag, PLACE) == 0)
        {
            unlockButton();
            playerCmd.numStep = 0;
            while(strcmp(tag,PLACE)==0)
                QCoreApplication::processEvents();
        }
        else if (strcmp(tag, TURN) == 0)
        {
            lockButton();
            strcpy(INPUT, "TURN");
            loop();
            draw_board();
            strcpy(tag, PLACE);
        }
        memset(INPUT, 0, sizeof(INPUT));
    }
}


void AI_draught::getButtonLoc()
{
    QString loc=dynamic_cast<QPushButton*>(sender())->objectName();
    playerCmd.x[playerCmd.numStep] = loc[1].toLatin1()-'0';
    playerCmd.y[playerCmd.numStep] = loc[3].toLatin1() - '0';
    playerCmd.numStep++; 
}

void AI_draught::lockButton()
{
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if ((i + j) % 2)
                flag[i][j]->setDisabled(true);
}

void AI_draught::unlockButton()
{
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if ((i + j) % 2)
                switch (myFlag)
                {
                case 1:
                    if (trueBoard[i][j] != ENEMY_FLAG && trueBoard[i][j]!=ENEMY_KING)
                        flag[i][j]->setDisabled(false);
                    break;
                case 0:
                    if (trueBoard[i][j] != MY_FLAG && trueBoard[i][j] != MY_KING)
                        flag[i][j]->setDisabled(false);
                    break;
                default:
                    break;
                }
                
}

void AI_draught::finishCMD()
{
    strcpy(playerCmdString,"PLACE ");
    strcat(playerCmdString, std::to_string(playerCmd.numStep).c_str());
    strcat(playerCmdString, " ");
    for (int i = 0; i < playerCmd.numStep; i++)
    {
        strcat(playerCmdString,std::to_string(playerCmd.x[i]).c_str());
        strcat(playerCmdString,",");
        strcat(playerCmdString, std::to_string(playerCmd.y[i]).c_str());
        strcat(playerCmdString, " ");
    }
    strcpy(INPUT, playerCmdString);
    loop();
    qDebug() << OUTPUT.numStep;
    if (OUTPUT.numStep != -1)
    {
        draw_board();
        lockButton();
        strcpy(tag, TURN);
    }
    else
    {
        if(playerCmd.numStep==0)
            QMessageBox::critical(this, QString::fromUtf16(u"错误"), QString::fromUtf16(u"尚未选择走子路径！"));
        else
            QMessageBox::critical(this, QString::fromUtf16(u"错误"), QString::fromUtf16(u"走子错误，请按照规则重新选择走子路径！"));
        memset(playerCmdString, 0, sizeof(playerCmdString));
        playerCmd.numStep = 0;
        return;
    }
    memset(playerCmdString, 0, sizeof(playerCmdString));
}

void AI_draught::gameOver(int winner)
{

    if(winner==1)
        QMessageBox::about(this, QString::fromUtf16(u"游戏结束"), QString::fromUtf16(u"黑方胜利！"));
    else if(winner==0)
        QMessageBox::about(this, QString::fromUtf16(u"游戏结束"), QString::fromUtf16(u"白方胜利！"));
    lockButton();
    lockcmd(1);
}

void AI_draught::Surrender()
{
    gameOver(myFlag);
}

void AI_draught::Help()
{
    QMessageBox::about(this, QString::fromUtf16(u"帮助"), QString::fromUtf16(u"<a href = 'https://m.idongde.com/c/Ef0D0aE414abBc37.shtml'>国际跳棋规则< / a>"));
}

void AI_draught::Draw()
{
    QMessageBox::about(this, QString::fromUtf16(u"提示"), QString::fromUtf16(u"对方拒绝和棋！"));
}

void AI_draught::lockcmd(int sw)
{
    if (sw)
    {
        surrender->setDisabled(true);
        finshMove->setDisabled(true);
        draw->setDisabled(true);
    }
    else
    {
        surrender->setDisabled(false);
        finshMove->setDisabled(false);
        draw->setDisabled(false);
    }
}

void AI_draught::closeEvent(QCloseEvent* event)
{
    isclose = true;
    event->accept();
}

