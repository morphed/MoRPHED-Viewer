#include "customgraphicsview.h"

CustomGraphicsView::CustomGraphicsView(QWidget* parent) : QGraphicsView (parent)
{
    setDragMode(ScrollHandDrag);
    m_scene = new QGraphicsScene;
}

void CustomGraphicsView::addDepth()
{
    m_scene->addItem(m_qgDepth);
}

void CustomGraphicsView::addDoD()
{
    m_scene->addItem(m_qgDod);
}

void CustomGraphicsView::addHlsd()
{
    m_scene->addItem(m_qgHlsd);
}

void CustomGraphicsView::align(int nUsBound)
{
    if (nUsBound == 1)
    {
        rotate(270);
    }
    else if (nUsBound == 2)
    {
        rotate(90);
    }
    else if (nUsBound == 3)
    {

    }
    else if (nUsBound == 4)
    {
        rotate(180);
    }
}

void CustomGraphicsView::clearScene()
{
    m_scene->clear();
}

void CustomGraphicsView::loadGraphicsItems(QVector<QString> pngPaths)
{
    QImage image;
    image = QImage(pngPaths[0]);
    m_qgHlsd = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    image = QImage(pngPaths[1]);
    m_qgDepth = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    image = QImage(pngPaths[2]);
    m_qgDod = new QGraphicsPixmapItem(QPixmap::fromImage(image));
}

void CustomGraphicsView::loadScene()
{
    setScene(m_scene);
}

void CustomGraphicsView::wheelEvent(QWheelEvent *event)
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    double scaleFactor = 1.1;

    if (event->delta() > 0)
    {
        scale(scaleFactor, scaleFactor);
    }
    else
    {
        scale(1/scaleFactor, 1/scaleFactor);
    }
}
