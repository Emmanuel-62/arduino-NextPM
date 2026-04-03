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
    NextPM_RS485(HardwareSerial &, uint8_t , int );
    NextPM_RS485(HardwareSerial &, uint8_t , int , int );

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
    bool read(PM_DATA &);
    void cleanSensor();

    // ==========================================================================
    // 
    // ==========================================================================
    bool readStatus(uint16_t &);
    bool readFirmwareVersion(uint16_t &);

    // ==========================================================================
    // Méthodes imposées par PM_Sensor
    // ==========================================================================
    /**
     *
     */
    bool read_10sec_rs485(PM_DATA &);
    bool read_1min_rs485(PM_DATA &);
    bool read_15min_rs485(PM_DATA &);

    // ==========================================================================
    // 
    // ==========================================================================
    bool readHumidity(float & );
    bool readTemperature(float & );

private:
    ModbusRTUMaster _modbus;
    HardwareSerial & _serial;

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
    uint8_t readHoldingRegisters(uint16_t , uint16_t *, uint8_t );
    const char *modbusErrorToString(uint8_t ) const;
};

#endif // NEXTPM_RS485_H