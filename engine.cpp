#include "engine.h"
#include "board.h"

Engine::Engine()
{
    isBlackTurn = true;
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            state[i][j] = EMPTY;

    resetProfit();

    state[3][3] = WHITE;
    state[4][4] = WHITE;
    state[3][4] = BLACK;
    state[4][3] = BLACK;

    updateMoves();
}
void Engine::reset()
{
    isBlackTurn = true;
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            state[i][j] = EMPTY;

    resetProfit();

    state[3][3] = WHITE;
    state[4][4] = WHITE;
    state[3][4] = BLACK;
    state[4][3] = BLACK;

    resetProfit();
    updateMoves();
}

bool Engine::hasMove(int x, int y)
{
    if (moveProfit[x][y])
        return true;

    return false;
}

bool Engine::canMove()
{
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            if (hasMove(i, j))
                return true;

    return false;
}

void Engine::resetProfit()
{
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            moveProfit[i][j] = 0;
}

QPair<int, int> Engine::aiMove()
{
    QPair<int, int> pair;
    pair.first = -1;
    pair.second = -1;
    int profit = 0;
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            if (moveProfit[i][j] > profit)
            {
                profit = moveProfit[i][j];
                pair.first = i;
                pair.second = j;
            }

    return pair;
}

int8_t Engine::getState(int x, int y)
{
    return state[x][y];
}

bool Engine::getTurn()
{
    return isBlackTurn;
}

void Engine::changeTurn()
{
    isBlackTurn = !isBlackTurn;
}

void Engine::updateStateByMove(int x, int y)
{
    if (isBlackTurn)
    {
        state[x][y] = BLACK;
        if (hasUpMove[x][y])
            for (int i = x-1; state[i][y] == WHITE; --i)
                state[i][y] = BLACK;
        if (hasDownMove[x][y])
            for (int i = x+1; state[i][y] == WHITE; ++i)
                state[i][y] = BLACK;
        if (hasRightMove[x][y])
            for (int i = y+1; state[x][i] == WHITE; ++i)
                state[x][i] = BLACK;
        if (hasLeftMove[x][y])
            for (int i = y-1; state[x][i] == WHITE; --i)
                state[x][i] = BLACK;

        if (hasUpLeftMove[x][y])
            for (int i = 1; state[x-i][y-i] == WHITE; ++i)
                state[x-i][y-i] = BLACK;
        if (hasUpRightMove[x][y])
            for (int i = 1; state[x-i][y+i] == WHITE; ++i)
                state[x-i][y+i] = BLACK;
        if (hasDownLeftMove[x][y])
            for (int i = 1; state[x+i][y-i] == WHITE; ++i)
                state[x+i][y-i] = BLACK;
        if (hasDownRightMove[x][y])
            for (int i = 1; state[x+i][y+i] == WHITE; ++i)
                state[x+i][y+i] = BLACK;
    }
    else
    {
        state[x][y] = WHITE;
        if (hasUpMove[x][y])
            for (int i = x-1; state[i][y] == BLACK; --i)
                state[i][y] = WHITE;
        if (hasDownMove[x][y])
            for (int i = x+1; state[i][y] == BLACK; ++i)
                state[i][y] = WHITE;
        if (hasRightMove[x][y])
            for (int i = y+1; state[x][i] == BLACK; ++i)
                state[x][i] = WHITE;
        if (hasLeftMove[x][y])
            for (int i = y-1; state[x][i] == BLACK; --i)
                state[x][i] = WHITE;

        if (hasUpLeftMove[x][y])
            for (int i = 1; state[x-i][y-i] == BLACK; ++i)
                state[x-i][y-i] = WHITE;
        if (hasUpRightMove[x][y])
            for (int i = 1; state[x-i][y+i] == BLACK; ++i)
                state[x-i][y+i] = WHITE;
        if (hasDownLeftMove[x][y])
            for (int i = 1; state[x+i][y-i] == BLACK; ++i)
                state[x+i][y-i] = WHITE;
        if (hasDownRightMove[x][y])
            for (int i = 1; state[x+i][y+i] == BLACK; ++i)
                state[x+i][y+i] = WHITE;
    }

    changeTurn();
    resetProfit();
    updateMoves();

    if (gameOver())
        return;

    if (!canMove())
    {
        changeTurn();
        resetProfit();
        updateMoves();
    }
}

bool Engine::gameOver()
{
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            if (state[i][j] == EMPTY)
                return false;

    return true;
}

void Engine::updateMoves()
{
    if (isBlackTurn)
    {
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
            {
                if (state[i][j] != EMPTY)
                {
                    hasUpMove[i][j] = hasDownMove[i][j] = false;
                    hasLeftMove[i][j] = hasRightMove[i][j] = false;
                    continue;
                }

                int h = i;
                while(h > 0 && state[--h][j] == WHITE);
                if (state[h][j] == BLACK && i-h > 1)
                {
                    hasUpMove[i][j] = true;
                    moveProfit[i][j] += i - h - 1;
                }
                else
                    hasUpMove[i][j] = false;

                h = i;
                while(h < 7 && state[++h][j] == WHITE);
                if (state[h][j] == BLACK && h-i > 1)
                {
                    hasDownMove[i][j] = true;
                    moveProfit[i][j] += h - i - 1;
                }
                else
                    hasDownMove[i][j] = false;

                h = j;
                while (h > 0 && state[i][--h] == WHITE);
                if (state[i][h] == BLACK && j-h > 1)
                {
                    hasLeftMove[i][j] = true;
                    moveProfit[i][j] += j - h - 1;
                }
                else
                    hasLeftMove[i][j] = false;

                h = j;
                while (h < 7 && state[i][++h] == WHITE);
                if (state[i][h] == BLACK && h-j > 1)
                {
                    hasRightMove[i][j] = true;
                    moveProfit[i][j] += h - j - 1;
                }
                else
                    hasRightMove[i][j] = false;

                h = i;
                int k = j;
                while (h > 0 && k > 0 && state[--h][--k] == WHITE);
                if (state[h][k] == BLACK && i-h > 1)
                {
                    hasUpLeftMove[i][j] = true;
                    moveProfit[i][j] += i - h - 1;
                }
                else
                    hasUpLeftMove[i][j] = false;

                h = i;
                k = j;
                while (h > 0 && k < 7 && state[--h][++k] == WHITE);
                if (state[h][k] == BLACK && i-h > 1)
                {
                    hasUpRightMove[i][j] = true;
                    moveProfit[i][j] += i - h - 1;
                }
                else
                    hasUpRightMove[i][j] = false;

                h = i;
                k = j;
                while (h < 7 && k < 7 && state[++h][++k] == WHITE);
                if (state[h][k] == BLACK && h-i > 1)
                {
                    hasDownRightMove[i][j] = true;
                    moveProfit[i][j] += h - i - 1;
                }
                else
                    hasDownRightMove[i][j] = false;

                h = i;
                k = j;
                while (h < 7 && k > 0 && state[++h][--k] == WHITE);
                if (state[h][k] == BLACK && h-i > 1)
                {
                    hasDownLeftMove[i][j] = true;
                    moveProfit[i][j] += h - i - 1;
                }
                else
                    hasDownLeftMove[i][j] = false;
            }
    }
    else
    {
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
            {
                if (state[i][j] != EMPTY)
                    continue;

                int h = i;
                while(h > 0 && state[--h][j] == BLACK);
                if (state[h][j] == WHITE && i-h > 1)
                {
                    hasUpMove[i][j] = true;
                    moveProfit[i][j] += i - h - 1;
                }
                else
                    hasUpMove[i][j] = false;

                h = i;
                while(h < 7 && state[++h][j] == BLACK);
                if (state[h][j] == WHITE && h-i > 1)
                {
                    hasDownMove[i][j] = true;
                    moveProfit[i][j] += h - i - 1;
                }
                else
                    hasDownMove[i][j] = false;

                h = j;
                while (h > 0 && state[i][--h] == BLACK);
                if (state[i][h] == WHITE && j-h > 1)
                {
                    hasLeftMove[i][j] = true;
                    moveProfit[i][j] += j - h - 1;
                }
                else
                    hasLeftMove[i][j] = false;

                h = j;
                while (h < 7 && state[i][++h] == BLACK);
                if (state[i][h] == WHITE && h-j > 1)
                {
                    hasRightMove[i][j] = true;
                    moveProfit[i][j] += h - j - 1;
                }
                else
                    hasRightMove[i][j] = false;

                h = i;
                int k = j;
                while (h > 0 && k > 0 && state[--h][--k] == BLACK);
                if (state[h][k] == WHITE && i-h > 1)
                {
                    hasUpLeftMove[i][j] = true;
                    moveProfit[i][j] += i - h - 1;
                }
                else
                    hasUpLeftMove[i][j] = false;

                h = i;
                k = j;
                while (h > 0 && k < 7 && state[--h][++k] == BLACK);
                if (state[h][k] == WHITE && i-h > 1)
                {
                    hasUpRightMove[i][j] = true;
                    moveProfit[i][j] += i - h - 1;
                }
                else
                    hasUpRightMove[i][j] = false;

                h = i;
                k = j;
                while (h < 7 && k < 7 && state[++h][++k] == BLACK);
                if (state[h][k] == WHITE && h-i > 1)
                {
                    hasDownRightMove[i][j] = true;
                    moveProfit[i][j] += h - i - 1;
                }
                else
                    hasDownRightMove[i][j] = false;

                h = i;
                k = j;
                while (h < 7 && k > 0 && state[++h][--k] == BLACK);
                if (state[h][k] == WHITE && h-i > 1)
                {
                    hasDownLeftMove[i][j] = true;
                    moveProfit[i][j] += h - i - 1;
                }
                else
                    hasDownLeftMove[i][j] = false;
            }
    }
}
