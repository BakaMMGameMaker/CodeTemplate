#include "TreeNode.h"
#include <bits/stdc++.h>
using namespace std;

// 合并两颗二叉树

// 递归解法 - not root1 返回 root2, 反之亦然, 否则节点值相加, 递归合并左子树和右子树,
// 注意, 原地修改 root1, 更省空间
TreeNode *mergeTrees(TreeNode *root1, TreeNode *root2) {
    if (not root1) return root2;
    if (not root2) return root1;
    root1->val += root2->val;
    root1->left = mergeTrees(root1->left, root2->left);
    root1->right = mergeTrees(root1->right, root2->right);
    return root1;
}

// 非递归解法 - 层序遍历
// 按层处理节点对 - node1.val += node2.val
// 如果都有左孩子, 一起加入即可
// 如果只有某一边有左孩子, 直接接上
// 右孩子同理
// 依然复用 root1
TreeNode *mergeTreesIterative(TreeNode *root1, TreeNode *root2) {
    if (not root1) return root2;
    if (not root2) return root1;

    queue<TreeNode *> q;
    q.push(root1);
    q.push(root2);
    while (not q.empty()) {
        auto node1 = q.front();
        q.pop();
        auto node2 = q.front();
        q.pop();
        node1->val += node2->val;

        if (node1->left and node2->left) {
            q.push(node1->left);
            q.push(node2->left);
        } else {
            // node1.left 与 node2.left 中至少有一个为 null
            // node1.left 存在时, 相当于 node2.left 不存在, 此时什么也不做
            if (not node1->left) node1->left = node2->left; // 不进入队列
        }
    }
    return root1;
}

// 新建树
// 递归
TreeNode *mergeTrees(const TreeNode *root1, const TreeNode *root2) {
    if (not root1 and not root2) return nullptr;

    int val = 0;
    if (root1) val += root1->val;
    if (root2) val += root2->val;

    auto root = new TreeNode(val);
    root->left = mergeTrees(
        root1 ? root1->left : nullptr,
        root2 ? root2->left : nullptr);
    root->right = mergeTrees(
        root1 ? root1->right : nullptr,
        root2 ? root2->right : nullptr);

    return root;
}

// 非递归新建树
// 维护三个信息, 新树的当前节点, 原树节点 node1 与 node2
// 弹出时分别处理左孩子和右孩子, 如果需要继续合并, 就入队
TreeNode *mergeTreesIterative(const TreeNode *root1, const TreeNode *root2) {
    if (not root1 and not root2) return nullptr;

    int rootVal = 0;
    if (root1) rootVal += root1->val;
    if (root2) rootVal += root2->val;
    auto root = new TreeNode(rootVal);
    queue<tuple<TreeNode *, const TreeNode *, const TreeNode *> > q;
    q.emplace(root, root1, root2);

    while (not q.empty()) {
        auto [cur, node1, node2] = q.front();
        q.pop();
        // 获取两个原树节点各自的左孩子
        const TreeNode *l1 = (node1 ? node1->left : nullptr);
        const TreeNode *l2 = (node2 ? node2->left : nullptr);
        if (l1 or l2) {
            int val = 0;
            if (l1) val += l1->val;
            if (l2) val += l2->val;
            cur->left = new TreeNode(val);
            q.emplace(cur->left, l1, l2);
        }
        const TreeNode *r1 = (node1 ? node1->right : nullptr);
        const TreeNode *r2 = (node2 ? node2->right : nullptr);
        if (r1 or r2) {
            int val = 0;
            if (r1) val += r1->val;
            if (r2) val += r2->val;
            cur->right = new TreeNode(val);
            q.emplace(cur->right, r1, r2);
        }
    }
    return root;
}
