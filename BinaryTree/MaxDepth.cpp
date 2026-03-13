#include "TreeNode.h"
#include <bits/stdc++.h>
using namespace std;

// 二叉树的最大深度

// 递归思路 最大深度等于左子树的最大深度与右子树的最大深度的最大值 +1
int maxDepth(const TreeNode *root) {
    if (not root) return 0;
    return max(maxDepth(root->left), maxDepth(root->right)) + 1;
}

// 非递归解法 1 - 层序遍历
int maxDepthBFS(const TreeNode *root) {
    if (not root) return 0;
    deque q{root};
    int depth = 0;

    while (not q.empty()) {
        size_t size = q.size();
        depth++;

        while (size--) {
            auto node = q.front();
            q.pop_front();

            if (node->left) q.push_back(node->left);
            if (node->right) q.push_back(node->right);
        }
    }
    return depth;
}

// 非递归写法 2 - DFS 栈模拟
int maxDepthDFS(const TreeNode *root) {
    if (not root) return 0;
    vector st{pair{root, 1}};
    int ans = 0;

    while (not st.empty()) {
        auto [node, depth] = st.back();
        st.pop_back();
        ans = max(ans, depth);

        if (node->left) st.emplace_back(node->left, depth + 1);
        if (node->right) st.emplace_back(node->right, depth + 1);
    }
    return ans;
}
