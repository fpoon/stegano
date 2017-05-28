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
#include "bmpcoder.h"

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

void MainWindow::on_pbOpen_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                "Choose file...",
                "~",
                "All supported files (*.mp3 *.bmp);;MP3 file (*.mp3);;All files (*)");
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

    changeMode(filename);

    auto w = this->findChild<QLabel*>("lOpen");
    string foo = filename.toStdString();
    int slash = foo.find_last_of('/');
    if (slash == -1)
        slash == foo.find_last_of('\\');
    w->setText(foo.substr(slash, foo.size()).c_str());

    if (coder != nullptr)
        delete coder;

    if (mode)
        switch (mode) {
        case MODE_MP3:
            coder = new MP3FrameCoder((uint8_t*)fileData.data(), fileData.size());
            break;
        case MODE_BMP:
            coder = new BMPCoder((uint8_t*)fileData.data(), fileData.size());
            break;
        case MODE_WAV:
        case MODE_JPG:
        default:
            coder = nullptr;
            log("Unknown file");
            return;
        }
    auto max = this->findChild<QLabel *>("lMaxSize");
    string s = "Max. size: ";
    s += to_string(coder->avaiableSpace());
    s += " characters";
    max->setText(s.c_str());
    log("File loaded!");
}

void MainWindow::on_pbSave_clicked()
{
    string filter, ext;
    if (mode)
        switch (mode) {
        case MODE_MP3:
            ext = ".mp3";
            filter = "MP3 File (*.mp3)";
            break;
        case MODE_BMP:
            ext = ".bmp";
            filter = "BMP File (*.bmp)";
            break;
        case MODE_WAV:
            ext = ".wav";
            filter = "WAV File (*.wav)";
            break;
        case MODE_JPG:
            ext = ".jpg";
            filter = "JPG File (*.jpg)";
            break;
        default:
            ext = "";
            filter = "";
        }

    QString filename = QFileDialog::getSaveFileName(
            this,
            "Save file",
            "output",
            filter.c_str() );
    if (filename == "")
        return;
    if (filename.lastIndexOf('.') < filename.size()-5)
        filename.append(ext.c_str());

    auto v = coder->Encode(getMessageFromWidget());

    ofstream f(filename.toStdString(), ios::binary);
    f.write((const char*)v.data(), v.size());
    log("Saved");

    auto w = this->findChild<QLabel*>("lSave");
    string foo = filename.toStdString();
    int slash = foo.find_last_of('/');
    if (slash == -1)
        slash == foo.find_last_of('\\');
    w->setText(foo.substr(slash, foo.size()).c_str());
}

void MainWindow::on_pbRead_clicked()
{
    if (!coder)
        return;
    string s = coder->Decode();
    auto w = this->findChild<QPlainTextEdit*>("pteMessageToHide");
    w->setPlainText(s.c_str());
}

void MainWindow::changeMode(QString filename)
{
    string file = filename.toStdString();
    string ext = file.substr(file.find_last_of('.'), file.size());
    if (ext == ".mp3")
    {
        log("Opened MP3 file");
        mode = MODE_MP3;
        return;
    }
    if (ext == ".bmp")
    {
        log("Opened BMP file");
        mode = MODE_BMP;
        return;
    }
    log("Opened unknown file");
    mode = MODE_NONE;
}

string MainWindow::getMessageFromWidget()
{
    auto w = this->findChild<QPlainTextEdit*>("pteMessageToHide");
    return w->toPlainText().toStdString();
}

void MainWindow::on_pbHide_clicked()
{
    if (!coder)
        return;
    coder->Encode(getMessageFromWidget());
}
