#include <iostream>
#include <maze.h>
#include <math.h>
//#include "gtest/gtest.h"

int main(int argc, char **argv)
{
    //Maze::createnode();
    // std::cout<<round(sqrt(a))<<std::endl;
    Maze a (14,14);

    a.showmaze();
    a.DFS();
}
