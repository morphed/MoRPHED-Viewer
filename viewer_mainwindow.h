#ifndef VIEWER_MAINWINDOW_H
#define VIEWER_MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QFileDialog>
#include "xmlreadwrite.h"
#include "viewer_exception.h"

namespace Ui {
class Viewer_MainWindow;
}

class Viewer_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit Viewer_MainWindow(QWidget *parent = 0);
    ~Viewer_MainWindow();

    void updateNumEvents(int nEvents);
    int loadXML();
    void readEventData();
    void readEventData(int nEvent);
    int setXmlFilename(QString filename);

private slots:
    void on_actionOpen_triggered();

signals:

private:
    Ui::Viewer_MainWindow *ui;

    QString m_xmlFilename;
    XMLReadWrite m_xmlDoc;
    int m_nEvents, m_nCurrentEvent;
    QVector<double> m_qvEventVols, m_qvTotalVols;
    QVector<QString> m_qvPngPaths;
};

#endif // VIEWER_MAINWINDOW_H
