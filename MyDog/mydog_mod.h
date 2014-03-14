/*
 * mydog_mod.h
 *
 *  Created on: Mar 14, 2014
 *      Author: andy
 */

#ifndef MYDOG_MOD_H_
#define MYDOG_MOD_H_
#include <ode/ode.h>

// sensors
int heightSensor();     // percieve the height of torso

// actions
void moveLimb(int jtnum, int act);

// models
void nearCallback(void *, dGeomID, dGeomID);
void start();
void stop();
void command(int);
void simLoop(int);

void initModel();
void destroyModel();

#endif /* MYDOG_MOD_H_ */
