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
