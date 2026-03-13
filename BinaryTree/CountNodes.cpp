#include <bits/stdc++.h>
#include "TreeNode.h"
using namespace std;

// 统计节点个数

// 递归
// - 左子树节点个数 + 右子树节点个数 + 1
int countNodes(const TreeNode *root) {
    if (not root) return 0;
    return countNodes(root->left) + countNodes(root->right) + 1;
}

// 非递归 - 层序遍历
int countNodesBFS(const TreeNode *root) {
    if (not root) return 0;

    deque q{root};
    int count = 0;
    while (!q.empty()) {
        auto node = q.front();
        q.pop_front();
        count++;

        if (node->left) q.push_back(node->left);
        if (node->right) q.push_back(node->right);
    }
    return count;
}

int countNodesDFS(const TreeNode *root) {
    if (not root) return 0;
    vector st{root};
    int count = 0;

    while (!st.empty()) {
        auto node = st.back();
        st.pop_back();
        count++;

        if (node->right) st.push_back(node->right);
        if (node->left) st.push_back(node->left);
    }
    return count;
}

// 完全二叉树的 CountNodes
// 性质 - 除了最后一层, 前面每一层都满, 最后一层的节点尽量靠左排
// 由此可知, 对于某一棵子树, 若其最左高度等于最右高度, 那么其为满二叉树
// 满二叉树的节点数量为 2^h - 1

// 解题思路
// 对于当前根节点
// - 一路往左得 leftHeight, 一路往右得 rightHeight
// - 二者相等 => 满二叉树 => (1 << leftHeight) - 1
// - 二者不等 => 1 + countNodes(root.left) + countNodes(root.right)
int getLeftHeight(const TreeNode *node) {
    int h = 0;
    while (node) {
        h++;
        node = node->left;
    }
    return h;
}

int getRightHeight(const TreeNode *node) {
    int h = 0;
    while (node) {
        h++;
        node = node->right;
    }
    return h;
}

int countNodesII(const TreeNode *root) {
    if (not root) return 0;
    int lh = getLeftHeight(root);
    int rh = getRightHeight(root);

    if (lh == rh) return (1 << lh) - 1;
    return 1 + countNodesII(root->left) + countNodesII(root->right);
}
