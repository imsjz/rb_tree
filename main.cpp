#include <iostream>
#include "rb_tree.h"
using namespace std;

int main()
{
    int a[]= {10, 40, 30, 60, 90, 70, 20, 50, 87};

    int array_length = sizeof(a) / sizeof(a[0]);

    cout << "== 原始数据: " << endl;
    for(int i = 0; i < array_length; ++i) {
        cout << a[i] << " ";
    }
    cout << endl;

    // 创建树
    RBTree<int>* tree = new RBTree<int>();

    for(int i = 0; i < array_length; ++i) {
        cout << "插入节点 " << a[i] << endl;
        tree->insert(a[i]);
        tree->inOrder();
    }
    // 遍历
    tree->inOrder();



    return 0;
}