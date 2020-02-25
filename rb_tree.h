#ifndef __RB_TREE_H__
#define __RB_TREE_H__

#include <iostream>

using namespace std;

enum RBTColor {
    RED,
    BLACK
};

template <typename T>
class RBTNode {
public:
    T key_;
    RBTColor color_;
    RBTNode* left_;
    RBTNode* right_;
    RBTNode* parent_;

    RBTNode(T value, RBTColor color, RBTNode* left, RBTNode* right, RBTNode* parent):
    key_(value), color_(color), left_(left), right_(right), parent_(parent){}
};

template <typename T>
class RBTree {
public:
    // 为了方面，把根节点设为public
    RBTNode<T>* root_;
    // 红黑树的主要操作：插入，插入修正， 左旋，右旋，
    // todo: 删除，删除修正，查找，等
    void leftRotate(RBTNode<T>* &root, RBTNode<T>* x);
    void rightRotate(RBTNode<T>* &root, RBTNode<T>* y);
    void insert(RBTNode<T>* &root, RBTNode<T>* node);
    void insertFix(RBTNode<T>* &root, RBTNode<T>* node);

    //辅助函数
    void insert(T key);
    void inOrder(RBTNode<T>*& root) const; // 中序遍历
    void inOrder();

};

template<typename T>
void RBTree<T>::leftRotate(RBTNode<T> *&root, RBTNode<T> *x) {
    // 要注意的是，左旋基于的点是插入点的父节点
    // 左旋的点肯定右子节点
    RBTNode<T>* y = x->right_;
    x->right_ = y->left_;
    // 如果y的左子节点不为空
    if(y->left_ != nullptr) {
        y->left_->parent_ = x;
    }

    // 将x的父亲舍为y的父亲
    y->parent_ = x->parent_;

    if(y->parent_ == nullptr) {
        root = y;
    }
    else {
        if(x->parent_->left_ == x) {
            x->parent_->left_ = y;
        }
        else {
            x->parent_->right_ = y;
        }
    }

    // 把y和x的关系
    y->left_ = x;
    x->parent_ = y;
}

template<typename T>
void RBTree<T>::rightRotate(RBTNode<T> *&root, RBTNode<T> *y) {
    RBTNode<T>* x = y->left_;
    y->left_ = x->right_;
    if(y->right_ != nullptr) {
        y->right_->parent_ = y->right_;
    }

    x->parent_ = y->parent_;
    if(x->parent_ == nullptr) {
        root = x;
    }
    else {
        if(x->parent_->left_ == y) {
            x->parent_->left_ = x;
        }
        else {
            x->parent_->right_ = x;
        }
    }


    x->right_ = y;
    y->parent_ = x;
}

template<typename T>
void RBTree<T>::insert(RBTNode<T> *&root, RBTNode<T> *node) {
    RBTNode<T>* y = nullptr;
    RBTNode<T>* x = root;
    // 根据二叉树查找到合适的插入点
    while(x != nullptr) {
        y = x;
        if(node->key_ < x->key_) {
            x = x->left_;
        }
        else {
            x = x->right_;
        }
    }

    node->parent_ = y;
    // 设置y的子节点关系
    if(y != nullptr) {
        if(node->key_ < y->key_) {
            y->left_ = node;
        }
        else {
            y->right_ = node;
        }
    }
    else {
//        node = root;
        root = node;
    }

    // 设置node为红色
    node->color_ = RED;

    // 修正为红黑树
    insertFix(root, node);
}

template<typename T>
void RBTree<T>::insertFix(RBTNode<T> *&root, RBTNode<T> *node) {
    // 首先要搞好node的父节点和grandparent节点
    RBTNode<T>* parent, *grandparent;
    // 如果父亲节点不为空
    while((parent = node->parent_) && (parent->color_ == RED)) {
        grandparent = parent->parent_;   // parent颜色为红色，所以不可能为root， 因此这个不可能为空

        // 当parent为grandparent的左节点时
        if(parent == grandparent->left_) {
            RBTNode<T>* uncle = grandparent->right_;
            if(uncle && uncle->color_ == RED) {
                // 1. 当uncle节点为红
                // 这个时候：parent和uncle都是红，node也是红，调整节点的颜色即可
                parent->color_ = BLACK;
                uncle->color_ = BLACK;
                grandparent->color_ = RED;
                // 要注意，把node换成grandparent
                node = grandparent;
                continue;
            }

            {
                // 2. uncle 节点为黑
                // 2.1 节点为parent 右节点： 这里要首先右再左
                if(node == parent->right_) {
                    RBTNode<T>* tmp;
                    leftRotate(root, parent);
                    parent = tmp;
                    parent = node;
                    node = tmp;
                }

                // 2.2 节点为parent的左节点
                parent->color_ = BLACK;
                grandparent->color_ = RED;
                rightRotate(root, grandparent);

            }

        }

        // 当parent为grandparent的右子节点时
        else {
            RBTNode<T>* uncle = grandparent->right_;
            // 1. uncle为红色
            if(uncle && uncle->color_ == RED) {
                uncle->color_ = BLACK;
                parent->color_= BLACK;
                grandparent->color_ = RED;
                node = grandparent;
                continue;
            }
            // 2. uncle 为黑色
            else {
                if(node == parent->left_){
                    // 2.1 node 为parent的左子节点
                    // 需要右旋
                    rightRotate(root, parent);
                    RBTNode<T>* tmp = parent;
                    parent = node;
                    node = tmp;
                }
                // 这里不用else
                parent->color_ = BLACK;
                grandparent->color_ = RED;
                leftRotate(root, grandparent);
            }
        }

    }


    root->color_ = BLACK;


}

template <typename T>
void RBTree<T>::insert(T key) {
    RBTNode<T>* tmp = nullptr;
    if((tmp = new RBTNode<T>(key, BLACK, nullptr, nullptr, nullptr)) == nullptr) {
        return;
    }

    insert(root_, tmp);
}

template<typename T>
void RBTree<T>::inOrder(RBTNode<T>* &root) const {
    if(root != nullptr) {
        inOrder(root->left_);
        cout<< "key: " << root->key_ << " color: " << ((root->color_ == RED) ? "red" : "black") << endl;
        inOrder(root->right_);
    }
}

template<typename T>
void RBTree<T>::inOrder() {
    inOrder(this->root_);
}


#endif