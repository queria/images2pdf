#include "fileinput.h"
#include <QHBoxLayout>
#include <QFileDialog>
#include <QFileInfo>
#include <QDebug>

FileInput::FileInput(FileInputMode mode, QWidget *parent) :
    QWidget(parent)
{
    _mode = mode;

    _text = new QLineEdit();
    _browse = new QPushButton(trUtf8("Browse"));

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);

    layout->addWidget(_text);
    layout->addWidget(_browse);

    connect(_text, SIGNAL(textChanged(QString)), this, SIGNAL(valueChanged()));
    connect(_browse, SIGNAL(clicked()), this, SLOT(browse()));
}

QString FileInput::value() {
    return _text->text();
}
void FileInput::setValue(QString text) {
    _text->setText(text);
}

void FileInput::browse() {
    QString path;
    switch(_mode) {
    case DirectoryMode:
        path = QFileDialog::getExistingDirectory(this, dialog_caption, dialog_dir);
        break;
    case FileOpenMode:
        path = QFileDialog::getOpenFileName(this, dialog_caption, dialog_dir, dialog_filter);
        break;
    case FileSaveMode:
        path = QFileDialog::getSaveFileName(this, dialog_caption, dialog_dir, dialog_filter);
        break;
    }
    if(path.isEmpty()) {
        return;
    }
    _text->setText(path);
}

bool FileInput::isEmpty() {
    return _text->text().isEmpty();
}

bool FileInput::isReadableDir() {
    if(isEmpty()) return false;

    QFileInfo info(_text->text());
    return info.isDir() && info.isReadable();
}

bool FileInput::isWritableFile() {
    if(isEmpty()) return false;

    QFileInfo info(_text->text());
    if(info.exists()) {
        return info.isWritable();
    } else {
        // we check parent directory of selected file
        return QFileInfo(info.dir().absolutePath()).isWritable();
    }
}

