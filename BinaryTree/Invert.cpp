#include "TreeNode.h"
#include <bits/stdc++.h>
using namespace std;

// 镜像二叉树

// 递归原地
TreeNode *invertTree(TreeNode *root) {
    if (not root) return nullptr;
    swap(root->left, root->right);
    invertTree(root->left);
    invertTree(root->right);
    return root;
}

// 非递归原地 - BFS
// 层序遍历每一个节点, 对访问到的节点交换其左右孩子
TreeNode *invertTreeIterative(TreeNode *root) {
    if (not root) return nullptr;

    queue<TreeNode *> q;
    q.push(root);

    while (not q.empty()) {
        auto node = q.front();
        q.pop();
        swap(node->left, node->right);
        if (node->left) q.push(node->left);
        if (node->right) q.push(node->right);
    }
    return root;
}

// 递归非原地
TreeNode *invertTree(const TreeNode *root) {
    if (not root) return nullptr;
    auto newRoot = new TreeNode(root->val);
    newRoot->left = invertTree(root->right); // 注意别搞反
    newRoot->right = invertTree(root->left);
    return newRoot;
}

// 非递归非原地
TreeNode *invertTreeIterative(const TreeNode *root) {
    if (not root) return nullptr;

    auto newRoot = new TreeNode(root->val);
    queue<pair<const TreeNode *, TreeNode *> > q;
    q.emplace(root, newRoot);

    while (!q.empty()) {
        auto [Old, New] = q.front();
        q.pop();
        // 注意反向操作
        if (Old->left) {
            New->right = new TreeNode(Old->left->val);
            q.emplace(Old->left, New->right);
        }
        if (Old->right) {
            New->left = new TreeNode(Old->right->val);
            q.emplace(Old->right, New->left);
        }
    }
    return newRoot;
}
