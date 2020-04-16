#ifndef PARAMETER_ID_H
#define PARAMETER_ID_H

#include <cstdint>
#include <sstream>

struct ParameterId
{
    uint32_t musicDeviceIdx;
    uint32_t voiceId;
    uint32_t parameterId;
    bool operator==(const ParameterId& rhs) const noexcept
    {
        return musicDeviceIdx == rhs.musicDeviceIdx && voiceId == rhs.voiceId && parameterId == rhs.parameterId;
    }
    std::string toString() const noexcept
    {
        std::stringstream ss;
        ss << "musicDevIndex: " << musicDeviceIdx << " voiceId: " << voiceId << " parameterId: " << parameterId;
        return ss.str();
    }
    struct HashFn
    { 
        size_t operator()(const ParameterId& parameterId) const noexcept
        { 
            return parameterId.musicDeviceIdx << 20 + parameterId.voiceId << 16 + parameterId.parameterId; 
        } 
    };
};


#endif