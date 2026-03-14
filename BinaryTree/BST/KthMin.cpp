#include "../TreeNode.h"
#include <bits/stdc++.h>
using namespace std;

// BST 第 K 小元素 - 本质是中序遍历

// 递归写法 - 先遍历左子树, 再访问当前节点, k--, k==0 时节点值就是最终答案, 再遍历右子树
int kthSmallestRecursive(const TreeNode *root, int k) {
    int ans = -1;

    function<void(const TreeNode *)> inorder = [&](const TreeNode *node) {
        if (not node) return;
        inorder(node->left);
        if (--k == 0) {
            ans = root->val;
            return;
        }
        inorder(root->right);
    };

    inorder(root);
    return ans;
}

// 非递归写法 - 栈模拟中序遍历
int kthSmallestIterative(const TreeNode *root, int k) {
    stack<const TreeNode *> st;
    auto cur = root;
    while (cur or !st.empty()) {
        while (cur) {
            st.push(cur);
            cur = cur->left;
        }
        cur = st.top();
        st.pop();

        if (--k == 0) return cur->val;
        cur = cur->right;
    }
    return -1;
}
