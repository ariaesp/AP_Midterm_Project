#include <Maze.h>
#include <math.h>
#include <random>
#include <time.h>
//#define saveperlayer=2

Maze::Node::Node (double _value, Node& _parent, std::list<Node*> _children)  
    {
        value=_value;
        parent=&_parent;
        children=_children;
        order=get_order();
        _parent.children.push_back(this);
        for (auto &n : _children)
            n->parent=this;
    }

Maze::Node::Node(double _value) //: Node{_value,nullptr,std::list<Node*> ch{}}
{
    value=_value;
}

size_t Maze::Node::get_order() const
{
    if(this->children.empty())
        return 0;
    else {
        size_t n{};
        for (auto &child : this->children) {
            size_t c=child->get_order();
            if(c>n) n=c;
        }
        return n+1;
    }    
}

Maze::Node* Maze::merge(Node* n1, Node* n2)
{
        if (*n1 <= *n2){
        n1->children.push_back(n2);
        n2->parent=n1;
        return n1;
        }
        else {
        n2->children.push_back(n1);
        n1->parent=n2;
        return n2;
        }
}


Maze::Maze(double N) {
    if (N<9)
        throw std::logic_error("too little dimensions!");
    double layercount{round(sqrt(N))};
    double saveperlayer{2};
    head= new Node(0);
    double nodecount{1};
    N--;
    std::mt19937 gen(static_cast<long unsigned int>(time(0)));
    if (3>N-(saveperlayer*layercount))
        std::uniform_int_distribution<> distrib(1,N-(saveperlayer*layercount));
    else   
        std::uniform_int_distribution<> distrib(1,3);  
    double branches=distrib(gen);
    for (double i{};i<branches;i++) {
        Node* a=new Node(nodecount);
        nodecount++;
        N--;
        Node::merge(head,a);
    }
    newlayer(head->children,N,nodecount,layercount,saveperlayer);

}

void Maze::newlayer(std::list<Node*> layer, double N, double nodecount, double layercount, double saveperlayer) {
    if (layercount==1) {
        std::list<Node*> next;
        std::list<Node*>::iterator it=layer.begin();
        while(){
            for (double j{};j<3;j++) {
                Node* a=new Node(nodecount);
                Node::merge(*it,a);
                nodecount++;
                N--;
                if (N==0) {
                    flag=true;
                    break;
                }
                next.push_back(a);
            }
            it++;
        }
        
    }
    else {
    std::mt19937 gen(static_cast<long unsigned int>(time(0)));
    std::uniform_int_distribution<> distrib(1,layer.size());
    double num=distrib(gen);
    size_t cnt{};
    double x{};
    double branches{};
    std::list<Node*> next;
    std::list<Node*>::iterator it=layer.begin();
    for (double i{}; i<num; i++) {
        x=N-(saveperlayer*layercount)-(num-(i+1));
        if (3>x && x>1) {
            std::uniform_int_distribution<> distribb(1,x);
            branches=distribb(gen);
        }    
        else if (x<=1) {
            branches=1;
        }    
        else {   
            std::uniform_int_distribution<> distribb(1,3);
            branches=distribb(gen);
        }    
        for (double j{};j<branches;j++) {
            Node* a=new Node(nodecount);
            nodecount++;
            N--;
            Node::merge(*it,a);
            next.push_back(a);
        }
        it++;
    }
    layercount--;
    newlayer(next,N,nodecount,layercount,saveperlayer);
    }
    //if statements for last layer and also checking the random generator and the decremental range!
}