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

// 判断是否平衡二叉树
// 左右子树不平衡 => 返回 -1
// 高度差大于 1 => 返回 -1
// 否则返回当前子树高度
// 核心思路是改造 getHeight 函数, 让它顺便判断合法性
// 注意这是自底向上的写法, 因为会直接递归子节点先
// 自顶向下则是 get height of left and right, if abs > 1 return -1, return f left and f right
// 后者会导致大量重复计算
bool isBalanced(const TreeNode* root) {
    function<int(const TreeNode*)> getH = [&](const TreeNode* node) {
        if (not node) return 0;
        int lh = getH(node->left);
        if (lh == -1) return -1;

        int rh = getH(node->right);
        if (rh == -1) return -1;
        if (abs(lh - rh) > 1) return -1;

        return max(lh, rh) + 1;
    };
    return getH(root) != -1;
}

// 双栈模拟后序遍历
bool isBalanceIterative(const TreeNode* root) {
    if (not root) return true;

    stack<const TreeNode*> st1, st2;
    unordered_map<const TreeNode*, int> height; // 节点 -> 高度
    st1.push(root);

    while (!st1.empty()) {
        auto node = st1.top();
        st1.pop();
        st2.push(node);
        if (node->left)st1.push(node->left);
        if (node->right) st1.push(node->right);
    }

    while (!st2.empty()) {
        auto node = st2.top();
        st2.pop();

        int lh = node->left ? height[node->left] : 0;
        int rh = node->right ? height[node->right] : 0;

        if (abs(lh - rh) > 1)return false;
        height[node] = max(lh, rh) + 1;
    }
    return true;
}
