#pragma once

#include <vector>
#include <cstdint>

struct Detection {
    int x = 0;
    int y = 0;
    float v = 0.f;

    Detection(int x_, int y_, float v_) : x(x_), y(y_), v(v_) {}
};

class CfarDetector {
public:
    struct Params {
        int X = 0;
        int Y = 0;
        int Xb = 0;
        int Yb = 0;
        int Xs = 0;
        int Ys = 0;
        double err = 0.01;
    };

    std::vector<Detection> run(const std::vector<float>& data, const Params& params) const;

private:
    static bool validateParams(const Params& params) noexcept;
};
