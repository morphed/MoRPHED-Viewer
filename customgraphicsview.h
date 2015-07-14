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
    void addDoDEvent();
    void addDoDTotal();
    void addHlsd();
    void align(int nUsBound);
    void clearScene();
    void loadDepth(QString path);
    void loadHlsd(QString path);
    void loadGraphicsItems(QVector<QString> pngPaths);
    void loadScene();
    void removeAllLayers();
    void removeDepth();
    void removeDoDEvent();
    void removeDoDTotal();
    void removeHlsd();

protected:
    virtual void wheelEvent(QWheelEvent *event);

private:
    QGraphicsScene *m_scene;
    QGraphicsItem *m_qgDepth, *m_qgDodEvent, *m_qgDodTotal, *m_qgHlsd;

};

#endif // CUSTOMGRAPHICSVIEW_H
