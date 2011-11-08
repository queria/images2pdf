#ifndef FILEINPUT_H
#define FILEINPUT_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

class FileInput : public QWidget
{
    Q_OBJECT
public:
    enum FileInputMode { DirectoryMode, FileOpenMode, FileSaveMode };
    Q_DECLARE_FLAGS(FileInputModes, FileInputMode)

public:
    explicit FileInput(FileInputMode mode, QWidget *parent = 0);

    QString value();
    bool isEmpty();
    bool isReadableDir();
    bool isWritableFile();

    QString dialog_caption;
    QString dialog_dir;
    QString dialog_filter;


signals:
    void valueChanged();

public slots:
    void setValue(QString text);
    void browse();

private:
    FileInputMode _mode;

    QLineEdit *_text;
    QPushButton *_browse;

};


Q_DECLARE_OPERATORS_FOR_FLAGS(FileInput::FileInputModes)

#endif // FILEINPUT_H
