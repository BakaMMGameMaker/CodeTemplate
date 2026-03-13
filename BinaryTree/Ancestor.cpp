#include <bits/stdc++.h>
#include "TreeNode.h"
using namespace std;

// 最近公共祖先

// 普通二叉树
// 后续递归
// 如果当前节点为空 返回空
// 如果当前节点等于 p 或 q 直接返回当前节点
// 递归去左右子树找, 如果左右子树都找到了, 说明当前节点就是 LCA
// 只有一边找到, 就返回那一边, 都没找到就返回空
const TreeNode* LCA(const TreeNode* root, const TreeNode* p, const TreeNode* q) {
    if (not root or root == p or root == q) return root;

    // 这里递归的 LCA 并不是值 l 和 r 接收到的就是最终 LCA
    // 返回值更像线索, 代表了某一边有没有 p 或者 q
    auto l = LCA(root->left, p, q);
    auto r = LCA(root->right, p, q);

    if (l and r) return root;
    return l ? l : r;
}

// 迭代 + 哈希 (建立父子关系)
// 思路 不断遍历并用哈希表建立父子关系, 直到 p q 都存在于表中
// 由 p 往上构建祖先链条, q 再往上构建直到遇到第一个共同祖先
const TreeNode* LCAIterative(const TreeNode* root, const TreeNode* p, const TreeNode* q) {
    unordered_map<const TreeNode*, const TreeNode*> parent;
    stack<const TreeNode*> st;
    parent[root] = nullptr;
    st.push(root);

    while (!parent.contains(p) or !parent.contains(q)) { // 只要还有一个不在表中, 迭代就要继续
        auto node = st.top();
        st.pop();
        if (node->left) {
            parent[node->left] = node;
            st.push(node->left);
        }
        if (node->right) {
            parent[node->right] = node;
            st.push(node->right);
        }
    }

    unordered_set<const TreeNode*> path;

    // 记录 p 到根节点的整条链
    while (p) {
        path.emplace(p);
        p = parent[p];
    }

    // q 往上找, 直到找到任何一个在 path 中的, 就是 LCA
    while (!path.contains(q)) q = parent[q];
    return q;
}

// BST 的 LCA 更简单, 因为能利用大小关系进行剪枝
// 递归写法
const TreeNode* LCA_BST(const TreeNode* root, const TreeNode* p, const TreeNode* q) {
    if (not root) return nullptr;

    // p 和 q 都在 root 左边
    if (p->val < root->val and q->val < root->val) return LCA_BST(root->left, p, q);
    // p 和 q 都在 root 右边
    if (p->val > root->val and q->val > root->val) return LCA_BST(root->right, p, q);
    // p 和 q 在 root 两侧
    return root;
}

// 非递归写法
const TreeNode* LCAIterative_BST(const TreeNode* root, const TreeNode* p, const TreeNode* q) {
    auto cur = root;
    while (cur) {
        if (p->val < cur->val and q->val < cur->val) cur = cur->left;
        if (p->val > cur->val and q->val > cur->val) cur = cur->right;
        return cur;
    }
    return nullptr;
}
