#ifndef TOUCHSENSORCLASS_HPP
#define TOUCHSENSORCLASS_HPP

#include "src/Host APIs/CY8CMBR3xxx_Device.h"
#include "src/Host APIs/CY8CMBR3xxx_APIs.h"

class TouchSensorClass
{
private:
    int address;
    uint16 diffCounts[CY8CMBR3xxx_SENSOR_COUNT];

public:
    explicit TouchSensorClass(int slaveAddress)
        : address{slaveAddress}
    {
    }

    void begin(const unsigned char *config)
    {
        if (!CY8CMBR3xxx_Configure(address, config))
        {
            abort();
        }
    }

    bool capture(void)
    {
        return CY8CMBR3xxx_ReadDiffCounts(address, diffCounts);
    }

    uint8_t getValue(int index)
    {
        switch (index)
        {
        case 0:
            return diffCounts[1];
        case 1:
            return diffCounts[0];
        case 2:
            return diffCounts[5];
        case 3:
            return diffCounts[6];
        case 4:
            return diffCounts[7];
        case 5:
            return diffCounts[8];
        default:
            abort();
        }
    }
};

#endif // TOUCHSENSORCLASS_HPP
