#include "TreeNode.h"

// 上下翻转二叉树

TreeNode *upsidedown(TreeNode *root) {
    if (not root or not root->left) return root;

    TreeNode *newroot = upsidedown(root->left);
    root->left->left = root->right;
    root->left->right = root;

    root->left = nullptr;
    root->right = nullptr;

    return newroot;
}

TreeNode *upsidedowniterative(TreeNode *root) {
    TreeNode *curr = root;
    TreeNode *parent = nullptr;
    TreeNode *parentright = nullptr;

    while (curr) {
        TreeNode *next = curr->left;
        curr->left = parentright;
        parentright = curr->right;

        curr->right = parent;
        parent = curr;

        curr = next;
    }
    return parent;
}
