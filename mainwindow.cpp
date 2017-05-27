#include <QFileDialog>
#include <fstream>
#include <vector>
#include <cstdint>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loghelper.h"
#include "filehandler.h"

#include "coder.h"
#include "mp3framecoder.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pbOpenToEncode_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Choose file to hide message in", "", "MP3 file (*.mp3);;All files (*)");
    if (filename == "")
        return;
    ifstream f(filename.toStdString(), ios::binary | ios::ate);
    streamsize size = f.tellg();
    f.seekg(0, ios::beg);

    fileData.resize(size, 0);
    if (!f.read((char*)fileData.data(), size))
    {
        log("Loading file failed!");
        return;
    }
    log("File loaded!");
}

void MainWindow::on_pbSaveEncodedFile_clicked()
{
    Coder *coder = new MP3FrameCoder((uint8_t*)fileData.data(), fileData.size());
    auto v = coder->Encode("HELLO");


    ofstream f("output.mp3", ios::binary);
    f.write((const char*)v.data(), v.size());
    log("Encoded");

    delete coder;
}

void MainWindow::on_pbFileToDecode_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Choose file to find message", "", "MP3 file (*.mp3);;All files (*)");
    if (filename == "")
        return;
    ifstream f(filename.toStdString(), ios::binary | ios::ate);
    streamsize size = f.tellg();
    f.seekg(0, ios::beg);

    fileData.resize(size, 0);
    if (!f.read((char*)fileData.data(), size))
    {
        log("Loading file failed!");
        return;
    }
    log("File loaded!");
}

void MainWindow::on_pbRead_clicked()
{
    Coder *coder = new MP3FrameCoder((uint8_t*)fileData.data(), fileData.size());
    string s = coder->Decode();
    log(s);

    delete coder;
}
