#include "Game.hpp"

Game::Game()
    : scene{new QGraphicsScene}
    , grid(50, 40)
    , blocks(50, 40)
    , snake(Direction::Up, 10)
{
    setScene(scene);
    for (int i = 0; i < blocks.xBound; i++)
    {
        for (int j = 0; j < blocks.yBound; j++)
        {
            Block* rect = new Block;
            rect->setRect(i * pixelSize, j * pixelSize, pixelSize, pixelSize);
            scene->addItem(rect);
            blocks.at(i, j) = rect;
        }
    }

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setGeometry(0, 0, scene->width(), scene->height());
    showSnake();
    MakeApple();
    connect(&timer, SIGNAL(timeout()), this, SLOT(ContinueMove()));
    timer.start(100);
}

Game::~Game()
{
    delete scene;
}

bool Game::showSnake()
{
    QSet<int> set;
    for (Position p : snake.points)
    {
        set.insert(p.x * blocks.yBound + p.y);
    }
    bool ateApple = false;
    for (int i = 0; i < blocks.xBound; i++)
    {
        for (int j = 0; j < blocks.yBound; j++)
        {
            int index = i * blocks.yBound + j;
            if (set.contains(index))
            {
                if (blocks.at(i, j)->kind != BlockKind::Green)
                {
                    if (blocks.at(i, j)->kind == BlockKind::Red)
                    {
                        ateApple = true;
                    }
                    blocks.at(i, j)->setGreen();
                }
            }
            else
            {
                if (blocks.at(i, j)->kind == BlockKind::Green)
                {
                    blocks.at(i, j)->setWhite();
                }
            }
        }
    }
    return ateApple;
}

void Game::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Up)
    {
        if (snake.direction != Direction::Down)
        {
            snakeMove(Direction::Up);
        }
    }
    else if (event->key() == Qt::Key_Down)
    {
        if (snake.direction != Direction::Up)
        {
            snakeMove(Direction::Down);
        }
    }
    else if (event->key() == Qt::Key_Right)
    {
        if (snake.direction != Direction::Left)
        {
            snakeMove(Direction::Right);
        }
    }
    else if (event->key() == Qt::Key_Left)
    {
        if (snake.direction != Direction::Right)
        {
            snakeMove(Direction::Left);
        }
    }
}

void Game::snakeMove(Direction direction)
{
    if (snake.SnakeMove(direction, blocks))
    {
        if (showSnake())
        {
            MakeApple();
        }
    }
}

void Game::MakeApple()
{
    vector<int> positions;
    srand((unsigned) time(NULL));
    for (int i = 0, n = blocks.data.size(); i < n; i++)
    {
        if (blocks.data[i]->kind == BlockKind::White)
        {
            positions.push_back(i);
        }
    }
    int choice = rand() % positions.size();
    blocks.data[positions.at(choice)]->setRed();
}

void Game::ContinueMove()
{
    snakeMove(snake.direction);
}

Block::Block()
    : QGraphicsRectItem()
{
    setWhite();
}

void Block::setWhite()
{
    setBrush(QBrush(Qt::GlobalColor::white));
    kind = BlockKind::White;
    setPen(QPen(Qt::GlobalColor::black));
}

void Block::setGreen()
{
    setBrush(QBrush(Qt::GlobalColor::green));
    kind = BlockKind::Green;
    setPen(QPen(Qt::GlobalColor::black));
}

void Block::setRed()
{
    setBrush(QBrush(Qt::GlobalColor::red));
    kind = BlockKind::Red;
    setPen(QPen(Qt::GlobalColor::black));
}

Position::Position(int x, int y)
    : x(x)
    , y(y)
{
}

bool Position::isValid(int xBound, int yBound)
{
    return x >= 0 && y >= 0 && x < xBound && y < yBound;
}

Snake::Snake(Direction direction, int length)
    : direction(direction)
{
    int x = 30;
    int y = 30;
    points.push_back(Position(x, y));
    for (int i = 0; i < length - 1; i++)
    {
        x--;
        points.push_back(Position(x, y));
    }
}

bool Snake::SnakeMove(Direction direction, Grid<Block*> grid)
{
    this->direction = direction;
    Position head = points.front();
    Position tail = points.back();
    points.pop_back();
    Position next = moveDirection(direction, head);
    if (next.isValid(grid.xBound, grid.yBound))
    {
        if (grid.at(next.x, next.y)->kind == BlockKind::Green)
        {
            points.push_back(tail);
            return false;
        }
        else
        {
            points.push_front(next);
            if (grid.at(next.x, next.y)->kind == BlockKind::Red)
            {
                points.push_back(tail);
            }
            return true;
        }
    }
    else
    {
        points.push_back(tail);
        return false;
    }
}

Position Snake::moveDirection(Direction direction, Position head)
{
    switch (direction)
    {
    case Direction::Down:
        return Position(head.x, head.y + 1);
    case Direction::Up:
        return Position(head.x, head.y - 1);
    case Direction::Left:
        return Position(head.x - 1, head.y);
    default:
    case Direction::Right:
        return Position(head.x + 1, head.y);
    }
}
