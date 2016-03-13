// @name Alice Qin
// @date 01/29/2016
// @file mainwindow.cpp
// @brief This program determines the settings and set up for the game (ie. difficulty level)
//          Connects user input to game setup.




#include "mainwindow.h"
#include "gameboard.h"
#include "instr.h"
#include <QCloseEvent>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), level(0)
{
    setStyleSheet("MainWindow {background-image: url(:/image/bg11.jpg)}");

    ui->setupUi(this);
}


// calls game of different difficulties
void MainWindow::easy_game_begin() {
    ++level;
    if (level >= 4) {
        return; }
    board = new GameBoard(this,50,100,level,"Easy");
    this->setCentralWidget(board);
}

void MainWindow::medium_game_begin() {
    ++level;
    if (level >= 4) {
        return; }
    board = new GameBoard(this,50,50,level,"Medium");
    this->setCentralWidget(board);
}

void MainWindow::hard_game_begin() {
    ++level;
    if (level >= 4) {
        return; }
    board = new GameBoard(this,50,5,level,"Hard");
    this->setCentralWidget(board);
}

void MainWindow::pro_game_begin() {
    ++level;
    if (level >= 4) {
        return; }
    board = new GameBoard(this,50,1,level,"Pro");
    this->setCentralWidget(board);
}

//destructor
MainWindow::~MainWindow()
{
    delete ui;
}
// Connectors between user input and instruction window
void MainWindow::on_InstructionButton_clicked()
{
    myInstr = new Instr(this);
    myInstr->show();
}

// Connectors between user input and game start
void MainWindow::on_easypushButton_clicked()
{
    game_chose = 1;
    easy_game_begin();
}\

void MainWindow::on_mediumpushButton_clicked()
{
    game_chose = 2;
    medium_game_begin();
}

void MainWindow::on_hardpushButton_clicked()
{
    game_chose = 3;
    hard_game_begin();
}

void MainWindow::on_propushButton_clicked()
{
    game_chose = 4;
    pro_game_begin();
}


/* Connector between quit button and exiting game */
void MainWindow::on_quitButton_clicked()
{
    close();
}

/* game_over quits game window, displays message, and brings it back to the main menu */

void MainWindow::game_over() {

    QMessageBox::question( this, "", tr("Game Over!\n"), QMessageBox::Ok);

    // Take the current board game out of the central widget of MainWindow
    QWidget* wid = this->centralWidget();
    wid->setParent(nullptr);

    // Reset the MainWindow with the initial startup screen
    ui->setupUi(this);


}

/* new_level displays message in dialog box when level is passed*/

void MainWindow::new_level()
{
    if (level >= 3) {
        game_over();
        return; }

    QMessageBox::question(this, "", tr("New level!\n"), QMessageBox::Ok);

    switch(game_chose) {
    case 1: {easy_game_begin(); break;}
    case 2: {medium_game_begin(); break;}
    case 3: {hard_game_begin(); break;}
    case 4: {pro_game_begin(); break;}
    }
}

/* closeEvent displays dialog window when user tries quit & closes application
 * @param event takes in user action
 *
 * gives options to user before quitting
 */
void MainWindow::closeEvent (QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "",
                                                                tr("Are you sure?\n"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        event->accept();
    }
}
