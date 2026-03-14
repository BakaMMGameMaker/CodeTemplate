#include "../TreeNode.h"
#include "../../LinkedList/ListNode.h"
#include <bits/stdc++.h>
using namespace std;

// BST 转有序双向链表
// ListNode / 复用 TreeNode

// 新建 ListNode + 递归
DoublyListNode *bstToDoublyList(const TreeNode *root) {
    DoublyListNode *head = nullptr;
    DoublyListNode *prev = nullptr;

    function<void(const TreeNode *)> inorder = [&](const TreeNode *node) {
        if (not node) return;
        inorder(node->left);

        auto cur = new DoublyListNode(node->val);
        if (not head) head = cur;
        if (prev) {
            prev->next = cur;
            cur->prev = prev;
        }
        prev = cur;
        inorder(node->right);
    };

    inorder(root);
    return head;
}

// 新建 ListNode + 非递归
DoublyListNode *bstToDoublyListIterative(const TreeNode *root) {
    if (not root) return nullptr;
    stack<const TreeNode *> st;
    auto cur = root;

    DoublyListNode *head = nullptr;
    DoublyListNode *prev = nullptr;
    while (cur or not st.empty()) {
        while (cur) {
            st.push(cur);
            cur = cur->left;
        }

        cur = st.top();
        st.pop();

        auto node = new DoublyListNode(cur->val);
        if (not head) head = node;
        if (prev) {
            prev->next = node;
            node->prev = prev;
        }
        prev = node;
        cur = cur->right;
    }
    return head;
}

// 复用 TreeNode* + 递归
TreeNode *bstToDoublyListInplace(TreeNode *root) {
    TreeNode *head = nullptr;
    TreeNode *prev = nullptr;
    function<void(TreeNode *)> inorder = [&](TreeNode *node) {
        if (not node) return;
        inorder(node->left);

        if (not head) head = node;
        node->left = prev;
        if (prev) prev->right = node;
        prev = node;
        inorder(node->right);
    };

    inorder(root);
    return head;
}

// 复用 TreeNode* + 迭代
TreeNode *bstToDoublyListInplaceIterative(TreeNode *root) {
    if (not root) return nullptr;
    stack<TreeNode *> st;
    TreeNode *cur = root;

    TreeNode *head = nullptr;
    TreeNode *prev = nullptr;

    while (cur or not st.empty()) {
        while (cur) {
            st.push(cur);
            cur = cur->left;
        }
        cur = st.top();
        st.pop();
        if (not head) head = cur;
        cur->left = prev;
        if (prev) prev->right = cur;
        prev = cur;
        cur = cur->right;
    }
    return head;
}
