#include "snake.h"

#include <QTime>
#include <QMessageBox>
#include <QPalette>
#include <QColor>

SnakeGame::SnakeGame(){
     qsrand(uint(QTime::currentTime().msec()));
     this->resize(600,600);
     dire=D_RIGHT;
     foodLen=40;
     timerInteval=200;
     maxLen=5;
     snake.push_back(getFood());
     food=getFood();
 
     timer=new QTimer();
     timer->setInterval(timerInteval);
     timer->start();

     connect(timer,SIGNAL(timeout()),this,SLOT(snakeMove()));
}
SnakeGame::~SnakeGame(){

}

void SnakeGame::keyPressEvent(QKeyEvent *e){
    switch(e->key()){
        case Qt::Key_Up:
        dire=D_UP;
        break;
        case Qt::Key_Down:
        dire=D_DOWN;
        break;
        case Qt::Key_Left:
        dire=D_LEFT;
        break;
        case Qt::Key_Right:
        dire=D_RIGHT;
        break;
    }
}


QLabel* SnakeGame::getFood()
{
    food=new QLabel(this);
    food->resize(foodLen,foodLen);

    food->setAutoFillBackground(true);

    food->setPalette(
    QPalette(QColor(qrand()%256,
                    qrand()%256,
                    qrand()%256)));
    food->setFrameShape(QFrame::Box);
    food->show();

    int  w=this->width()-foodLen;
    int  h=this->height()-foodLen;
    food->move(((qrand()%w)/foodLen)*foodLen,
               ((qrand()%h)/foodLen)*foodLen);
    return food;
}


void    SnakeGame::snakeMove()
{
    int x=snake[0]->x();
    int y=snake[0]->y();
    if(x==food->x() && y==food->y()){
        snake.push_back(food);
        food=getFood();
    }
    if(snake.size()>=maxLen){
       QMessageBox  msg;
       msg.setText("game over");
       msg.setStandardButtons(
       QMessageBox::Yes|
       QMessageBox::No);
       switch(msg.exec()){
           case QMessageBox::Yes:
           this->close();
           break;
           case QMessageBox::No:
           maxLen=2*maxLen;
           break;
       }
    }
    for(int i=snake.size()-1;i>0;i--){
        snake[i]->move(
         snake[i-1]->x(),
         snake[i-1]->y());
    }
    switch(dire){
        case D_UP:
        y=y-foodLen;
        break;
        case D_DOWN:
        y=y+foodLen;
        break;
        case D_LEFT:
        x=x-foodLen;
        break;
        case D_RIGHT:
        x=x+foodLen;
        break;
    }
    snake[0]->move(x,y);
}







