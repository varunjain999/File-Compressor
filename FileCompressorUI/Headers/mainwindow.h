#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>  // Make sure you have this for QString usage
#include "huffman.hpp"  // Include Huffman class

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT  // This macro is necessary for Qt's signal-slot mechanism

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:  // Declare the slots here
    void onCompressClicked();
    void onDecompressClicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
