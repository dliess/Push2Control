#ifndef MAPPING_CURVE_QT_H
#define MAPPING_CURVE_QT_H

#include "ValueCurve.h"

#include <QObject>

namespace push2::qt
{

class MappingCurve : public ValueCurve
{
   Q_GADGET
   Q_PROPERTY(qreal outAtMax READ outAtMax)
   Q_PROPERTY(qreal outAtMin READ outAtMin)
   Q_PROPERTY(qreal bendValue READ bendValue)
public:
   MappingCurve(const ValueCurve& valueCurve);
   MappingCurve(qreal outAtMin, qreal outAtMax, qreal bendValue);
   ~MappingCurve() = default;
   MappingCurve() = default;
   MappingCurve(const MappingCurve&) = default;
   MappingCurve& operator=(const MappingCurve&) = default;
   Q_INVOKABLE qreal calcOutputValue(qreal value) const;
   qreal outAtMax() const;
   qreal outAtMin() const;
   qreal bendValue() const;
};

} // namespace qt
#endif