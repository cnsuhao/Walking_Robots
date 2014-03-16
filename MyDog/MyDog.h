/*
 * MyDog.h
 *
 *  Created on: Mar 14, 2014
 *      Author: andy
 */

#ifndef MYDOG_H_
#define MYDOG_H_
#include <math.h>
#include <gamcs/CSThreadAvatar.h>
#include "mydog_mod.h"

using namespace gamcs;

class MyDog: public CSThreadAvatar
{
    public:
        MyDog(std::string n) :
            CSThreadAvatar(n), cur_height(-1), count(0)
    {
    }
        ~MyDog()
        {
        }

    private:
        Agent::State percieveState()
        {
            const dReal *bpos = bodyPosition(0);    
            cur_height = round(bpos[2] * 10);   // save current torso height

            // state is the combination of hinge angles
            Agent::State st = 0;
            // thigs: 90' ~ -60'
            for (int i=0; i<4; i++)
            {
                dReal ang = hingeAngle(i);
                // convert to positive degree
                int dang = round(ang / 3.14 * 180) + 65;
                printf("thigh %d angle: %d\n", i, dang - 65);

                st += dang * pow(150, i);   // 150 
            }

            // calfs: -90'~0'
            for (int i=4; i<8; i++)
            {
                dReal ang = hingeAngle(i);
                // convert to positive degree
                int dang = round(ang / 3.14 * 180) + 95;
                printf("calf %d angle: %d\n", i, dang - 95);

                st += dang * pow(90, i);   // 90 
            }

            printf("Current State: %" ST_FMT "\n", st);
            return st;
        }

        void performAction(Agent::Action act)
        {
            printf("act: %" ACT_FMT "\n", act);
            int act_hinge;
            for (int i = 0; i < 8; i++)    // totally 8 hinges
            {
                act_hinge = act % 3;

                moveLimb(i, act_hinge);

                act = act / 3;
            }
        }

        OSpace availableActions(Agent::State st)
        {
            OSpace acts;
            if (count++ < 1000000)
            {
                acts.add(0, 6560, 1);
                return acts;
            }
            else
                return acts;
        }

        float originalPayoff(Agent::State st)
        {
            float fy = (float) (cur_height - 100);
            printf("height: %d, payoff: %f\n", cur_height, fy);
            return fy;
        }


    private:
        int cur_height;
        int count;
};       

#endif /* MYDOG_H_ */
