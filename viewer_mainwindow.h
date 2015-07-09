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
    void setupGUI();
    void setupPlots();
    int setXmlFilename(QString filename);
    int updatePlots();
    int updateView();

    static double findMaxVector(QVector<double> vector);

private slots:
    void on_actionOpen_triggered();

    void on_spinInt_event_valueChanged(int arg1);

    void on_tbtn_prev_clicked();

    void on_tbtn_next_clicked();

    void on_chbx_dod_stateChanged(int arg1);

    void on_chbx_depth_stateChanged(int arg1);

    void on_chbx_hlsd_stateChanged(int arg1);

signals:

private:
    Ui::Viewer_MainWindow *ui;

    QString m_xmlFilename;
    XMLReadWrite m_xmlDoc;
    int m_nEvents, m_nCurrentEvent, m_nUsBound;
    QVector<double> m_qvEventVols, m_qvTotalVols, m_qvBarTicks;
    QVector<QString> m_qvPngPaths, m_qvBarNames, m_qvLegendPaths;
};

#endif // VIEWER_MAINWINDOW_H
