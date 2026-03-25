/*
 *PM_Sensor.h
 *
 *	(c) Copyright © 2019, 2020, 2021, IRD & Université de Bourgogne
 *  Created on: Sep 6, 2020
 *      Author: Nicolas Marilleau
 *    
 */



#ifndef PM_SENSOR_H
#define PM_SENSOR_H

#include "Stream.h"


struct PM_DATA {
	// unit : mg/m3
	float PM_UG_1_0;
	float PM_UG_2_5;
	float PM_UG_10_0;

	// unit : PC/l
	float PM_PPM_1_0;
	float PM_PPM_2_5;
	float PM_PPM_10_0;
};


class PM_Sensor
{
public:

  virtual void configure() = 0;
  virtual bool shutdown() = 0;
  virtual bool powerOn() = 0;
  virtual bool read(PM_DATA &) = 0;
  virtual void cleanSensor() = 0;

  virtual bool read_1min(PM_DATA & ) = 0;
  virtual bool read_10sec(PM_DATA & ) = 0;
  virtual bool read_5min(PM_DATA & ) = 0;

  //PM_DATA readDuring(long delay);

  PM_Sensor(Stream & );

  virtual ~PM_Sensor() = default;

protected:
  Stream* hstream;

};
#endif // PM_Sensor
