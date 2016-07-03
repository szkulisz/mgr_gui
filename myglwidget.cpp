#include "myglwidget.h"
#include <QWidget>
#include <QtOpenGL>

void MyGLWidget::setNewPosition(double alpha, double x, double y)
{
    mAlpha = alpha;
    mXBase = x;
    mYBase = y;
}

void MyGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void MyGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    drawBase();
    drawPendulum();

}


void MyGLWidget::drawBase()
{
    // podstawa wahadla
    // szyna
    glColor3f( 0.91, 0.88, 0.7 );
    glBegin( GL_POLYGON );
    glVertex2d( -0.9, -0.2 );
    glVertex2d( 0.9, -0.2 );
    glVertex2d( 0.9, -0.1 );
    glVertex2d( -0.9, -0.1 );
    glEnd();
    // nogi
    glColor3f( 0.0, 0.0, 0.9 );
    glBegin( GL_POLYGON );
    glVertex2d( -0.8, -0.9 );
    glVertex2d( -0.7, -0.9 );
    glVertex2d( -0.7, -0.2 );
    glVertex2d( -0.8, -0.2 );
    glEnd();
    glBegin( GL_POLYGON );
    glVertex2d( 0.8, -0.9 );
    glVertex2d( 0.7, -0.9 );
    glVertex2d( 0.7, -0.2 );
    glVertex2d( 0.8, -0.2 );
    glEnd();
    glColor3f( 0.0, 0.0, 0.0 );
    glBegin( GL_POLYGON );
    glVertex2d( -0.82, -0.9 );
    glVertex2d( -0.68, -0.9 );
    glVertex2d( -0.68, -1.0 );
    glVertex2d( -0.82, -1.0 );
    glEnd();
    glBegin( GL_POLYGON );
    glVertex2d( 0.82, -0.9 );
    glVertex2d( 0.68, -0.9 );
    glVertex2d( 0.68, -1.0 );
    glVertex2d( 0.82, -1.0 );
    glEnd();
}

void MyGLWidget::drawPendulum()
{
    // wózek
    glColor3f( 0.85, 0.0, 0.0 );
    glBegin( GL_POLYGON );
    glVertex2d( mXBase - 0.15, mYBase - 0.1 );
    glVertex2d( mXBase - 0.15, mYBase + 0.1 );
    glVertex2d( mXBase + 0.15, mYBase + 0.1 );
    glVertex2d( mXBase + 0.15, mYBase - 0.1 );
    glEnd();
    // drazek
    glColor3f( 0.6, 0.6, 0.6 );
    glLineWidth(4);
    glBegin( GL_LINES );
    glVertex2d( mXBase, mYBase );
    glVertex2d( mXBase - mPoleLength*sin(mAlpha), mYBase + mPoleLength*cos(mAlpha) );
    glEnd();
    // ciezarek
    glColor3f( 0.6, 0.6, 0.6 );
    glBegin( GL_POLYGON );
    glVertex2d( mXBase - (mPoleLength-0.05)*sin(mAlpha) + 0.025*cos(mAlpha), mYBase + (mPoleLength-0.05)*cos(mAlpha) + 0.025*sin(mAlpha) );
    glVertex2d( mXBase - (mPoleLength-0.05)*sin(mAlpha) - 0.025*cos(mAlpha), mYBase + (mPoleLength-0.05)*cos(mAlpha) - 0.025*sin(mAlpha) );
    glVertex2d( mXBase - (mPoleLength+0.05)*sin(mAlpha) - 0.025*cos(mAlpha), mYBase + (mPoleLength+0.05)*cos(mAlpha) - 0.025*sin(mAlpha) );
    glVertex2d( mXBase - (mPoleLength+0.05)*sin(mAlpha) + 0.025*cos(mAlpha), mYBase + (mPoleLength+0.05)*cos(mAlpha) + 0.025*sin(mAlpha) );
    glEnd();

}

