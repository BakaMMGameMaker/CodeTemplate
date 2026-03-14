#include <bits/stdc++.h>
#include "TreeNode.h"
using namespace std;

// 二叉树三序遍历模板 递归与非递归
// 前 中 后序遍历分别代表父节点在遍历过程的相对位置 (1 2 3)，左节点永远先于右节点遍历

// 前序遍历的递归写法
void preorderRecursive(const TreeNode *root) {
    if (root == nullptr) return;

    // 中 左 右
    cout << root->val << ' ';
    preorderRecursive(root->left);
    preorderRecursive(root->right);
}

// 前序遍历的非递归写法
void preorderIterative(const TreeNode *root) {
    // 与递归相反，先压入右子树，再压入左子树
    // 这么做是为了让左子树先出栈，先被处理，随后才是右子树
    if (root == nullptr) return;

    stack<const TreeNode *> stk;
    stk.push(root);

    while (!stk.empty()) {
        const TreeNode *node = stk.top();
        stk.pop();

        cout << node->val << ' ';

        if (node->right) stk.push(node->right);
        if (node->left) stk.push(node->left);
    }
}

// 中序遍历的递归写法
void inorderRecursive(const TreeNode *root) {
    if (root == nullptr) return;

    inorderRecursive(root->left);
    cout << root->val << ' ';
    inorderRecursive(root->right);
}

// 中序遍历的非递归写法
void inorderIterative(const TreeNode *root) {
    // 一路往左边压栈，到底部后再弹出访问，并转向右子树
    stack<const TreeNode *> stk;
    const TreeNode *cur = root;

    // 注意条件和前序遍历不同，而且也没有提前把 root 压入栈中
    while (cur != nullptr || !stk.empty()) {
        // 一路往左边压栈
        while (cur != nullptr) {
            stk.push(cur);
            cur = cur->left;
        }

        cur = stk.top();
        stk.pop();

        cout << cur->val << ' ';
        cur = cur->right;
    }
}

// 后序遍历的递归写法
void postorderRecursive(const TreeNode *root) {
    if (root == nullptr) return;

    // 左 右 中
    postorderRecursive(root->left);
    postorderRecursive(root->right);
    cout << root->val << ' ';
}

// 后序遍历的非递归写法 - 双栈法
void postorderIterativeTwoStack(const TreeNode *root) {
    if (root == nullptr) return;

    stack<const TreeNode *> stk1, stk2;
    stk1.push(root);

    // 原理：前序是 中 左 右
    // 这里先反一下压栈顺序，变成 中 右 左
    // 随后先不着急输出，而是放到另一个栈中
    // 这样最终输出就会完全反过来，即 左 右 中，符合后序遍历
    while (!stk1.empty()) {
        const TreeNode *node = stk1.top();
        stk1.pop();
        stk2.push(node);

        if (node->left) stk1.push(node->left);
        if (node->right) stk1.push(node->right);
    }

    while (!stk2.empty()) {
        cout << stk2.top()->val << ' ';
        stk2.pop();
    }
}

// 后序遍历的非递归写法 - 单栈法
void postorderIterativeOneStack(const TreeNode *root) {
    stack<const TreeNode *> stk;
    const TreeNode *cur = root;
    const TreeNode *prev = nullptr; // 需要一个 prev 记录刚才访问过的节点

    // 条件和中序很像
    while (cur != nullptr || !stk.empty()) {
        // 依旧一路往左边压栈
        while (cur) {
            stk.push(cur);
            cur = cur->left;
        }

        cur = stk.top();
        // 中序遍历在这里，会直接输出并 pop，并转到右子树
        // 后序遍历在这里，则是先访问右子树，后期再来 pop
        if (cur->right != nullptr && prev != cur->right) { cur = cur->right; } else {
            cout << cur->val << ' ';
            stk.pop();
            prev = cur;
            cur = nullptr; // 防止再次进入左子树
        }
    }
}

// 层序遍历
void levelOrder(const TreeNode *root) {
    if (!root) return;

    queue<const TreeNode *> q;
    q.push(root);

    while (!q.empty()) {
        const TreeNode *node = q.front();
        q.pop();
        cout << node->val << ' ';

        if (node->left) q.push(node->left);
        if (node->right) q.push(node->right);
    }
}

// 分层的层序遍历
vector<vector<int> > levelOrderByLevel(const TreeNode *root) {
    vector<vector<int> > result; // 如果是右视图，那么只需要一个 vector<int> 来记录每一层最后一个节点值即可
    if (!root) return result;
    queue<const TreeNode *> q;
    q.push(root);

    while (!q.empty()) {
        // 由于在循环开头时，队列中的所有节点都位于同一层，所以直接记录 size 即可
        const size_t size = q.size();
        vector<int> level;

        for (size_t i = 0; i < size; ++i) {
            const TreeNode *node = q.front();
            q.pop();

            level.push_back(node->val);
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }

        // 如果需要自底部向上的层序遍历
        // reverse(result.begin(), result.end());
        result.push_back(level);
    }

    return result;
}

// 之字形遍历的层序遍历
vector<vector<int> > zigzagLevelOrder(const TreeNode *root) {
    vector<vector<int> > result;
    if (!root) return result;

    queue<const TreeNode *> q;
    q.push(root);
    bool leftToRight = true;

    while (!q.empty()) {
        const size_t size = q.size();
        vector<int> level(size); // 提前准备容量，待会可以直接对号入座，不用再后期 reverse

        for (size_t i = 0; i < size; ++i) {
            const TreeNode *node = q.front();
            q.pop();

            // 根据 left to right 状态判断存储位置
            const size_t index = leftToRight ? i : (size - 1 - i);
            level[index] = node->val;

            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        result.push_back(level);
        leftToRight = !leftToRight;
    }

    return result;
}

// 右视图 - 每一层取最后一个节点
vector<int> rightSideView(const TreeNode *root) {
    vector<int> ans;
    if (not root) return ans;

    queue<const TreeNode *> q;
    q.push(root);
    while (not q.empty()) {
        for (size_t i = 0; i < q.size(); ++i) {
            auto node = q.front();
            q.pop();

            if (i == q.size() - 1) ans.push_back(node->val);
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
    }
    return ans;
}

// 给定前序和中序, 输出右视图
// 一般分两步走, 先建树再层序遍历, 此处不再赘述
// 思路 - 恢复树的过程中已经知道了当前区间在哪棵子树, 第几层
// 需要注意 - 只是右子树比左子树优先级高, 不代表能不递归左子树
// 因为不是所有的最右节点都是通过根一路往右走就能到的
vector<int> getRightSideViewWithPreIn(const vector<int> &preOrder, const vector<int> &inOrder) {
    size_t n = preOrder.size();
    unordered_map<int, size_t> pos;
    for (size_t i = 0; i < n; ++i) pos[inOrder[i]] = i;
    vector<int> ans; // 存储右视图

    function<void(size_t, size_t, size_t, size_t, size_t)> dfs =
            [&](size_t preL, size_t preR, size_t inL, size_t inR, size_t depth) {
        if (preL > preR or inL > inR) return;

        int rootVal = preOrder[preL];
        if (depth == ans.size()) ans.push_back(rootVal); // 因为先往右边走, 每层的末尾最快到达此处
        size_t k = pos[rootVal];
        size_t leftSize = k - inL;

        // 先右后左, 保证每层第一次访问到的都是右视图节点
        dfs(preL + leftSize + 1, preR, k + 1, inR, depth + 1);
        dfs(preL + 1, preL + leftSize, inL, k - 1, depth + 1);
    };
    dfs(0, n - 1, 0, n - 1, 0);
    return ans;
}

// 中序后序 => 右视图
vector<int> getRightSideViewWithInPost(const vector<int> &inOrder, const vector<int> &postOrder) {
    size_t n = inOrder.size();
    unordered_map<int, size_t> pos;
    for (size_t i = 0; i < n; ++i) pos[inOrder[i]] = i;
    vector<int> ans;

    auto dfs = [&](auto &&self, size_t inL, size_t inR, size_t postL, size_t postR, size_t depth) {
        if (inL > inR or postL > postR) return;
        int rootVal = postOrder[postR];
        if (depth == ans.size()) ans.push_back(rootVal);
        size_t k = pos[rootVal];
        size_t leftSize = k - inL;

        self(self, k + 1, inR, postL + leftSize, postR - 1, depth + 1);
        self(self, inL, k - 1, postL, postL + leftSize - 1, depth + 1);
    };

    dfs(dfs, 0, n - 1, 0, n - 1, 0);
    return ans;
}
