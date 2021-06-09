#include <maze.h>

Maze::Node::Node(int _val,int _rp, int _cp): value(_val),rp(_rp),cp(_cp) {}

Maze::Node::Node(int _val): value(_val) {}

Maze::Node::Node(const Node& n, Maze& M) { 
    value=n.value;
    rp=n.rp;
    cp=n.cp;
    M.Nodes.push_back(this);
    if (!n.children.empty()) {
        for (auto i : n.children) {
            Node* a= new Node(*i,M);
            a->parent=this;
            this->children.push_back(a);
    }
    }
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
        n1->children.push_back(n2);
        n2->parent=n1;
        return n1;
}

Maze::Maze(int R, int C) {
    int N=R*C;
    int matR=3+2*(R-1);
    int matC=3+2*(C-1);
    if (N<9)
        throw std::logic_error("too little dimensions!");
    else{
    mat.resize(matR);
    int n{};
    for (int i{};i<matR;i++) {
        mat[i].resize(matC);
        for (int j{};j<matC;j++) {
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
    std::uniform_int_distribution<> distrib(0,C-1);
    int head_value=distrib(gen);
    //int head_cp=1+(head_value%C)*2;
    head=findnode(head_value);
    seen.insert(head_value);
    buildmaze(gen,head,1);
    if (seen.size()<N) {
        //std::cout<<"entered help builder!"<<std::endl;
        while(seen.size()!=N){
           // std::cout<<seen.size()<<std::endl;
            for (auto i:Nodes){
                if (!is_seen(i->value)) {
                    buildmaze(gen,i,2);
                    break;
                }
            }
        }
    }

    std::uniform_int_distribution<> distribb(0,C-1);
    int p=distribb(gen);
    for (int i{0};i<matR;i++) {
        for (int j{0};j<matC;j++) {
            if((i+j)%2!=0 && i!=matR-1 && j!=matC-1 && i!=0 && j!=0 && mat[i][j]==0) mat[i][j]=-2;
        }
    }
    for (int j{1};j<matC;j++) {
        if (mat[1][j]==head->value) mat[0][j]=-4;
        if(j==1+2*(p%C)) {
            mat[matR-1][j]=-4;
            exit=findnode(mat[matR-2][j]);
        }    

    }
}
}


void Maze::buildmaze(std::mt19937& gen,Node* input, int relation){
    std::set<size_t> direction{1,2,3,4};
    std::list<Node*> nextnodes;

    int up=mat[input->rp-1][input->cp];
    int down=mat[input->rp+1][input->cp];
    int left=mat[input->rp][input->cp-1];
    int right=mat[input->rp-1][input->cp+1];

    if(up==-2 || up==-1) direction.erase(1);
    if(down==-2 || down==-1) direction.erase(3);
    if(left==-2 || left==-1) direction.erase(4);
    if(right==-2 || right==-1) direction.erase(2);

    bool isolflag=true;
    if(relation==2) {
        seen.insert(input->value);
        for (auto i:direction){
            if (i==1 && is_seen(mat[input->rp-2][input->cp])) isolflag=false;
            else if (i==2 && is_seen(mat[input->rp][input->cp+2])) isolflag=false;
            else if (i==3 && is_seen(mat[input->rp+2][input->cp])) isolflag=false;
            else if (i==4 && is_seen(mat[input->rp][input->cp-2])) isolflag=false;
        }
    }    

    if (!direction.empty()) {
        std::uniform_int_distribution<> distrib(1,2);
        if(distrib(gen)==1) {
        std::set<size_t>::iterator it=direction.begin();
        std::uniform_int_distribution<> distribb(1,direction.size());
        for (size_t i{};i<distribb(gen);i++) {
            if(*it==1) {
                int next=mat[input->rp-2][input->cp];
                if(relation==1) {
                    if (is_seen(next)) continue;
                    else {
                        seen.insert(next);
                        mat[input->rp-1][input->cp]=-1;
                        Node* nxt=findnode(next);
                        nextnodes.push_back(nxt);
                        merge(input,nxt);
                    }
                }
                else {
                    mat[input->rp-1][input->cp]=-1;
                    Node* nxt=findnode(next);
                    if (is_seen(next)) {
                    merge(nxt,input);
                    break;
                    }
                    if (isolflag) merge(nxt,input);
                    else merge(input,nxt);
                } 
            }
            else if(*it==2) {
                int next=mat[input->rp][input->cp+2];
                if(relation==1) {
                    if (is_seen(next)) continue;
                    else {
                        seen.insert(next);
                        mat[input->rp][input->cp+1]=-1;
                        Node* nxt=findnode(next);
                        nextnodes.push_back(nxt);
                        merge(input,nxt);
                    }
                }
                else {
                    mat[input->rp][input->cp+1]=-1;
                    Node* nxt=findnode(next);
                    if (is_seen(next)) {
                    merge(nxt,input);
                    break;
                    }
                    if (isolflag) merge(nxt,input);
                    else merge(input,nxt);
                } 
            }
            else if(*it==3) {
                int next=mat[input->rp+2][input->cp];
                if(relation==1) {
                    if (is_seen(next)) continue;
                    else {
                        seen.insert(next);
                        mat[input->rp+1][input->cp]=-1;
                        Node* nxt=findnode(next);
                        nextnodes.push_back(nxt);
                        merge(input,nxt);
                    }
                }
                else {
                    mat[input->rp+1][input->cp]=-1;
                    Node* nxt=findnode(next);
                    if (is_seen(next)) {
                    merge(nxt,input);
                    break;
                    }
                    if (isolflag) merge(nxt,input);
                    else merge(input,nxt);
                } 
            }
            else if(*it==4) {
                int next=mat[input->rp][input->cp-2];
                if(relation==1) {
                    if (is_seen(next)) continue;
                    else{
                        seen.insert(next);
                        mat[input->rp][input->cp-1]=-1;
                        Node* nxt=findnode(next);
                        nextnodes.push_back(nxt);
                        merge(input,nxt);
                    }    
                }
                else {
                    mat[input->rp][input->cp-1]=-1;
                    Node* nxt=findnode(next);
                    if (is_seen(next)) {
                    merge(nxt,input);
                    break;
                    }
                    if (isolflag) merge(nxt,input);
                    else merge(input,nxt);
                } 
            }
            it++;
        }
    }
    else {
        std::set<size_t>::reverse_iterator it=direction.rbegin();
        std::uniform_int_distribution<> distribb(1,direction.size());
        for (size_t i{};i<distribb(gen);i++) {
            if(*it==1) {
                int next=mat[input->rp-2][input->cp];
                if(relation==1) {
                    if (is_seen(next)) continue;
                    else {
                        seen.insert(next);
                        mat[input->rp-1][input->cp]=-1;
                        Node* nxt=findnode(next);
                        nextnodes.push_back(nxt);
                        merge(input,nxt);
                    }
                }
                else {
                    mat[input->rp-1][input->cp]=-1;
                    Node* nxt=findnode(next);
                    if (is_seen(next)) {
                    merge(nxt,input);
                    break;
                    }
                    if (isolflag) merge(nxt,input);
                    else merge(input,nxt);
                } 
            }
            else if(*it==2) {
                int next=mat[input->rp][input->cp+2];
                if(relation==1) {
                    if (is_seen(next)) continue;
                    else {
                        seen.insert(next);
                        mat[input->rp][input->cp+1]=-1;
                        Node* nxt=findnode(next);
                        nextnodes.push_back(nxt);
                        merge(input,nxt);
                    }
                }
                else {
                    mat[input->rp][input->cp+1]=-1;
                    Node* nxt=findnode(next);
                    if (is_seen(next)) {
                    merge(nxt,input);
                    break;
                    }
                    if (isolflag) merge(nxt,input);
                    else merge(input,nxt);
                } 
            }
            else if(*it==3) {
                int next=mat[input->rp+2][input->cp];
                if(relation==1) {
                    if (is_seen(next)) continue;
                    else {
                        seen.insert(next);
                        mat[input->rp+1][input->cp]=-1;
                        Node* nxt=findnode(next);
                        nextnodes.push_back(nxt);
                        merge(input,nxt);
                    }
                }
                else {
                    mat[input->rp+1][input->cp]=-1;
                    Node* nxt=findnode(next);
                    if (is_seen(next)) {
                    merge(nxt,input);
                    break;
                    }
                    if (isolflag) merge(nxt,input);
                    else merge(input,nxt);
                } 
            }
            else if(*it==4) {
                int next=mat[input->rp][input->cp-2];
                if(relation==1) {
                    if (is_seen(next)) continue;
                    else{
                        seen.insert(next);
                        mat[input->rp][input->cp-1]=-1;
                        Node* nxt=findnode(next);
                        nextnodes.push_back(nxt);
                        merge(input,nxt);
                    }    
                }
                else {
                    mat[input->rp][input->cp-1]=-1;
                    Node* nxt=findnode(next);
                    if (is_seen(next)) {
                    merge(nxt,input);
                    break;
                    }
                    if (isolflag) merge(nxt,input);
                    else merge(input,nxt);
                } 
            }
            it++;
        }
    }
    if (!nextnodes.empty()) {
        for (auto i:nextnodes) buildmaze(gen,i,1);
    }
    }
}   

Maze::Node* Maze::findnode(int _value) {
    for(auto i:Nodes) {
        if(i->value==_value) return i;
    }
}

// void Maze::show() {
//         std::cout<< head->value << " ";
//         bool flag=false;
//         for (auto j:head->children){
//             if (flag) {
//                 std::cout<<"  ";
//                 flag=false;
//             }    
//             Nshow(*j,1,flag);
//         }
// }

// void Maze::Nshow(Node& n, int depth, bool & flag) {
//     std::cout<< n.value<< " ";
//     depth++;
//     if(n.children.empty()) {
//         std::cout<< std::endl;
//         flag=true;
//     }
//     else {
//         for (auto i:n.children) {
//             if (flag) {
//                 for (size_t j{}; j<depth; j++) std::cout<<"  ";
//                 flag=false;
//             }
//             Nshow(*i,depth,flag);
//         }
//     }    
// }

Maze::~Maze(){
    delete head;
}

void Maze::showmaze()
{
    for (size_t i{}; i < mat.size(); i++)
    {
        for (size_t j{}; j < mat[0].size(); j++) {
            if (mat[i][j]==-2){
            std::cout << "\x1b[2;30;47m   "; //\x1b[2;30;43m
            std::cout<<"\x1b[2;0;0m";
            }
            else 
            std::cout <<"   ";
        }
    std::cout<< std::endl;
    }        
}

Maze::Maze(const Maze& M) {
    mat=M.mat;
    head=new Node(*(M.head),*this);
}

Maze Maze::operator=(const Maze& M) {
    mat=M.mat;
    head=new Node(*(M.head),*this);
    return *this;
}

void Maze::DFS() {
    int target{exit->value};
    Maze copy{*this};
    std::list<Node*> branched;
    std::list<Node*> stack;
    stack.push_back(copy.head);
    int counter{1};
    //if children>1
    branched.push_back(copy.head);
    std::list<Node*>::iterator it=copy.head->children.begin();
    trackdown(stack,branched,*it,counter,target);
    showpath(stack);
    // for(auto i:stack) std::cout<<i->value<<" ";
    // std::cout<<std::endl;
}


void Maze::trackdown(std::list<Node*>& stack,std::list<Node*>& branched,Node* inp, int& counter, int& target) {
    stack.push_back(inp);
    counter++;
    if (inp->value==target) return;
    if (inp->children.size()>1) branched.push_back(inp);
    if (inp->children.empty()) {
        while((*stack.rbegin())->parent!=*(branched.rbegin())) stack.pop_back();
        std::list<Node*>::iterator it=(*branched.rbegin())->children.begin();
        while(*it!=*stack.rbegin()) it++;
        (*branched.rbegin())->children.erase(it);
        stack.pop_back();
        if((*branched.rbegin())->children.size()==1) branched.pop_back();
        trackdown(stack,branched,*((*stack.rbegin())->children.begin()),counter,target);
    }
    else trackdown(stack,branched,*(inp->children.begin()),counter,target);
}

void Maze::showpath(std::list<Node*> &stack) {
    std::cout<<std::endl<<std::endl;
    Matrix matt=mat;
    std::list<Node*>::iterator it=stack.begin();
    int r,c;
    
    while(true) {
        r=(*it)->rp;
        c=(*it)->cp;
        // std::cout<<r<<" "<<c<<std::endl;
        matt[r][c]=-3;
        it++;
        if (it==stack.end()) break;
        matt[((*it)->rp+r)/2][((*it)->cp+c)/2]=-3;
    }
    for (size_t i{}; i < matt.size(); i++)
    {
        for (size_t j{}; j < matt[0].size(); j++) {
            if (matt[i][j]==-2){
            std::cout << "\x1b[2;30;47m   "; //\x1b[2;30;43m
            std::cout<<"\x1b[2;0;0m";
            }
            else if(matt[i][j]==-3 || matt[i][j]==-4) {
            std::cout <<"\x1b[38;5;0m\x1b[48;5;216m * ";
            std::cout<<"\x1b[2;0;0m";
            }
            else 
            std::cout << "   ";
        }
    std::cout<< std::endl;
    }    
}