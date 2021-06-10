#ifndef MAZE_H
#define MAZE_H

#include <iostream>
#include <list>
#include <set>
#include <compare>
#include <memory>
#include <time.h>
#include <random>
#include <math.h>
#include <vector>
using Matrix= std::vector<std::vector<int>>;

class Maze {
private:
    size_t N{};
    class Node {
    public:
        int value;
        int rp;
        int cp;
        Node* parent{nullptr};
        std::list<Node*> children;
        Node()=default;
        Node(int);
        Node(int _val,int _rp, int _cp);
        Node(const Node& n, Maze&);
        };
    std::list<Node*> Nodes;
    Node* head;
    Node* exit;
    Matrix mat;
    std::set<int> seen;
    Node* findnode(int);
    static Node* merge(Node*, Node*);
    void buildmaze(std::mt19937&,Node* input,int relation,bool help); //relation: input is a child or parent. (parent=1,child=2)
    void helpbuilder(std::mt19937&);
    bool is_seen(int val) {if (seen.find(val)!=seen.end()) return true; else return false;}
    void trackdown(std::list<Node*>&,std::list<Node*>&,Node*,int&,int&);
    void showpath(std::list<Node*>&,int,int);

public:
    Maze()=default;
    Maze(int,int);
    ~Maze();
    Maze(const Maze&);
    Maze operator=(const Maze&);
    void DFS();
    void BFS();
    void bidirectional();
    void showmaze();
};



#endif

