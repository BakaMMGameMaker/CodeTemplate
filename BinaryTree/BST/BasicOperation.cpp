#include "../TreeNode.h"
#include <bits/stdc++.h>
using namespace std;

// 二叉树的搜索插入与删除

// 搜索
TreeNode *searchBST(TreeNode *root, int val) {
    if (not root or root->val == val) return root;

    if (val < root->val) return searchBST(root->left, val);
    return searchBST(root->right, val);
}

// 迭代写法
TreeNode *searchBSTIterative(TreeNode *root, int val) {
    TreeNode *cur = root;
    while (cur) {
        if (cur->val == val) return cur;
        if (val < cur->val) cur = cur->left;
        else cur = cur->right;
    }
    return nullptr;
}

// 插入
TreeNode *insertIntoBST(TreeNode *root, int val) {
    if (not root) return new TreeNode(val);
    if (val < root->val) root->left = insertIntoBST(root->left, val);
    else if (val > root->val) root->right = insertIntoBST(root->right, val);
    return root; // val 与当前已有根节点的值一致, 不插入重复值
}

// 迭代写法
// cur - 当前遍历的节点
// parent - cur 的父节点
TreeNode *insertIntoBSTIterative(TreeNode *root, int val) {
    if (not root) return new TreeNode(val);

    TreeNode *cur = root;
    TreeNode *parent = nullptr;
    while (cur) {
        parent = cur;
        if (val == cur->val) return root;
        if (val < cur->val) cur = cur->left;
        else if (val > cur->val) cur = cur->right;
    }

    // cur 定位到了要插入的位置, parent 就是 cur 最终的父节点
    if (val < parent->val) parent->left = new TreeNode(val);
    else parent->right = new TreeNode(val);

    return root;
}

// 删除
// 如果要删除 root
// - 没有左孩子 - 直接返回右孩子顶上来
// - 没有右孩子 - 直接返回左孩子顶上来
// - 左右孩子都有 - 右子树最小节点替换当前节点 / 左子树最大节点替换当前节点

TreeNode *GetMin(TreeNode *node) {
    while (node->left) node = node->left;
    return node;
}

TreeNode *deleteNode(TreeNode *root, int key) { // 返回删除节点之后树的根节点
    if (not root) return nullptr;

    if (key < root->val) {
        root->left = deleteNode(root->left, key);
    } else if (key > root->val) {
        root->right = deleteNode(root->right, key);
    } else {
        if (not root->left) {
            auto rightNode = root->right;
            delete root;
            return rightNode;
        }
        // root.left not null
        if (not root->right) {
            auto leftNode = root->left;
            delete root;
            return leftNode;
        }
        // root.right also not null
        TreeNode *successor = GetMin(root->right);
        root->val = successor->val;
        root->right = deleteNode(root->right, successor->val);
    }
    return root;
}
