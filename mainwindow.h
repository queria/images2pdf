#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QAction>

#include <QDebug>

#include "fileinput.h"
#include "images2pdf.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void revalidate();
    void process();
    void processFinished(bool success);
    void processProcessing(QString fileName);
    void processProgress(int done);
    void processAbort();

private:
    void _init_actions();
    void _init_gui();
    void _init_connections();

private:
    QAction *_quit;

    FileInput *_folder;
    FileInput *_file;

    QPushButton *_process;
    QPushButton *_abort;
    QLabel *_status;
    QProgressBar *_progress;

    Images2PDF *_convertor;

};

#endif // MAINWINDOW_H
