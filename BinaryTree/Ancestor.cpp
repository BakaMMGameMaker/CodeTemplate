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
const TreeNode* LCAHash(const TreeNode* root, const TreeNode* p, const TreeNode* q) {
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
