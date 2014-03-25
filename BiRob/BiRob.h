/*
 * BiRob.h
 *
 *  Created on: Mar 13, 2014
 *      Author: andy
 */

#ifndef BIROB_H_
#define BIROB_H_
#include <gamcs/Avatar.h>
#include "birob_mod.h"

using namespace gamcs;

class BiRob: public Avatar
{
    public:
        BiRob(int i) :
                Avatar(i), cur_height(-1), count(0)
        {
        }
        ~BiRob()
        {
        }

    private:
        Agent::State percieveState()
        {
            const dReal *bpos = bodyPosition(0);    
            cur_height = round(bpos[2]);   // save current torso height

            // state is the combination of hinge angles
            Agent::State st = 0;

            // thighs
            int thigh_angle_num =  thigh_histop - thigh_lostop;

            dReal ang;
            int dang;

            ang = hingeAngle(0);
            // convert to positive degree
            dang = round(ang / 3.14 * 180) - thigh_lostop;
            dang /= 10;     // reduce tenfold
            printf("thigh 0 angle: %d\n", dang);
            st += dang;   

            ang = hingeAngle(1);
            // convert to positive degree
            dang = round(ang / 3.14 * 180) - thigh_lostop;
            dang /= 10;     // reduce tenfold
            printf("thigh 1 angle: %d\n", dang);
            st += dang * pow(thigh_angle_num / 10, 1);   

            // calfs
            int calf_angle_num = calf_histop - calf_lostop;
            
            ang = hingeAngle(2);
            // convert to positive degree
            dang = round(ang / 3.14 * 180) - calf_lostop;
            dang /= 10;     // reduce tenfold
            printf("calf 2 angle: %d\n", dang);
            st += dang * pow(thigh_angle_num / 10, 2);   // yeah, it's thigh_angle_num

            ang = hingeAngle(3);
            // convert to positive degree
            dang = round(ang / 3.14 * 180) - calf_lostop;
            dang /= 10;     // reduce tenfold
            printf("calf 3 angle: %d\n", dang);
            st += dang * pow(calf_angle_num / 10, 3);   

            // plus torso height as the 4th dim
            st += cur_height * pow(calf_angle_num / 10, 4);  // yeah, it's calf_angle_num

            printf("Current State: %" ST_FMT "\n", st);
            return st;
        }

        void performAction(Agent::Action act)
        {
            printf("act: %" ACT_FMT "\n", act);
            int act_hinge;
            for (int i = 0; i < 4; i++)    // totally 4 hinges
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
                acts.add(0, 80, 1);
                return acts;
            }
            else
                return acts;
        }

        float originalPayoff(Agent::State st)
        {
            float fy = (float) (cur_height * 10 - 65);  // height time 10
            printf("height: %d, payoff: %f\n", cur_height, fy);
            return fy;
        }

    private:
        int cur_height;
        int count;
};

#endif /* BIROB_H_ */
