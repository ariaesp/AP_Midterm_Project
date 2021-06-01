#include <Maze.h>
#include <math.h>
#include <random>
#include <time.h>

Maze::Maze(double N) {
    if (N<9)
        throw std::logic_error("too little dimensions!");
    double layers{round(sqrt(N))};
    double saveperlayer{2};
    head= new Node(0);
    double counter{1};
    N--;
    std::mt19937 gen(static_cast<long unsigned int>(time(0)));
    std::uniform_int_distribution<> distrib(1,N-(saveperlayer*layers));
    double branches=distrib(gen);
    for (double i{};i<branches;i++) {
        Node* a=new Node(counter);
        counter++;
        N--;
        Node::merge(head,a);
    }
    newlayer(head->children,N,counter,layers);

}

void Maze::newlayer(std::list<Node*> layer, double N, double counter, double laytimes) {

}