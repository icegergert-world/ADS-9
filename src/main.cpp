// Copyright 2022 NNTU-CS
#include <chrono>
#include <iostream>
#include <random>
#include <fstream>
#include <matplotlibcpp.h>
#include "tree.h"

namespace plt = matplotlibcpp;

using Clock = std::chrono::high_resolution_clock;

template<typename F>
double measure(F &&func)
{
    auto t0 = Clock::now();
    func();
    return std::chrono::duration<double, std::milli>(Clock::now()-t0).count();
}

int main()
{
    // ======= короткая демонстрация =========================================
    std::vector<char> in = {'1','2','3'};
    PMTree tree(in);

    auto all = getAllPerms(tree);
    for (auto& p : all) {
        for (char c : p) std::cout << c;
        std::cout << '\n';
    }

    std::cout << "perm1(2) = ";
    for (char c : getPerm1(tree, 2)) std::cout << c;  // 132
    std::cout << "\nperm2(5) = ";
    for (char c : getPerm2(tree, 5)) std::cout << c;  // 321
    std::cout << "\n\n";

    // ======= вычислительный эксперимент ====================================
    std::mt19937_64 rng(std::random_device{}());

    std::vector<int>      Ns   = {4,5,6,7,8,9,10};  // ширина алфавита
    std::vector<double>   tAll, tP1, tP2;

    for (int n : Ns) {
        std::vector<char> alpha;
        for (int i = 0; i < n; ++i) alpha.push_back(static_cast<char>('a'+i));
        PMTree t(alpha);
        std::uniform_int_distribution<size_t> dist(1, t.fac());

        double ta = measure([&]{ getAllPerms(t); });
        size_t   ask = dist(rng);
        double tp1 = measure([&]{ getPerm1(t, ask); });
        double tp2 = measure([&]{ getPerm2(t, ask); });

        tAll.push_back(ta);
        tP1 .push_back(tp1);
        tP2 .push_back(tp2);

        std::cout << "n="<<n
                  << " getAll="<<ta<<"ms"
                  << " getPerm1="<<tp1<<"ms"
                  << " getPerm2="<<tp2<<"ms\n";
    }

    // -------- график --------------------------------------------------------
    plt::semilogy(Ns, tAll, "o-", {{"label", "getAllPerms"}});
    plt::semilogy(Ns, tP1 , "s-", {{"label", "getPerm1"}});
    plt::semilogy(Ns, tP2 , "^-", {{"label", "getPerm2"}});
    plt::xlabel("n (|alphabet|)");
    plt::ylabel("time, ms (log)");
    plt::legend();
    plt::grid(true);
    plt::save("result/plot.png");
    std::cout << "График сохранён в result/plot.png\n";
    return 0;
}
