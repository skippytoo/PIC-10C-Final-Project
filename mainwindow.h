// @file mainwindow.h
// @brief contains class declarations for MainWindow class
// @author Alice Qin
// @date 2/5/16
//  class contains prompters to different levels of game
//  connects pushbuttons to a new gameboard



#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gameboard.h"
#include "instr.h"
#include "ui_mainwindow.h"
#include "ui_Instr.h"
#include <QMessageBox>


namespace Ui {
class MainWindow;
}

/* @class MainWindow
 * @brief the parent of GameBoard
 *
 * calls new game based on user input
 * calls new game if level is passed
 * calls instructions if user calls
 */


class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void easy_game_begin();
    void medium_game_begin();
    void hard_game_begin();
    void pro_game_begin();

    void game_over();
    void new_level();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void closeEvent (QCloseEvent *event);


private:
    Ui::MainWindow *ui;
    GameBoard* board;
    int game_chose;
    int level;

    Instr* myInstr;

private slots:
    void on_InstructionButton_clicked();

    void on_easypushButton_clicked();
    void on_mediumpushButton_clicked();
    void on_hardpushButton_clicked();
    void on_propushButton_clicked();

    void on_quitButton_clicked();
};


#endif // MAINWINDOW_H
