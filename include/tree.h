// Copyright 2022 NNTU-CS
#ifndef INCLUDE_TREE_H_
#define INCLUDE_TREE_H_

#include <vector>
#include <memory>

/// Узел дерева перестановок
struct PMNode {
    char                     key;                // символ, хранящийся в узле
    std::vector<std::unique_ptr<PMNode>> child;  // потомки

    explicit PMNode(char k) : key(k) {}
};

/// Дерево вариантов (Permutation-tree)
class PMTree {
 public:
    explicit PMTree(const std::vector<char>& in);      // построение дерева
    const PMNode*  root()  const { return root_.get(); }
    size_t         size()  const { return n_; }        // ширина алфавита
    size_t         fac()   const { return factorial_; }

 private:
    std::unique_ptr<PMNode> root_;
    size_t                  n_   = 0;
    size_t                  factorial_ = 1;

    void buildLevel(PMNode* parent, const std::vector<char>& rest);
};

/// --- API из задания ---------------------------------------------------------
std::vector<std::vector<char>> getAllPerms(const PMTree& tree);
std::vector<char>              getPerm1(const PMTree& tree, size_t num);
std::vector<char>              getPerm2(const PMTree& tree, size_t num);

#endif  // INCLUDE_TREE_H_
