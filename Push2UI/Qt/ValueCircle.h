    #ifndef CLOCKCIRCLE_H
    #define CLOCKCIRCLE_H
     
    #include <QtQuick/QQuickPaintedItem>
    #include <QColor>
    #include <QBrush>
    #include <QPen>
    #include <QPainter>
    #include <QTime>
    #include <QTimer>
     
    class ValueCircle : public QQuickPaintedItem
    {
        Q_OBJECT
        Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
        Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
        Q_PROPERTY(QColor borderActiveColor READ borderActiveColor WRITE setBorderActiveColor NOTIFY borderActiveColorChanged)
        Q_PROPERTY(QColor borderNonActiveColor READ borderNonActiveColor WRITE setBorderNonActiveColor NOTIFY borderNonActiveColorChanged)
        Q_PROPERTY(qreal angle READ angle WRITE setAngle NOTIFY angleChanged)
     
    public:
        explicit ValueCircle(QQuickItem *parent = 0);
     
        void paint(QPainter *painter) override; // Override the method in which the object will be rendered to our
     
        // Methods available from QML for ...
        Q_INVOKABLE void clear();   // ... time cleaning ...
     
        QString name() const;
        QColor backgroundColor() const;
        QColor borderActiveColor() const;
        QColor borderNonActiveColor() const;
        qreal angle() const;
     
    public slots:
        void setName(const QString name);
        void setBackgroundColor(const QColor backgroundColor);
        void setBorderActiveColor(const QColor borderActiveColor);
        void setBorderNonActiveColor(const QColor borderNonActiveColor);
        void setAngle(const qreal angle);
     
    signals:
        void cleared();
     
        void nameChanged(const QString name);
        void backgroundColorChanged(const QColor backgroundColor);
        void borderActiveColorChanged(const QColor borderActiveColor);
        void borderNonActiveColorChanged(const QColor borderNonActiveColor);
        void angleChanged(const qreal angle);
     
    private:
        QString     m_name;                 // The name of the object
        QColor      m_backgroundColor;      // The main background color
        QColor      m_borderActiveColor;    // The color of the border, filling with the progress bezel timer
        QColor      m_borderNonActiveColor; // The color of the background of the border
        qreal       m_angle;                // The rotation angle of the pie chart type, will generate progress on the rim
};
     
    #endif // CLOCKCIRCLE_H