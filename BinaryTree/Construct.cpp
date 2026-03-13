#include <bits/stdc++.h>
#include "TreeNode.h"
using namespace std;

// 构造二叉树

// 前序与中序构造二叉树
// 前序是 中左右 中序是 左中右
// 所以前序首位一定是根 => 在中序中找到根的位置 => 左边就是左子树, 右边就是右子树, 根据左子树节点数量去前序中切分左右
TreeNode* buildTreeWithPreAndInOrder(const vector<int>& preorder, const vector<int>& inorder) {
    int n = static_cast<int>(preorder.size());
    unordered_map<int, int> pos; // 中序值对应的下标
    for (int i = 0; i < n; ++i) pos[inorder[i]] = i;

    function<TreeNode*(int, int, int, int)> dfs = [&](int pl, int pr, int il, int ir) -> TreeNode* {
        if (pl > pr or il > ir) return nullptr;

        // 前序遍历的第一个位置就是根节点
        int rootVal = preorder[pl];
        auto root = new TreeNode(rootVal);

        int k = pos[rootVal];  // 根节点在 inorder 中的位置
        int leftSize = k - il; // 左(il)|中(k)|右 => 左子树的节点个数

        // 左子树区间
        // 前序 => 中(pl)|左(pl+1...pl+leftSize)|右
        // 中序 => 左(il...k-1)|中(k)|右
        root->left = dfs(pl + 1, pl + leftSize, il, k - 1);
        root->right = dfs(pl + leftSize + 1, pr, k + 1, ir);
        return root;
    };
    return dfs(0, n - 1, 0, n - 1);
}
