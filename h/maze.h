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
        int rp;
        int cp;
        int value;
        Node* parent{nullptr};
        std::list<Node*> children;
        Node()=default;
        Node(int);
        Node(int _val,int _rp, int _cp);
        Node(const Node& n, Maze&);
        //explicit Node(int, Node&, std::list<Node*>);
        //Node(const Node& n, BinomialHeap& outBH );
        size_t get_order() const;
        void set_order(size_t new_order) {order=new_order;};

        std::partial_ordering operator<=> (const Node& n1) {return this->value <=> n1.value;}
        bool operator== (const Node& n1) {return this->value == n1.value;}
        
        friend std::ostream &operator<< (std::ostream &os, Node const& n1) {
            if (n1.parent==nullptr)
                return os << "(value:" << n1.value << ", parent: " << ", order:" << n1.order<< ")" << std::endl;
            else 
                return os << "(value:" << n1.value << ", parent:" << n1.parent->value << ", order:" << n1.order<< ")" << std::endl;
        }

        // static bool is_min_heap(Node const&);
        // static bool is_max_heap(Node const&);
    private:
        size_t order{0};  
    };
    std::list<Node*> Nodes;
    Node* head;
    Node* exit;
    Node* findnode(int);
    static Node* merge(Node*, Node*);
    Matrix mat;
    void buildmaze(std::mt19937&,Node* input,int relation); //relation: input is a child or parent. (parent=1,child=2)
    std::set<int> seen;
    bool is_seen(int val) {if (seen.find(val)!=seen.end()) return true; else return false;}

public:
    Maze()=default;
    Maze(int,int);
    ~Maze();
    Maze(const Maze&);
    Maze operator=(const Maze&);
    // void newlayer(std::mt19937&, std::list<Node*>, int, int, int, int);
    // void show();
    // void Nshow(Node&, int, bool&);
    void showmaze();
    void DFS();
    void trackdown(std::list<Node*>&,std::list<Node*>&,Node*,int&,int&);
    void showpath(std::list<Node*>&);
};

#endif
