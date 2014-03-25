/*
 * MyDog.h
 *
 *  Created on: Mar 14, 2014
 *      Author: andy
 */

#ifndef MYDOG_H_
#define MYDOG_H_
#include <math.h>
#include <gamcs/Avatar.h>
#include "mydog_mod.h"

using namespace gamcs;

class MyDog: public Avatar
{
    public:
        MyDog(int i) :
            Avatar(i), cur_x(-1), cur_y(-1), cur_z(-1), pre_x(-1), pre_y(-1), count(0)
    {
    }
        ~MyDog()
        {
        }

    private:
        Agent::State percieveState()
        {
            const dReal *bpos = bodyPosition(0);    
            // save position
            pre_x = cur_x;
            pre_y = cur_y;
            // get current position
            cur_x = round(bpos[0]); 
            cur_y = round(bpos[1]);
            cur_z = round(bpos[2]);   

            // state is the combination of hinge angles
            Agent::State st = 0;

            // state is the combination of hinge angles
            // thighs
            int thigh_angle_num = thigh_histop - thigh_lostop;
            dReal ang;
            int dang;

            ang = hingeAngle(0);
            // convert to positive degree
            dang = round(ang / 3.14 * 180) - thigh_lostop;
            dang /= 10;
            printf("thigh 0 angle: %d\n", dang);
            st += dang;

            ang = hingeAngle(1);
            // convert to positive degree
            dang = round(ang / 3.14 * 180) - thigh_lostop;
            dang /= 10;
            printf("thigh 1 angle: %d\n", dang);
            st += dang * pow(thigh_angle_num / 10, 1);

            ang = hingeAngle(2);
            // convert to positive degree
            dang = round(ang / 3.14 * 180) - thigh_lostop;
            dang /= 10;
            printf("thigh 2 angle: %d\n", dang);
            st += dang * pow(thigh_angle_num / 10, 2);

            ang = hingeAngle(3);
            // convert to positive degree
            dang = round(ang / 3.14 * 180) - thigh_lostop;
            dang /= 10;
            printf("thigh 3 angle: %d\n", dang);
            st += dang * pow(thigh_angle_num / 10, 3);

            // calfs
            int calf_angle_num = calf_histop - calf_lostop;

            ang = hingeAngle(4);
            // convert to positive degree
            dang = round(ang / 3.14 * 180) - calf_lostop;
            dang /= 10;     // reduce tenfold
            printf("calf 4 angle: %d\n", dang);
            st += dang * pow(thigh_angle_num / 10, 4);   // yeah, it's thigh_angle_num

            ang = hingeAngle(5);
            // convert to positive degree
            dang = round(ang / 3.14 * 180) - calf_lostop;
            dang /= 10;     // reduce tenfold
            printf("calf 5 angle: %d\n", dang);
            st += dang * pow(calf_angle_num / 10, 5);   

            ang = hingeAngle(6);
            // convert to positive degree
            dang = round(ang / 3.14 * 180) - calf_lostop;
            dang /= 10;     // reduce tenfold
            printf("calf 6 angle: %d\n", dang);
            st += dang * pow(calf_angle_num / 10, 6);   

            ang = hingeAngle(7);
            // convert to positive degree
            dang = round(ang / 3.14 * 180) - calf_lostop;
            dang /= 10;     // reduce tenfold
            printf("calf 7 angle: %d\n", dang);
            st += dang * pow(calf_angle_num / 10, 7);   

            // plus torso height as the 4th dim
            //st += cur_z * pow(calf_angle_num / 10, 8);  // yeah, it's calf_angle_num

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

        // let my dog run!
        float originalPayoff(Agent::State st)
        {
            //float fy = (float) (cur_z * 10 - 70);  // height times 10
            //printf("height: %d, payoff: %f\n", cur_z, fy);
            //return fy;

            // get forwad direction
            const dReal *fltpos = bodyPosition(1);  // forth left thigh
            const dReal *bltpos = bodyPosition(5);  // back left thigh
            float fx = fltpos[0] - bltpos[0];
            float fy = fltpos[1] - bltpos[1];

            float delta_x = cur_x - pre_x;
            float delta_y = cur_y - pre_y;

            float dot_product = fx * delta_x + fy * delta_y;

            float fnorm = sqrtf(fx*fx + fy*fy);

            float speed = dot_product / fnorm;

            printf("speed: %f, payoff: %f\n", speed, speed - 10);
            return speed - 10;
        }


    private:
        int cur_x;
        int cur_y;
        int cur_z;
        int pre_x;
        int pre_y;
        int count;
};       

#endif /* MYDOG_H_ */
