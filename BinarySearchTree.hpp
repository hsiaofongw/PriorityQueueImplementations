//
// Created by 韦晓枫 on 2022/5/30.
//

#ifndef PRIORITYQUEUEIMPLEMENTATIONS_BINARYSEARCHTREE_HPP
#define PRIORITYQUEUEIMPLEMENTATIONS_BINARYSEARCHTREE_HPP

#include <concepts>
#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace BST {

    template<typename T>
    concept Comparable = requires(const T& a, const T& b)
    {
        a > b; a == b; a < b;  // Require that below expressions: a > b, a == b, and a < b all compiles.
    };

    template <Comparable KeyType>
    using KeyPtr = std::shared_ptr<KeyType>;

    template <typename ValueType>
    using ValuePtr = std::shared_ptr<ValueType>;

    template <Comparable KeyType, typename ValueType>
    struct BSTNode;

    template <Comparable KeyType, typename ValueType>
    using NodePtr = std::shared_ptr<BSTNode<KeyType, ValueType>>;

    template <Comparable KeyType, typename ValueType>
    struct BSTNode {
        KeyPtr<KeyType> keyPtr;
        ValuePtr<ValueType> valuePtr;
        NodePtr<KeyType, ValueType> leftPtr;
        NodePtr<KeyType, ValueType> rightPtr;
    };

    template <Comparable KeyType, typename ValueType>
    decltype(auto) makeEmptyNode() {
        return std::make_shared<BSTNode<KeyType, ValueType>>();
    }

    template <Comparable KeyType, typename ValueType>
    decltype(auto) makeNil() {
        return std::shared_ptr<BSTNode<KeyType, ValueType>> {};
    }

    template<Comparable KeyType, typename ValueType>
    class BSTHandle {

    using NodePtr = BST::NodePtr<KeyType, ValueType>;
    using KeyPtr = BST::KeyPtr<KeyType>;
    using ValuePtr = BST::ValuePtr<ValueType>;

    public:
        BSTHandle( );

        explicit BSTHandle(NodePtr nodePtr);

        NodePtr get();

        [[nodiscard]] size_t size() const;

        void insert(const KeyPtr& keyPtr, const ValuePtr& valuePtr);

        static size_t size(const NodePtr& nodePtr);

        static void insert(NodePtr& nodePtr, const KeyPtr &keyPtr, const ValuePtr &valuePtr);

        static BST::ValuePtr<ValueType> search(const NodePtr& nodePtr, const KeyPtr &keyPtr);

        BST::ValuePtr<ValueType> search(const KeyPtr &keyPtr);

        static BST::NodePtr<KeyType, ValueType> min(const NodePtr& nodePtr);

        BST::NodePtr<KeyType, ValueType> min();

        static void deleteMin(NodePtr& nodePtr);

        static void deleteMinWithNodeKey(NodePtr& nodePtr, const NodePtr& nodeKey);

        void deleteMin();

        [[nodiscard]] bool empty() const;
    private:
        NodePtr nodePtr;
    };

    template<Comparable KeyType, typename ValueType>
    BSTHandle<KeyType, ValueType>::BSTHandle() : nodePtr() { }

    template<Comparable KeyType, typename ValueType>
    BSTHandle<KeyType, ValueType>::BSTHandle(NodePtr _nodePtr) : nodePtr(std::move(_nodePtr)) { }

    template<Comparable KeyType, typename ValueType>
    NodePtr<KeyType, ValueType> BSTHandle<KeyType, ValueType>::get() {
        return this->nodePtr;
    }

    template<Comparable KeyType, typename ValueType>
    size_t BSTHandle<KeyType, ValueType>::size() const {
        return BSTHandle::size(this->nodePtr);
    }

    template<Comparable KeyType, typename ValueType>
    size_t BSTHandle<KeyType, ValueType>::size(const NodePtr& nodePtr) {
        if (!nodePtr) {
            return 0;
        }

        return 1 + BSTHandle<KeyType, ValueType>::size(nodePtr->leftPtr) + BSTHandle<KeyType, ValueType>::size(nodePtr->rightPtr);
    }

    template<Comparable KeyType, typename ValueType>
    void BSTHandle<KeyType, ValueType>::insert(NodePtr &nodePtr, const KeyPtr &keyPtr, const ValuePtr &valuePtr) {
        if (!nodePtr) {
            nodePtr = makeEmptyNode<KeyType, ValueType>();
            nodePtr->keyPtr = keyPtr;
            nodePtr->valuePtr = valuePtr;
            return;
        }

        const KeyType& lhs = *keyPtr;
        const KeyType& rhs = *nodePtr->keyPtr;
        if (lhs > rhs) {
            BSTHandle<KeyType, ValueType>::insert(nodePtr->rightPtr, keyPtr, valuePtr);
        } else if (lhs < rhs) {
            BSTHandle<KeyType, ValueType>::insert(nodePtr->leftPtr, keyPtr, valuePtr);
        } else {
            nodePtr->keyPtr = keyPtr;
            nodePtr->valuePtr = valuePtr;
        }
    }

    template<Comparable KeyType, typename ValueType>
    void BSTHandle<KeyType, ValueType>::insert(const KeyPtr &keyPtr, const ValuePtr &valuePtr) {
        BSTHandle<KeyType, ValueType>::insert(this->nodePtr, keyPtr, valuePtr);
    }

    template<Comparable KeyType, typename ValueType>
    ValuePtr<ValueType> BSTHandle<KeyType, ValueType>::search(const NodePtr &nodePtr, const KeyPtr &keyPtr) {
        auto nil = BST::ValuePtr<ValueType> {};
        if (!nodePtr) {
            return nil;
        }

        const KeyType& lhs = *keyPtr;
        const KeyType& rhs = *nodePtr->keyPtr;
        if (lhs > rhs) {
            return BSTHandle<KeyType, ValueType>::search(nodePtr->rightPtr);
        } else if (lhs < rhs) {
            return BSTHandle<KeyType, ValueType>::search(nodePtr->leftPtr);
        } else {
            return nodePtr->valuePtr;
        }
    }

    template<Comparable KeyType, typename ValueType>
    ValuePtr<ValueType> BSTHandle<KeyType, ValueType>::search(const KeyPtr &keyPtr) {
        return BSTHandle<KeyType, ValueType>::search(this->nodePtr, keyPtr);
    }

    template<Comparable KeyType, typename ValueType>
    NodePtr<KeyType, ValueType> BSTHandle<KeyType, ValueType>::min(const NodePtr &_nodePtr) {
        if (!_nodePtr) {
            return _nodePtr;
        }

        if (!_nodePtr->leftPtr) {
            return _nodePtr;
        }

        return BSTHandle<KeyType, ValueType>::min(_nodePtr->leftPtr);
    }

    template<Comparable KeyType, typename ValueType>
    NodePtr<KeyType, ValueType> BSTHandle<KeyType, ValueType>::min() {
        return BSTHandle<KeyType, ValueType>::min(this->nodePtr);
    }

    template<Comparable KeyType, typename ValueType>
    void BSTHandle<KeyType, ValueType>::deleteMin(NodePtr &nodePtr) {
        auto minNodePtr = BSTHandle<KeyType, ValueType>::min(nodePtr);
        BSTHandle<KeyType, ValueType>::deleteMinWithNodeKey(nodePtr, minNodePtr);
    }

    template<Comparable KeyType, typename ValueType>
    bool BSTHandle<KeyType, ValueType>::empty() const {
        return !this->nodePtr;
    }

    template<Comparable KeyType, typename ValueType>
    void BSTHandle<KeyType, ValueType>::deleteMin() {
        BSTHandle<KeyType, ValueType>::deleteMin(this->nodePtr);
    }

    template<Comparable KeyType, typename ValueType>
    void BSTHandle<KeyType, ValueType>::deleteMinWithNodeKey(NodePtr &nodePtr, const NodePtr &nodeKey) {
        if (nodePtr && nodeKey) {
            if (nodePtr == nodeKey) {
                if (nodePtr->rightPtr) {
                    nodePtr = nodePtr->rightPtr;
                } else {
                    nodePtr = makeNil<KeyType, ValueType>();
                }
                return;
            }

            BSTHandle<KeyType, ValueType>::deleteMinWithNodeKey(nodePtr->leftPtr, nodeKey);
        }
    }
}

#endif //PRIORITYQUEUEIMPLEMENTATIONS_BINARYSEARCHTREE_HPP
