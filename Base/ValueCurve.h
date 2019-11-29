#ifndef VALUE_CURVE_H
#define VALUE_CURVE_H

class ValueCurve
{
public:
    ValueCurve(float outAtMin, float outAtMax, float bendValue);
    ValueCurve() = default;
    float calcOutputValue(float value) const noexcept;
    void setOutAtMax(float value) noexcept;
    void setOutAtMin(float value) noexcept;
    void setBendValue(float bendValue) noexcept;

    float outAtMax() const noexcept;
    float outAtMin() const noexcept;
    float bendValue() const noexcept;

private:
    static constexpr float outMin = 0.0;
    static constexpr float outMax = 1.0;
    static constexpr float inMin = 0.0;
    static constexpr float inMax = 1.0;
    float m_outAtMax{outMax};
    float m_outAtMin{outMin};
    float m_bendValue{0.0};
};

#endif