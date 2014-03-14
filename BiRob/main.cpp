/*
 * SimWorld.cpp
 *
 *  Created on: Mar 13, 2014
 *      Author: andy
 */
#include <ode/ode.h>
#include <drawstuff/drawstuff.h>
#include <math.h>
#include <gimcs/CSOSAgent.h>
#include <gimcs/Mysql.h>
#include "mydog_mod.h"
//#include "BiRob.h"

#define DRAWSTUFF_TEXTURE_PATH "/home/andy/Software/ode-0.13/drawstuff/textures"

int main(int argc, char **argv)
{
    // setup drawstuff callback functions
    dsFunctions fn;
    fn.version = DS_VERSION;
    fn.start = &start;
    fn.step = &simLoop;
    fn.command = &command;
    fn.stop = &stop;
    fn.path_to_textures = DRAWSTUFF_TEXTURE_PATH;

    // create world
    dInitODE2(0);
    initModel();    // initialize model

    // set up Agent and Avatar
//    Mysql mysql;
//    mysql.setDBArgs("localhost", "root", "huangk", "BiRob");
//
//    CSOSAgent agent(1, 0.9, 0.01);
//    BiRob br("my BiRob");
//    br.connectAgent(&agent);
//    br.setSps(2);
//    // launch avatar
//    br.threadLaunch();

    // simulate the world
    dsSimulationLoop(argc, argv, 352, 288, &fn);

    dCloseODE();

    // avatar
//    agent.dumpMemoryToStorage(&mysql);

    destroyModel();

    return 0;
}

