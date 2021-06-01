#include <Maze.h>
#include <math.h>

Maze::Maze(double N) {
    if (N<9)
        throw std::logic_error("too little dimensions!");
    double branchingtimes{round(sqrt(N))};
    double counter{};    
    //while()

}

std::list<Maze::Node*> Maze::branching() {

}