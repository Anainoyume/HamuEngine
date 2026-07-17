#include <iostream>
#include <random>
#include <tuple>
#include <vector>

int main() {
    constexpr int MAX_ITER = 1e7;
    constexpr int N        = 25;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 4);

    int n {};
    std::cin >> n;

    int board[5][5];

    auto print = [&board]() {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                std::cout << board[i][j] << ' ';
            }
            std::cout << '\n';
        }
    };

    for (int i = 0; i < N; i++) {
        *(&board[0][0] + i) = 0;

        if (i < n) {
            std::cin >> *(&board[0][0] + i);
        }
    }

    auto cal = [&board]() {
        int wx {}, wy {};
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                wx += board[i][j] * (i - 2);
                wy += board[i][j] * (j - 2);
            }
        }
        return std::tuple {wx, wy};
    };

    int wx {INT_MAX}, wy {INT_MAX}, it {};
    while (it < MAX_ITER) {
        auto [p1, p2] = cal();
        if (std::abs(p1) + std::abs(p2) < std::abs(wx) + std::abs(wy)) {
            wx = p1;
            wy = p2;
        }

        if (wx == 0 && wy == 0) break;

        auto ax = dis(gen);
        auto ay = dis(gen);
        auto bx = dis(gen);
        auto by = dis(gen);

        std::swap(board[ax][ay], board[bx][by]);
        it++;
    }

    std::cout << "最佳力矩为：" << wx << ' ' << wy << '\n';
    print();
}