#ifndef VALUE_CURVE_DRAWING_H
#define VALUE_CURVE_DRAWING_H
    
#include <QtQuick/QQuickPaintedItem>
#include <QColor>
#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QTime>
#include <QTimer>
#include "MappingCurveQt.h"

namespace push2::qt
{
    
class MappingCurveDrawing : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(QColor axisColor READ axisColor WRITE setAxisColor NOTIFY axisColorChanged)
    Q_PROPERTY(QColor curveColor READ curveColor WRITE setCurveColor NOTIFY curveColorChanged)
    Q_PROPERTY(QColor activePointColor READ activePointColor WRITE setActivePointColor NOTIFY activePointColorChanged)

    Q_PROPERTY(qreal outLow READ outLow WRITE setOutLow NOTIFY outLowChanged)
    Q_PROPERTY(qreal outHigh READ outHigh WRITE setOutHigh NOTIFY outHighChanged)
    Q_PROPERTY(qreal bendValue READ bendValue WRITE setBendValue NOTIFY bendValueChanged)

public:
    explicit MappingCurveDrawing(QQuickItem *parent = 0);
    MappingCurveDrawing(const MappingCurveDrawing&) = default;
    void paint(QPainter *painter) override;   
    
    Q_INVOKABLE void setMappingCurve(MappingCurve mappingCurve);
    Q_INVOKABLE void setInValue(qreal value);

    QColor backgroundColor() const;
    QColor axisColor() const;
    QColor curveColor() const;
    QColor activePointColor() const;

    qreal outLow() const;
    qreal outHigh() const;
    qreal bendValue() const;
    
public slots:
    void setBackgroundColor(const QColor backgroundColor);
    void setAxisColor(const QColor axisColor);
    void setCurveColor(const QColor curveColor);
    void setActivePointColor(const QColor activePointColor);

    void setOutLow(const qreal outLow);
    void setOutHigh(const qreal outHigh);
    void setBendValue(const qreal bendValue);
    
signals:
    void backgroundColorChanged(const QColor backgroundColor);
    void axisColorChanged(const QColor axisColor);
    void curveColorChanged(const QColor curveColor);
    void activePointColorChanged(const QColor activePointColor);

    void outLowChanged(const qreal outLow);
    void outHighChanged(const qreal outHigh);
    void bendValueChanged(const qreal bendValue);
    
private:
    MappingCurve m_mappingCurve;
    QColor       m_backgroundColor;
    QColor       m_axisColor;
    QColor       m_curveColor;
    QColor       m_activePointColor;
    qreal        m_inValue{0.0};
};

} // namespace push2:qt
#endif // VALUE_CURVE_DRAWING_H