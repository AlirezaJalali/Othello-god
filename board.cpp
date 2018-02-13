#include "board.h"
#include "ui_board.h"

#include <QMainWindow>
#include <QGridLayout>
#include "engine.h"
#include <QPushButton>
#include <QDialogButtonBox>
#include <QIcon>
#include <QSize>
#include <QTimer>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>


Board::Board(bool blackHumanity, bool whiteHumanity, QWidget *parent) :
    QMainWindow(parent), blackIsHuman(blackHumanity), whiteIsHuman(whiteHumanity)
{
    engine = new Engine();

    whiteIcon = new QIcon(":/images/white.png");
    blackIcon = new QIcon(":/images/black.png");
    validIcon = new QIcon(":/images/validgreen.png");
    emptyIcon = new QIcon(":/images/green.png");


    this->setFixedSize(BOARD_WIDTH*8, BOARD_HEIGHT*8 + 30);
    initializeButtons();
    updateIcons();

    menuBar()->setGeometry(0, 0, BOARD_WIDTH*8, 30);
    menuBar()->addAction("Player vs AI", this, SLOT(resetHumanAi()));
    menuBar()->addAction("AI vs Player", this, SLOT(resetAiHuman()));
    menuBar()->addAction("2 Player", this, SLOT(reset2Player()));
    menuBar()->addAction("AI vs AI", this, SLOT(reset2Ai()));
}

void Board::initializeButtons()
{
    int x = 0;
    int y = 0;
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            cells[i][j] = new QPushButton(this);
            connect(cells[i][j], SIGNAL(clicked()), this, SLOT(clickHandler()));
            cells[i][j]->setFlat(true);
            cells[i][j]->setObjectName(QString::number(i) + QString::number(j));
            cells[i][j]->setIconSize(QSize(BOARD_WIDTH, BOARD_HEIGHT));
            cells[i][j]->setGeometry(x, y + 30, BOARD_WIDTH, BOARD_HEIGHT);
            y += BOARD_WIDTH;
        }
        x += BOARD_HEIGHT;
        y = 0;
    }
}

void Board::updateScores()
{
    int x = 0;
    int y = 0;

    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
        {
            if (engine->getState(i, j) == BLACK)
                x++;
            else if (engine->getState(i, j) == WHITE)
                y++;
        }

    this->setWindowTitle("Black Score: " + QString::number(x) + " | White Score: " + QString::number(y));
}

void Board::updateIcons()
{
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
        {
            int8_t state = engine->getState(i, j);
            if (state == WHITE)
                cells[i][j]->setIcon(*whiteIcon);
            else if (state == BLACK)
                cells[i][j]->setIcon(*blackIcon);
            else if (state == EMPTY)
            {
                if (engine->hasMove(i, j))
                    cells[i][j]->setIcon(*validIcon);
                else
                    cells[i][j]->setIcon(*emptyIcon);
            }
        }

    updateScores();
    if ((engine->getTurn() && !blackIsHuman) || (engine->getTurn() == false && !whiteIsHuman))
        QTimer::singleShot(AI_DELAY, this, SLOT(aiPlay()));
}

void Board::clickHandler()
{
    if ((engine->getTurn() && blackIsHuman) || (engine->getTurn() == false && whiteIsHuman))
    {
        QString name = sender()->objectName();
        int x = name.at(0).digitValue();
        int y = name.at(1).digitValue();
        if (engine->hasMove(x,y))
        {
            engine->updateStateByMove(x, y);
            updateIcons();
        }
    }

}

void Board::aiPlay()
{
    QPair<int, int> move = engine->aiMove();
    engine->updateStateByMove(move.first, move.second);
    updateIcons();
}

void Board::resetHumanAi()
{
    engine->reset();
    blackIsHuman = true;
    whiteIsHuman = false;
    updateIcons();
}

void Board::resetAiHuman()
{
    engine->reset();
    blackIsHuman = false;
    whiteIsHuman = true;
    updateIcons();
}

void Board::reset2Player()
{
    engine->reset();
    blackIsHuman = true;
    whiteIsHuman = true;
    updateIcons();
}

void Board::reset2Ai()
{
    engine->reset();
    blackIsHuman = false;
    whiteIsHuman = false;
    updateIcons();
}
