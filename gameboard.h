// @file gameboard.h
// @brief contains class declarations for Gameboard class
// @author Alice Qin
// @date 2/5/16
//  contains the functionality of the game



#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QWidget>
#include <QMainWindow>
#include <QKeyEvent>
#include <QLabel>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <random>
#include <QVector>
#include <chrono>
#include <QTimer>
#include <iostream>

namespace Ui {
class GameBoard;
}


/* @class GameBoard
 * @brief contains implementation of game
 */

class GameBoard : public QWidget
{
    Q_OBJECT

public slots:
       void updateCloud();
       void updateChoco();
       void updateBones();
       void updateBalls();

signals:
       void game_over();
       void new_level();

public:
    explicit GameBoard(QWidget *parent = 0, size_t board_size = 10, size_t cloud_speed = 10, int tlevel=1, QString tdiff="Easy");
    virtual ~GameBoard();

    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *e);
    void showEvent(QShowEvent *e);
    void closeEvent (QCloseEvent *event);


    int getlevel() const {return level;}
    void munchObj(int type);
    void updateDog(int px, int py, int nx, int ny);
    void createNewChoco();
    void createNewBone();
    void createNewBalls();
    bool isDogObj(int chx, int chy);
    int typeofObj(int chx, int chy);

private:
    Ui::GameBoard *ui;

    QLayout* layout;

    QPoint *dog_position;
    QPoint *cloud_position;
    QVector<QPoint*> choco_positions;
    QVector<QPoint*> bone_positions;
    QVector<QPoint*> ball_positions;

    // Pic containers
    QPixmap* heart_image;
    const QPixmap* dog_image;
    QPixmap* cloud_image;
    QPixmap* choco_image;
    QPixmap* bone_image;
    QPixmap* ball_image;

    // Top bar variables
    int numLives;
    QLabel* stage_value;
    QLabel* level_value;
    QWidget* Top;
    QLabel** lives;
    int getNumBones;
    int getNumBalls;
    QLabel** goals;
    QLabel** goals2;


    // Board Variables
    QWidget *Board;
    int numChoco;
    int numBones;
    int numBalls;
    int level;
    QString diff;

    int cloud_speed;
    size_t board_size;
    QLabel** labels;
};


#endif // GAMEBOARD_H
