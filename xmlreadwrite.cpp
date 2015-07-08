//#define MORPHED_GUTSSHARED_EXPORT

#include "xmlreadwrite.h"

XMLReadWrite::XMLReadWrite()
{

}

void XMLReadWrite::clearNodeData(QString tagName)
{
    QDomElement element = root.firstChildElement(tagName);
    element.removeChild(element.firstChild());
}

void XMLReadWrite::clearNodeData(QString parent, QString tagName)
{
    QDomElement eParent = root.firstChildElement(parent);
    QDomElement element = eParent.firstChildElement(tagName);
    element.removeChild(element.firstChild());
}

void XMLReadWrite::clearNodeData(QString parent, QString subParent, QString tagName)
{
    QDomElement eParent = root.firstChildElement(parent);
    QDomElement eSubParent = eParent.firstChildElement(subParent);
    QDomElement element = eSubParent.firstChildElement(tagName);
    element.removeChild(element.firstChild());
}

void XMLReadWrite::createNewElement(QString tagName)
{
    QDomElement element = doc.createElement(tagName);
    root.appendChild(element);
}

void XMLReadWrite::createNewElement(QString parent, QString tagName)
{
    QDomElement eParent = root.firstChildElement(parent);
    QDomElement element = doc.createElement(tagName);
    eParent.appendChild(element);
}

void XMLReadWrite::createNewElement(QString parent, QString subParent, QString tagName)
{
    QDomElement eParent = root.firstChildElement(parent);
    QDomElement eSubParent = eParent.firstChildElement(subParent);
    QDomElement element = doc.createElement(tagName);
    eSubParent.appendChild(element);
}

QString XMLReadWrite::getDocumentFilename()
{
    return docFilename;
}

void XMLReadWrite::loadDocument(QString filename, int type)
{
    QFile file(filename);
    if (file.exists())
    {
        docFilename = filename;

        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            if (doc.setContent(&file))
            {
                //successful load
            }
            else
            {
                //error loading document
                qDebug()<<"error loading xml document";
            }
        }
        else
        {
            //error opening file
            qDebug()<<"error opening xml file";
        }
    }
    if (type == 1)
    {
        root = doc.firstChildElement("MORPHED_Project");
    }
    else if (type == 2)
    {
        root = doc.firstChildElement("MORPHED_Outputs");
    }
}

void XMLReadWrite::printXML()
{
    QFile file (docFilename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {

    }
    else
    {
        QTextStream stream(&file);
        stream << doc.toString();
        file.close();
    }
}

void XMLReadWrite::printXML(QString filename)
{
    QFile file (filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {

    }
    else
    {
        QTextStream stream(&file);
        stream << doc.toString();
        file.close();
    }

}

QString XMLReadWrite::readNodeData(QString tagName)
{
    QString data;
    QDomElement element = root.firstChildElement(tagName);
    data = element.text();
    return data;
}

QString XMLReadWrite::readNodeData(QString parent, QString tagName)
{
    QString data;
    QDomElement eParent = root.firstChildElement(parent);
    QDomElement element = eParent.firstChildElement(tagName);
    data = element.text();
    return data;
}

QString XMLReadWrite::readNodeData(QString parent, QString subParent, QString tagName)
{
    QString data;
    QDomElement eParent = root.firstChildElement(parent);
    QDomElement eSubParent = eParent.firstChildElement(subParent);
    QDomElement element = eSubParent.firstChildElement(tagName);
    data = element.text();
    return data;
}

void XMLReadWrite::setDocumentFilename(QString filename)
{
    docFilename = filename;
}

void XMLReadWrite::writeNodeData(QString tagName, QString nodeData)
{
    QDomElement element = root.firstChildElement(tagName);
    QDomText textNode = doc.createTextNode(nodeData);
    element.removeChild(element.firstChild());
    if (element.firstChild().isNull())
    {
        element.appendChild(textNode);
    }
}

void XMLReadWrite::writeNodeData(QString parent, QString tagName, QString nodeData)
{
    QDomElement eParent = root.firstChildElement(parent);
    QDomElement element = eParent.firstChildElement(tagName);
    QDomText textNode = doc.createTextNode(nodeData);
    element.removeChild(element.firstChild());
    if (element.firstChild().isNull())
    {
        element.appendChild(textNode);
    }
}

void XMLReadWrite::writeNodeData(QString parent, QString subParent, QString tagName, QString nodeData)
{
    QDomElement eParent = root.firstChildElement(parent);
    QDomElement eSubParent = eParent.firstChildElement(subParent);
    QDomElement element = eSubParent.firstChildElement(tagName);
    QDomText textNode = doc.createTextNode(nodeData);
    element.removeChild(element.firstChild());
    if (element.firstChild().isNull())
    {
        element.appendChild(textNode);
    }
}

void XMLReadWrite::writeXMLdocGUI()
{
    doc = QDomDocument("MORPHED_Configuration");
    root = doc.createElement("MORPHED_Project");
    doc.appendChild(root);
    createNewElement("ProjectDirectory");
    createNewElement("OriginalDEMPath");
    createNewElement("OriginalHydroSediPath");
    createNewElement("Description");
    createNewElement("Description", "Reach");
    createNewElement("Description", "Date");
    createNewElement("Description", "DatesModeled");
    createNewElement("Description", "DescriptionDetail");
    createNewElement("Inputs");
    createNewElement("Inputs", "DEMPath");
    createNewElement("Inputs", "ModelIterations");
    createNewElement("Inputs", "HydroSediPath");
    createNewElement("Inputs", "ImportType");
    createNewElement("MorphedParameters");
    createNewElement("MorphedParameters", "BankShearThresh");
    createNewElement("MorphedParameters", "BankSlopeThresh");
    createNewElement("MorphedParameters", "BankAreaThresh");
    createNewElement("MorphedParameters", "ErosionFactor");
    createNewElement("MorphedParameters", "GrainSize");
    createNewElement("MorphedParameters", "PathLength1");
    createNewElement("MorphedParameters", "PathLength1", "DistributionLength");
    createNewElement("MorphedParameters", "PathLength1", "DistributionType");
    createNewElement("MorphedParameters", "PathLength1", "SigA");
    createNewElement("MorphedParameters", "PathLength1", "MuB");
    createNewElement("MorphedParameters", "PathLength1", "CustomTxt");
    createNewElement("MorphedParameters", "PathLengthImport");
    createNewElement("MorphedParameters", "PathLengthImport", "DistributionLength");
    createNewElement("MorphedParameters", "PathLengthImport", "DistributionType");
    createNewElement("MorphedParameters", "PathLengthImport", "SigA");
    createNewElement("MorphedParameters", "PathLengthImport", "MuB");
    createNewElement("MorphedParameters", "PathLengthImport", "CustomTxt");
    createNewElement("Delft3DParameters");
    createNewElement("Delft3DParameters", "DelftPath");
    createNewElement("Delft3DParameters", "USBoundLocation");
    createNewElement("Delft3DParameters", "DSBoundLocation");
    createNewElement("Delft3DParameters", "SimTime");
    createNewElement("Delft3DParameters", "TimeStep");
    createNewElement("Delft3DParameters", "Roughness");
    createNewElement("Delft3DParameters", "HEV");
    createNewElement("Outputs");
    createNewElement("Outputs", "FullType");
    createNewElement("Outputs", "SparseType");
    createNewElement("Outputs", "FullInterval");
    createNewElement("Outputs", "SparseInterval");
    createNewElement("Outputs", "DEM");
    createNewElement("Outputs", "DoDCumulative");
    createNewElement("Outputs", "DoDSinceLast");
    createNewElement("Outputs", "MorphUnits");
    createNewElement("Outputs", "GrainSize");
    createNewElement("Outputs", "Slope");
    createNewElement("Outputs", "Veg");
    createNewElement("Outputs", "ReadMe");
    createNewElement("Outputs", "WaterDepth");
    createNewElement("Outputs", "ShearStress");
    createNewElement("Outputs", "XVel");
    createNewElement("Outputs", "YVel");
    createNewElement("DEMProperties");
    createNewElement("DEMProperties", "Rows");
    createNewElement("DEMProperties", "Cols");
    createNewElement("DEMProperties", "CellWidth");
    createNewElement("DEMProperties", "CellHeight");
    createNewElement("DEMProperties", "TopLeftX");
    createNewElement("DEMProperties", "TopLeftY");

}

void XMLReadWrite::writeXMLdocViewer()
{
    doc = QDomDocument("MORPHED_ViewerData");
    root = doc.createElement("MORPHED_Outputs");
    doc.appendChild(root);

    createNewElement("InputHydroSedi");
    createNewElement("InitialDEM");
    createNewElement("InitialHillshade");
    createNewElement("Events");
    writeNodeData("Events", "0");

}

void XMLReadWrite::writeEvent(int nCurrentFlood)
{
    QString floodName;
    writeNodeData("Events", QString::number(nCurrentFlood));
    floodName = "Event"+QString::number(nCurrentFlood);
    createNewElement(floodName);
    createNewElement(floodName,"EventID");
    writeNodeData(floodName,"EventID",QString::number(nCurrentFlood));
    createNewElement(floodName, "Date");
    createNewElement(floodName, "Discharge");
    createNewElement(floodName,"DEMPath");
    createNewElement(floodName,"DoDRecentPath");
    createNewElement(floodName,"DoDCumulativePath");
    createNewElement(floodName,"HillshadePath");
    createNewElement(floodName,"WaterDepthPath");
    createNewElement(floodName,"ExportedSediment");
    createNewElement(floodName,"ExportedSediment","Event");
    createNewElement(floodName,"ExportedSediment","Total");
    createNewElement(floodName,"ImportedSediment");
    createNewElement(floodName,"ImportedSediment","Event");
    createNewElement(floodName,"ImportedSediment","Total");
    createNewElement(floodName,"BedErosion");
    createNewElement(floodName,"BedErosion","Event");
    createNewElement(floodName,"BedErosion","Total");
    createNewElement(floodName,"BedDeposition");
    createNewElement(floodName,"BedDeposition","Event");
    createNewElement(floodName,"BedDeposition","Total");
    createNewElement(floodName,"BankErosion");
    createNewElement(floodName,"BankErosion","Event");
    createNewElement(floodName,"BankErosion","Total");
    createNewElement(floodName,"BankDeposition");
    createNewElement(floodName,"BankDeposition","Event");
    createNewElement(floodName,"BankDeposition","Total");
}

