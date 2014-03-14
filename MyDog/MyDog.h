/*
 * MyDog.h
 *
 *  Created on: Mar 14, 2014
 *      Author: andy
 */

#ifndef MYDOG_H_
#define MYDOG_H_
#include <gimcs/CSThreadAvatar.h>
#include "mydog_mod.h"

using namespace gimcs;

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
            int height = heightSensor();
            cur_height = height;    // save current height
            printf("Height: %d\n", height);
            return height;
        }

        void performAction(Agent::Action act)
        {

            printf("act: %" ACT_FMT ": ", act);
            int act_hinge;
            for (int i = 0; i < 8; i++)    // totally 8 hinges
            {
                act_hinge = act % 3;

                moveLimb(i, act_hinge);

                act = act / 3;
            }
        }

        OSpace actionCandidates(Agent::State st)
        {
            OSpace acts;
            if (count++ < 1000000)
            {
                acts.add(0, 3280, 1);
                return acts;
            }
            else
                return acts;
        }

        float originalPayoff(Agent::State st)
        {
            float fy = (float) (cur_height - 100);
            printf("payoff: %f\n", fy);
            return fy;
        }


    private:
        int cur_height;
        int count;
};       

#endif /* MYDOG_H_ */
