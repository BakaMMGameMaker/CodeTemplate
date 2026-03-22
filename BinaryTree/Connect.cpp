// 填充节点的下一个右侧节点指针

class Node {
public:
    int val;
    Node *left = nullptr;
    Node *right = nullptr;
    Node *next = nullptr;

    Node() : val(0) {}
    explicit Node(int _val) : val(_val) {}
};

// I - 每个父节点都有两个子节点，所有叶子节点都在同一层
Node *connecti(Node *root) {
    if (not root) return root;

    Node *leftmost = root;
    while (leftmost->left) {
        // 从左到右遍历这一层，连接下一层
        Node *cur = leftmost;
        while (cur) {
            cur->left->next = cur->right; // 左孩子的右侧节点就是右孩子
            // 右孩子的右侧节点就是父节点右侧节点的左孩子
            if (cur->next) cur->right->next = cur->next->left;
            cur = cur->next; // 前往当前层的下一个节点
        }
        leftmost = leftmost->left; // 前往下一层
    }
    return root;
}

// II - 普通的二叉树
// 按层处理，用 dm 把下一层串联起来
// 遍历当前层时，若左孩子存在，接到下一层链表的尾部，右孩子存在也同理
// 遍历完当前层后下一层起点就是 dm.next
Node *connectii(Node *root) {
    if (not root) return root;

    Node *cur = root;
    while (cur) {
        Node dm;
        Node *tail = &dm;
        while (cur) {
            if (cur->left) {
                tail->next = cur->left;
                tail = tail->next;
            }
            if (cur->right) {
                tail->next = cur->right;
                tail = tail->next;
            }
            cur = cur->next;
        }
        cur = dm.next; // 前往下一层
    }
    return root;
}
