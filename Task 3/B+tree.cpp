#include <iostream>
#include <vector>

using namespace std;

class BTNode{
    int t;          // minimum degree
    int *keys;      // list of keys
    int nkeys;      // current no of keys
    BTNode **child; // list of child pointers
    bool isLeaf;    // true if leaf
    BTNode *next;   // pointer to next leaf node (if leaf)

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
    this->next = NULL;
}

void BTNode::splitChildNode(int ind, BTNode *c){
    BTNode *newBTNode = new BTNode(c->t, c->isLeaf);

    if(c->isLeaf){
        newBTNode->nkeys = t;
        for(int j=0; j<t; j++)
            newBTNode->keys[j] = c->keys[t-1+j];
        
            c->nkeys = t-1;
            newBTNode->next = c->next;
            c->next = newBTNode;
    }
    else{
        newBTNode->nkeys = t-1;
        for(int j=0; j<t-1; j++){
            newBTNode->keys[j] = c->keys[t+j];
        }
        for(int j=0; j<t; j++){
            newBTNode->child[j] = c->child[t+j];
        }
        c->nkeys = t-1;
    }

    for(int j = nkeys; j>=ind+1; j--){
        child[j+1] = child[j];
    }
    child[ind+1] = newBTNode;

    for(int j=nkeys; j>= ind+1; j--){
        keys[j+1] = keys[j];
    }
    if(c->isLeaf){
        keys[ind] = newBTNode->keys[0];
    }
    else{
        keys[ind] = c->keys[t-1];
    }
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
            if(keys[ind+1]<=k) ind++;
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
    if(!isLeaf){
        while(i<nkeys && k>=keys[i]) i++;
        return child[i]->search(k);
    }

    while(i<nkeys && k>keys[i]) i++;
    if(i<nkeys && keys[i]==k) return this;

    return NULL;
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
            if(newBTNode->keys[0]<=k) i++;
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