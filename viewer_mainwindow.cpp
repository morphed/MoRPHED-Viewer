#include "viewer_mainwindow.h"
#include "ui_viewer_mainwindow.h"

Viewer_MainWindow::Viewer_MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Viewer_MainWindow)
{
    ui->setupUi(this);

    m_nEvents = 0;
    m_nCurrentEvent = 0;
    setupGUI();
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

int Viewer_MainWindow::loadInputText()
{
    m_qvDates.clear();
    m_qvDischarge.clear();
    m_qvSediment.clear();

    //declare temp variables to hold stream data
    QString qsDate, qsQ, qsDSWE, qsImport;
    QDateTime tempDate;
    int count = 0;

    //load file
    qDebug()<<m_qsHydro;
    QFile in(m_qsHydro);
    if (in.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&in);
        while (!stream.atEnd())
        {
            //read elements from stream to temp variable, convert to double, and store in a QVector
            stream >> qsDate;
            tempDate = QDateTime::fromString(qsDate, "MM/dd/yyyy,hh:mm");
            m_qvDates.append(tempDate.toTime_t());
            stream >> qsQ;
            m_qvDischarge.append(qsQ.toDouble());
            stream >> qsDSWE;
            stream >> qsImport;
            m_qvSediment.append(qsImport.toDouble());

            //Each line represents 1 model iteration, increment model iterations after each line is read
            //qDebug()<<date[nIterations]<<" "<<q[nIterations]<<" "<<dswe[nIterations]<<" "<<import[nIterations];
            count++;
        }
    }

    return PROCESS_OK;
}

int Viewer_MainWindow::loadInitialLayers()
{
    ui->gv_main->clearScene();

    ui->gv_main->loadDepth(m_qsInitialDepth);
    ui->gv_main->loadHlsd(m_qsInitialHlsd);

    ui->gv_main->addHlsd();
    ui->gv_main->addDepth();

    ui->gv_main->loadScene();

    QFileInfo fi(m_qsInitialDepth);
    QString base = fi.baseName() + "_legend.png";
    QString path = fi.absolutePath();
    QString name = path + "/" + base;

    QImage legendImage;
    legendImage.load(name);
    ui->lbl_legend->setPixmap(QPixmap::fromImage(legendImage));
    ui->lbl_legendTitle->setText("Water Depth (m)");

    return PROCESS_OK;
}

int Viewer_MainWindow::loadXML()
{
    m_xmlDoc.loadDocument(m_xmlFilename, 2);
    updateNumEvents(m_xmlDoc.readNodeData("Events").toInt());

    m_qsHydro = m_xmlDoc.readNodeData("InputHydroSedi");
    m_qsInitialDepth = m_xmlDoc.readNodeData("InitialDepth");
    m_qsInitialHlsd = m_xmlDoc.readNodeData("InitialHillshade");

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
    m_qvPngPaths.append(m_xmlDoc.readNodeData(eventName, "DoDRecentPath"));
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
        if (!fi.exists() && m_nCurrentEvent != 0)
        {
            QMessageBox::information(this, "File not found", "File not found!\n" + m_qvPngPaths[i] + "\nThis file does not exist at this location");
        }
        else
        {
            QString base = fi.baseName() + "_legend.png";
            QString path = fi.absolutePath();
            m_qvLegendPaths[i] = path + "/" + base;
        }
    }
}

void Viewer_MainWindow::redrawLayers()
{
    ui->gv_main->removeAllLayers();

    if (ui->chbx_hlsd->isChecked())
    {
        ui->gv_main->addHlsd();
    }
    if (ui->chbx_depth->isChecked())
    {
        ui->gv_main->addDepth();
    }
    if (ui->chbx_dodEvent->isChecked())
    {
        ui->gv_main->addDoDEvent();
    }
    if (ui->chbx_dodTotal->isChecked())
    {
        ui->gv_main->addDoDTotal();
    }

    updateLegend();
}

void Viewer_MainWindow::setupGUI()
{
    m_qvLegendPaths.resize(4);
    ui->lbl_legendTitle->setWordWrap(true);
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
    ui->plot_event->yAxis->setLabel("Cubic Meters");

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
    ui->plot_total->yAxis->setLabel("Cubic Meters");

    //hydro plot
    ui->plot_hydro->yAxis->setLabel("Discharge (cms)");
    ui->plot_hydro->xAxis->setLabel("Date");
}

void Viewer_MainWindow::setupHydroPlot()
{
    //hydro plot
    double maxQ;
    ui->plot_hydro->addGraph();
    ui->plot_hydro->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
    ui->plot_hydro->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->plot_hydro->graph(0)->setPen(QPen(Qt::blue));
    ui->plot_hydro->yAxis->setLabel("Discharge (cms)");
    ui->plot_hydro->xAxis->setLabel("Date");

    ui->plot_hydro->graph(0)->setData(m_qvDates, m_qvDischarge);
    ui->plot_hydro->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->plot_hydro->xAxis->setDateTimeFormat("MM/dd\nyyyy");
    ui->plot_hydro->xAxis->setAutoTickStep(true);
    ui->plot_hydro->xAxis->setRange(m_qvDates.first()-24*3600, m_qvDates.last()+24*3600);
    ui->plot_hydro->yAxis->setAutoTickStep(true);

    maxQ = findMaxVector(m_qvDischarge);
    m_qvDisMax.resize(2), m_qvDateCurrent.resize(2);
    m_qvDisMax[0] = 0, m_qvDisMax[1] = maxQ + 20;
    m_qvDateCurrent[0] = 0, m_qvDateCurrent[1] = 0;

    ui->plot_hydro->yAxis->setRange(0, maxQ + (maxQ * 0.05));

    ui->plot_hydro->addGraph();
    ui->plot_hydro->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone));
    ui->plot_hydro->graph(1)->setLineStyle(QCPGraph::lsLine);
    ui->plot_hydro->graph(1)->setPen(QPen(Qt::red));
    ui->plot_hydro->graph(1)->setData(m_qvDateCurrent, m_qvDisMax);

    ui->plot_hydro->replot();
    ui->plot_hydro->update();
    ui->plot_hydro->repaint();
}

int Viewer_MainWindow::setXmlFilename(QString filename)
{
    m_xmlFilename = filename;

    return PROCESS_OK;
}

int Viewer_MainWindow::updateLegend()
{

    QImage legendImage;
    bool legend = false;

    if (ui->chbx_dodTotal->isChecked())
    {
        legendImage.load(m_qvLegendPaths[3]);
        ui->lbl_legendTitle->setText("Total Elevation Change (m)");
        legend = true;
    }
    else if (ui->chbx_dodEvent->isChecked())
    {
        legendImage.load(m_qvLegendPaths[2]);
        ui->lbl_legendTitle->setText("Event Elevation Change (m)");
        legend = true;
    }
    else if (ui->chbx_depth->isChecked())
    {
        legendImage.load(m_qvLegendPaths[1]);
        ui->lbl_legendTitle->setText("Water Depth (m)");
        legend = true;
    }

    if (legend)
    {
        ui->lbl_legend->setPixmap(QPixmap::fromImage(legendImage));
    }
    else
    {
        ui->lbl_legend->clear();
        ui->lbl_legendTitle->setText("Legend");
    }

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

    ui->plot_hydro->graph(1)->setData(m_qvDateCurrent, m_qvDisMax);

    ui->plot_hydro->replot();
    ui->plot_hydro->update();
    ui->plot_hydro->repaint();

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
    if (ui->chbx_dodEvent->isChecked())
    {
        ui->gv_main->addDoDEvent();
    }
    if (ui->chbx_dodTotal->isChecked())
    {
       ui->gv_main->addDoDTotal();
    }

    ui->gv_main->loadScene();
    updateLegend();

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
        m_nUsBound = m_xmlDoc.readNodeData("USBound").toInt();
        ui->gv_main->align(m_nUsBound);
        loadInputText();
        setupHydroPlot();
        loadInitialLayers();
    }
}

void Viewer_MainWindow::on_spinInt_event_valueChanged(int arg1)
{
    m_nCurrentEvent = arg1;
    if (arg1 > 0)
    {
        m_qvDateCurrent[0] = m_qvDates[arg1-1], m_qvDateCurrent[1] = m_qvDates[arg1-1];
        readEventData();
        updateView();
        updatePlots();
    }
    else
    {
        m_qvDateCurrent[0] = 0, m_qvDateCurrent[1] = 0;
        loadInitialLayers();
    }
}

void Viewer_MainWindow::on_tbtn_prev_clicked()
{
    if (m_nCurrentEvent > 0)
    {
        m_nCurrentEvent--;
        ui->spinInt_event->setValue(m_nCurrentEvent);
    }
}

void Viewer_MainWindow::on_tbtn_next_clicked()
{
    if (m_nCurrentEvent < m_nEvents)
    {
        m_nCurrentEvent++;
        ui->spinInt_event->setValue(m_nCurrentEvent);
    }
}

void Viewer_MainWindow::on_chbx_dodEvent_stateChanged(int arg1)
{
    if (arg1 == 0)
    {
        //unchecked
        ui->gv_main->removeDoDEvent();
    }
    else if (arg1 == 1)
    {
        //partially checked
    }
    else if (arg1 == 2)
    {
        //checked
        redrawLayers();
    }

    ui->gv_main->loadScene();
    updateLegend();
}

void Viewer_MainWindow::on_chbx_depth_stateChanged(int arg1)
{
    if (arg1 == 0)
    {
        //unchecked
        ui->gv_main->removeDepth();
    }
    else if (arg1 == 1)
    {
        //partially checked
    }
    else if (arg1 == 2)
    {
        //checked
        redrawLayers();
    }

    ui->gv_main->loadScene();
    updateLegend();
}

void Viewer_MainWindow::on_chbx_hlsd_stateChanged(int arg1)
{
    if (arg1 == 0)
    {
        //unchecked
        ui->gv_main->removeHlsd();
    }
    else if (arg1 == 1)
    {
        //partially checked
    }
    else if (arg1 == 2)
    {
        //checked
        redrawLayers();
    }

    ui->gv_main->loadScene();
    updateLegend();
}

void Viewer_MainWindow::on_chbx_dodTotal_stateChanged(int arg1)
{
    if (arg1 == 0)
    {
        //unchecked
        ui->gv_main->removeDoDTotal();
    }
    else if (arg1 == 1)
    {
        //partially checked
    }
    else if (arg1 == 2)
    {
        //checked
        redrawLayers();
    }

    ui->gv_main->loadScene();
    updateLegend();
}
