#include "mainwindow.h"
#include <QFormLayout>
#include <QDialog>
#include <QApplication>
#include <QIcon>
#include <QMenuBar>
#include <QMenu>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _convertor(NULL)
{
    // workaround because themed icons dont work in fluxbox as they should
    if(QIcon::themeName().isEmpty() || QIcon::themeName() == "hicolor") {
        QIcon::setThemeName("Tango");
    }

    _init_actions();
    _init_gui();
    _init_connections();

    revalidate();
}

MainWindow::~MainWindow()
{
}

void MainWindow::_init_actions()
{
    _quit = new QAction(QIcon::fromTheme("exit"), trUtf8("Quit"), this);
}

void MainWindow::_init_gui()
{
    setMinimumSize(600, 200);

    QMenu *menu = this->menuBar()->addMenu(trUtf8("Images2PDF"));
    menu->addAction(_quit);

    QWidget *cw = new QWidget();
    setCentralWidget(cw);
    QFormLayout *layout = new QFormLayout(cw);

    _folder = new FileInput(FileInput::DirectoryMode);
    _folder->dialog_caption = trUtf8("Images directory");
    _file = new FileInput(FileInput::FileSaveMode);
    _file->dialog_caption = trUtf8("Save as PDF");

    _process = new QPushButton(trUtf8("Merge"));
    _abort = new QPushButton(trUtf8("Abort"));
    _status = new QLabel();
    _progress = new QProgressBar();

    _abort->setVisible(false);

    _progress->setMinimum(0);
    _progress->setVisible(false);

    layout->addRow(trUtf8("Folder with images"), _folder);
    layout->addRow(trUtf8("Target pdf file:"), _file);
    layout->addRow(_process);
    layout->addRow(_abort);
    layout->addRow(_status);
    layout->addRow(_progress);
}

void MainWindow::_init_connections()
{
    connect(_folder, SIGNAL(valueChanged()), this, SLOT(revalidate()));
    connect(_file, SIGNAL(valueChanged()), this, SLOT(revalidate()));

    connect(_quit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(_process, SIGNAL(clicked()), this, SLOT(process()));
    connect(_abort, SIGNAL(clicked()), this, SLOT(processAbort()));
}

void MainWindow::revalidate()
{
    qDebug() << "revalidating";
    if(_folder->isReadableDir() && _file->isWritableFile()) {
        _status->setText(trUtf8("You can press Merge button now."));
        _process->setEnabled(true);
    } else {
        _status->setText(trUtf8("Select path to dir with images and where to save pdf."));
        _process->setEnabled(false);
    }
}

void MainWindow::process()
{
    if(_convertor) {
        return;
    }
    _status->setText(trUtf8("Preparing ..."));

    _folder->setEnabled(false);
    _file->setEnabled(false);
    _process->setEnabled(false);

    _convertor = new Images2PDF(_folder->value(), _file->value());

    _status->setText(trUtf8("Preparing to process %1 images").arg(_convertor->imageCount()));
    _progress->setValue(0);
    _progress->setMaximum(_convertor->imageCount());
    _progress->setVisible(true);

    _abort->setVisible(true);

    connect(_convertor, SIGNAL(processing(QString)), this, SLOT(processProcessing(QString)));
    connect(_convertor, SIGNAL(progress(int)), this, SLOT(processProgress(int)));
    connect(_convertor, SIGNAL(finished(bool)), this, SLOT(processFinished(bool)));

    _convertor->start();
}

void MainWindow::processFinished(bool success)
{
    _folder->setEnabled(true);
    _file->setEnabled(true);
    _process->setEnabled(true);

    _abort->setVisible(false);;
    _progress->setVisible(false);

    if(success) {
        _status->setText(trUtf8("All done, enjoy your pdf ;)"));
    } else if(_convertor) {
        _status->setText(_convertor->error());
    }
    if(_convertor) {
        delete _convertor;
        _convertor = NULL;
    }
}

void MainWindow::processProcessing(QString fileName)
{
    _status->setText(trUtf8("Processing %1").arg(fileName));
}

void MainWindow::processProgress(int done)
{
    _progress->setValue(done);
}

void MainWindow::processAbort()
{
    if(_convertor) {
        _convertor->abort();
    }
}
