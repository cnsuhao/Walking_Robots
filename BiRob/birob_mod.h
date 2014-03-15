/*
 * birob_mod.h
 *
 *  Created on: Mar 14, 2014
 *      Author: andy
 */
#include <ode/ode.h>

#ifndef BIROB_MOD_H_
#define BIROB_MOD_H_

// stops
const int thigh_histop = 88;
const int thigh_lostop = -85;
const int calf_histop = 0;
const int calf_lostop = -170;

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

#endif /* BIROB_MOD_H_ */
