# CFARDetector (Qt)

Desktop-приложение на **Qt Widgets (C++)** для обнаружения целей в двумерной матрице данных методом **CFAR** (Constant False Alarm Rate).

> Проект выполнен по тестовому заданию: загрузка бинарного файла `float32`, настройка параметров окна CFAR, запуск обработки и вывод списка детекций.

---

## Скриншоты

**Light theme**
<img width="1147" height="529" alt="image" src="https://github.com/user-attachments/assets/ca1f2a49-518a-4d59-907e-52051a86ad5e" />

**Dark theme**
<img width="1136" height="533" alt="image" src="https://github.com/user-attachments/assets/e7fdba5e-e56e-49ad-9092-b2af25f09d2d" />

**Creamy theme**
<img width="1139" height="528" alt="image" src="https://github.com/user-attachments/assets/2ec1ef67-54ec-416c-8799-6c5101dec54f" />

---

## Возможности

- Загрузка бинарного файла с матрицей значений (`float`, row-major).
- Параметры матрицы: `X` (столбцы), `Y` (строки).
- Параметры CFAR-окон:
  - `Xb, Yb` — размер большого окна (background)
  - `Xs, Ys` — размер малого окна (guardian cells)
- Проверка корректности параметров по ТЗ (например, нечётность размеров окон).
- Запуск обработки и вывод детекций в таблицу: `y`, `x`, `V`.
- Поддержка тем оформления: **Light / Dark / Creamy** (через `.qss` и `resources.qrc`).

---

## Алгоритм CFAR (кратко)

Для каждой проверяемой точки `(x, y)` вычисляются суммы:
- `Sb` — сумма значений в большом окне `Xb×Yb`
- `Ss` — сумма значений в малом окне `Xs×Ys`

Оценка фона:  
`background = Sb - Ss`

Порог вычисляется из заданной вероятности ложной тревоги `err`:

`Th = (1/err)^(1/sq) - 1`, где `sq = Xb*Yb - Xs*Ys`

Детекция фиксируется, если:  
`background * Th < V(x, y)`

> Граничные точки, где окно не помещается, не обрабатываются.

---

## Формат входного файла

- Файл содержит **ровно `X * Y` значений типа `float` (32-bit)**.
- Порядок: **построчно (row-major)**, индекс: `idx = y * X + x`.

Проверка размера файла:  
`bytes == X * Y * sizeof(float)`

---

## Рекомендованные параметры для тестирования

Пример для матрицы 64×64:
- `X = 64`, `Y = 64`
- `Xb = 9`, `Yb = 9`
- `Xs = 3`, `Ys = 3`
- `err = 0.01`

---

## Сборка и запуск

Проект поддерживает **CMake** и **qmake**.

---

## Вариант 1: CMake (рекомендуется)

### Требования

- Qt 5.x (MinGW 32-bit), например Qt 5.7.0 MinGW 32-bit  
- CMake ≥ 3.10  
- MinGW той же версии, что используется Qt Kit  

---

### Сборка (Windows / MinGW)

```bat
git clone https://github.com/BortonGo/CFAR-Detector.git
cd CFAR-Detector
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
cmake --build .
```

---

### Запуск (Windows)

Qt-приложение требует, чтобы рядом с .exe или в PATH находились Qt DLL.

---

### Способ A — запуск через PATH (для разработки)

Подставь свой путь к Qt:
```bat
set PATH=C:\Qt\QT\5.7\mingw53_32\bin;C:\Qt\QT\Tools\mingw530_32\bin;%PATH%
.\CFARDetector.exe
```

---

### Способ B — сделать портативную папку (для проверки)

Команда скопирует все Qt DLL и плагины рядом с .exe:
```bat
C:\Qt\QT\5.7\mingw53_32\bin\windeployqt.exe .\CFARDetector.exe
.\CFARDetector.exe
```
>Если появится ошибка про libstdc++-6.dll, libgcc_s_dw2-1.dll, libwinpthread-1.dll — добавь в PATH папку MinGW (...\Tools\mingw...\bin) или скопируй эти DLL рядом с .exe.

---

## Вариант 2: Qt Creator (CMake)

1. Открыть CMakeLists.txt
2. Выбрать Kit: Desktop Qt 5.x.x MinGW 32-bit
3. Configure → Build → Run

---

## Вариант 3: Qt Creator (qmake)

1. Открыть CFARDetector.pro
2. Собрать и запустить

---

## Тестовые данные

В папке `Tests/` хранятся бинарные файлы для проверки работы алгоритма.  

---

## Структура проекта

```
CFARDetector/
├── main.cpp
├── mainwindow.ui
├── resources.qrc
├── CFARDetector.pro
├── README.md
├── .gitignore
├── src/
│   ├── mainwindow.h
│   ├── mainwindow.cpp
│   ├── cfardetector.h
│   └── cfardetector.cpp
├── styles/
│   ├── light.qss
│   ├── dark.qss
│   └── creamy.qss
└── Tests/
    └── test.bin
```

---

## Автор

BortonGo

   


