#ifndef SNAKE
#define SNAKE

#include <QDialog>
#include <QLabel>
#include <QList>
#include <QKeyEvent>
#include <QTimer>
/*Ã¶¾Ù·½Ïò*/
enum  DIRECTION {D_UP,D_DOWN,D_LEFT,D_RIGHT};

class SnakeGame : public QDialog
{
    Q_OBJECT
 public:
    SnakeGame();
    ~SnakeGame();

public:
    void keyPressEvent(QKeyEvent *e);

public:
    QLabel*   getFood();
   
public slots:
    void   snakeMove();
private:
	QList<QLabel*>  snake;
	QLabel          *food;
	DIRECTION       dire;
	int             foodLen;
	int             timerInteval;
	int             maxLen;
	QTimer          *timer;

};

#endif
