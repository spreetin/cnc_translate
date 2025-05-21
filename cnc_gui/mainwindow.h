#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace NCParser {
class parser;
class generator;
}

class ISOHighlighter;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void textEntered();
    void leftComboChanged(int index);
    void rightComboChanged(int index);

private:
    Ui::MainWindow *ui;
    std::unique_ptr<NCParser::parser> p;
    std::unique_ptr<NCParser::generator> g;
    ISOHighlighter *colorcoder_l;
    ISOHighlighter *colorcoder_r;
};
#endif // MAINWINDOW_H
