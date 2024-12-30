#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "encode.cpp"   // Include the refactored encode function
#include "decode.cpp"   // Include the refactored decode function

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connect the compress button to the onCompressClicked function
    connect(ui->CompressButton, &QPushButton::clicked, this, &MainWindow::onCompressClicked);

    // Connect the decompress button to the onDecompressClicked function
    connect(ui->DecompressButton, &QPushButton::clicked, this, &MainWindow::onDecompressClicked);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onCompressClicked() {
    QString inputFilePath = ui->InputFileLineEdit->text();
    QString outputFilePath = ui->OutputFileLineEdit->text();

    if(inputFilePath.isEmpty() || outputFilePath.isEmpty()) {
        ui->statusLabel->setText("Please provide valid file paths.");
        return;
    }

    // Call the function from encode.cpp
    encodeFile(inputFilePath.toStdString().c_str(), outputFilePath.toStdString().c_str());

    ui->statusLabel->setText("Compression successful!");
}

void MainWindow::onDecompressClicked() {
    QString inputFilePath = ui->InputFileLineEdit->text();
    QString outputFilePath = ui->OutputFileLineEdit->text();

    if(inputFilePath.isEmpty() || outputFilePath.isEmpty()) {
        ui->statusLabel->setText("Please provide valid file paths.");
        return;
    }

    // Call the function from decode.cpp
    decodeFile(inputFilePath.toStdString().c_str(), outputFilePath.toStdString().c_str());

    ui->statusLabel->setText("Decompression successful!");
}
