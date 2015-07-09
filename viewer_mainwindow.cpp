#include "viewer_mainwindow.h"
#include "ui_viewer_mainwindow.h"

Viewer_MainWindow::Viewer_MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Viewer_MainWindow)
{
    ui->setupUi(this);

    m_nEvents = 0;
    m_nCurrentEvent = 0;
    setupPlots();
}

Viewer_MainWindow::~Viewer_MainWindow()
{
    delete ui;
}

void Viewer_MainWindow::updateNumEvents(int nEvents)
{
    m_nEvents = nEvents;
    ui->spinInt_event->setMaximum(nEvents);
    ui->lbl_events->setText(QString::number(m_nEvents));
}

int Viewer_MainWindow::loadXML()
{
    m_xmlDoc.loadDocument(m_xmlFilename, 2);
    updateNumEvents(m_xmlDoc.readNodeData("Events").toInt());

    return PROCESS_OK;
}

void Viewer_MainWindow::readEventData()
{
    readEventData(m_nCurrentEvent);
}

void Viewer_MainWindow::readEventData(int nEvent)
{
    QString eventName = "Event" + QString::number(nEvent);

    m_qvEventVols.clear();
    m_qvTotalVols.clear();
    m_qvPngPaths.clear();

    m_qvPngPaths.append(m_xmlDoc.readNodeData(eventName, "HillshadePath"));
    m_qvPngPaths.append(m_xmlDoc.readNodeData(eventName, "WaterDepthPath"));
    m_qvPngPaths.append(m_xmlDoc.readNodeData(eventName, "DoDCumulativePath"));

    m_qvEventVols.append(m_xmlDoc.readNodeData(eventName, "ExportedSediment", "Event").toDouble());
    m_qvTotalVols.append(m_xmlDoc.readNodeData(eventName, "ExportedSediment", "Total").toDouble());
    m_qvEventVols.append(m_xmlDoc.readNodeData(eventName, "ImportedSediment", "Event").toDouble());
    m_qvTotalVols.append(m_xmlDoc.readNodeData(eventName, "ImportedSediment", "Total").toDouble());
    m_qvEventVols.append(m_xmlDoc.readNodeData(eventName, "BedErosion", "Event").toDouble());
    m_qvTotalVols.append(m_xmlDoc.readNodeData(eventName, "BedErosion", "Total").toDouble());
    m_qvEventVols.append(m_xmlDoc.readNodeData(eventName, "BedDeposition", "Event").toDouble());
    m_qvTotalVols.append(m_xmlDoc.readNodeData(eventName, "BedDeposition", "Total").toDouble());
    m_qvEventVols.append(fabs(m_xmlDoc.readNodeData(eventName, "BankErosion", "Event").toDouble()));
    m_qvTotalVols.append(fabs(m_xmlDoc.readNodeData(eventName, "BankErosion", "Total").toDouble()));
    m_qvEventVols.append(m_xmlDoc.readNodeData(eventName, "BankDeposition", "Event").toDouble());
    m_qvTotalVols.append(m_xmlDoc.readNodeData(eventName, "BankDeposition", "Total").toDouble());

    for  (int i=0; i<m_qvPngPaths.size(); i++)
    {
        QFileInfo fi(m_qvPngPaths[i]);
        if (!fi.exists())
        {
            QMessageBox::information(this, "File not found", "File not found!\n" + m_qvPngPaths[i] + "\nThis file does not exist at this location");
        }
    }
}

void Viewer_MainWindow::setupPlots()
{
    m_qvBarTicks << 1 << 2 << 3 << 4 << 5 << 6;
    m_qvBarNames << "Export" << "Import" << "Bed Ero."<< "Bed Depo." << "Bank Ero." << "Bank Depo.";

    //event plot
    ui->plot_event->plotLayout()->insertRow(0);
    ui->plot_event->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->plot_event, "Event Volume"));
    ui->plot_event->xAxis->setAutoTicks(false);
    ui->plot_event->xAxis->setAutoTickLabels(false);
    ui->plot_event->xAxis->setTickVector(m_qvBarTicks);
    ui->plot_event->xAxis->setTickVectorLabels(m_qvBarNames);
    ui->plot_event->xAxis->setTickLabelRotation(60);
    ui->plot_event->xAxis->setSubTickCount(0);
    ui->plot_event->xAxis->setTickLength(0, 4);
    ui->plot_event->xAxis->setRange(0, 7);

    //total plot
    ui->plot_total->plotLayout()->insertRow(0);
    ui->plot_total->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->plot_total, "Total Volume"));
    ui->plot_total->xAxis->setAutoTicks(false);
    ui->plot_total->xAxis->setAutoTickLabels(false);
    ui->plot_total->xAxis->setTickVector(m_qvBarTicks);
    ui->plot_total->xAxis->setTickVectorLabels(m_qvBarNames);
    ui->plot_total->xAxis->setTickLabelRotation(60);
    ui->plot_total->xAxis->setSubTickCount(0);
    ui->plot_total->xAxis->setTickLength(0, 4);
    ui->plot_total->xAxis->setRange(0, 7);
}

int Viewer_MainWindow::setXmlFilename(QString filename)
{
    m_xmlFilename = filename;

    return PROCESS_OK;
}

int Viewer_MainWindow::updatePlots()
{
    double eventMax, totalMax;
    eventMax = findMaxVector(m_qvEventVols);
    totalMax = findMaxVector(m_qvTotalVols);

    ui->plot_event->clearPlottables();
    ui->plot_total->clearPlottables();

    QCPBars *volEvent = new QCPBars(ui->plot_event->xAxis, ui->plot_event->yAxis);
    ui->plot_event->addPlottable(volEvent);
    QPen pen;
    pen.setWidthF(1.2);
    pen.setColor(QColor(255, 131, 0));
    volEvent->setPen(pen);
    volEvent->setBrush(QColor(255, 131, 0));
    volEvent->setData(m_qvBarTicks, m_qvEventVols);
    ui->plot_event->yAxis->setRange(0, (eventMax + eventMax * 0.05));

    QCPBars *volTotal = new QCPBars(ui->plot_total->xAxis, ui->plot_total->yAxis);
    ui->plot_total->addPlottable(volTotal);
    pen.setWidthF(1.2);
    pen.setColor(QColor(255, 131, 0));
    volTotal->setPen(pen);
    volTotal->setBrush(QColor(255, 131, 0));
    volTotal->setData(m_qvBarTicks, m_qvTotalVols);
    ui->plot_total->yAxis->setRange(0, (totalMax + totalMax * 0.05));

    ui->plot_event->replot();
    ui->plot_event->update();
    ui->plot_event->repaint();

    ui->plot_total->replot();
    ui->plot_total->update();
    ui->plot_total->repaint();

    return PROCESS_OK;
}

int Viewer_MainWindow::updateView()
{
    ui->gv_main->clearScene();
    ui->gv_main->loadGraphicsItems(m_qvPngPaths);

    if (ui->chbx_hlsd->isChecked())
    {
        ui->gv_main->addHlsd();
    }
    if (ui->chbx_depth->isChecked())
    {
        ui->gv_main->addDepth();
    }
    if (ui->chbx_dod->isChecked())
    {
        ui->gv_main->addDoD();
    }

    ui->gv_main->loadScene();

    return PROCESS_OK;
}

double Viewer_MainWindow::findMaxVector(QVector<double> vector)
{
    double max =0;

    for (int i=0; i<vector.size(); i++)
    {
        if (i == 0)
        {
            max = vector[i];
        }

        else
        {
            if (max < vector[i])
            {
                max = vector[i];
            }
        }
    }
    return max;
}

void Viewer_MainWindow::on_actionOpen_triggered()
{
    QString filename;
    filename = QFileDialog::getOpenFileName(this, "Select *.display.morph file to open");
    QFileInfo fi(filename);

    if (!fi.exists())
    {
        QMessageBox::information(this, "Input file error", "Input file does not exist");
        return;
    }
    else
    {
        setXmlFilename(filename);
        loadXML();
        m_nCurrentEvent = 0;
    }
}

void Viewer_MainWindow::on_spinInt_event_valueChanged(int arg1)
{
    m_nCurrentEvent = arg1;
    readEventData();
    updateView();
    updatePlots();
}

void Viewer_MainWindow::on_tbtn_prev_clicked()
{
    if (m_nCurrentEvent > 0)
    {
        m_nCurrentEvent--;
        ui->spinInt_event->setValue(m_nCurrentEvent);
        ui->spinInt_event->valueChanged(m_nCurrentEvent);
    }
}

void Viewer_MainWindow::on_tbtn_next_clicked()
{
    if (m_nCurrentEvent < m_nEvents)
    {
        m_nCurrentEvent++;
        ui->spinInt_event->setValue(m_nCurrentEvent);
        ui->spinInt_event->valueChanged(m_nCurrentEvent);
    }
}
