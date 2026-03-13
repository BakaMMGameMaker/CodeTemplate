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

// 对称二叉树 - 两棵树是否镜像
// 思路 - 左子树的左边是否等于右子树的右边
// 左子树的右边是否等于右子树的左边
bool isSymmetric(const TreeNode* root) {
    if (not root) return true;

    function<bool(const TreeNode*, const TreeNode*)> isMirror = [&](
        const TreeNode* p, const TreeNode* q) {
        if (not p and not q) return true;
        if (not p or not q) return false;
        if (p->val != q->val) return false;

        return isMirror(p->left, q->right) and isMirror(p->right, q->left);
    };

    return isMirror(root->left, root->right);
}

// BFS 解法
bool isSymmetricBFS(const TreeNode* root) {
    if (not root) return true;

    deque q{root->left, root->right};

    while (!q.empty()) {
        auto l = q.front();
        q.pop_front();
        auto r = q.front();
        q.pop_front();

        if (not l and not r) continue;
        if (not l or not r) return false;
        if (l->val != r->val) return false;

        // 注意添加顺序
        q.push_back(l->left);
        q.push_back(r->right);

        q.push_back(l->right);
        q.push_back(r->left);
    }
    return true;
}
