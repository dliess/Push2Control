#include "MappingCurveDrawing.h"
#include <iostream>

using namespace push2::qt;

MappingCurveDrawing::MappingCurveDrawing(QQuickItem *parent) :
    QQuickPaintedItem(parent),
    m_backgroundColor(Qt::black),
    m_axisColor(Qt::blue),
    m_curveColor(Qt::black)
{
}
    
void MappingCurveDrawing::paint(QPainter *painter)
{
    //std::cout << "Painter called" << std::endl;
    QBrush  brushAxis(m_axisColor);

    painter->setPen(QPen(Qt::white, 1));
    painter->setRenderHints(QPainter::Antialiasing, true);
    auto canvas = boundingRect().adjusted(1,1,-1,-1);
    painter->setBrush(brushAxis);
    painter->setTransform(QTransform(1, 0, 0, -1, 0, canvas.height()));
    painter->drawLine(0, 0, 0, canvas.height());
    painter->drawLine(0, 0, canvas.width(), 0);

    const auto lowerY = m_mappingCurve.outAtMax() * canvas.height();
    const auto upperY = m_mappingCurve.outAtMin() * canvas.height();
    painter->setPen(QPen(Qt::yellow, 1, Qt::DotLine));
    painter->drawLine(0, lowerY, canvas.width(), lowerY);
    painter->setPen(QPen(Qt::green, 1, Qt::DotLine));
    painter->drawLine(0, upperY, canvas.width(), upperY);


    painter->setPen(QPen(Qt::red, 1, Qt::SolidLine));
    int x_prev = 0;
    int y_prev = 0;
    for(unsigned x = 0; x < canvas.width(); ++x)
    {
        const int y = m_mappingCurve.calcOutputValue( qreal(x+1.0) / canvas.width() ) * canvas.height();
        if(0 != x)
        {
            painter->drawLine(x_prev, y_prev, x, y);
        }
        x_prev = x;
        y_prev = y;
    }
    const auto x = m_inValue * canvas.width();
    const auto y = m_mappingCurve.calcOutputValue(m_inValue) * canvas.height();

    painter->setPen(QPen(QColor(Qt::gray), 1, Qt::DashDotLine));
    painter->drawLine(x, 0, x, y);
    painter->drawLine(0, y, x, y);

    painter->setPen(QPen(QColor(Qt::blue), 6));
    painter->drawPoint(x, 0);

    painter->setPen(QPen(QColor(Qt::red), 6, Qt::SolidLine));
    painter->drawLine(0, 0, 0, y);


/*
    QPainterPath path;
    for(unsigned x = 0; x < canvas.width(); ++x)
    {
        const int y = m_mappingCurve.calcOutputValue( qreal(x+1.0) / canvas.width() ) * canvas.height();
        if(0 == x)
            path.moveTo(x, y);
        else
            path.lineTo(x, y);
    }
    painter->drawPath(path.simplified());
*/
    painter->resetTransform();
}

void MappingCurveDrawing::setMappingCurve(MappingCurve mappingCurve)
{
    m_mappingCurve = mappingCurve;
    QQuickPaintedItem::update();
}

void MappingCurveDrawing::setInValue(qreal value)
{
    m_inValue = value;
    QQuickPaintedItem::update();
}
        
QColor MappingCurveDrawing::backgroundColor() const
{
    return m_backgroundColor;
}
    
QColor MappingCurveDrawing::axisColor() const
{
    return m_axisColor;
}
    
QColor MappingCurveDrawing::curveColor() const
{
    return m_curveColor;
}

QColor MappingCurveDrawing::activePointColor() const
{
    return m_activePointColor;
}


qreal MappingCurveDrawing::outLow() const
{
    return 0.0; //TODO
}

qreal MappingCurveDrawing::outHigh() const
{
    return 0.0; //TODO
}

qreal MappingCurveDrawing::bendValue() const
{
    return 0.0; //TODO
}

void MappingCurveDrawing::setBackgroundColor(const QColor backgroundColor)
{
    if (m_backgroundColor == backgroundColor)
        return;
    
    m_backgroundColor = backgroundColor;
    emit backgroundColorChanged(backgroundColor);
}
    
void MappingCurveDrawing::setAxisColor(const QColor axisColor)
{
    if (m_axisColor == axisColor)
        return;
    
    m_axisColor = axisColor;
    emit axisColorChanged(axisColor);
}
    
void MappingCurveDrawing::setCurveColor(const QColor curveColor)
{
    if (m_curveColor == curveColor)
        return;
    
    m_curveColor = curveColor;
    emit curveColorChanged(curveColor);
}

void MappingCurveDrawing::setActivePointColor(const QColor activePointColor)
{
    if (m_activePointColor == activePointColor)
        return;
    
    m_activePointColor = activePointColor;
    emit activePointColorChanged(activePointColor);
}

void MappingCurveDrawing::setOutLow(const qreal outLow)
{
    // TODO
}

void MappingCurveDrawing::setOutHigh(const qreal outHigh)
{
    // TODO
}

void MappingCurveDrawing::setBendValue(const qreal bendValue)
{
    // TODO
}
