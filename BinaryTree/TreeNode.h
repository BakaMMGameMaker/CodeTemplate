#pragma once

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;

    explicit TreeNode(const int x) : val(x), left(nullptr), right(nullptr) {
    }
};
