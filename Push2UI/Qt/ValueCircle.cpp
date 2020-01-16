    #include "ValueCircle.h"
     
    ValueCircle::ValueCircle(QQuickItem *parent) :
        QQuickPaintedItem(parent),
        m_backgroundColor(Qt::black),
        m_borderActiveColor(Qt::blue),
        m_borderNonActiveColor(Qt::black),
        m_angle(0)
    {
    }
     
    void ValueCircle::paint(QPainter *painter)
    {
        QBrush  brush(m_backgroundColor);
        QBrush  brushActive(m_borderActiveColor);
        QBrush  brushNonActive(m_borderNonActiveColor);
     
        painter->setPen(Qt::NoPen);
        painter->setRenderHints(QPainter::Antialiasing, true);
     
        painter->setBrush(brushNonActive);
        painter->drawPie(boundingRect().adjusted(1,1,-1,-1),
                         -120 * 16,
                         -300.0 * 16);


        painter->setBrush(brushActive);
        auto angle = m_bipolar ? m_angle - 180 : m_angle;
        angle = -angle * (300.0/360.0) * 16;
        painter->drawPie(boundingRect().adjusted(1,1,-1,-1),
                         m_bipolar ? -270 * 16 : -120 * 16, 
                         angle);
     
        painter->setBrush(brush);
        painter->drawEllipse(boundingRect().adjusted(5,5,-5,-5));
    }
     
    void ValueCircle::clear()
    {
        setAngle(0);
        update();
        emit cleared();
    }
         
    QString ValueCircle::name() const
    {
        return m_name;
    }
     
    QColor ValueCircle::backgroundColor() const
    {
        return m_backgroundColor;
    }
     
    QColor ValueCircle::borderActiveColor() const
    {
        return m_borderActiveColor;
    }
     
    QColor ValueCircle::borderNonActiveColor() const
    {
        return m_borderNonActiveColor;
    }
     
    qreal ValueCircle::angle() const
    {
        return m_angle;
    }

    bool ValueCircle::bipolar() const
    {
        return m_bipolar;
    }
         
    void ValueCircle::setName(const QString name)
    {
        if (m_name == name)
            return;
     
        m_name = name;
        emit nameChanged(name);
    }
     
    void ValueCircle::setBackgroundColor(const QColor backgroundColor)
    {
        if (m_backgroundColor == backgroundColor)
            return;
     
        m_backgroundColor = backgroundColor;
        emit backgroundColorChanged(backgroundColor);
    }
     
    void ValueCircle::setBorderActiveColor(const QColor borderActiveColor)
    {
        if (m_borderActiveColor == borderActiveColor)
            return;
     
        m_borderActiveColor = borderActiveColor;
        emit borderActiveColorChanged(borderActiveColor);
    }
     
    void ValueCircle::setBorderNonActiveColor(const QColor borderNonActiveColor)
    {
        if (m_borderNonActiveColor == borderNonActiveColor)
            return;
     
        m_borderNonActiveColor = borderNonActiveColor;
        emit borderNonActiveColorChanged(borderNonActiveColor);
    }
     
    void ValueCircle::setAngle(const qreal angle)
    {
        if (m_angle == angle)
            return;
     
        m_angle = angle;
     
        /* This addition is made to reset the rotation when the timer 60 seconds
         * */
        if(m_angle <= -360) m_angle = 0;
        update();
        emit angleChanged(m_angle);
    }

 void ValueCircle::setBipolar(bool bipolar)
 {
    if(m_bipolar == bipolar) return;
    m_bipolar = bipolar;
    emit bipolarChanged();
 }
