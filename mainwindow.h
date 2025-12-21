#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextStream>

namespace Ui {
class MainWindow;
}

enum class LogLevel{
    Info,
    Warning,
    Error,
    Success
};

enum class DetectResult{
    CSV,            //CSV形式
    JSON,           //JSON形式
    OPEN_FAILED,    //開くのに失敗
    UNKOWN,         //未知の形式
    NOT_SELECTED,   //未選択
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void selectInputFile();
    void convert();

private:
    DetectResult detectFormat(const QString& filePath);
    bool convertCsvToJson(const QString& input, const QString& output);
    bool convertJsonToCsv(const QString& input, const QString& output);
    bool parseCsvLine(QTextStream& ts, QStringList& row);
    bool isHeader(const QStringList& fields, const QStringList& nextField);
    void log(const QString& message, LogLevel level);

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
