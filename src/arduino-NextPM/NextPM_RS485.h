#ifndef NEXTPM_RS485_H
#define NEXTPM_RS485_H

#include <Arduino.h>
#include <ModbusRTUMaster.h>

#include "PM_Sensor.h"
#include "NextPM_config.h"

class NextPM_RS485 : public PM_Sensor
{

public:
    // ==========================================================================
    // Constructeur
    // ==========================================================================

    /**
     *
     */
    NextPM_RS485(HardwareSerial &serial, uint8_t slaveId, int reDePin);
    NextPM_RS485(HardwareSerial &serial, uint8_t slaveId, int dePin, int rePin);

    /**
     *
     */
    virtual ~NextPM_RS485();

    // ==========================================================================
    // Méthodes imposées par PM_Sensor
    // ==========================================================================
    /**
     *
     */
    void configure();
    bool shutdown();
    bool powerOn();
    bool read(PM_DATA &data);
    void cleanSensor();

    // ==========================================================================
    // Méthodes imposées par PM_Sensor
    // ==========================================================================
    bool readStatus(uint16_t &statusWord);
    bool readFirmwareVersion(uint16_t &version);

    // ==========================================================================
    // Méthodes imposées par PM_Sensor
    // ==========================================================================
    /**
     *
     */
    bool read_10sec_rs485(PM_DATA &data);
    bool read_1min_rs485(PM_DATA &data);
    bool read_15min_rs485(PM_DATA &data);

private:
    ModbusRTUMaster _modbus;
    HardwareSerial &_serial;

    uint8_t _slaveId;
    int _rePin;
    int _dePin;

    float _temperature;
    float _humidity;

    PM_DATA sub_data;

    bool is_sleeping;
    bool is_degraded;
    bool is_ready;

    // ==========================================================================
    // Outils Modbus
    // ==========================================================================
    /**
     *
     */
    void combinePM(uint16_t, uint16_t, float &) const;
    uint8_t readHoldingRegisters(uint16_t startReg, uint16_t *reg, uint8_t numRegs);
    const char *modbusErrorToString(uint8_t error) const;
};

#endif // NEXTPM_RS485_H