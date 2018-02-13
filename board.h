#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include <QMainWindow>

#define BOARD_WIDTH 60
#define BOARD_HEIGHT 60
#define AI_DELAY 1500

class QPushButton;
class QDialogButtonBox;
class Engine;
class Player;
class QIcon;
class QMessageBox;


class Board : public QMainWindow
{
    Q_OBJECT

public:
    explicit Board(bool blackHumanity = true, bool whiteHumanity = false, QWidget *parent = 0);

private:

    Engine *engine;
    QPushButton *cells[8][8];
    QIcon *whiteIcon, *blackIcon, *validIcon, *emptyIcon;
    bool blackIsHuman;
    bool whiteIsHuman;

    QMessageBox *about;

    void initializeButtons();
    void updateIcons();
    void updateScores();

private slots:
    void clickHandler();
    void aiPlay();
    void resetAiHuman();
    void resetHumanAi();
    void reset2Player();
    void reset2Ai();
};

#endif // BOARD_H
