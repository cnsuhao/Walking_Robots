/*
 * mydog_mod.h
 *
 *  Created on: Mar 14, 2014
 *      Author: andy
 */

#ifndef MYDOG_MOD_H_
#define MYDOG_MOD_H_
#include <ode/ode.h>

// stops
const int thigh_histop = 90;
const int thigh_lostop = -60;
const int calf_histop = 0;
const int calf_lostop = -90;

// ****** sensors
const dReal *bodyPosition(int bid);
// hinges
void hingeAnchor(int hid, dVector3 anchor);
dReal hingeAngle(int hid);

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
