#pragma once

#include <QMainWindow>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_btnBrowse_clicked();
    void on_btnRun_clicked();

    void on_comboTheme_currentTextChanged(const QString &arg1);

private:
    Ui::MainWindow* ui = nullptr;

    bool readMatrixFile(const QString& path, int X, int Y, std::vector<float>& outData, QString& outError) const;

    bool validateUi(QString& outError) const;

    void applyTheme(const QString& theme);
};
