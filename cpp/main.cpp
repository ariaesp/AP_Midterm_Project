#include <iostream>
#include <maze.h>
#include <math.h>

int main(int argc, char **argv)
{

    Maze a (17,25);

    a.showmaze();
    a.DFS();
    a.BFS();
    a.bidirectional();
}
