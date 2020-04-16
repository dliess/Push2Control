#include "ValueCurve.h"
#include <math.h>       /* fabs */

ValueCurve::ValueCurve(float outAtMin, float outAtMax, float bendValue) :
    m_outAtMax(outAtMax),
    m_outAtMin(outAtMin),
    m_bendValue(bendValue)
{}


float ValueCurve::calcOutputValue(float value) const noexcept
{
    value = std::max(value, inMin);
    value = std::min(value, inMax);

    if(fabs(m_outAtMin - m_outAtMax) <= std::numeric_limits<float>::epsilon())
    {
        return m_outAtMin;
    }
    if(fabs(m_bendValue) <= std::numeric_limits<float>::epsilon())
    {
        // value = value;
    }
    else if(m_bendValue < 0.0)
    {
        // e^(k*x) - 1
        // ------------    ]0 .. ++[
        // e^k - 1
        const float bendValue = -1.0 * m_bendValue;
        value = (exp(bendValue * value) - 1) / (exp(bendValue) - 1);
    }
    else if(m_bendValue > 0.0)
    {
        // ln(e^k*x + 1)
        // -----------     ]0 .. ++[
        // ln(e^k + 1)
        const float bendValue = exp(m_bendValue);
        value = log(bendValue * value + 1) / log(bendValue + 1);
    }
    const float compressed = (value * fabs(m_outAtMin - m_outAtMax));
    if(m_outAtMax > m_outAtMin)
    {
        return m_outAtMin + compressed;        
    }
    else
    {
        return m_outAtMin + (-1 * compressed);
    }
}

void ValueCurve::setOutAtMax(float value) noexcept
{
    m_outAtMax = std::min(std::max(value, outMin), outMax);
}

void ValueCurve::setOutAtMin(float value) noexcept
{
    m_outAtMin = std::max(std::min(value, outMax), outMin);
}

void ValueCurve::setBendValue(float bendValue) noexcept
{
    m_bendValue = bendValue;
}

float ValueCurve::outAtMax() const noexcept
{
    return m_outAtMax;
}

float ValueCurve::outAtMin() const noexcept
{
    return m_outAtMin;
}

float ValueCurve::bendValue() const noexcept
{
    return m_bendValue;
}
