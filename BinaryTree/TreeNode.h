#pragma once

struct TreeNode {
    int val = 0;
    TreeNode *left = nullptr;
    TreeNode *right = nullptr;

    explicit TreeNode(int x = 0) : val(x) {
    }
};
