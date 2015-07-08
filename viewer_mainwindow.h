#ifndef VIEWER_MAINWINDOW_H
#define VIEWER_MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include "xmlreadwrite.h"

namespace Ui {
class Viewer_MainWindow;
}

class Viewer_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit Viewer_MainWindow(QWidget *parent = 0);
    ~Viewer_MainWindow();

    int loadXML();
    int setXmlFilename(QString filename);

private slots:
    void on_actionOpen_triggered();

private:
    Ui::Viewer_MainWindow *ui;

    QString m_xmlFilename;
    XMLReadWrite m_xmlDoc;
    int m_nFloods, m_nCurrentFlood;
};

#endif // VIEWER_MAINWINDOW_H
