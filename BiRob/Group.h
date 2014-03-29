/*
 * Group.h
 *
 *  Created on: Mar 29, 2014
 *      Author: andy
 */

#ifndef GROUP_H_
#define GROUP_H_
#include <gamcs/Avatar.h>
#include <gamcs/CSOSAgent.h>
#include <math.h>
#include "birob_mod.h"

using namespace gamcs;

class Cell: public Avatar
{
    public:
        Cell(int i) :
                Avatar(i), cur_height(-1), cur_angle(-1)
        {
        }
        ~Cell()
        {
        }

    private:
        Agent::State percieveState()
        {
            Agent::State st = 0;

            dVector3 hanchor;
            hingeAnchor(id, hanchor);
            if (id == 0 || id == 1)    // thighs are sensitive to height but not sensitive to angles
            {
                cur_height = round(hanchor[2] * 10);

                dReal ang = hingeAngle(id);
                // convert to degree
                cur_angle = round(ang / 3.14 * 18);
            }
            else    // calf are sensitive to angles but not to height
            {
                cur_height = round(hanchor[2]);

                dReal ang = hingeAngle(id);
                // convert to degree
                cur_angle = round(ang / 3.14 * 180);

            }

            st += cur_height;
            st += cur_angle * pow(100, 1);    // cur_height range: 0~100

            printf("Cell %d, height: %d, angle: %d, state: %" ST_FMT "\n", id,
                    cur_height, cur_angle, st);
            return st;
        }

        void performAction(Agent::Action act)
        {
            moveLimb(id, act);
        }

        OSpace availableActions(Agent::State st)
        {
            OSpace acts;
            acts.add(0, 2, 1);    // 0, 1, 2
            return acts;
        }

        float originalPayoff(Agent::State st)
        {
            float py = 0;
            if (id == 0 || id == 1)    // thighs are more sensitive on height
                py = 0.6 * (cur_height - 60) - 0.4 * abs(cur_angle);
            else
                // calfs are more sensitive on angle
                py = 0.4 * (cur_height - 60) - 0.6 * abs(cur_angle);
            printf("cell %d, opayoff: %f\n", id, py);
            return py;
        }

        int cur_height;
        int cur_angle;
};

class CellGroup
{
    public:
        CellGroup()
        {
            for (int i = 0; i < 4; i++)
            {
                agents[i] = new CSOSAgent(1, 0.95, 0.01);
                cells[i] = new Cell(i);
                cells[i]->connectAgent(agents[i]);
            }
        }

        ~CellGroup()
        {
            for (int i = 0; i < 4; i++)
            {
                delete agents[i];
                delete cells[i];
            }
        }

        void groupStep()
        {
            for (int i = 0; i < 4; i++)
            {
                cells[i]->step();
            }
        }

    private:
        CSOSAgent *agents[4];
        Cell *cells[4];
};

#endif /* GROUP_H_ */
