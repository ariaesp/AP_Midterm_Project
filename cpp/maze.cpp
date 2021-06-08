#include <maze.h>

//#define saveperlayer=2

// Maze::Node::Node (int _value, Node& _parent, std::list<Node*> _children)  
//     {
//         value=_value;
//         parent=&_parent;
//         children=_children;
//         order=get_order();
//         _parent.children.push_back(this);
//         for (auto &n : _children)
//             n->parent=this;
//     }

Maze::Node::Node(int _val,int _rp, int _cp): value(_val),rp(_rp),cp(_cp) {}

Maze::Node::Node(int _val): value(_val) {}


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
                Node* mat= new Node(n,i,j);
                Nodes.push_back(mat);
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

    for (int i{1};i<matR;i++) {
        for (int j{1};j<matC;j++) {
            if( (i+j)%2!=0 && i!=matR-1 && j!=matC-1 && mat[i][j]==0) mat[i][j]=-2;
        }
    }

    // int layercount{round(sqrt(N))};
    // int saveperlayer{2};
    // head= new Node(0);
    // int nodecount{1};
    // N--;
    // int branches{};
    // int x=N-(saveperlayer*layercount);
    // if (3>x){
    //     branches=distrib(gen);
    // }    
    // else {  
    //     std::uniform_int_distribution<> distrib(1,3);
    //     branches=distrib(gen);
    // }
    // for (int i{};i<branches;i++) {
    //     Node* mat{new Node(nodecount)};
    //     nodecount++;
    //     N--;
    //     merge(head,mat);
    // }
    // std::cout <<"passed first layer and nodecount:"<<nodecount<<std::endl;
    // layercount--;
    // newlayer(gen,head->children,N,nodecount,layercount,saveperlayer);
    // }
}
}


void Maze::buildmaze(std::mt19937& gen,Node* input, int relation){
    std::set<size_t> direction{1,2,3,4};
    std::list<Node*> nextnodes;
    if(relation==2) seen.insert(input->value);

    int up=mat[input->rp-1][input->cp];
    int down=mat[input->rp+1][input->cp];
    int left=mat[input->rp][input->cp-1];
    int right=mat[input->rp-1][input->cp+1];

    if(up==-2 || up==-1) direction.erase(1);
    if(down==-2 || down==-1) direction.erase(3);
    if(left==-2 || left==-1) direction.erase(4);
    if(right==-2 || right==-1) direction.erase(2);

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
                    merge(nxt,input);
                    break;
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
                    merge(nxt,input);
                    break;
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
                    merge(nxt,input);
                    break;
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
                    merge(nxt,input);
                    break;
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
                    merge(nxt,input);
                    break;
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
                    merge(nxt,input);
                    break;
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
                    merge(nxt,input);
                    break;
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
                    merge(nxt,input);
                    break;
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
//     int direction=distrib(gen); // 1=up or down , 2=right or left
//     int back_front=distrib(gen); // 1=up and right 2=down and left
//     if (direction==1) 
// }


Maze::Node* Maze::findnode(int _value) {
    for(auto i:Nodes) {
        if(i->value==_value) return i;
    }
}

// void Maze::newlayer(std::mt19937& gen,std::list<Node*> layer, int N, int nodecount, int layercount, int saveperlayer) {
//     if (layercount==1) {
//         std::list<Node*> next;
//         std::list<Node*>::iterator it=layer.begin();
//         bool flag=false;
//         while(true){
//             for (int j{};j<3;j++) {
//                 Node* mat{new Node(nodecount)};
//                 merge(*it,mat);
//                 nodecount++;
//                 N--;
//                 if (N==0) {
//                     flag=true;
//                     break;
//                 }
//                 next.push_back(mat);
//             }
//             if (flag) break;
//             it++;
//             if(it==layer.end() && N>0) {
//                 newlayer(gen,next,N,nodecount,1,saveperlayer);
//                 break;
//             }
//         }    
//     }
//     else {
//     int num{};
//     int max= round((N-(layercount-1)*saveperlayer)/3)+1;
//     if (max>layer.size()) {
//         std::uniform_int_distribution<> distrib(1,layer.size());
//         num=distrib(gen);}
//     else {  
//         std::uniform_int_distribution<> distrib(1,max);
//         num=distrib(gen); }
//     std::cout<<"num:"<<num<<std::endl;
//     //size_t cnt{};
//     int x{};
//     int branches{};
//     bool flag=false;
//     bool mainflag=false;
//     std::list<Node*> next;
//     std::list<Node*>::iterator it=layer.begin();
//     for (int i{}; i<num; i++) {
//         x=N-(saveperlayer*layercount)-(num-(i+1));
//         std::cout<<"x:"<<x<<std::endl;
//         if (3>x && x>1) {
//             std::uniform_int_distribution<> distribb(1,x);
//             branches=distribb(gen);
//         }    
//         else if (x<=1) {
//             branches=1;
//             flag=true;
//         }    
//         else {   
//             std::uniform_int_distribution<> distribb(1,3);
//             branches=distribb(gen);
//         } 
//         std::cout<<"branches:"<<branches<<std::endl;   
//         for (int j{};j<branches;j++) {
//             Node* mat{new Node(nodecount)};
//             nodecount++;
//             N--;
//             merge(*it,mat);
//             if (N==0) {
//                 mainflag=true;
//                 break;
//             }    
//             next.push_back(mat);
//         }
//         if (flag || mainflag) break;
//         it++;
//     }
//     if (!mainflag) {
//     std::cout<<"passed second! layer"<<std::endl;
//     layercount--;
//     newlayer(gen,next,N,nodecount,layercount,saveperlayer);
//     }
//     }
//     //if statements for last layer and also checking the random generator and the decremental range!
// }

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

void Maze::Nshow(Node& n, int depth, bool & flag) {
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

void Maze::matshow()
{
    for (size_t i{}; i < mat.size(); i++)
    {
        for (size_t j{}; j < mat[0].size(); j++) {
            if (mat[i][j]==-2){
            std::cout << "\x1b[2;30;47m # "; //\x1b[2;30;43m
            std::cout<<"\x1b[2;0;0m";
            }
            else if (mat[i][j]==-1)
            std::cout << "   ";
            else { if (mat[i][j]>9) {
                int k=round(mat[i][j]/10);
                if (k>9) k=k/10;
                std::cout <<" "<< k<<" ";
            }
            else
            std::cout <<" "<< mat[i][j]<<" ";
        }
        }
        std::cout<<"\x1b[2;0;0m";
    std::cout<< std::endl;
    }        
}