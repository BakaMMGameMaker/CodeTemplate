#include <bits/stdc++.h>
#include "TreeNode.h"
using namespace std;

// 判断二叉树的性质

// 两棵树是否相等
bool isSameTree(const TreeNode* p, const TreeNode* q) {
    if (not p and not q) return true;
    if (not p or not q) return false;
    if (p->val != q->val) return false;
    return isSameTree(p->left, q->left) and isSameTree(p->right, q->right);
}

// 队列 BFS
// 栈 DFS 完全同理, 只是把 dq 换成 st
bool isSameTreeBFS(const TreeNode* p, const TreeNode* q) {
    deque dq{p, q};
    while (!dq.empty()) {
        auto a = dq.front();
        dq.pop_front();
        auto b = dq.front();
        dq.pop_front();
        if (not a and not b) continue;
        if (not a or not b) return false;
        if (a->val != b->val) return false;

        // 注意空指针照样推
        dq.push_back(a->left);
        dq.push_back(a->right);
        dq.push_back(b->left);
        dq.push_back(b->right);
    }
    return true;
}
