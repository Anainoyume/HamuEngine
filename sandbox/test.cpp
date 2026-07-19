#include <iostream>
#include <random>
#include <tuple>
#include <vector>

// clang-format off
int mask[5][5] = {
    {0, 1, 1, 1, 1}, 
    {0, 1, 0, 0, 1}, 
    {0, 1, 0, 0, 1}, 
    {0, 1, 1, 1, 1}, 
    {0, 0, 0, 0, 0}
};
// clang-format on

int main() {
    constexpr int MAX_ITER = 1e7;
    constexpr int N        = 25;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 4);

    std::cout << "关卡遮罩:\n";
    for (int i = 0; i < 5; i++) {
        std::string s;
        std::cin >> s;
        for (int j = 0; j < 5; j++) {
            mask[i][j] = s[j] - '0';
        }
    }

    std::cout << "气球总数量:\n";
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
        
    std::cout << "各个气球浮力:\n";
    int cnt {};
    for (int i = 0; i < N; i++) {
        *(&board[0][0] + i) = 0;

        if (*(&mask[0][0] + i) && cnt < n) {
            std::cin >> *(&board[0][0] + i);
            cnt += 1;
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

        if (mask[ax][ay] && mask[bx][by]) {
            std::swap(board[ax][ay], board[bx][by]);
        }
        it++;
    }

    std::cout << "最佳力矩为：" << wx << ' ' << wy << '\n';
    print();

    system("pause");
}