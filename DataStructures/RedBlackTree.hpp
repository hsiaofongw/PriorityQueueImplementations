//
// Created by 韦晓枫 on 2022/6/8.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_REDBLACKTREE_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_REDBLACKTREE_HPP

#include <memory>
#include <string>
#include <iostream>
#include <deque>
#include <vector>
#include <functional>
#include <algorithm>

/** 表颜色 */
enum class LinkType {
    RED, BLACK
};

/** 红黑树节点 */
template <typename KeyT, typename ValT>
struct RedBlackNode {

    RedBlackNode(const std::shared_ptr<KeyT>& k, const std::shared_ptr<ValT>& v, LinkType color)
    : key(k), value(v), color(color), left(nullptr), right(nullptr), size(1) { }

    LinkType color;
    std::shared_ptr<RedBlackNode<KeyT, ValT>> left;
    std::shared_ptr<RedBlackNode<KeyT, ValT>> right;
    std::shared_ptr<KeyT> key;
    std::shared_ptr<ValT> value;
    size_t size;
};

/** 红黑树节点指针 */
template <typename KeyT, typename ValT>
using RedBlackNodePtr = std::shared_ptr<RedBlackNode<KeyT, ValT>>;

/** 红黑树操作句柄 */
template <typename KeyT, typename ValT>
class RedBlackTreeHandle {
    using RedBlackNode = RedBlackNode<KeyT, ValT>;
    using NodePtr = RedBlackNodePtr<KeyT, ValT>;
    using KeyPtr = std::shared_ptr<KeyT>;
    using ValuePtr = std::shared_ptr<ValT>;

public:

    /** 获取一颗红黑树上存储的键值对的数量 */
    static size_t getSize(NodePtr root) {
        if (root) {
            return root->size;
        }

        return 0;
    }

    /** 插入操作 */
    static NodePtr insert(NodePtr root, const KeyPtr& k, const ValuePtr& v) {
        NodePtr result = doInsert(root, k, v);
        result->color = LinkType::BLACK;
        return result;
    }

    /** 打印 Key 表达式 */
    static void debugPrintTreeExpr(NodePtr root) {

        auto printSeparator = []() -> void {
            std::cout << ", ";
        };

        auto printNodeKeyValuePair = [printSeparator](const NodePtr& root) -> void {
            std::cout << "Pair[";
            std::cout << "Key -> ";
            std::cout << *root->key;
            printSeparator();
            std::cout << "Value -> ";
            std::cout << *root->value;
            std::cout << "]";
        };

        auto print3Node = [printNodeKeyValuePair, printSeparator](const NodePtr& root) -> void {
            std::cout << "Pairs[";
            printNodeKeyValuePair(root->left);
            printSeparator();
            printNodeKeyValuePair(root);
            std::cout << "]";
        };

        auto print4Node = [printNodeKeyValuePair, printSeparator](const NodePtr& root) -> void {
            std::cout << "Pairs[";
            printNodeKeyValuePair(root->left);
            printSeparator();
            printNodeKeyValuePair(root);
            printSeparator();
            printNodeKeyValuePair(root->right);
            std::cout << "]";
        };

        if (root) {
            // Non-Null
            if (root->left) {
                // Non-Leaf
                if (isRed(root->left)) {
                    if (isRed(root->right)) {
                        // 4-Node
                        std::cout << "NodeType4[";
                        print4Node(root);
                        std::cout << "]";

                        // Children of 4-Node
                        std::cout << "[";
                        debugPrintTreeExpr(root->left->left);
                        printSeparator();
                        debugPrintTreeExpr(root->left->right);
                        printSeparator();
                        debugPrintTreeExpr(root->right->left);
                        printSeparator();
                        debugPrintTreeExpr(root->right->right);
                        std::cout << "]";
                    } else {
                        // 3-Node
                        std::cout << "NodeType3[";
                        print3Node(root);
                        std::cout << "]";

                        // Children of 3-Node
                        std::cout << "[";
                        debugPrintTreeExpr(root->left->left);
                        printSeparator();
                        debugPrintTreeExpr(root->left->right);
                        printSeparator();
                        debugPrintTreeExpr(root->right);
                        std::cout << "]";
                    }
                } else {
                    // 2-Node
                    std::cout << "NodeType2[";
                    printNodeKeyValuePair(root);
                    std::cout << "]";

                    // Children of 2-Node

                    std::cout << "[";
                    debugPrintTreeExpr(root->left);
                    printSeparator();
                    debugPrintTreeExpr(root->right);
                    std::cout << "]";
                }
            } else {
                // Leaf
                std::cout << "NodeLeaf[";
                printNodeKeyValuePair(root);
                std::cout << "]";
            }
        }
    }

    /**
     * 检验红黑树定义是否被满足：
     *
     * 定义 1（Red-Black BST): 一个 Red-Black BST 是满足下列要求的 BST:
     *
     * (1) 红边总是指左；
     * (2) 每个节点都不会同时出现两个红边；
     * (3) 从根节点到每一个空边的路径中经过的黑边的数量都是相等的；
     */
     static bool debugCheckDefinition(NodePtr root, bool suppressDebug = false) {

         auto pass = checkDef1_1(root, suppressDebug) && checkDef1_2(root, suppressDebug) && checkDef1_3(root, suppressDebug);
         if (!suppressDebug) {
             std::cout << "Checking definition 1... ";
             std::cout << pass << "\n";
         }

         return pass;
     }

     /** 删除红黑树中最小的 key 对应的节点，返回一个指针指向删除了最小节点之后的树 */
     static NodePtr deleteMin(NodePtr root) {
         return doDeleteMin(root);
     }

private:

    /** 尝试更新一个节点的 size */
    static void updateSize(NodePtr root) {
        if (root) {
            root->size = 1 + getSize(root->left) + getSize(root->right);
        }
    }

    /** 检验定义 1 的条件 (1) */
    static bool checkDef1_1(NodePtr root, bool suppressDebug = false) {
        if (!suppressDebug) {
            std::cout << "Checking condition (1) of definition 1...";
        }

        bool pass = doCheckDef1_1Recursive(root);

        if (!suppressDebug) {
            std::cout << pass << "\n";
        }

        return pass;
    }
    static bool doCheckDef1_1Recursive(NodePtr root) {
        if (root) {
            bool checkLeft = false;
                checkLeft = doCheckDef1_1Recursive(root->left);

            bool checkRight = false;
            checkRight = doCheckDef1_1Recursive(root->right);
            if (root->right) {
                if (isRed(root->right)) {
                    return false;
                }
            }

            return checkLeft && checkRight;
        }

        return true;
    }

    /** 检验定义 1 的条件 (2) */
    static bool checkDef1_2(NodePtr root, bool suppressDebug = false) {
        if (!suppressDebug) {
            std::cout << "Checking condition (2) of definition 1... ";
        }

        bool pass = doCheckDef1_2Recursive(root);

        if (!suppressDebug) {
            std::cout << pass << "\n";
        }

        return pass;
    }
    static bool doCheckDef1_2Recursive(NodePtr root) {
        if (root) {
            if (root->left && root->right) {
                if (isRed(root->left) && isRed(root->right)) {
                    return false;
                }
            }

            return doCheckDef1_2Recursive(root->left) && doCheckDef1_2Recursive(root->right);
        }

        return true;
    }

    /** 检验定义 1 的条件 (3) */
    static bool checkDef1_3(NodePtr root, bool suppressDebug = false) {
        if (!suppressDebug) {
            std::cout << "Traversing every path from root to nil... \n";
        }

        std::deque<NodePtr> path {};
        std::vector<std::deque<NodePtr>> paths {};
        std::vector<size_t> heights {};
        auto onPathAddedDoNothing = [](
            std::deque<NodePtr>& path,
            size_t height,
            std::vector<std::deque<NodePtr>>& paths,
            std::vector<size_t>& heights
        ) {};

        auto onPathAdded = [](
            std::deque<NodePtr>& path,
            size_t height,
            std::vector<std::deque<NodePtr>>& paths,
            std::vector<size_t>& heights
        ) -> void {
            std::cout << "Path: ";
            size_t segmentIndex = 0;
            for (const auto& segment : path) {
                if (segmentIndex != 0) {
                    if (segment->color == LinkType::RED) {
                        std::cout << " -- ";
                    } else {
                        std::cout << " --> ";
                    }
                }
                std::cout << (*segment->key);
                ++segmentIndex;
            }
            std::cout << " -> (nil), ";
            std::cout << "Height: ";
            std::cout << height;
            std::cout << "\n";
        };

        if (suppressDebug) {
            traverseAllPaths(
                    root,
                    0,
                    path,
                    paths,
                    heights,
                    onPathAddedDoNothing
            );
        } else {
            traverseAllPaths(
                    root,
                    0,
                    path,
                    paths,
                    heights,
                    onPathAdded
            );
        }

        auto beginIt = std::begin(heights);
        auto endIt = std::end(heights);
        auto minHeight = std::min_element(beginIt, endIt);
        auto maxHeight = std::max_element(beginIt, endIt);

        auto pass = *minHeight == *maxHeight;
        if (!suppressDebug) {
            std::cout << "Checking condition (3) of definition 1... ";
            std::cout << pass << "\n";
        }
        return pass;
    }

    /** 递归遍历每一条路径，到达 nil 链的时候将路径以及路径长度记录下来 */
    static void traverseAllPaths(
            NodePtr root,
            size_t height,
            std::deque<NodePtr>& path,
            std::vector<std::deque<NodePtr>>& paths,
            std::vector<size_t>& heights,
            const std::function<void (
                    std::deque<NodePtr>& path,
                    size_t height,
                    std::vector<std::deque<NodePtr>>& paths,
                    std::vector<size_t>& heights
            )>& onPathAdded
    ) {
        if (root) {
            path.emplace_back(root);
            traverseAllPaths(root->left, isRed(root->left) ? height : height + 1, path, paths, heights, onPathAdded);
            traverseAllPaths(root->right, isRed(root->right) ? height: height + 1, path, paths, heights, onPathAdded);
            path.pop_back();
        }
        else {
            paths.emplace_back(std::deque<NodePtr> { path });
            heights.push_back(height);

            onPathAdded(path, height, paths, heights);
        }
    }

    /** 从左到右旋转，右旋 */
    static NodePtr rotateRight(NodePtr root) {
        assert((root->left));

        NodePtr result { root->left };
        root->left = result->right;
        result->right = root;
        result->color = root->color;
        root->color = LinkType::RED;
        result->size = root->size;
        updateSize(root);

        return result;
    }

    /** 从右到左旋转，左旋 */
    static NodePtr rotateLeft(NodePtr root) {
        assert((root->right));

        NodePtr result { root->right };
        root->right = result->left;
        result->left = root;
        result->color = root->color;
        root->color = LinkType::RED;
        result->size = root->size;
        updateSize(root);

        return result;
    }

    /** 颜色反转 */
    static void flipColor(NodePtr root) {
        if (root) {
            root->color = LinkType::RED;

            if (root->left) {
                root->left->color = LinkType::BLACK;
            }

            if (root->right) {
                root->right->color = LinkType::BLACK;
            }
        }
    }

    /** 判断一个节点的父边是否是红色的 */
    static bool isRed(NodePtr root) {
        if (root) {
            if (root->color == LinkType::RED) {
                return true;
            }
        }

        return false;
    }

    /** 判断一个节点的父边是否是黑色的 */
    static bool isBlack(NodePtr root) {
        if (root) {
            if (root->color == LinkType::BLACK) {
                return true;
            }
        }

        return false;
    }

    /**
     * 设 root 是一个 2-node 的指针，该 2-node 的左右 child 都是 2-node,
     * 那么此函数就把这 3 个 2-node 合并成一个 4-node, 然后返回一个指向这个 4-node 的指针。
     */
    static void merge2Nodes(NodePtr root) {
        if (root) {
            if (isBlack(root->left) && isBlack(root->right)) {
                root->left->color = LinkType::RED;
                root->right->color = LinkType::RED;
            }
        }
    }

    /**
     * 2(2, 3) => 2(3, 2), 2(2, 4) => 2(3, 3)
     * b(a, cd) => c(ab, d), b(a, cde) => c(ab, de)
     */
    static NodePtr moveSiblingFromRight(NodePtr root) {
        if (root && root->left && root->right && isRed(root->right->left)) {
            root->left->color = LinkType::RED;
            root->right = rotateRight(root->right);
            auto originColor = root->color;
            NodePtr newRoot = rotateLeft(root);
            newRoot->left->color = LinkType::BLACK;
            newRoot->right->color = LinkType::BLACK;
            newRoot->color = originColor;

            updateSize(root->left->left);
            updateSize(root->left);
            updateSize(root->right);
            updateSize(root);

            return newRoot;
        }

        // Trying to moveSiblingFromRight a NodePtr that shouldn't.
        assert((false));
    }

    static NodePtr moveSiblingFromLeft(NodePtr root) {
        if (root && root->left && isRed(root->left->left) && root->right) {
            LinkType originColor = root->color;
            NodePtr newRoot = rotateRight(root);
            newRoot->color = originColor;
            newRoot->left->color = LinkType::BLACK;
            newRoot->right->color = LinkType::BLACK;
            newRoot->right = rotateLeft(newRoot->right);

            updateSize(root->right->left);
            updateSize(root->right);
            updateSize(root);

            return newRoot;
        }

        assert((false));
    }

    /** 插入操作 */
    static NodePtr doInsert(NodePtr root, const KeyPtr& k, const ValuePtr& v) {
        if (!root) {
            return std::make_shared<RedBlackNode>(k, v, LinkType::RED);
        }

        if (*k > *root->key) {
            root->right = doInsert(root->right, k, v);
        } else if (*k < *root->key) {
            root->left = doInsert(root->left, k, v);
        } else {
            root->value = v;
        }

        // color 和 size 的更新都是在左、右旋的过程中进行
        if (root->left && isRed(root->left) && isRed(root->left->left)) {
            root = rotateRight(root);
        }

        if ((!isRed(root->left)) && isRed(root->right)) {
            root = rotateLeft(root);
        }

        if (isRed(root->left) && isRed(root->right)) {
            flipColor(root);
        }

        updateSize(root);

        return root;
    }

    /** 判定是否是 2-Node */
    static bool is2Node(NodePtr root) {
        if (root) {
            if ((!isRed(root->left)) && (!isRed(root->right))) {
                return true;
            }
        }

        return false;
    }

    /** 判定是否是 3-Node */
    static bool is3Node(NodePtr root) {
        if (root && isRed(root->left) && (!isRed(root->right))) {
            return true;
        }

        return false;
    }

    /** 判定是否是 4-Node */
    static bool is4Node(NodePtr root) {
        if (root && isRed(root->left) && isRed(root->right)) {
            return true;
        }

        return false;
    }

    /** 删除 key 最小的节点 */
    static NodePtr doDeleteMin(NodePtr root) {
        // 在根节点部位，先进行一下预处理：
        // 主要针对的是当 root 为 2-Node 时 root->left 为 2-Node 或者当 root 为 3-Node 时，root->left->left 为 2-Node 的情况。
        if (root) {
            if (is2Node(root)) {
                if (is2Node(root->left)) {
                    if (is2Node(root->right)) {
                        merge2Nodes(root);
                        root = doDeleteMinRecursive(root);
                    } else if (is3Node(root->right)) {
                        root = moveSiblingFromRight(root);
                        root->left = doDeleteMinRecursive(root->left);
                    } else {
                        // root->left point to a 2-Node, but root->right is neither 2-Node nor 3-Node
                        assert((false));
                    }
                } else if (is3Node(root->left)) {
                    root->left = doDeleteMinRecursive(root->left);
                } else {
                    return nullptr;
                }
            } else if (is3Node(root)) {
                root = doDeleteMinRecursive(root);
            } else {
                assert((false));
            }

            updateSize(root);
        }

        return root;
    }
    static NodePtr doDeleteMinRecursive(NodePtr root) {
        // 经过 doDeleteMin 函数的初步处理，到这里时，root 已经不再可能是一个 2-Node，只可能会是 3-Node 或者 4-Node.
        if (is3Node(root) || is4Node(root)) {
            NodePtr nextLevelLeft = root->left->left;
            NodePtr nextLevelRight = root->left->right;
            if (is2Node(nextLevelLeft)) {
                if (is2Node(nextLevelRight)) {
                    merge2Nodes(root->left);
                    root->left->color = LinkType::BLACK;
                    root->left = doDeleteMinRecursive(root->left);
                    updateSize(root->left);
                    updateSize(root);
                } else if (is3Node(nextLevelRight)) {
                    root->left = moveSiblingFromRight(root->left);
                    root->left->left = doDeleteMinRecursive(root->left->left);
                    updateSize(root->left->left);
                    updateSize(root->left);
                    updateSize(root);
                } else {
                    assert((false));
                }
            } else if (is3Node(nextLevelLeft) || is4Node(nextLevelLeft)) {
                root->left->left = doDeleteMinRecursive(root->left->left);
                updateSize(root->left->left);
                updateSize(root->left);
                updateSize(root);
            } else {
                // 到达最底层
                if (is3Node(root)) {
                    root->left = nullptr;
                    updateSize(root);
                    return root;
                } else if (is4Node(root)) {
                    root->left = nullptr;
                    NodePtr newRoot = rotateLeft(root);
                    updateSize(newRoot);
                    return newRoot;
                } else {
                    return nullptr;
                }
            }

            // 后续处理
            if (isRed(root->left) && isRed(root->left->left)) {
                // 条件 isRed(root->left) 蕴含了 root->left 不为 null
                root = rotateRight(root);
            }

            if ((!isRed(root->left)) && isRed(root->right)) {
                root = rotateLeft(root);
            }

            if (isRed(root->left) && isRed(root->right)) {
                flipColor(root);
            }

            updateSize(root);

            return root;
        } else {
            return nullptr;
        }
    }

    /** 交换一个 2-Node 的两个节点的位置 */
    static void swap2Node(NodePtr root) {
        if (root && root->left && root->right) {
            NodePtr tmp = root->left;
            root->left = root->right;
            root->right = tmp;
        }
    }

    /** Delete root in a 2-Node where left child is a 3-Node */
    static NodePtr deleteRootInA2NodeWhereLeftChildIsA3Node(NodePtr root) {
        NodePtr newRoot = root->left;
        LinkType originColor = root->color;
        newRoot->left->color = LinkType::BLACK;
        newRoot->right = root->right;
        updateSize(newRoot);
        newRoot->color = originColor;
        return newRoot;
    }

    /** Delete root in a 2-Node where right child is a 3-Node */
    static NodePtr deleteRootInA2NodeWhereRightChildIsA3Node(NodePtr root) {
        NodePtr newRoot = root->right->left;
        LinkType originColor = root->color;
        root->right->left = newRoot->left;
        newRoot->left = root->left;
        root->right->right = newRoot->right;
        newRoot->right = root->right;
        newRoot->color = originColor;
        updateSize(newRoot);
        return newRoot;
    }

    /** 删除给定 key 对应的节点，从而让相应的键值对被删除掉 */
    static NodePtr doDeleteNodeByKey(NodePtr root, const KeyT& key) {
        if (is2Node(root)) {
            // 也是做类似 doDeleteMin 开头那样的预处理

            // 考虑这个 2-Node 的两个 child 都是 2-Node 的情况，这种的就直接合并
            if (is2Node(root->left) && is2Node(root->right)) {
                merge2Nodes(root);
                root = doDeleteNodeByKeyRecursive(root, key);
                updateSize(root);
                return root;
            }

            // 假如说 root->left 是 null 呢？那么根据红黑树的黑平衡性，root->right 也是。
            // 那也就是说 root 就是这棵树唯一的节点，所以就只拿 key 和 *root->key 比较是否相等就足够了
            if (!(root->left)) {
                if (key == *root->key) {
                    return nullptr;    // key 匹配，就删除这个节点
                } else {
                    return root;    // key 不匹配，不删除任何节点
                }
            }

            // 现在 root->left 和 root->right 都不为 null
            // 并且，root->left 或者 root->right 中至少一个为 3-Node, 还有可能两个都是的
            // 先看一下 key 和当前 key 也就是 *root->key 的相对大小如何
            if (key > *root->key) {

            } else if (key < *root->key) {

            } else {
                // root 本身就是那个要被删除的节点，
                // 不用担心，反正我们知道 root->left 和 root->right 至少有一个是 3-Node.
                if (is3Node(root->left)) {
                    return deleteRootInA2NodeWhereLeftChildIsA3Node(root);
                } else if (is3Node(root->right)) {
                    return deleteRootInA2NodeWhereRightChildIsA3Node(root);
                } else {
                    assert((false)); // Never.
                }
            }

            bool changeBack = false;
            if (key > *root->key) {
                changeBack = true;
                swap2Node(root);
            } else if (key < *root->key) {
                ;    // No op.
            } else {

            }

            if (changeBack) {
                swap2Node(root);
            }

            if (key > *root->key) {
                if (is2Node(root->right)) {
                    if (is2Node(root->left)) {
                        merge2Nodes(root);
                        return doDeleteNodeByKeyRecursive(root, key);
                    } else if (is3Node(root->left)) {
                        root = moveSiblingFromLeft(root);
                        root->right = doDeleteNodeByKeyRecursive(root->right, key);
                        updateSize(root);
                        return root;
                    } else {
                        assert((false));
                    }
                } else if (is3Node(root->right)) {
                    root->right = doDeleteNodeByKeyRecursive(root->right, key);
                    updateSize(root);
                    return root;
                } else {
                    assert((false));
                }
            } else if (key < *root->key) {

            } else {

            }

            if (is2Node(root->left) && is2Node(root->right)) {
                merge2Nodes(root);
                return doDeleteNodeByKeyRecursive(root, key);
            } else if (is2Node(root->left) && is3Node(root->left)) {
                root = moveSiblingFromRight(root);
                root->left = doDeleteNodeByKeyRecursive(root->left, key);
                return root;
            } else if (is3Node(root->left) && is2Node(root->right)) {

            }
        } else if (is3Node(root)) {
            // 直接调用递归过程，因为在此处 root 本身的非 2 性已经得到了保证。
            return doDeleteNodeByKeyRecursive(root, key);
        } else {
            // root 大概率本身就是个空指针
            return root;
        }
    }

    static NodePtr doDeleteNodeByKeyRecursive(NodePtr root, const KeyT& key) {
        return nullptr;
    }
};

#endif //DATASTRUCTUREIMPLEMENTATIONS_REDBLACKTREE_HPP