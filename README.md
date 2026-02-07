# CFARDetector (Qt)

Desktop-приложение на **Qt Widgets (C++)** для обнаружения целей в двумерной матрице данных методом **CFAR** (Constant False Alarm Rate).

> Проект выполнен по тестовому заданию: загрузка бинарного файла `float32`, настройка параметров окна CFAR, запуск обработки и вывод списка детекций.

---

## Скриншоты

> Вставь сюда изображения (GitHub будет показывать их прямо на странице).

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

### Вариант 1: Qt Creator (qmake)
1. Открыть `CFARDetector.pro`
2. Собрать и запустить

> **CMake-сборка будет добавлена позже**.

---

## Тестовые данные

В папке `Tests/` хранятся бинарные файлы для проверки работы алгоритма.  


---

## Структура проекта

CFARDetector/
│
├── main.cpp
│
├── mainwindow.ui
│
├── resources.qrc
│
├── CFARDetector.pro
│
├── README.md
│
├── .gitignore
│
├── src/
│   ├── mainwindow.h
│   ├── mainwindow.cpp
│   ├── сfardetector.h
│   └── сfardetector.cpp
│
├── styles/
│   ├── light.qss
│   ├── dark.qss
│   └── creamy.qss
│
└── Tests/
   └── (test .bin )

---

## Планы / улучшения

- [ ] Добавить сборку через **CMake**  
- [ ] Дополнить README инструкциями CMake-сборки
- [ ] Добавить больше тестовых сценариев и файлов

   


