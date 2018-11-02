#ifndef GAME_HPP
#define GAME_HPP
#include "Grid.hpp"
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QSet>
#include <QTimer>
#include <ctime>
#include <list>
#include <vector>
using std::list;
using std::vector;
class Block;
class Snake;
enum class Direction
{
    Up,
    Down,
    Left,
    Right
};
class Position
{
public:
    int x;
    int y;
    Position(int x, int y);
    bool isValid(int xBound, int yBound);
};
static const int pixelSize = 20;

class Snake
{
public:
    Direction direction;
    list<Position> points;
    Snake(Direction direction, int length);
    bool SnakeMove(Direction direction, Grid<Block*> grid);
    Position moveDirection(Direction direction, Position head);
};

class Game : public QGraphicsView
{
    Q_OBJECT
public:
    QGraphicsScene* scene;
    Grid<int> grid;
    Grid<Block*> blocks;
    Snake snake;
    QTimer timer;
    Game();
    ~Game();
    bool showSnake();
    void keyPressEvent(QKeyEvent* event) override;
    void snakeMove(Direction direction);
    void MakeApple();
public slots:
    void ContinueMove();
};
enum class BlockKind
{
    White,
    Green,
    Red

};

class Block : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    BlockKind kind;
    Block();
    void setWhite();
    void setGreen();
    void setRed();
};

#endif // GAME_HPP
