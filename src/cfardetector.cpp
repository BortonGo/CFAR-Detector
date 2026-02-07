#include "cfardetector.h"
#include <cmath>
#include <algorithm>

bool CfarDetector::validateParams(const Params& params) noexcept {
    if (params.X <= 40 || params.Y <= 40) return false;
    if (params.Xb <= 0 || params.Yb <= 0 || params.Xs <= 0 || params.Ys <= 0) return false;

    // Все размеры окон должны быть нечетными
    auto odd = [](int value) { return (value % 2) != 0; };
    if (!odd(params.Xb) || !odd(params.Yb) || !odd(params.Xs) || !odd(params.Ys)) return false;

    if (!(params.Xb > params.Xs && params.Yb > params.Ys)) return false;
    if (!(params.Xb < params.X && params.Yb < params.Y)) return false;

    // sq = Xb*Yb - Xs*Ys должно быть > 0
    const int sq = params.Xb * params.Yb - params.Xs * params.Ys;
    if (sq <= 0) return false;

    // err > 0
    if (params.err <= 0.0) return false;

    return true;
}

std::vector<Detection> CfarDetector::run(const std::vector<float>& data, const Params& params) const {
    std::vector<Detection> out;

    if (!validateParams(params)) {
        return out;
    }

    const std::size_t expected = static_cast<std::size_t>(params.X) * static_cast<std::size_t>(params.Y);
    if (data.size() != expected) {
        return out;
    }

    // Th = (1/err)^(1/sq) - 1
    const int sq = params.Xb * params.Yb - params.Xs * params.Ys;
    const double Th = std::pow(1.0 / params.err, 1.0 / static_cast<double>(sq)) - 1.0;

    const int radiusXb = params.Xb / 2;
    const int radiusYb = params.Yb / 2;
    const int radiusXs = params.Xs / 2;
    const int radiusYs = params.Ys / 2;

    // Точки у краев не проверяем
    const int x0 = radiusXb;
    const int x1 = params.X - radiusXb - 1;
    const int y0 = radiusYb;
    const int y1 = params.Y - radiusYb - 1;

    out.reserve(1024); // на глаз, чтобы меньше реаллокаций

    for (int y = y0; y <= y1; ++y) {
        for (int x = x0; x <= x1; ++x) {
            double Sb = 0.0;
            for (int yy = y - radiusYb; yy <= y + radiusYb; ++yy) {
                const std::size_t base = static_cast<std::size_t>(yy) * static_cast<std::size_t>(params.X);
                for (int xx = x - radiusXb; xx <= x + radiusXb; ++xx) {
                    Sb += static_cast<double>(data[base + static_cast<std::size_t>(xx)]);
                }
            }

            double Ss = 0.0;
            for (int yy = y - radiusYs; yy <= y + radiusYs; ++yy) {
                const std::size_t base = static_cast<std::size_t>(yy) * static_cast<std::size_t>(params.X);
                for (int xx = x - radiusXs; xx <= x + radiusXs; ++xx) {
                    Ss += static_cast<double>(data[base + static_cast<std::size_t>(xx)]);
                }
            }

            const float V = data[idx(x, y, params.X)];
            const double background = (Sb - Ss);

            // (Sb - Ss) * Th < V
            if (background * Th < static_cast<double>(V)) {
                out.push_back(Detection{ x, y, V });
            }
        }
    }

    return out;
}
