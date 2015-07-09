#ifndef CUSTOMGRAPHICSVIEW_H
#define CUSTOMGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QtWidgets>
#include <QWidget>

class CustomGraphicsView : public QGraphicsView
{
public:
    CustomGraphicsView(QWidget* parent=0);

    void addDepth();
    void addDoD();
    void addHlsd();
    void clearScene();
    void loadGraphicsItems(QVector<QString> pngPaths);
    void loadScene();

protected:
    virtual void wheelEvent(QWheelEvent *event);

private:
    QGraphicsScene *m_scene;
    QGraphicsItem *m_qgDepth, *m_qgDod, *m_qgHlsd;

};

#endif // CUSTOMGRAPHICSVIEW_H
