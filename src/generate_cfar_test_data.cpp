#include <vector>
#include <fstream>
#include <random>
#include <iostream>
#include <cmath>

// Общие параметры матрицы

constexpr int X = 64;
constexpr int Y = 64;

// Вспомогательная функция записи
void writeBinary(const std::string& filename,
                 const std::vector<float>& data)
{
    std::ofstream f(filename, std::ios::binary);
    if (!f) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return;
    }

    f.write(reinterpret_cast<const char*>(data.data()),
            data.size() * sizeof(float));

    std::cout << "Written: " << filename
              << " (" << data.size() << " floats)" << std::endl;
}


// Генерация шума
std::vector<float> makeNoise(float minVal = 0.0f, float maxVal = 1.0f)
{
    std::vector<float> data(X * Y);

    std::mt19937 gen(42);
    std::uniform_real_distribution<float> dist(minVal, maxVal);

    for (float& v : data)
        v = dist(gen);

    return data;
}

// Добавление "цели"
void addPeak(std::vector<float>& data, int x, int y, float amp)
{
    if (x < 0 || x >= X || y < 0 || y >= Y)
        return;

    data[y * X + x] += amp;
}


void generateCfarTestBinaries()
{
    // 1) Нормальный тест: шум + цель в центре
    {
        auto data = makeNoise();
        addPeak(data, 32, 32, 20.0f);   // цель в центре
        writeBinary("cfar_ok_center.bin", data);
    }

    // 2) Цель на краю (должна игнорироваться)
    {
        auto data = makeNoise();
        addPeak(data, 0, 0, 50.0f);         // левый верх
        addPeak(data, 63, 63, 50.0f);       // правый низ
        writeBinary("cfar_edge_peaks_ignored.bin", data);
    }

    // 3) Цель внутри + цель у края
    {
        auto data = makeNoise();
        addPeak(data, 3, 3, 40.0f);         // близко к краю (игнор)
        addPeak(data, 10, 10, 25.0f);       // нормальная цель
        writeBinary("cfar_edge_vs_inside.bin", data);
    }

    // 4) Константный фон + сильный импульс
    {
        std::vector<float> data(X * Y, 1.0f);
        addPeak(data, 32, 32, 50.0f);
        writeBinary("cfar_const_bg.bin", data);
    }

    // 5) Все нули (детекций быть не должно)

    {
        std::vector<float> data(X * Y, 0.0f);
        writeBinary("cfar_all_zeros.bin", data);
    }

    // 6) NaN / Inf (стресс-тест)
    {
        auto data = makeNoise();
        data[5 * X + 5] = std::nanf("");
        data[6 * X + 6] = std::numeric_limits<float>::infinity();
        data[7 * X + 7] = -std::numeric_limits<float>::infinity();
        addPeak(data, 20, 20, 30.0f);
        writeBinary("cfar_nan_inf.bin", data);
    }

    // 7) Неверный размер файла (половина данных)
    {
        auto data = makeNoise();
        data.resize((X * Y) / 2);   // ОШИБКА размера специально
        writeBinary("cfar_wrong_size.bin", data);
    }

    std::cout << "All test files generated." << std::endl;
}
