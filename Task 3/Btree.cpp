#include <iostream>
#include <vector>

using namespace std;

class BTNode{
    int t;          // minimum degree
    int *keys;      // list of keys
    int nkeys;      // current no of keys
    BTNode **child; // list of child pointers
    bool isLeaf;    // true if leaf

public:
    BTNode(int t, bool isLeaf);
    void splitChildNode(int i, BTNode *c);
    void insertNonFull(int k);
    void traverse();
    BTNode* search(int k);

friend class Btree;
};

BTNode::BTNode(int t, bool isLeaf){
    this->t = t;
    this->nkeys = 0;
    this->isLeaf = isLeaf;
    this->keys = new int[2*t-1];
    this->child = new BTNode*[2*t];
}

void BTNode::splitChildNode(int ind, BTNode *c){
    BTNode *newBTNode = new BTNode(c->t, c->isLeaf);

    for(int i=0; i<t-1; i++){
        newBTNode->keys[i] = c->keys[t+i];
    }
    c->nkeys = t-1;
    newBTNode->nkeys = t-1;
    
    if(!c->isLeaf){
        for(int i=0; i<t; i++){
            newBTNode->child[i] = c->child[t+i];
        }
    }

    for(int i=nkeys; i>=ind+1; i--){
        child[i+1] = child[i];
    }

    child[ind+1] = newBTNode;

    for(int i=nkeys-1; i>=ind; i--){
        keys[i+1] = keys[i];
    }

    keys[ind] = c->keys[t-1];
    nkeys++;
}

void BTNode::insertNonFull(int k){
    int ind = nkeys-1;

    if(isLeaf){
        while(ind>=0 && keys[ind]>k){
            keys[ind+1] = keys[ind];
            ind--;
        }
        keys[ind+1] = k;
        nkeys++;
    }
    
    else{
        while(ind>=0 && keys[ind]>k){
            ind--;
        }

        if(child[ind+1]->nkeys == 2*t-1){
            splitChildNode(ind+1, child[ind+1]);
            if(keys[ind+1]<k) ind++;
        }
        child[ind+1]->insertNonFull(k);
    }
}

void BTNode::traverse(){
    int i;
    for(i=0; i<nkeys; i++){
        if(!isLeaf) child[i]->traverse();
        cout<<" "<<keys[i];
    }
    if(!isLeaf) child[i]->traverse();
}

BTNode* BTNode::search(int k){
    int i=0;
    while(i<nkeys && k>keys[i]) i++;
    
    if(keys[i] == k) return this;
    if(isLeaf) return NULL;
    return child[i]->search(k);
}

class Btree{
    BTNode *root;
    int t;
public:
    Btree(int t){
        this->root = NULL;
        this->t = t;
    }
    void insert(int k);
    void traverse(){
        if(root!=NULL) root->traverse();
    }
    BTNode* search(int k){
        if(root==NULL) return NULL;
        else return root->search(k);
    }
};

void Btree::insert(int k){
    if(root==NULL){
        root = new BTNode(t, true);
        root->keys[0] = k;
        root->nkeys = 1;
    }
    else{
        if(root->nkeys == 2*t-1){
            BTNode *newBTNode = new BTNode(t, false);
            newBTNode->child[0] = root;
            newBTNode->splitChildNode(0, root);

            int i=0;
            if(newBTNode->keys[0]<k) i++;
            newBTNode->child[i]->insertNonFull(k);

            root = newBTNode;
        }
        else root->insertNonFull(k);
    }
}

int main(){
    Btree bt(2);
    int t;
    cout<<"Enter the no of elements to insert: ";
    cin>>t;
    while(t--){
        int x;
        cin>>x;
        bt.insert(x);
    }

    cout<<"Traversal is "<<endl;
    bt.traverse();

    cout<<"\nEnter no of elements to search: ";
    int k; cin>>k;
    while(k--){
        int x;
        cin>>x;
        if(bt.search(x)==NULL) cout<<"Not found"<<endl;
        else cout<<"Found"<<endl;
    }
}