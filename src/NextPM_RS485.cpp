#include "NextPM_RS485.h"

// ================================================================================================
// CONSTRUCTEUR
// ================================================================================================
/**
 *
 */
NextPM_RS485::NextPM_RS485(HardwareSerial &serial, uint8_t slaveId, int dePin, int rePin) : PM_Sensor(serial),
                                                                                            _serial(serial),
                                                                                            _slaveId(slaveId),
                                                                                            _dePin(dePin),
                                                                                            _rePin(rePin),
                                                                                            _modbus(serial, dePin, rePin)
{
    this->_temperature = 0.0f;
    this->_humidity = 0.0f;
    this->is_sleeping = false;
    this->is_degraded = false;
    this->is_ready = false;

    sub_data.PM_UG_1_0 = 0.0f;
    sub_data.PM_UG_2_5 = 0.0f;
    sub_data.PM_UG_10_0 = 0.0f;
    sub_data.PM_PPM_1_0 = 0.0f;
    sub_data.PM_PPM_2_5 = 0.0f;
    sub_data.PM_PPM_10_0 = 0.0f;
}

NextPM_RS485::NextPM_RS485(HardwareSerial &serial, uint8_t slaveId, int reDePin) : PM_Sensor(serial),
                                                                                   _serial(serial),
                                                                                   _slaveId(slaveId),
                                                                                   _dePin(reDePin),
                                                                                   _rePin(reDePin),
                                                                                   _modbus(serial, reDePin, reDePin)
{
    this->_temperature = 0.0f;
    this->_humidity = 0.0f;
    this->is_sleeping = false;
    this->is_degraded = false;
    this->is_ready = false;

    sub_data.PM_UG_1_0 = 0.0f;
    sub_data.PM_UG_2_5 = 0.0f;
    sub_data.PM_UG_10_0 = 0.0f;
    sub_data.PM_PPM_1_0 = 0.0f;
    sub_data.PM_PPM_2_5 = 0.0f;
    sub_data.PM_PPM_10_0 = 0.0f;
}

/**
 *
 */
NextPM_RS485::~NextPM_RS485()
{
    // TODO Auto-generated destructor stub
}

// ================================================================================================
// Méthodes imposées par PM_Sensor
// ================================================================================================
/**
 *
 */
void NextPM_RS485::configure()
{
    if (this->_rePin >= 0)
        pinMode(this->_rePin, OUTPUT);
    if (this->_dePin >= 0)
        pinMode(this->_dePin, OUTPUT);
    if (this->_rePin >= 0)
        digitalWrite(this->_rePin, LOW);
    if (this->_dePin >= 0)
        digitalWrite(this->_dePin, LOW);

    this->_serial.begin(NEXTPM_BAUD, NEXTPM_PARITY);
    this->_modbus.begin(NEXTPM_BAUD, NEXTPM_PARITY);
    delay(NEXTPM_TIME_WARM_UP);
}

/**
 *
 */
bool NextPM_RS485::read(PM_DATA &data)
{
    return read_1min_rs485(data);
}

/**
 *
 */
/////////////////////////////////////////////////////////////
bool NextPM_RS485::shutdown() { return false; }
bool NextPM_RS485::powerOn() { return false; }
void NextPM_RS485::cleanSensor() {}
// ================================================================================================
//
// ================================================================================================
/**
 *
 */
bool NextPM_RS485::read_10sec_rs485(PM_DATA &data)
{
    uint16_t regs[6] = {0};

    uint8_t error = readHoldingRegisters(56, regs, 6);

    if (error != 0)
        return false;

    combinePM(regs[0], regs[1], data.PM_UG_1_0);
    combinePM(regs[2], regs[3], data.PM_UG_2_5);
    combinePM(regs[4], regs[5], data.PM_UG_10_0);

    return true;
}

/**
 *
 */
bool NextPM_RS485::read_1min_rs485(PM_DATA &data)
{
    uint16_t regs[6] = {0};

    uint8_t error = readHoldingRegisters(68, regs, 6);

    if (error != 0)
        return false;

    combinePM(regs[0], regs[1], data.PM_UG_1_0);
    combinePM(regs[2], regs[3], data.PM_UG_2_5);
    combinePM(regs[4], regs[5], data.PM_UG_10_0);

    return true;
}

/**
 *
 */
bool NextPM_RS485::read_15min_rs485(PM_DATA &data)
{
    uint16_t regs[6] = {0};

    uint8_t error = readHoldingRegisters(80, regs, 6);

    if (error != 0)
        return false;

    combinePM(regs[0], regs[1], data.PM_UG_1_0);
    combinePM(regs[2], regs[3], data.PM_UG_2_5);
    combinePM(regs[4], regs[5], data.PM_UG_10_0);

    return true;
}
// ================================================================================================
// Outils Modbus
// ================================================================================================
void NextPM_RS485::combinePM(uint16_t regLow, uint16_t regHigh, float &pmValue) const
{
    uint32_t raw = ((uint32_t)regHigh << 16) | regLow;
    pmValue = raw / 1000.0f;
}

uint8_t NextPM_RS485::readHoldingRegisters(uint16_t startReg, uint16_t *regs, uint8_t numRegs)
{
    if (regs == nullptr)
        return 7;

    return this->_modbus.readHoldingRegisters(this->_slaveId, startReg, regs, numRegs);
}

bool NextPM_RS485::readStatus(uint16_t &statusWord)
{
    return this->_modbus.readHoldingRegisters(this->_slaveId, 19, &statusWord, 1) == 0;
}

const char *NextPM_RS485::modbusErrorToString(uint8_t error) const
{
    switch (error)
    {
    case 0:
        return "Success";
    case 4:
        return "Response timeout";
    case 5:
        return "Frame error";
    case 6:
        return "CRC error";
    case 7:
        return "Unknown comm error";
    case 8:
        return "Unexpected unit ID";
    case 9:
        return "Exception response";
    case 10:
        return "Unexpected function code";
    case 11:
        return "Unexpected response length";
    case 12:
        return "Unexpected byte count";
    case 13:
        return "Unexpected address";
    case 14:
        return "Unexpected value";
    case 15:
        return "Unexpected quantity";
    default:
        return "Unknown error";
    }
}