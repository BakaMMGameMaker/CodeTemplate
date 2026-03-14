#include <bits/stdc++.h>
#include "TreeNode.h"
using namespace std;

// 判断二叉树的性质

// 两棵树是否相等
bool isSameTree(const TreeNode *p, const TreeNode *q) {
    if (not p and not q) return true;
    if (not p or not q) return false;
    if (p->val != q->val) return false;
    return isSameTree(p->left, q->left) and isSameTree(p->right, q->right);
}

// 队列 BFS
// 栈 DFS 完全同理, 只是把 dq 换成 st
bool isSameTreeBFS(const TreeNode *p, const TreeNode *q) {
    deque dq{p, q};
    while (!dq.empty()) {
        auto a = dq.front();
        dq.pop_front();
        auto b = dq.front();
        dq.pop_front();
        if (not a and not b) continue;
        if (not a or not b) return false;
        if (a->val != b->val) return false;

        // 注意空指针照样推
        dq.push_back(a->left);
        dq.push_back(a->right);
        dq.push_back(b->left);
        dq.push_back(b->right);
    }
    return true;
}

// 对称二叉树 - 两棵树是否镜像
// 思路 - 左子树的左边是否等于右子树的右边
// 左子树的右边是否等于右子树的左边
bool isSymmetric(const TreeNode *root) {
    if (not root) return true;

    function<bool(const TreeNode *, const TreeNode *)> isMirror = [&](
        const TreeNode *p, const TreeNode *q) {
        if (not p and not q) return true;
        if (not p or not q) return false;
        if (p->val != q->val) return false;

        return isMirror(p->left, q->right) and isMirror(p->right, q->left);
    };

    return isMirror(root->left, root->right);
}

// BFS 解法
bool isSymmetricBFS(const TreeNode *root) {
    if (not root) return true;

    deque q{root->left, root->right};

    while (!q.empty()) {
        auto l = q.front();
        q.pop_front();
        auto r = q.front();
        q.pop_front();

        if (not l and not r) continue;
        if (not l or not r) return false;
        if (l->val != r->val) return false;

        // 注意添加顺序
        q.push_back(l->left);
        q.push_back(r->right);

        q.push_back(l->right);
        q.push_back(r->left);
    }
    return true;
}

// 判断是否平衡二叉树
// 左右子树不平衡 => 返回 -1
// 高度差大于 1 => 返回 -1
// 否则返回当前子树高度
// 核心思路是改造 getHeight 函数, 让它顺便判断合法性
// 注意这是自底向上的写法, 因为会直接递归子节点先
// 自顶向下则是 get height of left and right, if abs > 1 return -1, return f left and f right
// 后者会导致大量重复计算
bool isBalanced(const TreeNode *root) {
    function<int(const TreeNode *)> getH = [&](const TreeNode *node) {
        if (not node) return 0;
        int lh = getH(node->left);
        if (lh == -1) return -1;

        int rh = getH(node->right);
        if (rh == -1) return -1;
        if (abs(lh - rh) > 1) return -1;

        return max(lh, rh) + 1;
    };
    return getH(root) != -1;
}

// 双栈模拟后序遍历
bool isBalanceIterative(const TreeNode *root) {
    if (not root) return true;

    stack<const TreeNode *> st1, st2;
    unordered_map<const TreeNode *, int> height; // 节点 -> 高度
    st1.push(root);

    while (!st1.empty()) {
        auto node = st1.top();
        st1.pop();
        st2.push(node);
        if (node->left) st1.push(node->left);
        if (node->right) st1.push(node->right);
    }

    while (!st2.empty()) {
        auto node = st2.top();
        st2.pop();

        int lh = node->left ? height[node->left] : 0;
        int rh = node->right ? height[node->right] : 0;

        if (abs(lh - rh) > 1) return false;
        height[node] = max(lh, rh) + 1;
    }
    return true;
}

// 是否二叉搜索树
// 递归思路 左子树所有节点小于当前节点值 右子树所有节点大于当前节点值 递归期间维护好上下界
bool isValidBSTRecursive(const TreeNode *root) {
    function<bool(const TreeNode *, int64_t, int64_t)> dfs = [&](
        const TreeNode *node, int64_t low, int64_t high) {
        if (not node) return true;
        if (node->val <= low or node->val >= high) return false;
        return dfs(node->left, low, node->val) and dfs(node->right, node->val, high);
    };
    return dfs(root, INT64_MIN, INT64_MAX);
}

// 非递归 - 中序遍历 + 前驱值
bool isValidBSTIterative(const TreeNode *root) {
    stack<const TreeNode *> st;
    auto cur = root;
    int64_t prev = INT64_MIN;

    while (cur or !st.empty()) {
        // 中序遍历经典之先往左走到底部
        // 空后弹栈, 访问, 往右边走
        while (cur) {
            st.push(cur);
            cur = cur->left;
        }

        cur = st.top();
        st.pop();
        if (prev >= cur->val) return false;

        prev = cur->val;
        cur = cur->right;
    }
    return true;
}

// 是否完全二叉树
// 定义 - 除了最后一层每一层都满, 最后一层所有节点靠左排列
// 非递归 - 层序遍历
// 思路 - 如果遇到了空节点, 那么后面就不能再出现非空节点了
bool isCompleteTree(const TreeNode *root) {
    queue<const TreeNode *> q;
    q.push(root);
    bool seenNull = false;
    while (!q.empty()) {
        auto node = q.front();
        q.pop();
        if (not node) {
            seenNull = true;
        } else {
            if (seenNull) return false;
            q.push(node->left); // 空也推
            q.push(node->right);
        }
    }
    return true;
}

// 递归解法 - 罕见的更加麻烦
// 看成数组, 根为 idx, 左孩子为 2 idx, 右为 2 idx + 1
// 如果是完全二叉树, 如果一共有 n 个节点, 那么所有节点的编号都一定在 [1,n] 范围内, 不会跳过编号
bool isCompleteTreeRecursive(const TreeNode *root) {
    function<int(const TreeNode *)> countNodes = [&](const TreeNode *node) {
        if (not node) return 0;
        return 1 + countNodes(root->left) + countNodes(root->right);
    };

    function<bool(const TreeNode *node, int, int)> check =
            [&](const TreeNode *node, int idx, int total) {
        if (not root) return true;
        if (idx > total) return false;
        return check(root->left, idx * 2, total) and check(root->right, idx * 2 + 1, total);
    };

    int total = countNodes(root);
    return check(root, 1, total); // 检查的节点, 理论编号, 最大编号
}

// 判断满二叉树
// 满二叉树 - 每一层的节点数都到达最大值, 若高为 h, 节点数必须为 1<<h - 1
bool isFullTree(const TreeNode *root) {
    function<int(const TreeNode *)> countNodes =
            [&](const TreeNode *node) {
        if (not node) return 0;
        return 1 + countNodes(root->left) + countNodes(root->right);
    };
    function<int(const TreeNode *)> getHeight =
            [&](const TreeNode *node) {
        if (not node) return 0;
        return 1 + max(getHeight(node->left), getHeight(node->right));
    };
    return countNodes(root) == (1 << getHeight(root)) - 1;
}

// 写法二
// 左子树是满二叉树, 右子树是满二叉树, 且左右子树高度一致
bool isFullTreeII(const TreeNode *root) {
    struct Info {
        bool isFull;
        int height;
    };
    function<Info(const TreeNode *)> dfs =
            [&](const TreeNode *node) {
        if (not node) return Info{true, 0};
        Info l = dfs(node->left);
        Info r = dfs(node->right);
        bool isFull = l.isFull and r.isFull and (l.height == r.height);
        int height = max(l.height, r.height);
        return Info{isFull, height};
    };
    return dfs(root).isFull;
}
