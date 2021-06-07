#include <maze.h>

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

Maze::Node::Node(double _val,double _rp, double _cp): value(_val),rp(_rp),cp(_cp) {}

Maze::Node::Node(double _val): value(_val) {}


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

Maze::Maze(double R, double C) {
    double N=R*C;
    double matR=3+2*(R-1);
    double matC=3+2*(C-1);
    if (N<9)
        throw std::logic_error("too little dimensions!");
    else{
    mat.resize(matR);
    double n{};
    for (double i{};i<matR;i++) {
        mat[i].resize(C);
        for (double j{};j<matC;j++) {
            if(i==0 || j==0 || i==matR-1 || j==matC-1)
                mat[i][j]=-2;
            else if (i%2==0 && j%2==0)
                mat[i][j]=-2;
            else if (i%2!=0 && j%2!=0) {
                mat[i][j]=n;
                Node* a= new Node(n,i,j);
                Nodes.push_back(a);
                n++;
            }
        }
    }

    std::mt19937 gen(static_cast<long unsigned int>(time(0)));
    std::set<double> seen;
    std::uniform_int_distribution<> distrib(0,C-1);
    double head_value=distrib(gen);
    //double head_cp=1+(head_value%C)*2;
    head=this->findnode(head_value);
    seen.insert(head_value);
    this->buildmaze(head);



    // double layercount{round(sqrt(N))};
    // double saveperlayer{2};
    // head= new Node(0);
    // double nodecount{1};
    // N--;
    // double branches{};
    // double x=N-(saveperlayer*layercount);
    // if (3>x){
    //     branches=distrib(gen);
    // }    
    // else {  
    //     std::uniform_int_distribution<> distrib(1,3);
    //     branches=distrib(gen);
    // }
    // for (double i{};i<branches;i++) {
    //     Node* a{new Node(nodecount)};
    //     nodecount++;
    //     N--;
    //     merge(head,a);
    // }
    // std::cout <<"passed first layer and nodecount:"<<nodecount<<std::endl;
    // layercount--;
    // newlayer(gen,head->children,N,nodecount,layercount,saveperlayer);
    // }
}

void Maze::buildmaze(std::mt19937 gen,Node* head){

}

Maze::Node* Maze::findnode(double _value) {
    for(auto i:Nodes) {
        if(i->value==_value) return i;
    }
}

void Maze::newlayer(std::mt19937& gen,std::list<Node*> layer, double N, double nodecount, double layercount, double saveperlayer) {
    if (layercount==1) {
        std::list<Node*> next;
        std::list<Node*>::iterator it=layer.begin();
        bool flag=false;
        while(true){
            for (double j{};j<3;j++) {
                Node* a{new Node(nodecount)};
                merge(*it,a);
                nodecount++;
                N--;
                if (N==0) {
                    flag=true;
                    break;
                }
                next.push_back(a);
            }
            if (flag) break;
            it++;
            if(it==layer.end() && N>0) {
                newlayer(gen,next,N,nodecount,1,saveperlayer);
                break;
            }
        }    
    }
    else {
    double num{};
    double max= round((N-(layercount-1)*saveperlayer)/3)+1;
    if (max>layer.size()) {
        std::uniform_int_distribution<> distrib(1,layer.size());
        num=distrib(gen);}
    else {  
        std::uniform_int_distribution<> distrib(1,max);
        num=distrib(gen); }
    std::cout<<"num:"<<num<<std::endl;
    //size_t cnt{};
    double x{};
    double branches{};
    bool flag=false;
    bool mainflag=false;
    std::list<Node*> next;
    std::list<Node*>::iterator it=layer.begin();
    for (double i{}; i<num; i++) {
        x=N-(saveperlayer*layercount)-(num-(i+1));
        std::cout<<"x:"<<x<<std::endl;
        if (3>x && x>1) {
            std::uniform_int_distribution<> distribb(1,x);
            branches=distribb(gen);
        }    
        else if (x<=1) {
            branches=1;
            flag=true;
        }    
        else {   
            std::uniform_int_distribution<> distribb(1,3);
            branches=distribb(gen);
        } 
        std::cout<<"branches:"<<branches<<std::endl;   
        for (double j{};j<branches;j++) {
            Node* a{new Node(nodecount)};
            nodecount++;
            N--;
            merge(*it,a);
            if (N==0) {
                mainflag=true;
                break;
            }    
            next.push_back(a);
        }
        if (flag || mainflag) break;
        it++;
    }
    if (!mainflag) {
    std::cout<<"passed second! layer"<<std::endl;
    layercount--;
    newlayer(gen,next,N,nodecount,layercount,saveperlayer);
    }
    }
    //if statements for last layer and also checking the random generator and the decremental range!
}

void Maze::show() {
        std::cout<< head->value << " ";
        bool flag=false;
        for (auto j:head->children){
            if (flag) {
                std::cout<<"  ";
                flag=false;
            }    
            Nshow(*j,1,flag);
        }
    }

void Maze::Nshow(Node& n, double depth, bool & flag) {
    std::cout<< n.value<< " ";
    depth++;
    if(n.children.empty()) {
        std::cout<< std::endl;
        flag=true;
    }
    else {
        for (auto i:n.children) {
            if (flag) {
                for (size_t j{}; j<depth; j++) std::cout<<"  ";
                flag=false;
            }
            Nshow(*i,depth,flag);
        }
    }    
}

Maze::~Maze(){
    delete head;
}