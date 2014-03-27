/*
 * mydog_mod.cpp
 *
 *  Created on: Mar 14, 2014
 *      Author: andy
 */

#include <ode/ode.h>
#include <drawstuff/drawstuff.h>
#include "mydog_mod.h"

#define DRAWSTUFF_TEXTURE_PATH "/home/andy/Software/ode-0.13/drawstuff/textures"

// select correct drawing functions
#ifdef dDOUBLE
#define dsDrawBox dsDrawBoxD
#define dsDrawSphere dsDrawSphereD
#define dsDrawCylinder dsDrawCylinderD
#define dsDrawCapsule dsDrawCapsuleD
#endif

// some constants
#define TORSO_STARTX 0
#define TORSO_STARTY 0
#define TORSO_STARTZ 8

#define TORSO_LENGTH 15 
#define TORSO_WIDTH 8
#define TORSO_HEIGHT 1
#define TORSO_MASS 4

#define THIGH_LENGTH 0.5
#define THIGH_WIDTH 1
#define THIGH_HEIGHT 3
#define THIGH_MASS 1

#define CALF_LENGTH 0.5
#define CALF_WIDTH 0.6
#define CALF_HEIGHT 3
#define CALF_MASS 1

int save_memory = 0;

// dynamics and objects (torso, 8 limbs)
static dWorldID world;
static dSpaceID space;
static dBodyID body[9];
static dJointID joint[8];
static dJointGroupID contact_group;
static dGeomID ground;
static dSpaceID robot_space;
static dGeomID box[9];

static dReal delta_angle = 0.36;    // about 20'
static dReal gain = 2.5;
static dReal max_force = 500;

// sensors
const dReal *bodyPosition(int bid)
{
    return dBodyGetPosition(body[bid]);
}

void hingeAnchor(int hid, dVector3 anchor)
{
    dJointGetHingeAnchor(joint[hid], anchor);
}

dReal hingeAngle(int hid)
{
    return dJointGetHingeAngle(joint[hid]);
}

void moveLimb(int jtnum, int act)
{
    /**
     * | num : 0   1   2   3   4   5   6   7
     * | limb: flt frt blt brt flc frc blc brc
     */
    switch (act)
    {
        case 0:    // move back
            dJointSetHingeParam(joint[jtnum], dParamFMax, max_force);
            dJointSetHingeParam(joint[jtnum], dParamVel, -delta_angle * gain);
            break;
        case 1:    // hold
            dJointSetHingeParam(joint[jtnum], dParamFMax, max_force);
            dJointSetHingeParam(joint[jtnum], dParamVel, 0);
            break;
        case 2:    // move forth
            dJointSetHingeParam(joint[jtnum], dParamFMax, max_force);
            dJointSetHingeParam(joint[jtnum], dParamVel, delta_angle * gain);
            break;
        default:
            printf("*** act is invalid!\n");
            break;
    }
}

// this is called by dSpaceCollide when two objects in space are
// potentially colliding.
void nearCallback(void *data, dGeomID o1, dGeomID o2)
{
    int i, n;

    // only collide things with the ground
    if (o1 != ground && o2 != ground) return;

    const int N = 500;
    dContact contact[N];
    n = dCollide(o1, o2, N, &contact[0].geom, sizeof(dContact));
    if (n > 0)
    {
        for (i = 0; i < n; i++)
        {
            contact[i].surface.mode = dContactSlip1 | dContactSlip2
                    | dContactSoftERP | dContactSoftCFM | dContactApprox1;
            contact[i].surface.mu = dInfinity;
            contact[i].surface.slip1 = 0.1;
            contact[i].surface.slip2 = 0.1;
            contact[i].surface.soft_erp = 0.4;
            contact[i].surface.soft_cfm = 0.2;
            dJointID c = dJointCreateContact(world, contact_group, &contact[i]);
            dJointAttach(c, dGeomGetBody(contact[i].geom.g1),
                    dGeomGetBody(contact[i].geom.g2));
        }
    }
}

// start simulation - set viewpoint
void start()
{
    dAllocateODEDataForThread(dAllocateMaskAll);

    static float xyz[3] = { 15.8317f, -5.9817f, 10.8000f };
    static float hpr[3] = { 160.0000f, -27.5000f, 0.0000f };
    dsSetViewpoint(xyz, hpr);
}

void stop()
{
    printf("****************** stop!\n");
}

void command(int cmd)
{
    switch (cmd)
    {
        case 'f':
            dBodyAddForce(body[0], 10, 0, 0);
            break;
        case 'b':
            dBodyAddForce(body[0], -10, 0, 0);
            break;
        case 's':
            save_memory = 1;
            break;
    }
}

// simulation loop
void simLoop(int pause)
{
    if (!pause)
    {
        // set stops
        for (int i = 0; i < 4; i++)    // 4 thighs
        {
            dJointSetHingeParam(joint[i], dParamHiStop, thigh_histop * 3.14 / 180);    // about 90'
            dJointSetHingeParam(joint[i], dParamLoStop, thigh_lostop * 3.14 / 180);    // about -60'
        }

        for (int i = 4; i < 8; i++)    // 4 calfs
        {
            dJointSetHingeParam(joint[i], dParamHiStop, calf_histop * 3.14 / 180);    // about 0'
            dJointSetHingeParam(joint[i], dParamLoStop, calf_lostop * 3.14 / 180);    // about -90'
        }

        dSpaceCollide(space, 0, &nearCallback);
        dWorldStep(world, 0.05);
        // remove all contact joints
        dJointGroupEmpty(contact_group);
    }

    // drawing
    // draw torso
    dsSetColor(1, 0, 0);
    dsSetTexture(DS_NONE);
    dReal torso_sides[3] = { TORSO_LENGTH, TORSO_WIDTH, TORSO_HEIGHT };
    dsDrawBox(dBodyGetPosition(body[0]), dBodyGetRotation(body[0]),
            torso_sides);

    // draw thighs
    dsSetColor(0, 1, 0);
    dReal thigh_sides[3] = { THIGH_LENGTH, THIGH_WIDTH, THIGH_HEIGHT };
    for (int i = 1; i < 5; i++)
    {
        dsDrawBox(dBodyGetPosition(body[i]), dBodyGetRotation(body[i]),
                thigh_sides);
    }

    // draw calfs
    dsSetColor(0, 0, 1);
    dReal calf_sides[3] = { CALF_LENGTH, CALF_WIDTH, CALF_HEIGHT };
    for (int i = 5; i < 9; i++)
    {
        dsDrawBox(dBodyGetPosition(body[i]), dBodyGetRotation(body[i]),
                calf_sides);
    }
}

void initModel()
{
    int i;
    dMass m;

    world = dWorldCreate();
    space = dHashSpaceCreate(0);
    contact_group = dJointGroupCreate(0);
    dWorldSetGravity(world, 0, 0, -9.8);
    ground = dCreatePlane(space, 0, 0, 1, 0);

    // robot torso
    body[0] = dBodyCreate(world);
    dBodySetPosition(body[0], TORSO_STARTX, TORSO_STARTY, TORSO_STARTZ);
    dMassSetBox(&m, 1, TORSO_LENGTH, TORSO_WIDTH, TORSO_HEIGHT);
    dMassAdjust(&m, TORSO_MASS);
    dBodySetMass(body[0], &m);
    box[0] = dCreateBox(0, TORSO_LENGTH, TORSO_WIDTH, TORSO_HEIGHT);
    dGeomSetBody(box[0], body[0]);

    // front left thigh: 1
    body[1] = dBodyCreate(world);
    dBodySetPosition(body[1], -0.5 * (TORSO_LENGTH - THIGH_LENGTH),
            -0.5 * (TORSO_WIDTH - THIGH_WIDTH),
            TORSO_STARTZ - 0.5 * (TORSO_HEIGHT + THIGH_HEIGHT));
    dMassSetBox(&m, 1, THIGH_LENGTH, THIGH_WIDTH, THIGH_HEIGHT);
    dMassAdjust(&m, THIGH_MASS);
    dBodySetMass(body[1], &m);
    box[1] = dCreateBox(0, THIGH_LENGTH, THIGH_WIDTH, THIGH_HEIGHT);
    dGeomSetBody(box[1], body[1]);

    // front right thigh: 2
    body[2] = dBodyCreate(world);
    dBodySetPosition(body[2], -0.5 * (TORSO_LENGTH - THIGH_LENGTH),
            0.5 * (TORSO_WIDTH - THIGH_WIDTH),
            TORSO_STARTZ - 0.5 * (TORSO_HEIGHT + THIGH_HEIGHT));
    dMassSetBox(&m, 1, THIGH_LENGTH, THIGH_WIDTH, THIGH_HEIGHT);
    dMassAdjust(&m, THIGH_MASS);
    dBodySetMass(body[2], &m);
    box[2] = dCreateBox(0, THIGH_LENGTH, THIGH_WIDTH, THIGH_HEIGHT);
    dGeomSetBody(box[2], body[2]);

    // back left thigh: 3
    body[3] = dBodyCreate(world);
    dBodySetPosition(body[3], 0.5 * (TORSO_LENGTH - THIGH_LENGTH),
            -0.5 * (TORSO_WIDTH - THIGH_WIDTH),
            TORSO_STARTZ - 0.5 * (TORSO_HEIGHT + THIGH_HEIGHT));
    dMassSetBox(&m, 1, THIGH_LENGTH, THIGH_WIDTH, THIGH_HEIGHT);
    dMassAdjust(&m, THIGH_MASS);
    dBodySetMass(body[3], &m);
    box[3] = dCreateBox(0, THIGH_LENGTH, THIGH_WIDTH, THIGH_HEIGHT);
    dGeomSetBody(box[3], body[3]);

    // back right thigh: 4
    body[4] = dBodyCreate(world);
    dBodySetPosition(body[4], 0.5 * (TORSO_LENGTH - THIGH_LENGTH),
            0.5 * (TORSO_WIDTH - THIGH_WIDTH),
            TORSO_STARTZ - 0.5 * (TORSO_HEIGHT + THIGH_HEIGHT));
    dMassSetBox(&m, 1, THIGH_LENGTH, THIGH_WIDTH, THIGH_HEIGHT);
    dMassAdjust(&m, THIGH_MASS);
    dBodySetMass(body[4], &m);
    box[4] = dCreateBox(0, THIGH_LENGTH, THIGH_WIDTH, THIGH_HEIGHT);
    dGeomSetBody(box[4], body[4]);

    // front left calf: 5
    body[5] = dBodyCreate(world);
    dBodySetPosition(body[5], -0.5 * (TORSO_LENGTH - THIGH_LENGTH),
            -0.5 * (TORSO_WIDTH - THIGH_WIDTH),
            TORSO_STARTZ - 0.5 * (TORSO_HEIGHT + CALF_HEIGHT) - THIGH_HEIGHT);
    dMassSetBox(&m, 1, CALF_LENGTH, CALF_WIDTH, CALF_HEIGHT);
    dMassAdjust(&m, CALF_MASS);
    dBodySetMass(body[5], &m);
    box[5] = dCreateBox(0, CALF_LENGTH, CALF_WIDTH, CALF_HEIGHT);
    dGeomSetBody(box[5], body[5]);

    // front right calf: 6
    body[6] = dBodyCreate(world);
    dBodySetPosition(body[6], -0.5 * (TORSO_LENGTH - THIGH_LENGTH),
            0.5 * (TORSO_WIDTH - THIGH_WIDTH),
            TORSO_STARTZ - 0.5 * (TORSO_HEIGHT + CALF_HEIGHT) - THIGH_HEIGHT);
    dMassSetBox(&m, 1, CALF_LENGTH, CALF_WIDTH, CALF_HEIGHT);
    dMassAdjust(&m, CALF_MASS);
    dBodySetMass(body[6], &m);
    box[6] = dCreateBox(0, CALF_LENGTH, CALF_WIDTH, CALF_HEIGHT);
    dGeomSetBody(box[6], body[6]);

    // back left calf: 7
    body[7] = dBodyCreate(world);
    dBodySetPosition(body[7], 0.5 * (TORSO_LENGTH - THIGH_LENGTH),
            -0.5 * (TORSO_WIDTH - THIGH_WIDTH),
            TORSO_STARTZ - 0.5 * (TORSO_HEIGHT + CALF_HEIGHT) - THIGH_HEIGHT);
    dMassSetBox(&m, 1, CALF_LENGTH, CALF_WIDTH, CALF_HEIGHT);
    dMassAdjust(&m, CALF_MASS);
    dBodySetMass(body[7], &m);
    box[7] = dCreateBox(0, CALF_LENGTH, CALF_WIDTH, CALF_HEIGHT);
    dGeomSetBody(box[7], body[7]);

    // back right calf: 8
    body[8] = dBodyCreate(world);
    dBodySetPosition(body[8], 0.5 * (TORSO_LENGTH - THIGH_LENGTH),
            0.5 * (TORSO_WIDTH - THIGH_WIDTH),
            TORSO_STARTZ - 0.5 * (TORSO_HEIGHT + CALF_HEIGHT) - THIGH_HEIGHT);
    dMassSetBox(&m, 1, CALF_LENGTH, CALF_WIDTH, CALF_HEIGHT);
    dMassAdjust(&m, CALF_MASS);
    dBodySetMass(body[8], &m);
    box[8] = dCreateBox(0, CALF_LENGTH, CALF_WIDTH, CALF_HEIGHT);
    dGeomSetBody(box[8], body[8]);

    // ************ hinges
    // front left thigh --> torso: 0
    joint[0] = dJointCreateHinge(world, 0);
    dJointAttach(joint[0], body[0], body[1]);
    dJointSetHingeAnchor(joint[0], -0.5 * (TORSO_LENGTH - THIGH_LENGTH),
            -0.5 * (TORSO_WIDTH - THIGH_WIDTH),
            TORSO_STARTZ - 0.5 * TORSO_HEIGHT);
    dJointSetHingeAxis(joint[0], 0, 1, 0);    // along y axis

    // hinge: 1
    joint[1] = dJointCreateHinge(world, 0);
    dJointAttach(joint[1], body[0], body[2]);
    dJointSetHingeAnchor(joint[1], -0.5 * (TORSO_LENGTH - THIGH_LENGTH),
            +0.5 * (TORSO_WIDTH - THIGH_WIDTH),
            TORSO_STARTZ - 0.5 * TORSO_HEIGHT);
    dJointSetHingeAxis(joint[1], 0, 1, 0);    // along y axis

    // hinge: 2
    joint[2] = dJointCreateHinge(world, 0);
    dJointAttach(joint[2], body[0], body[3]);
    dJointSetHingeAnchor(joint[2], +0.5 * (TORSO_LENGTH - THIGH_LENGTH),
            -0.5 * (TORSO_WIDTH - THIGH_WIDTH),
            TORSO_STARTZ - 0.5 * TORSO_HEIGHT);
    dJointSetHingeAxis(joint[2], 0, 1, 0);    // along y axis

    // hinge: 3
    joint[3] = dJointCreateHinge(world, 0);
    dJointAttach(joint[3], body[0], body[4]);
    dJointSetHingeAnchor(joint[3], 0.5 * (TORSO_LENGTH - THIGH_LENGTH),
            0.5 * (TORSO_WIDTH - THIGH_WIDTH),
            TORSO_STARTZ - 0.5 * TORSO_HEIGHT);
    dJointSetHingeAxis(joint[3], 0, 1, 0);    // along y axis

    // hinge: 4
    joint[4] = dJointCreateHinge(world, 0);
    dJointAttach(joint[4], body[1], body[5]);
    dJointSetHingeAnchor(joint[4], -0.5 * (TORSO_LENGTH - THIGH_LENGTH),
            -0.5 * (TORSO_WIDTH - THIGH_WIDTH),
            TORSO_STARTZ - 0.5 * TORSO_HEIGHT - THIGH_HEIGHT);
    dJointSetHingeAxis(joint[4], 0, 1, 0);

    // hinge: 5
    joint[5] = dJointCreateHinge(world, 0);
    dJointAttach(joint[5], body[2], body[6]);
    dJointSetHingeAnchor(joint[5], -0.5 * (TORSO_LENGTH - THIGH_LENGTH),
            0.5 * (TORSO_WIDTH - THIGH_WIDTH),
            TORSO_STARTZ - 0.5 * TORSO_HEIGHT - THIGH_HEIGHT);
    dJointSetHingeAxis(joint[5], 0, 1, 0);

    // hinge: 6
    joint[6] = dJointCreateHinge(world, 0);
    dJointAttach(joint[6], body[3], body[7]);
    dJointSetHingeAnchor(joint[6], 0.5 * (TORSO_LENGTH - THIGH_LENGTH),
            -0.5 * (TORSO_WIDTH - THIGH_WIDTH),
            TORSO_STARTZ - 0.5 * TORSO_HEIGHT - THIGH_HEIGHT);
    dJointSetHingeAxis(joint[6], 0, 1, 0);

    // hinge: 7
    joint[7] = dJointCreateHinge(world, 0);
    dJointAttach(joint[7], body[4], body[8]);
    dJointSetHingeAnchor(joint[7], 0.5 * (TORSO_LENGTH - THIGH_LENGTH),
            0.5 * (TORSO_WIDTH - THIGH_WIDTH),
            TORSO_STARTZ - 0.5 * TORSO_HEIGHT - THIGH_HEIGHT);
    dJointSetHingeAxis(joint[7], 0, 1, 0);

    // set joint suspension
    for (i = 0; i < 8; i++)
    {
        dJointSetHingeParam(joint[i], dParamSuspensionERP, 0.4);
        dJointSetHingeParam(joint[i], dParamSuspensionCFM, 0.8);
    }

    // create robot space and add it to the top level space
    robot_space = dSimpleSpaceCreate(space);
    dSpaceSetCleanup(robot_space, 0);
    for (i = 0; i < 9; i++)
        dSpaceAdd(robot_space, box[i]);

    // environment
}

void destroyModel()
{
    //cleanup
    for (int i = 0; i < 9; i++)
        dGeomDestroy(box[i]);
    dJointGroupDestroy(contact_group);
    dSpaceDestroy(space);
    dWorldDestroy(world);
}
