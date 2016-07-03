#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>


class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit MyGLWidget(QWidget *parent = 0) : QOpenGLWidget(parent) { }

    void setNewPosition(double alpha, double x, double y );

signals:

public slots:

private:
    double mXBase = 0, mYBase= 0, mXVertex = 0, mYVertex = -0.8, mPoleLength = 0.7;
    double mAlpha = 0;
    void initializeGL();
    void paintGL();

    void drawBase();
    void drawPendulum();
};

#endif // MYGLWIDGET_H
