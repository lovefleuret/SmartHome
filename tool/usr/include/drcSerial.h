/*
 * drcSerial.h:
 *	Extend wiringPi with the DRC Serial protocol (e.g. to Arduino)
 *	Copyright (c) 2013 Gordon Henderson
 ***********************************************************************
 * This file is part of wiringPi:
 *	https://projects.drogon.net/raspberry-pi/wiringpi/
 *
 *    wiringPi is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as
 *    published by the Free Software Foundation, either version 3 of the
 *    License, or (at your option) any later version.
 *
 *    wiringPi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with wiringPi.
 *    If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************
 */

#ifndef DRC_SERIAL_H
#define DRC_SERIAL_H

#ifdef __cplusplus
extern "C" {
#endif

extern int drcSetupSerial (const int pinBase, const int numPins, const char *device, const int baud) ;

#ifdef __cplusplus
}
#endif


#endif /* DRC_SERIAL_H */
