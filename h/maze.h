#ifndef MAZE_H
#define MAZE_H

#include <iostream>
#include <list>
#include <set>
#include <compare>

class Maze {
private:
    size_t N{};
    class Node {
    public:
        double value{0};
        Node* parent{nullptr};
        std::list<Node*> children;
        Node()=default;
        Node(double);
        explicit Node(double, Node&, std::list<Node*>);
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
    static Node* merge(Node* , Node* );
    Node* head;
public:
    Maze()=default;
    Maze(double);
    ~Maze();
    Maze(const Maze&);
    void newlayer(std::list<Node*>, double, double, double, double);

};





#endif