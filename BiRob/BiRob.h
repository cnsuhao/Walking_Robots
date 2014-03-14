/*
 * BiRob.h
 *
 *  Created on: Mar 13, 2014
 *      Author: andy
 */

#ifndef BIROB_H_
#define BIROB_H_
#include <gimcs/CSThreadAvatar.h>
#include "birob_mod.h"

using namespace gimcs;

class BiRob: public CSThreadAvatar
{
    public:
        BiRob(std::string n) :
                CSThreadAvatar(n), cur_height(-1), count(0)
        {
        }
        ~BiRob()
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
            // decode act to corresponding hinges
            printf("act: %" ACT_FMT ": ", act);
            int act_hinge;
            for (int i = 0; i < 4; i++)    // totally 4 hinges
            {
                act_hinge = act % 3;

                if (i == 0)    // Right Calf
                {
                    switch (act_hinge)
                    {
                        case 0:
                            printf(", backRC");
                            backRightCalf();
                            break;
                        case 1:
                            printf(", holdRC");
                            holdRightCalf();
                            break;
                        case 2:
                            printf(", forthRC");
                            forthRightCalf();
                            break;
                    }
                }
                else if (i == 1)    // Left Calf
                {
                    switch (act_hinge)
                    {
                        case 0:
                            printf(", backLC");
                            backLeftCalf();
                            break;
                        case 1:
                            printf(", holdLC");
                            holdLeftCalf();
                            break;
                        case 2:
                            printf(", forthLC");
                            forthLeftCalf();
                            break;
                    }
                }
                else if (i == 2)    // Right Thigh
                {
                    switch (act_hinge)
                    {
                        case 0:
                            printf(", backRT");
                            backRightThigh();
                            break;
                        case 1:
                            printf(", holdRT");
                            holdRightThigh();
                            break;
                        case 2:
                            printf(", forthRT");
                            forthRightThigh();
                            break;
                    }
                }
                else    // Left Thigh
                {
                    switch (act_hinge)
                    {
                        case 0:
                            printf(", backLT");
                            backLeftThigh();
                            break;
                        case 1:
                            printf(", holdLT");
                            holdLeftThigh();
                            break;
                        case 2:
                            printf(", forthLT");
                            forthLeftThigh();
                            break;
                    }
                }

                act = act / 3;    // rest of act
            }
        }

        OSpace actionCandidates(Agent::State st)
        {
            OSpace acts;
            if (count++ < 1000000)
            {
                acts.add(0, 80, 1);
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

#endif /* BIROB_H_ */
