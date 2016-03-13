// @name Alice Qin
// @date 01/29/2016
// @file gameboard.cpp
// @brief This program calls for a custom-designed gameboard. Implements moving dog/cloud/chocolates/bones
//          and interactions between them
// still need to work on bugs - pointer being freed not allocated, cloud/chocolate doesn't move sometimes, random crashes

#include "gameboard.h"
#include "mainwindow.h"
#include "ui_gameboard.h"
#include <QPushButton>


unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator (seed);


// Initialize values and sets up gameboard
GameBoard::GameBoard(QWidget *parent, size_t board_sz, size_t cloud_spd, int tlevel, QString tdiff) :
    QWidget(parent),
    ui(new Ui::GameBoard), board_size(board_sz), cloud_speed(cloud_spd), level(tlevel), diff(tdiff)
{
     ui->setupUi(this);

    // connect QLabels with corresponding pictures
    QString heart_fileName(":/image/heart.png");
    heart_image = new QPixmap(heart_fileName);

    QString dog_fileName(":/image/dog.png");
    dog_image = new QPixmap(dog_fileName);

    QString cloud_fileName(":/image/cloud.png");
    cloud_image = new QPixmap(cloud_fileName);

    QString choco_fileName(":/image/chocolate.png");
    choco_image = new QPixmap(choco_fileName);

    QString bone_fileName(":/image/bone.png");
    bone_image = new QPixmap(bone_fileName);

    QString ball_fileName(":/image/ball.png");
    ball_image = new QPixmap(ball_fileName);

    // set up top bar of lives/level/stage
    Top = new QWidget;
    QHBoxLayout *top_bar = new QHBoxLayout(Top);

    // set semi-transparent background of top bar
    QPalette pal = palette();
        pal.setBrush(QPalette::Window, QColor(102, 204, 255, 128));
        Top->setPalette(pal);
        Top->setAutoFillBackground(true);

    // Fix a total of 4 lives for now, can change later.
    // H H H

    // One life is currently playing, the others are listed above
    numLives = 4;
    lives = new QLabel*[numLives-1]; //array of pointers of lives


    // Set the labels to have the heart image, add to the top bar layout.
    // Add in amount of hearts according to lives left
    for(size_t i=0;i<numLives-1;i++) {
        lives[i] = new QLabel;
        lives[i]->setPixmap(*heart_image);
        lives[i]->setMinimumSize(40,40);
        lives[i]->setMaximumSize(40,40);
        lives[i]->setScaledContents(true);
        top_bar->addWidget(lives[i]);
        }

    // Set Stage and Level boxes
    // H H H    Difficulty: Easy
    QLabel* stage_text = new QLabel("Difficulty: ");
    stage_value = new QLabel(diff); // start with easy level.

    QSpacerItem* horizontal_space = new QSpacerItem(20,20);

    // H H H    Difficulty: Easy     Level: 1
    QLabel* level_text = new QLabel("Level: ");
    level_value = new QLabel(QString::number(level)); // start with level 1.

    QSpacerItem* horizontal_space_2 = new QSpacerItem(*horizontal_space);

    // H H H    Difficulty: Easy     Level: 1    To do: B B B
    QLabel* todo_text = new QLabel("To do: ");


    top_bar->addWidget(stage_text);
    top_bar->addWidget(stage_value);
    top_bar->addSpacerItem(horizontal_space);
    top_bar->addWidget(level_text);
    top_bar->addWidget(level_value);
    top_bar->addSpacerItem(horizontal_space_2);
    top_bar->addWidget(todo_text);


    // generate random number of bones/balls to collect
    std::uniform_int_distribution<int> increment(2,4);
    getNumBones = increment(generator);
    getNumBones += level;

    // only generate balls if not level 1
    if (level > 1) {
        getNumBalls = increment(generator);
        getNumBalls += level;
    }
    else getNumBalls = 0;

    // Set the labels to have the bone image, add to the top bar layout.
    // Add in amount of bones according to how many left
    goals = new QLabel*[getNumBones-1]; //array of pointers of bones left
    for(size_t i=0;i<getNumBones;i++) {
        goals[i] = new QLabel;
        goals[i]->setPixmap(*bone_image);
        goals[i]->setMinimumSize(20,20);
        goals[i]->setMaximumSize(20,20);
        goals[i]->setScaledContents(true);
        top_bar->addWidget(goals[i]);
        }

    if (getNumBalls != 0) {
        // Set the labels to have the ball image, add to the top bar layout.
        // Add in amount of ball according to how many left
        goals2 = new QLabel*[getNumBalls-1]; //array of pointers of bones left
        for(size_t i=0;i<getNumBalls;i++) {
            goals2[i] = new QLabel;
            goals2[i]->setPixmap(*ball_image);
            goals2[i]->setMinimumSize(20,20);
            goals2[i]->setMaximumSize(20,20);
            goals2[i]->setScaledContents(true);
            top_bar->addWidget(goals2[i]);
            }
        }

    //This code creates the Board
    Board = new QWidget;
    labels = new QLabel*[board_size*board_size] ;

    QGridLayout *SquareGrid = new QGridLayout(Board);

    SquareGrid->setGeometry(QRect());
    SquareGrid->setSpacing(0);

    // Create each label for the board, initialize with random value.
    for(size_t i=0;i<board_size;i++) {
        for(size_t j=0;j<board_size;j++) {

            // Create label and set properties.
            labels[i*board_size+j] = new QLabel;
            labels[i*board_size+j]->setMinimumSize(40,40);
            labels[i*board_size+j]->setMaximumSize(40,40);
            labels[i*board_size+j]->setAlignment(Qt::AlignCenter);

            // Add label to the layout
            SquareGrid -> addWidget(labels[i*board_size+j] ,i,j);

        }
    }


    // Initialize the dog at the bottom left corner
    dog_position = new QPoint(0,board_size-5);

    Board->setFixedSize(800,400);

/*  Now piece everything together
    QWidget*& CoreWidget = this;
    // Central Widget composed of Top and Board
    //    Top
    // --------
    //   Board
*/
    // Do not allow board to be resized, otherwise spacings get messed up
    setFixedSize(800,475);

    // Create a vertical box layout for the two pieces
    QVBoxLayout *piece_together = new QVBoxLayout;

    piece_together->addWidget(Top,0,Qt::AlignCenter);
    piece_together->addWidget(Board,0,Qt::AlignCenter);

    this->setLayout(piece_together);

    // Random number generator for C++11
    std::uniform_int_distribution<int> distribution(0,100);

    // Initialize the cloud at the top left corner, coordinate (0,0).
    cloud_position = new QPoint(0,0);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateCloud()));
    timer->start(cloud_speed);


    // Initialize the chocolate under the cloud
    numChoco = 1;
    int chx = cloud_position->rx();
    int chy = cloud_position->ry();

    choco_positions.push_back(new QPoint);
    choco_positions[0]->setX(chx);
    choco_positions[0]->setY(chy+5);

    // Initialize bones, ball
    numBones = 0;
    numBalls = 0;

    // Set speeds for chocolates, bones
    connect(timer, SIGNAL(timeout()), this, SLOT(updateChoco()));
    timer->start(cloud_speed);

    connect(timer, SIGNAL(timeout()), this, SLOT(updateBones()));
    timer->start(cloud_speed);

    connect(timer, SIGNAL(timeout()), this, SLOT(updateBalls()));
    timer->start(cloud_speed);

    // Connectors to new game/quit signals

    connect(this, SIGNAL(game_over()), parent, SLOT(game_over()));
    connect(this, SIGNAL(new_level()), parent, SLOT(new_level()));
}


/* keyPressEvent takes user input and changes game accordingly
 * @param event User input
 */

void GameBoard::keyPressEvent(QKeyEvent *event){

    int x = dog_position->rx();
    int y = dog_position->ry();

    switch (event->key()) {
    case Qt::Key_Left:
        if(dog_position->rx() != 0)
            updateDog(x,y,x-1,y);
        break;

    case Qt::Key_Right:
        if(dog_position->rx() != board_size-5)
            updateDog(x,y,x+1,y);
        break;

    case Qt::Key_Space:
        munchObj(typeofObj(x,y));
        break;

    default:
        QWidget::keyPressEvent(event);
    }

    return;

}

/* paintEvent places each element in its specified position
 * @param e QPaintEvent needed to call function
 *
 * assumes dog's position is within boundaries of board
 */

void GameBoard::paintEvent(QPaintEvent *e) {

    // Place dog
    int x = dog_position->rx();
    int y = dog_position->ry();

    labels[y*board_size+x]->setPixmap(*dog_image);
    labels[y*board_size+x]->setScaledContents(true);

    // Place cloud
    int cx = cloud_position->rx();
    int cy = cloud_position->ry();

    if(cx >= 0 && cy >= 0 && cx < (int)board_size && cy < (int)board_size) { //place only if within board
        labels[cy*board_size+cx]->setPixmap(*cloud_image);
        labels[cy*board_size+cx]->setScaledContents(true);
    }

    // Place chocolates
    for (size_t i=0; i < numChoco; ++i) {
        int chx = choco_positions[i]->rx();
        int chy = choco_positions[i]->ry();

            if(chx >= 0 && chy >= 0 && chx < (int)board_size && chy < (int)board_size) {
                labels[chy*board_size+chx]->setPixmap(*choco_image);
                labels[chy*board_size+chx]->setScaledContents(true);
            }

   }

    // Place bones
    for (size_t i=0; i < numBones; ++i) {
        int bx = bone_positions[i]->rx();
        int by = bone_positions[i]->ry();

            if(bx >= 0 && by >= 0 && bx < (int)board_size && by < (int)board_size) {
                labels[by*board_size+bx]->setPixmap(*bone_image);
                labels[by*board_size+bx]->setScaledContents(true);
            }

   }

    // Place balls
    for (size_t i=0; i < numBalls; ++i) {
        int bx = ball_positions[i]->rx();
        int by = ball_positions[i]->ry();

            if(bx >= 0 && by >= 0 && bx < (int)board_size && by < (int)board_size) {
                labels[by*board_size+bx]->setPixmap(*ball_image);
                labels[by*board_size+bx]->setScaledContents(true);
            }

   }


}

/* showEvent makes the gameboard visible
 * @param e QShowEvent needed to call function to show
 */

void GameBoard::showEvent(QShowEvent *e) {

    this->activateWindow();
    this->setFocus();
    QWidget::showEvent(e);
}

/* destructor deallocates manually managed memory */

GameBoard::~GameBoard()
{
    delete [] dog_position;
    delete [] cloud_position;
    delete [] goals;
    //delete [] goals2;

    for (size_t i = 0; i<getNumBalls; ++i)
        ball_positions.pop_back();

    for (size_t i = 0; i<getNumBones; ++i)
        bone_positions.pop_back();

    delete ui;
}

/* updateDog deletes picture of dog's previous position and sets it to the new given position
 * @param px old x-coord
 * @param py old y-coord
 * @param nx new x-coord
 * @param ny new y-coord
 */

void GameBoard::updateDog(int px, int py, int nx, int ny) {

    labels[py*board_size+px]->clear();

    dog_position->setX(nx);
    dog_position->setY(ny);

}

/* updateCloud repositions cloud and creates "rain"
 *
 * erases old spot and sets it to new random position, and randomly determines whether to rain
 * a chocolate or bone
 */

void GameBoard::updateCloud() {

    // random number generator
    std::uniform_int_distribution<int> increment(-5,6);

    int px = cloud_position->rx();
    int py = cloud_position->ry();

    // erase cloud's previous spot
    if(px>=0 && py >= 0 && px<=(int)board_size && py<=(int)board_size)
        labels[py*board_size+px]->clear();

    int nx = px;

    // set new random position for cloud within board limits
        nx = px + 2*increment(generator)-1;
            while(!(nx >= 0 && nx+5 < (int)board_size))
                nx = px + 2*increment(generator)-1;

                cloud_position->setX(nx);


    // cloud randomly chooses to rain either chocolate or bone
    switch(increment(generator)) {
    case 0: createNewChoco();
        break;
    case 1: createNewBone();
        break;
    case 2: {
        if (level>=2)
            createNewChoco();
        break; }
    case 3:{
        if (level==3)
            createNewChoco();
        break; }
    case 4:{
        if (diff=="Pro")
            createNewChoco();
        break; }
    case 5:{
        if (diff=="Hard")
            createNewChoco();
        break; }
    case 6:{
        if (level>=2)
            createNewBalls();
        break; }
    default: break; }

    repaint();
    QCoreApplication::processEvents();

}

/* updateChoco makes the chocolate rain down
 * manually manages memory of chocolate array
 * makes dog lose life if it falls on same position as dog
 */

void GameBoard::updateChoco() {

    // for each chocolate...
    for (size_t i=0; i<numChoco;++i) {
        int chx = choco_positions[i]->rx();
        int chy = choco_positions[i]->ry();

        // erase chocolate's previous spot
        if(chx>=0 && chy >= 0 && chx<(int)board_size && chy<(int)board_size)
            labels[chy*board_size+chx]->clear();

        // make chocolate "fall" until it hits the ground
        if (chy+1 < (int)board_size)
            choco_positions[i]->setY(chy+1);

        // erase chocolate's position from board once it hits the ground
        if (chy+5 == (int)board_size) {
            choco_positions.removeAt(i);

            --numChoco;


        // lose life if chocolate and dog are on same point
        if (isDogObj(chx, chy)) {
            --numLives;
            if (numLives>=1)
                lives[numLives-1]->clear();
            if (numLives<1)
                this->game_over(); // quit game if no more lives
            }
        }
    }

    repaint();
    QCoreApplication::processEvents();
}

/* updateBones makes bone rain down */

void GameBoard::updateBones() {

    // for each bone...
    for (size_t i=0; i<numBones;++i) {
        int bx = bone_positions[i]->rx();
        int by = bone_positions[i]->ry();

        // erase bone's previous spot
        if(bx>=0 && by >= 0 && bx<(int)board_size && by<(int)board_size)
            labels[by*board_size+bx]->clear();

        // make bone "fall" until it hits the ground
        if (by+5 < (int)board_size)
            bone_positions[i]->setY(by+1);
    }


    repaint();
    QCoreApplication::processEvents();
}

/* updateBalls makes balls rain down */

void GameBoard::updateBalls() {

    // for each ball...
    for (size_t i=0; i<numBalls;++i) {
        int bx = ball_positions[i]->rx();
        int by = ball_positions[i]->ry();

        // erase ball's previous spot
        if(bx>=0 && by >= 0 && bx<(int)board_size && by<(int)board_size)
            labels[by*board_size+bx]->clear();

        // make ball "fall" until it hits the ground
        if (by+5 < (int)board_size)
            ball_positions[i]->setY(by+1);
    }


    repaint();
    QCoreApplication::processEvents();
}

/* munchObj determines if dog is on a bone
 * @param type =1 if bone, =2 if ball
 * manually manages memory of bone array
 * clears bone once munched
 * exits once goal number of bones are finished
 */

void GameBoard::munchObj(int type) {
    // if bone..
    if (type==1 && getNumBones != 0) {
        for (size_t i=0; i<numBones; ++i) {
         int chx = bone_positions[i]->rx();
         int chy = bone_positions[i]->ry();

            // If dog is on a bone
         if (isDogObj(chx,chy)) {
            // bone is consumed (delete from array)
            bone_positions.removeAt(i);
            --numBones;

            // clear bone once it's munched
            if (getNumBones>0)
                goals[getNumBones-1]->clear();

            --getNumBones;

            break;
            }
         }
    }

    // if ball..
    if (type==2 && getNumBalls != 0) {
        for (size_t i=0; i<numBalls; ++i) {
         int chx = ball_positions[i]->rx();
         int chy = ball_positions[i]->ry();

            // If dog is on a ball
         if (isDogObj(chx,chy)) {
            // ball is consumed (delete from array)
            ball_positions.removeAt(i);
            --numBalls;

            // clear ball once it's munched
            if (getNumBalls>0)
                goals2[getNumBalls-1]->clear();

            --getNumBalls;

            break;
            }
         }
    }

    if (getNumBones<1 && getNumBalls<1) //{
        this->new_level(); // new level if no more bones&balls



    repaint();
    QCoreApplication::processEvents();
}

/*isDogObj determines if there is an object where the dog is
 * @param chx x-coord of object
 * @param chy y-coord of object
 * @return true if dog is on object
 */

bool GameBoard::isDogObj(int chx,int chy) {

        int dx = dog_position->rx();
        int dy = dog_position->ry();

        // return true if dog and object are in the same coordinates
        if (chx==dx && chy==dy)
            return true;

    return false;

}
/* typeofObj returns the type of object on the coordinates given
 * @param gx x-coord to check
 * @param gy y-coord to check
 * @return 0=nothing, 1=bone, 2=ball
 */

int GameBoard::typeofObj(int gx, int gy)
{
    int dx = dog_position->rx();
    int dy = dog_position->ry();

    // iterate through bone position vector to find match
    for (int i = 0; i<numBones; ++i)
    {
        int bx = bone_positions[i]->rx();
        int by = bone_positions[i]->ry();

        if (bx==dx && by==dy)
            return 1;
    }

    // iterate through ball position vector to find match
    for (int i = 0; i<numBalls; ++i)
    {
        int bx = ball_positions[i]->rx();
        int by = ball_positions[i]->ry();

        if (bx==dx && by==dy)
            return 2;
    }

    // no match
    return 0;
}

/* createNewChoco creates new chocolates underneath the cloud
 * increases total count of chocolates
 */

void GameBoard::createNewChoco(){

    // get cloud's position
    int cx = cloud_position->rx();
    int cy = cloud_position->ry();

    // set chocolate underneath cloud
    choco_positions.push_back(new QPoint);
    choco_positions[numChoco]->setX(cx);
    choco_positions[numChoco]->setY(cy+5);

    ++numChoco;
}

/* createNewBone creates new bones underneath the cloud
* increases total count of bones
*/

void GameBoard::createNewBone(){

    // get cloud's position
    int cx = cloud_position->rx();
    int cy = cloud_position->ry();

    // set chocolate underneath cloud
    bone_positions.push_back(new QPoint);
    bone_positions[numBones]->setX(cx);
    bone_positions[numBones]->setY(cy+5);

    ++numBones;
}

/* createNewBone creates new balls underneath the cloud
* increases total count of balls
*/
void GameBoard::createNewBalls(){

    // get cloud's position
    int cx = cloud_position->rx();
    int cy = cloud_position->ry();

    // set chocolate underneath cloud
    ball_positions.push_back(new QPoint);
    ball_positions[numBalls]->setX(cx);
    ball_positions[numBalls]->setY(cy+5);

    ++numBalls;
}

/* closeEvent displays dialog window when user tries quit & closes application
 * @param event takes in user action
 *
 * gives options to user before quitting
 */
void GameBoard::closeEvent (QCloseEvent *event)
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

