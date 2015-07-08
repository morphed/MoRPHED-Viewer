#include "viewer_mainwindow.h"
#include "ui_viewer_mainwindow.h"

Viewer_MainWindow::Viewer_MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Viewer_MainWindow)
{
    ui->setupUi(this);
}

Viewer_MainWindow::~Viewer_MainWindow()
{
    delete ui;
}

void Viewer_MainWindow::updateNumEvents(int nEvents)
{
    m_nEvents = nEvents;
    ui->lbl_floods->setText(QString::number(m_nEvents));
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
    m_qvEventVols.append(m_xmlDoc.readNodeData(eventName, "BankErosion", "Event").toDouble());
    m_qvTotalVols.append(m_xmlDoc.readNodeData(eventName, "BankErosion", "Total").toDouble());
    m_qvEventVols.append(m_xmlDoc.readNodeData(eventName, "BankDeposition", "Event").toDouble());
    m_qvTotalVols.append(m_xmlDoc.readNodeData(eventName, "BankDeposition", "Total").toDouble());
}

int Viewer_MainWindow::setXmlFilename(QString filename)
{
    m_xmlFilename = filename;

    return PROCESS_OK;
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
    }
}
