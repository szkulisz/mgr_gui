#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#define _USE_MATH_DEFINES
#include <math.h>


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
    double mAlpha = M_PI;
    void initializeGL();
    void paintGL();

    void drawBase();
    void drawPendulum();
};

#endif // MYGLWIDGET_H
