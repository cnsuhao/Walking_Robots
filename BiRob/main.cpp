/*
 * SimWorld.cpp
 *
 *  Created on: Mar 13, 2014
 *      Author: andy
 */
#include <ode/ode.h>
#include <drawstuff/drawstuff.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include <gamcs/CSOSAgent.h>
#include <gamcs/Mysql.h>
#include "birob_mod.h"
#include "BiRob.h"
#include "Group.h"

#define DRAWSTUFF_TEXTURE_PATH "/home/andy/Software/ode-0.13/drawstuff/textures"

void *thread_avatar(void *args)
{
    // set up Agent and Avatar
    printf("start avatar ...\n");

    // BiRob *****************************
//    Mysql mysql;
//    mysql.setDBArgs("localhost", "root", "huangk", "BiRob");
//
//    CSOSAgent agent(1, 0.95, 0.01);
//    BiRob br(0);
//    br.connectAgent(&agent);
//    // launch avatar
//    unsigned long count = 0;
//    while (count < 99999999)
//    {
//        br.step();
//
//        count++;
//        usleep(200000);
//    }
    // ***********************************
    // Cells *****************************
    printf("start cells\n");
    CellGroup cell_group;
    unsigned long count = 0;
    while (count < 99999999)
    {
        cell_group.groupStep();

        count++;
        usleep(200000);
    }

    // ***********************************

    return NULL;
}

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

    pthread_t tid;
    pthread_create(&tid, NULL, thread_avatar, NULL);

    // simulate the world
    dsSimulationLoop(argc, argv, 352, 288, &fn);

    dCloseODE();

    // avatar
//    agent.dumpMemoryToStorage(&mysql);

    destroyModel();

    return 0;
}

