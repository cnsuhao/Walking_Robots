/*
 * birob_mod.h
 *
 *  Created on: Mar 14, 2014
 *      Author: andy
 */
#include <ode/ode.h>

#ifndef BIROB_MOD_H_
#define BIROB_MOD_H_

// sensors
int heightSensor();     // percieve the height of torso

// actions
void forthLeftThigh();
void holdLeftThigh();
void backLeftThigh();

void forthRightThigh();
void holdRightThigh();
void backRightThigh();

void forthLeftCalf();
void holdLeftCalf();
void backLeftCalf();

void forthRightCalf();
void holdRightCalf();
void backRightCalf();

// models
void nearCallback(void *, dGeomID, dGeomID);
void start();
void stop();
void command(int);
void simLoop(int);

void initModel();
void destroyModel();

#endif /* BIROB_MOD_H_ */
