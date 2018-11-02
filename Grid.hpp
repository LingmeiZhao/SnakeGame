#ifndef GRID_HPP
#define GRID_HPP

#include <vector>

using std::vector;

template <typename T>
class Grid
{
public:
    int xBound;
    int yBound;
    vector<T> data;
    Grid(int xBound, int yBound);
    T& at(int i, int j);
    bool isValid(int i, int j);
};

template <typename T>
Grid<T>::Grid(int xBound, int yBound)
    : xBound(xBound)
    , yBound(yBound)
    , data(xBound * yBound)
{
}
template <typename T>
T& Grid<T>::at(int i, int j)
{
    return data.at(i * yBound + j);
}
template <typename T>
bool Grid<T>::isValid(int i, int j)
{
    return i >= 0 && i < xBound && j >= 0 && j < yBound;
}
#endif // GRID_HPP
