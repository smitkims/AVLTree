#include <iostream>
#include <algorithm>
#include <optional>
#include <unistd.h>

template <typename T, typename U>
class AVLNode {

public:
    T key;
    U value;
    AVLNode<T, U>* left;
    AVLNode<T, U>* right;
    int height;

    AVLNode<T, U>(const T& k, const U& v)
    {
        key = k;
        value = v;
        left = nullptr;
        right = nullptr;
        height = 1;
    }

};

template <typename T, typename U>
class AVLTree {
public:
    AVLNode<T, U>* root = nullptr;
    ~AVLTree() {
        removeall(root);
    }

    void insert(const T& key, const U& value);
    U search(const T& key);
    bool remove(const T& key);

    //for checking
    void preorder(AVLNode<T, U>*& node) {
        if (!node) return;

        std::cout << node->key << ": " << node->value << std::endl;
        preorder(node->left);
        preorder(node->right);
    }
    void inorder(AVLNode<T, U>*& node) {
        if (!node) return;

        inorder(node->left);
        std::cout << node->key << ": " << node->value << std::endl;
		inorder(node->right);
    }


private:
    int getHeight(AVLNode<T, U>*& node);
    int getBalance(AVLNode<T, U>*& node);
    AVLNode<T, U>* rotate_left(AVLNode<T, U>* node);
    AVLNode<T, U>* rotate_right(AVLNode<T, U>* node);

    AVLNode<T, U>* insert(AVLNode<T, U>*& node, const T& key, const U& value);
    U search(AVLNode<T, U>*& node, const T& key);
    AVLNode<T, U>* remove(AVLNode<T, U>*& node, const T& key);
    void removeall(AVLNode<T, U>*& node);

};

template <typename T, typename U>
int AVLTree<T, U>::getHeight(AVLNode<T, U>*& node) {
    if (!node) return 0;
    return node->height;
}

template <typename T, typename U>
int AVLTree<T, U>::getBalance(AVLNode<T, U>*& node) {
    if (!node) return 0;
    return getHeight(node->left) - getHeight(node->right);
}

template<typename T, typename U>
void AVLTree<T, U>::insert(const T& key, const U& value) {
    root = insert(root, key, value);
}

template<typename T, typename U>
U AVLTree<T, U>::search(const T& key) {
    return search(root, key);
}

template<typename T, typename U>
bool AVLTree<T, U>::remove(const T& key) {
    if (!search(root, key)) return false;
    root = remove(root, key);
    return true;
}

template<typename T, typename U>
AVLNode<T, U>* AVLTree<T, U>::rotate_left(AVLNode<T, U>* node) {

    AVLNode<T, U>* a = node->right;
    AVLNode<T, U>* b = a->left;

    // rotation
    a->left = node;
    node->right = b;

    // height
    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
    a->height = 1 + std::max(getHeight(a->left), getHeight(a->right));
    
    return a; // return parent node
}

template<typename T, typename U>
AVLNode<T, U>* AVLTree<T, U>::rotate_right(AVLNode<T, U>* node) {
    AVLNode<T, U>* a = node->left;
    AVLNode<T, U>* b = a->right;

    // rotation
    a->right = node;
    node-> left = b;

    // height   
    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
    a->height = 1 + std::max(getHeight(a->left), getHeight(a->right));

    return a; // return parent node
}

template<typename T, typename U>
AVLNode<T, U>* AVLTree<T, U>::insert(AVLNode<T, U>*& node, const T& key, const U& value) {
    if (node == nullptr)
    {  
        node = new AVLNode<T, U>(key, value);
        
        return node;
    }

    ////////// traverse to place to insert
    if (key.compare(node->key)<0) 
        node->left = insert(node->left, key, value); //traverse left

    else if (key.compare(node->key)>0)
        node->right = insert(node->right, key, value);  //traverse right

    else // key already exists
        node->value = value; // rewrite the value
    

    // update height
    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));

    ////////// rotation to balance
    int bal = getBalance(node);
    
    // LL & LR
    if (bal > 1)
    {
        //LL
        if (getBalance(node->left) >= 0) // includes child balance equal to 0
            return rotate_right(node); //return parent node
        

        //LR
        else // getBalance(node->left) < 0
        {
            // make LL
            node->left = rotate_left(node->left);
            return rotate_right(node);
        }
    }

    // RR & RL
    if (bal < -1)
    {
        // RR
        if (getBalance(node->right) <= 0) // includes child balance equal to 0
            return rotate_left(node);
        

        // RL
        else // getBalance(node->right) > 0
        {
            // make RR
            node->right = rotate_right(node->right);
            return rotate_left(node);
        }
    }
    return node;
}

template<typename T, typename U>
U AVLTree<T, U>::search(AVLNode<T, U>*& node, const T& key) {
    //TODO

    AVLNode<T, U>* temp = node;
    while (temp) // do until key is found or reaches the leaf node
    {
        if (key.compare(temp->key) == 0) 
            return temp->value;
        if (key.compare(temp->key) < 0)
        {
            temp = temp->left; // traverse left
        }
        else if (key.compare(temp->key) > 0)
        {
            temp = temp->right; // traverse right
        }
    }
    if (temp == nullptr) return 0; // key not found
	return temp->value;
}

template<typename T, typename U>
AVLNode<T, U>* AVLTree<T, U>::remove(AVLNode<T, U>*& node, const T& key) {
	// the public remove function deals with the case when key doesn't exist

    // root is null
    if (node == nullptr) return NULL;
    // key smaller than current node key
    if (key.compare(node->key) < 0) node->left = remove(node->left, key);
    // key greater than current node key
    else if (key.compare(node->key) > 0) node->right = remove(node->right, key);

    // same key
    else
    {
        // no child
        if (node->left == NULL && node->right == NULL)
        {   
            delete(node);
            node = nullptr;
            return NULL;
        }
        // one right child
        else if (node->left == NULL) {
			AVLNode<T, U>* temp;
			temp = node->right; // point temp_one_right to node->right then delete node
			delete(node);
			node = temp;
			return node;
        }
        // one left child
        else if (node->right == NULL) {
			AVLNode<T, U>* temp;
			temp = node->left;
			delete(node);
			node = temp;
			return node;
        }
        // two children
        else
        {
            // find min of right subtree
            AVLNode<T, U>* temp;
            temp = node->right;
            while (temp && temp->left != nullptr)
                temp = temp->left;
            

			// copy values to node (remove the corresponding object in the subtree)
            node->key = temp->key;
            node->value = temp->value;
            node->right = remove(node->right, temp->key);
        }
    }

    // update height
    node->height = 1 + std::max(getHeight(node->left),getHeight(node->right));

    ////////// rotation to balance
    int bal = getBalance(node);

    // LL & LR
    if (bal > 1)
    {
        //LL
        if (getBalance(node->left) >= 0) // includes child balance equal to 0
            return rotate_right(node); //return parent node
        

        //LR
        else // getBalance(node->left) < 0
        {
            // make LL
            node->left = rotate_left(node->left);
            return rotate_right(node);
        }
    }

    // RR & RL
    if (bal < -1)
    {
        // RR
        if (getBalance(node->right) <= 0) // includes child balance equal to 0
            return rotate_left(node);
        

        // RL
        else // getBalance(node->right) > 0
        {
            // make RR
            node->right = rotate_right(node->right);
            return rotate_left(node);
        }
    }
    return node;
}

template<typename T, typename U>
void AVLTree<T, U>::removeall(AVLNode<T, U>*& node) {
	// when node is null return
    if (!node) return;

	// traverse the tree to delete nodes
    removeall(node->left);
    removeall(node->right);
    delete(node);
    node = nullptr;
}
