// Copyright 2022 NNTU-CS
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include  <iostream>
#include  <fstream>
#include  <locale>
#include  <cstdlib>
#include  "tree.h"


// ---------------- PMTree ----------------------------------------------------
PMTree::PMTree(const std::vector<char>& in) : n_(in.size()) {
    // факториал сразу пригодится для getPerm2
    for (size_t i = 2; i <= n_; ++i) factorial_ *= i;

    root_ = std::make_unique<PMNode>('*');             // технический корень
    std::vector<char> alphabet = in;
    std::sort(alphabet.begin(), alphabet.end());       // строго по возрастанию
    buildLevel(root_.get(), alphabet);
}

void PMTree::buildLevel(PMNode* parent, const std::vector<char>& rest) {
    if (rest.empty()) return;
    for (char c : rest) {
        auto child = std::make_unique<PMNode>(c);
        std::vector<char> next(rest);
        next.erase(std::find(next.begin(), next.end(), c));
        buildLevel(child.get(), next);
        parent->child.push_back(std::move(child));
    }
}

// ---------------- геттеры перестановок --------------------------------------
namespace {

void dfs(const PMNode* node, std::vector<char>& buf,
         std::vector<std::vector<char>>& out) {
    if (node->key != '*') buf.push_back(node->key); // '*' – искусственный корень
    if (node->child.empty()) {
        out.push_back(buf);
    } else {
        for (const auto& ch : node->child)
            dfs(ch.get(), buf, out);
    }
    if (!buf.empty() && node->key != '*') buf.pop_back();
}

std::vector<char> factorialDecode(size_t num, const std::vector<char>& alphabet) {
    // num &ge; 1; алфавит уже отсортирован
    size_t n = alphabet.size();
    std::vector<size_t> fact(n);
    fact[0] = 1;
    for (size_t i = 1; i < n; ++i) fact[i] = fact[i-1]*i;

    if (num > fact[n-1]*n) return {};      // выходит за диапазон

    std::vector<char> pool = alphabet;
    std::vector<char> perm;
    --num;                                 // переводим в 0-based
    for (size_t i = n; i-- > 0; ) {
        size_t idx = num / fact[i];
        num       %= fact[i];
        perm.push_back(pool[idx]);
        pool.erase(pool.begin()+idx);
    }
    return perm;
}

} // namespace

std::vector<std::vector<char>> getAllPerms(const PMTree& tree) {
    std::vector<std::vector<char>> out;
    std::vector<char> buf;
    dfs(tree.root(), buf, out);
    return out;
}

std::vector<char> getPerm1(const PMTree& tree, size_t num) {
    auto all = getAllPerms(tree);
    if (num == 0 || num > all.size()) return {};
    return all[num-1];
}

std::vector<char> getPerm2(const PMTree& tree, size_t num) {
    // &laquo;умная&raquo; навигация без полного обхода – факториальная нотация
    std::vector<char> alphabet;
    for (const auto& chPtr : tree.root()->child) alphabet.push_back(chPtr->key);
    return factorialDecode(num, alphabet);
}

