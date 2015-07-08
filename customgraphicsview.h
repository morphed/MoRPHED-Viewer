#ifndef CUSTOMGRAPHICSVIEW_H
#define CUSTOMGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QtWidgets>
#include <QWidget>

class CustomGraphicsView : public QGraphicsView
{
public:
    CustomGraphicsView(QWidget* parent=0);

protected:
    virtual void wheelEvent(QWheelEvent *event);

private:
    QGraphicsScene *scene;

};

#endif // CUSTOMGRAPHICSVIEW_H
