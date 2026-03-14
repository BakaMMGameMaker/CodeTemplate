#include "TreeNode.h"
#include <bits/stdc++.h>
using namespace std;

// 序列化和反序列化二叉树

// 递归做法 - 前序遍历
void serializeDFS(const TreeNode *root, string &s) {
    if (not root) { // 记录空节点
        s += "#,";
        return;
    }
    s += to_string(root->val) + ",";
    serializeDFS(root->left, s);
    serializeDFS(root->right, s);
}

TreeNode *deserializeDFS(queue<string> &q) {
    string cur = q.front();
    q.pop();
    if (cur == "#") return nullptr;
    auto root = new TreeNode(stoi(cur));
    // 因为序列化结果由前序遍历而来, 所以这里中左右十分自然
    root->left = deserializeDFS(q);
    root->right = deserializeDFS(q);
    return root;
}

// encodes a tree to a single string
string serialize(const TreeNode *root) {
    string s;
    serializeDFS(root, s);
    return s;
}

// decodes encoded data to a string
TreeNode *deserialize(const string &data) {
    queue<string> q;
    string cur;
    for (char c : data) {
        // 注意 逗号不进队列
        if (c == ',') {
            q.push(cur);
            cur.clear();
        } else {
            cur += c;
        }
    }
    return deserializeDFS(q);
}

// 非递归写法 - 层序遍历
vector<string> split(const string &data) {
    vector<string> tokens;
    string cur;
    for (char c : data) {
        if (c == ',') {
            tokens.emplace_back(std::move(cur));
            cur.clear();
        } else {
            cur += c;
        }
    }
    return tokens;
}

string serializeIterative(const TreeNode *root) {
    if (not root) return "#,";

    string res;
    queue<const TreeNode *> q;
    q.push(root);
    while (!q.empty()) {
        auto node = q.front();
        q.pop();

        // 这里也表明了空节点也要加入队列
        if (not node) {
            res += "#,";
            continue;
        }

        res += to_string(node->val) + ",";
        q.push(node->left);
        q.push(node->right);
    }
    return res;
}

TreeNode *deserializeIterative(const string &data) {
    vector<string> tokens = split(data);
    if (tokens.empty() or tokens[0] == "#") return nullptr;
    auto root = new TreeNode(stoi(tokens[0]));
    queue<TreeNode *> q;
    q.push(root);

    int i = 1; // 已经处理的节点数量
    while (!q.empty() and i < tokens.size()) {
        auto node = q.front(); // 准备 new 孩子的父节点
        q.pop();

        if (i < tokens.size() and tokens[i] != "#") {
            node->left = new TreeNode(stoi(tokens[i]));
            q.push(node->left);
        }
        i++;

        if (i < tokens.size() and tokens[i] != "#") {
            node->right = new TreeNode(stoi(tokens[i]));
            q.push(node->right);
        }
        i++;
    }
    return root;
}
