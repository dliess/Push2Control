#include "MappingCurveQt.h"

using namespace push2::qt;

MappingCurve::MappingCurve(const ValueCurve& valueCurve) :
   ValueCurve(valueCurve)
{}

MappingCurve::MappingCurve(qreal outAtMin, qreal outAtMax, qreal bendValue) :
   ValueCurve(outAtMin, outAtMax, bendValue)
{}

qreal MappingCurve::calcOutputValue(qreal value) const
{
   return ValueCurve::calcOutputValue(value);
}

qreal MappingCurve::outAtMax() const
{
   return ValueCurve::outAtMax();
}

qreal MappingCurve::outAtMin() const
{
   return ValueCurve::outAtMin();
}
   
qreal MappingCurve::bendValue() const
{
   return ValueCurve::bendValue();
}
