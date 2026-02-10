#include "cfardetector.h"
#include <cmath>
#include <vector>
#include <cstddef>

static inline std::size_t idx(int x, int y, int X) noexcept {
    return static_cast<std::size_t>(y) * static_cast<std::size_t>(X)
         + static_cast<std::size_t>(x);
}

bool CfarDetector::validateParams(const Params& params) noexcept
{
    if (params.X <= 40 || params.Y <= 40) {
        return false;
    }
    if (params.Xb <= 0 || params.Yb <= 0 || params.Xs <= 0 || params.Ys <= 0) {
        return false;
    }

    auto odd = [](int v) { return (v & 1) != 0; };
    if (!odd(params.Xb) || !odd(params.Yb) || !odd(params.Xs) || !odd(params.Ys)) {
        return false;
    }

    if (!(params.Xb > params.Xs && params.Yb > params.Ys)) {
        return false;
    }

    if (!(params.Xb < params.X && params.Yb < params.Y)) {
        return false;
    }

    const int sq = params.Xb * params.Yb - params.Xs * params.Ys;
    if (sq <= 0) {
        return false;
    }

    return true;
}

static void boxSumSeparableNoEdges(const float* data, int X, int Y, int Xw, int Yw, std::vector<float>& out, std::vector<float>& tmp)
{
    out.assign(static_cast<std::size_t>(X) * static_cast<std::size_t>(Y), 0.0f);
    tmp.assign(static_cast<std::size_t>(X) * static_cast<std::size_t>(Y), 0.0f);

    if (Xw > X || Yw > Y) return;

    const int radiusX = Xw / 2;
    const int radiusY = Yw / 2;

    // допустимые центры окна по X и Y (только где окно целиком внутри)
    const int xMin = radiusX;
    const int xMax = X - radiusX - 1;
    const int yMin = radiusY;
    const int yMax = Y - radiusY - 1;

    if (xMin > xMax || yMin > yMax) {
        return;
    }

    // Горизонтальный проход: tmp[y][x] = сумма по Xw вокруг x
    for (int y = 0; y < Y; ++y) {
        const float* row = data + static_cast<std::size_t>(y) * static_cast<std::size_t>(X);
        float* trow = tmp.data() + static_cast<std::size_t>(y) * static_cast<std::size_t>(X);

        float accumulator = 0.0f;
        for (int xx = 0; xx <= 2 * radiusX; ++xx) {
            accumulator += row[xx];
        }
        trow[xMin] = accumulator;

        // sliding вправо: acc += вошёл справа - вышел слева
        for (int x = xMin + 1; x <= xMax; ++x) {
            accumulator += row[x + radiusX] - row[x - radiusX - 1];
            trow[x] = accumulator;
        }
    }

    // Вертикальный проход: out[y][x] = сумма по Yw вокруг y
    for (int x = xMin; x <= xMax; ++x) {

        float accumulator = 0.0f;
        for (int yy = 0; yy <= 2 * radiusY; ++yy) {
            accumulator += tmp[idx(x, yy, X)];
        }
        out[idx(x, yMin, X)] = accumulator;

        // sliding вниз: accumulator += вошло снизу - вышло сверху
        for (int y = yMin + 1; y <= yMax; ++y) {
            accumulator += tmp[idx(x, y + radiusY, X)] - tmp[idx(x, y - radiusY - 1, X)];
            out[idx(x, y, X)] = accumulator;
        }
    }
}

std::vector<Detection> CfarDetector::run(const std::vector<float>& data, const Params& params) const
{
    std::vector<Detection> out;

    if (!validateParams(params)) {
        return out;
    }

    const std::size_t expected = static_cast<std::size_t>(params.X) * static_cast<std::size_t>(params.Y);
    if (data.size() != expected) {
        return out;
    }

    const int X = params.X;
    const int Y = params.Y;

    const int sq = params.Xb * params.Yb - params.Xs * params.Ys;
    const double Th = std::pow(1.0 / params.err, 1.0 / static_cast<double>(sq)) - 1.0;

    // Проверяем только центры, где большое окно полностью внутри матрицы
    const int radiusXb = params.Xb / 2;
    const int radiusYb = params.Yb / 2;
    const int x0 = radiusXb;
    const int x1 = X - radiusXb - 1;
    const int y0 = radiusYb;
    const int y1 = Y - radiusYb - 1;

    if (x0 > x1 || y0 > y1) {
        return out;
    }

    // Предварительно считаем карты сумм для большого и малого окна:
    std::vector<float> sumB, sumS, tmp;
    boxSumSeparableNoEdges(data.data(), X, Y, params.Xb, params.Yb, sumB, tmp);
    boxSumSeparableNoEdges(data.data(), X, Y, params.Xs, params.Ys, sumS, tmp);

    out.reserve(1024);

    // Основная проверка CFAR
    for (int y = y0; y <= y1; ++y) {
        const std::size_t rowBase = static_cast<std::size_t>(y) * static_cast<std::size_t>(X);
        for (int x = x0; x <= x1; ++x) {
            const std::size_t i = rowBase + static_cast<std::size_t>(x);

            const float V = data[i];
            const double background = static_cast<double>(sumB[i] - sumS[i]);

            if (background * Th < static_cast<double>(V)) {
                out.push_back(Detection{ x, y, V });
            }
        }
    }

    return out;
}
