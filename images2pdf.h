#ifndef IMAGES2PDF_H
#define IMAGES2PDF_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QFileInfo>
#include <QPainter>

class Images2PDF : public QObject
{
    Q_OBJECT
public:
    Images2PDF(QString source_dir, QString target_pdf);
    ~Images2PDF();

    int imageCount();
    const QString error();

public slots:
    void start();
    void abort();

signals:
    void processing(const QString imageName); // at start of processing next image
    void progress(int processedCount); // after image was added
    void finished(bool success); // when pdf was succesfully saved and we are done

private:
    void addImage(int imageIndex, QPainter *printer);

private:
    bool _aborted;
    QDir *_source_dir;
    QFileInfo *_target_pdf;
    QStringList _images;
    QStringList _processed;

    QString _error;
};

#endif // IMAGES2PDF_H
