#include <iostream>

using namespace std;

enum Color{red, black};

struct Node{
    int data;
    Color color;
    Node *left, *right, *parent;

    Node(int data){
        this->data = data;
        left = right = parent = nullptr;
        this->color = red;
    }
};

class RBTree{
private:
    Node *root;

protected:
    void rotateLeft(Node* &ptr){
        if(ptr == nullptr || ptr->right == nullptr)
            return;

        Node *rightChild = ptr->right;
        ptr->right = rightChild->left;

        if(ptr->right != nullptr)
            ptr->right->parent = ptr;

        rightChild->parent = ptr->parent;

        if(ptr->parent == nullptr)
            root = rightChild;
        else if(ptr == ptr->parent->left)
            ptr->parent->left = rightChild;
        else
            ptr->parent->right = rightChild;

        rightChild->left = ptr;
        ptr->parent = rightChild;
    }

    void rotateRight(Node* &ptr){
        if(ptr == nullptr || ptr->left == nullptr)
            return;

        Node *leftChild = ptr->left;
        ptr->left = leftChild->right;

        if(ptr->left != nullptr)
            ptr->left->parent = ptr;

        leftChild->parent = ptr->parent;

        if(ptr->parent == nullptr)
            root = leftChild;
        else if(ptr == ptr->parent->left)
            ptr->parent->left = leftChild;
        else
            ptr->parent->right = leftChild;

        leftChild->right = ptr;
        ptr->parent = leftChild;
    }

    void fixViolation(Node* &ptr){
        Node *parentPtr = nullptr;
        Node *grandParentPtr = nullptr;

        while(ptr != root &&
              ptr->parent != nullptr &&
              ptr->parent->color == red){

            parentPtr = ptr->parent;
            grandParentPtr = parentPtr->parent;

            if(grandParentPtr == nullptr)
                break;

            if(parentPtr == grandParentPtr->left){
                Node *unclePtr = grandParentPtr->right;

                if(unclePtr != nullptr && unclePtr->color == red){
                    grandParentPtr->color = red;
                    parentPtr->color = black;
                    unclePtr->color = black;
                    ptr = grandParentPtr;
                } 
                else{
                    if(ptr == parentPtr->right){
                        rotateLeft(parentPtr);
                        ptr = parentPtr;
                        parentPtr = ptr->parent;
                    }
                    
                    rotateRight(grandParentPtr);
                    swap(parentPtr->color, grandParentPtr->color);
                    ptr = parentPtr;
                }
            }
            else{
                Node *unclePtr = grandParentPtr->left;

                if(unclePtr != nullptr && unclePtr->color == red){
                    grandParentPtr->color = red;
                    parentPtr->color = black;
                    unclePtr->color = black;
                    ptr = grandParentPtr;
                }
                else{
                    if(ptr == parentPtr->left){
                        rotateRight(parentPtr);
                        ptr = parentPtr;
                        parentPtr = ptr->parent;
                    }

                    rotateLeft(grandParentPtr);
                    swap(parentPtr->color, grandParentPtr->color);
                    ptr = parentPtr;
                }
            }
        }

        root->color = black;
    }

public:
    RBTree(){ 
        root = nullptr;
    }

    void insert(const int &data){
        Node *ptr = new Node(data);

        if(root == nullptr){
            ptr->color = black;
            root = ptr;
        }
        else{
            Node *temp = root;
            Node *parent = nullptr;

            while(temp != nullptr){
                parent = temp;
                if(ptr->data < temp->data)
                    temp = temp->left;
                else
                    temp = temp->right;
            }

            ptr->parent = parent;
            if(ptr->data < parent->data)
                parent->left = ptr;
            else
                parent->right = ptr;

            fixViolation(ptr);
        }
    }

    void inorder(Node *ptr){
        if(ptr == nullptr) return;

        inorder(ptr->left);
        cout<<ptr->data<<"(" 
            <<(ptr->color == black ? "B" : "R")<<") ";
        inorder(ptr->right);
    }

    void display(){ 
        inorder(root);
        cout<<endl; 
    }
};

int main(){
    RBTree tree;

    int t;
    cout<<"Enter the no of elements to insert: ";
    cin>>t;

    while(t--){
        int x;
        cin>>x;
        tree.insert(x);
    }

    cout<<"Inorder Traversal"<<endl;
    tree.display();
}
