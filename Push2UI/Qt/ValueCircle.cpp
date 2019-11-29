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
        // ????????? ???????
        QBrush  brush(m_backgroundColor);               // Choose a background color, ...
        QBrush  brushActive(m_borderActiveColor);       // active color of border, ...
        QBrush  brushNonActive(m_borderNonActiveColor); // not active color of border
     
        painter->setPen(Qt::NoPen);                             // remove the outline
        painter->setRenderHints(QPainter::Antialiasing, true);  // Enable antialiasing
     
        painter->setBrush(brushNonActive);                          // Draw the lowest background in a circle
/*        painter->drawEllipse(boundingRect().adjusted(1,1,-1,-1));   // with adjustment to the current dimensions, which
                                                                    // will be determined in QML-layer.
                                                                    // It will not be an active background rim
*/
        painter->drawPie(boundingRect().adjusted(1,1,-1,-1),    // to fit to the size of the layer in QML
                         -120*16,       // The starting point
                         -300.0 *16);   // the angle of rotation, which is necessary to render the object


        // The progress bar will be formed by drawing Pie chart
        painter->setBrush(brushActive);                         // Draw rim active in the background, depending on the angle of rotation
        painter->drawPie(boundingRect().adjusted(1,1,-1,-1),    // to fit to the size of the layer in QML
                         -120*16,         // The starting point
                         -m_angle * (300.0/360.0) *16);   // the angle of rotation, which is necessary to render the object
     
        painter->setBrush(brush);       // the basic background of the timer, which overlap on top
        painter->drawEllipse(boundingRect().adjusted(5,5,-5,-5));   // Border (aka the progress bar) will be formed
    }
     
    void ValueCircle::clear()
    {
        setAngle(0);                    // Expose turn to zero
        update();                       // update object
        emit cleared();                 // Emits a clear signal
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