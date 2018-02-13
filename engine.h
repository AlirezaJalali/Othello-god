#ifndef ENGINE_H
#define ENGINE_H

#include <QWidget>

#define WHITE 1
#define BLACK -1
#define EMPTY 0

class Player;
class Board;

class Engine
{
public:
    explicit Engine();

    bool gameOver();
    bool canMove();
    bool hasMove(int x, int y);
    bool getTurn();
    int getMoveProfit(int x, int y);
    int8_t getState(int x, int y);
    void updateStateByMove(int x, int y);
    QPair<int, int> aiMove();
    void reset();

private:
    int8_t state[8][8];
    bool hasUpMove[8][8];
    bool hasDownMove[8][8];
    bool hasLeftMove[8][8];
    bool hasRightMove[8][8];

    bool hasUpLeftMove[8][8];
    bool hasUpRightMove[8][8];
    bool hasDownLeftMove[8][8];
    bool hasDownRightMove[8][8];
    int moveProfit[8][8];

    bool isBlackTurn;

    void updateMoves();
    void resetProfit();
    void changeTurn();

signals:

public slots:
};

#endif // GAMEENGINE_H
