#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cfardetector.h"

#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QApplication>
#include <QDebug>

#include <cstring>   // memcpy

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->comboTheme->setCurrentText("Dark");
    applyTheme("dark");

    // Можно поджать колонки:
    ui->tableDetections->horizontalHeader()->setStretchLastSection(true);

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_btnBrowse_clicked() {
    const QString path = QFileDialog::getOpenFileName(
        this,
        "Выберите бинарный файл float",
        QString(),
        "Binary files (*.bin *.dat *.*)"
    );

    if (!path.isEmpty()) {
        ui->editFilePath->setText(path);
    }
}

bool MainWindow::validateUi(QString& outError) const {
    const int X  = ui->spinX->value();
    const int Y  = ui->spinY->value();
    const int Xb = ui->spinXb->value();
    const int Yb = ui->spinYb->value();
    const int Xs = ui->spinXs->value();
    const int Ys = ui->spinYs->value();

    if (X <= 40 || Y <= 40) {
        outError = "X и Y должны быть больше 40 (по ТЗ).";
        return false;
    }

    auto odd = [](int v){ return (v % 2) != 0; };
    if (!odd(Xb) || !odd(Yb) || !odd(Xs) || !odd(Ys)) {
        outError = "Xb, Yb, Xs, Ys должны быть нечетными (по ТЗ).";
        return false;
    }

    if (!(Xb > Xs && Yb > Ys)) {
        outError = "Должно выполняться Xb > Xs и Yb > Ys (по ТЗ).";
        return false;
    }

    if (Xb >= X || Yb >= Y) {
        outError = "Большое окно (Xb,Yb) должно помещаться в матрицу (X,Y).";
        return false;
    }

    const QString path = ui->editFilePath->text().trimmed();
    if (path.isEmpty()) {
        outError = "Не выбран файл данных.";
        return false;
    }

    return true;
}

bool MainWindow::readMatrixFile(const QString& path, int X, int Y,
                                std::vector<float>& outData, QString& outError) const {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        outError = "Не удалось открыть файл: " + path;
        return false;
    }

    const qint64 expectedBytes = static_cast<qint64>(X) * static_cast<qint64>(Y) * static_cast<qint64>(sizeof(float));
    if (file.size() != expectedBytes) {
        outError = QString("Неверный размер файла.\nОжидалось: %1 байт (X*Y*sizeof(float)), фактически: %2 байт.")
                       .arg(expectedBytes)
                       .arg(file.size());
        return false;
    }

    const QByteArray raw = file.readAll();
    if (raw.size() != expectedBytes) {
        outError = "Не удалось прочитать файл целиком.";
        return false;
    }

    outData.resize(static_cast<std::size_t>(X) * static_cast<std::size_t>(Y));
    std::memcpy(outData.data(), raw.constData(), static_cast<std::size_t>(raw.size()));
    return true;
}

void MainWindow::on_btnRun_clicked() {
    ui->tableDetections->setRowCount(0);

    QString err;
    if (!validateUi(err)) {
        QMessageBox::warning(this, "Ошибка параметров", err);
        return;
    }

    const int X  = ui->spinX->value();
    const int Y  = ui->spinY->value();
    const int Xb = ui->spinXb->value();
    const int Yb = ui->spinYb->value();
    const int Xs = ui->spinXs->value();
    const int Ys = ui->spinYs->value();

    const QString path = ui->editFilePath->text().trimmed();

    std::vector<float> data;
    if (!readMatrixFile(path, X, Y, data, err)) {
        QMessageBox::critical(this, "Ошибка чтения файла", err);
        return;
    }

    CfarDetector det;
    CfarDetector::Params params;
    params.X = X; params.Y = Y;
    params.Xb = Xb; params.Yb = Yb;
    params.Xs = Xs; params.Ys = Ys;
    params.err = 0.01;

    const auto detections = det.run(data, params);

    ui->tableDetections->setRowCount(static_cast<int>(detections.size()));
    for (int i = 0; i < static_cast<int>(detections.size()); ++i) {
        const auto& d = detections[static_cast<std::size_t>(i)];

        ui->tableDetections->setItem(i, 0, new QTableWidgetItem(QString::number(d.y)));
        ui->tableDetections->setItem(i, 1, new QTableWidgetItem(QString::number(d.x)));
        ui->tableDetections->setItem(i, 2, new QTableWidgetItem(QString::number(d.v, 'g', 8)));
    }

    // Поджать колонки по содержимому
    ui->tableDetections->resizeColumnsToContents();

    QMessageBox::information(this, "Готово", QString("Обработка завершена.\nНайдено детекций: %1").arg(detections.size()));
}

void MainWindow::applyTheme(const QString &theme) {
    const QString path = QString(":/styles/styles/%1.qss").arg(theme);
    QFile file(path);

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return;
    }

    const QString style = QString::fromUtf8(file.readAll());

    qApp->setStyleSheet(style);
}

void MainWindow::on_comboTheme_currentTextChanged(const QString &text)
{
    if (text == "Dark") {
        applyTheme("dark");
    } else if (text == "Light") {
        applyTheme("light");
    } else if (text == "Creamy") {
        applyTheme("creamy");
    }
}
