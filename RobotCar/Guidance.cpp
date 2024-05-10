#include "Guidance.h"

CGuidance::CGuidance()
{
    target_ID = START_MARKER;
    distance.push_back(1000.0);
    phase = 0;
    firing = false;
    right_pwm = PWM_RIGHT;
    elapsed_time = 0;
    automatic = false;
}

CGuidance::~CGuidance()
{
    // nothing for now
}

void CGuidance::update()
{
        // Phase 0 : Drive foward to first target
        if(automatic)
        {
        if(phase == 0)
        {
            std::cout << "Entered phase 0" << std::endl;
            target_ID = START_MARKER;

            auto_key = "forward";
            if(ids.size() > 0)
            {
                if(ids.at(0) == target_ID && distance.at(0) < 61.0)
                {
                    auto_key = "stop";
                    //keep_center();
                    target_ID = TARGET_1;
                    distance.at(0) = 1000.0;
                    right_pwm = PWM_RIGHT;
                    phase += 1;
                } else if(ids.at(0) == target_ID)
                {
                    //keep_center();
                    auto_key = "forward";
                }
            }
            else if(distance.at(0) > 61.0)
            {
                // keep_center();
                if(ids.size() == 0)
                auto_key = "forward";
            }
        }
        // Phase 1 : Rotate servo until first target is identified
        else if (phase == 1)
        {
            std::cout << "Entered phase 1" << std::endl;

            if(ids.size() > 0)
            {
                std::cout << "id.at(0) = " << ids.at(0) << std::endl;
                if(ids.at(0) == TARGET_1)
                {
                    auto_key = "stop";
                    elapsed_time = cv::getTickCount();
                    phase += 1;
                }
                else if(ids.at(0) == START_MARKER)
                {
                    auto_key = "rotate left";
                }
            } else
            {
                std::cout << "Entered phase 1" << std::endl;
                auto_key = "rotate left";
            }
        }
        // Phase 2 : Center marker on the screen and fire
        else if (phase == 2)
        {
            std::cout << "Entered phase 2" << std::endl;
            if(pixels_off.size() > 0)
                std::cout << "Entered phase 2, pixels_off = " << pixels_off.at(0) << std::endl;

            if(abs(pixels_off.at(0)) < 30 || ((cv::getTickCount() - elapsed_time)/cv::getTickFrequency() > 0.8))
            {
                std::cout << "Firing" << std::endl;
                auto_key = "fire";
                phase += 1;
                target_ID = TARGET_2;
            }
            else if(pixels_off.at(0) >= 30)
            {
                auto_key == "rotate left";
            }
            else if(pixels_off.at(0) <= -30)
            {
                auto_key == "rotate right";
            }
        }
        // Phase 3 : Do a jittery right turn until the second target is seen
        else if(phase == 3)
        {
            std::cout << "Entered phase 3" << std::endl;
            target_ID = TARGET_2;

            std::cout << "elapsed time = " << (cv::getTickCount() - elapsed_time)/cv::getTickFrequency() << std::endl;
            if((cv::getTickCount() - elapsed_time)/cv::getTickFrequency() > TURN_DELAY)
            {
                std::cout << "Reset elapsed time" << std::endl;
                elapsed_time = cv::getTickCount();
            }

            if((cv::getTickCount() - elapsed_time)/cv::getTickFrequency() < 0.15)
                auto_key = "right";
            else
                auto_key = "stop";

            if(ids.size() > 0)
            {
                if(ids.size() > 1 && pixels_off.size() > 1)
                {
                    if((ids.at(0) == TARGET_2) && (pixels_off.at(0) < 30))
                    {
                        auto_key = "stop";
                        distance.at(0) = 1000.0;
                        right_pwm = PWM_RIGHT;
                        phase += 1;
                    }
                    else if((ids.at(1) == TARGET_2) && (pixels_off.at(1) < 30))
                    {
                        auto_key = "stop";
                        distance.at(0) = 1000.0;
                        right_pwm = PWM_RIGHT;
                        phase += 1;
                    }
                    else if((ids.at(0) == GUIDE_MARKER_1) && (pixels_off.at(0) < 120))
                    {
                        auto_key = "stop";
                        distance.at(0) = 1000.0;
                        right_pwm = PWM_RIGHT;
                        phase += 1;
                    }
                    else if((ids.at(1) == GUIDE_MARKER_1) && (pixels_off.at(1) < 120))
                    {
                        auto_key = "stop";
                        distance.at(0) = 1000.0;
                        right_pwm = PWM_RIGHT;
                        phase += 1;
                    }
                    else if((ids.at(1) == OBSTACTLE_1) || (ids.at(0) == OBSTACTLE_1))
                    {
                        auto_key = "stop";
                        distance.at(0) = 1000.0;
                        right_pwm = PWM_RIGHT;
                        phase += 1;
                    }

                }
                else if((ids.at(0) == target_ID) && (pixels_off.at(0) < 30))
                {
                    auto_key = "stop";
                    distance.at(0) = 1000.0;
                    right_pwm = PWM_RIGHT;
                    phase += 1;
                }
                else if((ids.at(0) == GUIDE_MARKER_1) && (pixels_off.at(0) < 120))
                {
                    auto_key = "stop";
                    distance.at(0) = 1000.0;
                    right_pwm = PWM_RIGHT - 20;
                    phase += 1;
                }
                else if(ids.at(0) == OBSTACTLE_1)
                {
                    auto_key = "stop";
                    distance.at(0) = 1000.0;
                    right_pwm = PWM_RIGHT;
                    phase += 1;
                }
            }
        }
        // Phase 4 : Home in on Target 2
        else if (phase == 4)
        {
            std::cout << "Entered phase 4" << std::endl;
            target_ID = TARGET_2;

            auto_key = "forward";
            if(ids.size() > 0)
            {
                if(ids.size() > 1)
                {
                    for(size_t i = 0; i < ids.size(); i++)
                    {
                        if(ids.at(0) == target_ID)
                        {
                            keep_center(i);
                            auto_key = "forward";
                        }
                    }
                }
                else
                {
                    if(ids.at(0) == target_ID && distance.at(0) < 33.0)
                    {
                        auto_key = "stop";
                        distance.at(0) = 1000.0;
                        right_pwm = PWM_RIGHT;
                        phase += 1;
                    }
                    else if(ids.at(0) == target_ID)
                    {
                        if(abs(pixels_off.at(0)) > 50)
                        {
                            keep_center(0);
                            auto_key = "forward";
                        }
                    }
                    else if(ids.at(0) == GUIDE_MARKER_1 && distance.at(0) < 55.0)
                    {
                        auto_key = "stop";
                        distance.at(0) = 1000.0;
                        right_pwm = PWM_RIGHT;
                        phase += 1;
                    }
                }
            }
            else
            {
                auto_key = "forward";
            }
        }
        // Phase 5 : Aim and fire at target 2
        else if (phase == 5)
        {
            std::cout << "Entered phase 5" << std::endl;

            std::cout << "elapsed time = " << (cv::getTickCount() - elapsed_time)/cv::getTickFrequency() << std::endl;
            if((cv::getTickCount() - elapsed_time)/cv::getTickFrequency() > 1.5)
            {
                std::cout << "Reset elapsed time" << std::endl;
                elapsed_time = cv::getTickCount();
            }

            if(ids.size() > 0)
            {
                if(pixels_off.size() > 0)
                std::cout << "Phase 5, pixels_off = " << pixels_off.at(0) << std::endl;

                if(abs(pixels_off.at(0)) < 50 && ids.at(0) == target_ID)
                {
                    std::cout << "Firing" << std::endl;
                    auto_key = "fire";
                    phase += 1;
                    target_ID = TARGET_3;
                }
                else if((cv::getTickCount() - elapsed_time)/cv::getTickFrequency() > AIM_DELAY)
                {
                    auto_key = "rotate left";
                }
            }
            else if((cv::getTickCount() - elapsed_time)/cv::getTickFrequency() > AIM_DELAY)
            {
                auto_key = "rotate left";
            }

        }
        // Phase 6 : Do a jittery right turn until the third target is seen
        else if (phase == 6)
        {
            std::cout << "Entered phase 6" << std::endl;
            auto_key = "stop";

            target_ID = TARGET_3;

            std::cout << "elapsed time = " << (cv::getTickCount() - elapsed_time)/cv::getTickFrequency() << std::endl;
            if((cv::getTickCount() - elapsed_time)/cv::getTickFrequency() > TURN_DELAY)
            {
                std::cout << "Reset elapsed time" << std::endl;
                elapsed_time = cv::getTickCount();
            }

            if((cv::getTickCount() - elapsed_time)/cv::getTickFrequency() < 0.15)
                auto_key = "right";
            else
                auto_key = "stop";

            if(ids.size() > 0)
            {
                if(ids.size() > 1 && pixels_off.size() > 1)
                {
                    if((ids.at(0) == TARGET_3) && (pixels_off.at(0) < -30))
                    {
                        auto_key = "stop";
                        distance.at(0) = 1000.0;
                        right_pwm = PWM_RIGHT;
                        phase += 1;
                    }
                    else if((ids.at(1) == TARGET_3) && (pixels_off.at(1) < -30))
                    {
                        auto_key = "stop";
                        distance.at(0) = 1000.0;
                        right_pwm = PWM_RIGHT;
                        phase += 1;
                    }
                }
                else if((ids.at(0) == target_ID) && (pixels_off.at(0) < -30))
                {
                    auto_key = "stop";
                    distance.at(0) = 1000.0;
                    right_pwm = PWM_RIGHT;
                    phase += 1;
                }
            }
        }
        // Phase 7 : Home in on Target 3
        else if(phase == 7)
        {
            std::cout << "Entered phase 7" << std::endl;
            target_ID = TARGET_3;

            auto_key = "forward";
            if(ids.size() > 0)
            {
                if(ids.at(0) == target_ID && distance.at(0) < 33)
                {
                    auto_key = "stop";
                    distance.at(0) = 1000.0;
                    right_pwm = PWM_RIGHT;
                    phase += 1;
                }
                else if(ids.at(0) == END_MARKER && distance.at(0) < 65.0)
                    {
                        auto_key = "stop";
                        distance.at(0) = 1000.0;
                        right_pwm = PWM_RIGHT;
                        phase += 1;
                    }
                else if(ids.at(0) == target_ID)
                {
                    if(abs(pixels_off.at(0)) > 50)
                    {
                        keep_center(0);
                        auto_key = "forward";
                    }
                }
            }
            else
            {
                auto_key = "forward";
            }
        }
        // Phase 8 : Aim and fire at target 3
        else if(phase == 8)
        {
            std::cout << "Entered phase 8" << std::endl;

            std::cout << "elapsed time = " << (cv::getTickCount() - elapsed_time)/cv::getTickFrequency() << std::endl;
            if((cv::getTickCount() - elapsed_time)/cv::getTickFrequency() > 1.5)
            {
                std::cout << "Reset elapsed time" << std::endl;
                elapsed_time = cv::getTickCount();
            }

            if(pixels_off.size() > 0)
                std::cout << "Phase 8, pixels_off = " << pixels_off.at(0) << std::endl;
            /*
            if(abs(pixels_off.at(0)) < 50)
            {
                std::cout << "Firing" << std::endl;
                auto_key = "fire";
                phase += 1;
                target_ID = TARGET_4;
            }
            else if(pixels_off.at(0) >= 50)
            {
                auto_key == "rotate left";
            }
            else if(pixels_off.at(0) <= -50)
            {
                auto_key == "rotate right";
            }
            */
            if(ids.size() > 0)
            {
                if(pixels_off.size() > 0)
                std::cout << "Phase 5, pixels_off = " << pixels_off.at(0) << std::endl;

                if(abs(pixels_off.at(0)) < 50 && ids.at(0) == target_ID)
                {
                    std::cout << "Firing" << std::endl;
                    auto_key = "fire";
                    phase += 1;
                    target_ID = TARGET_4;
                }
                else if((cv::getTickCount() - elapsed_time)/cv::getTickFrequency() > AIM_DELAY)
                {
                    auto_key = "rotate left";
                }
            }
            else if((cv::getTickCount() - elapsed_time)/cv::getTickFrequency() > AIM_DELAY)
            {
                auto_key = "rotate left";
            }
        }
        // Phase 9 : Do a jittery right turn until the fourth target is seen
        else if(phase == 9)
        {
            std::cout << "Entered phase 9" << std::endl;
            auto_key = "stop";

            target_ID = TARGET_4;

            std::cout << "elapsed time = " << (cv::getTickCount() - elapsed_time)/cv::getTickFrequency() << std::endl;
            if((cv::getTickCount() - elapsed_time)/cv::getTickFrequency() > TURN_DELAY)
            {
                std::cout << "Reset elapsed time" << std::endl;
                elapsed_time = cv::getTickCount();
            }

            if((cv::getTickCount() - elapsed_time)/cv::getTickFrequency() < 0.15)
                auto_key = "right";
            else
                auto_key = "stop";

            if(ids.size() > 0)
            {
                if(ids.size() > 1 && pixels_off.size() > 1)
                {
                    if((ids.at(0) == TARGET_4) && (pixels_off.at(0) < 0))
                    {
                        auto_key = "stop";
                        distance.at(0) = 1000.0;
                        right_pwm = PWM_RIGHT;
                        phase += 1;
                    }
                    else if((ids.at(1) == TARGET_4) && (pixels_off.at(1) < 0))
                    {
                        auto_key = "stop";
                        distance.at(0) = 1000.0;
                        right_pwm = PWM_RIGHT;
                        phase += 1;
                    }
                }
                else if((ids.at(0) == target_ID) && (pixels_off.at(0) < 30))
                {
                    auto_key = "stop";
                    distance.at(0) = 1000.0;
                    right_pwm = PWM_RIGHT;
                    phase += 1;
                }
            }
        }
        // Phase 10 : Drive forward to target 4
        else if(phase == 10)
        {
            std::cout << "Entered phase 10" << std::endl;
            target_ID = TARGET_4;

            auto_key = "forward";
            if(ids.size() > 0)
            {
                if(ids.at(0) == target_ID && distance.at(0) < 33)
                {
                    auto_key = "stop";
                    distance.at(0) = 1000.0;
                    right_pwm = PWM_RIGHT;
                    phase += 1;
                }
                else if(ids.at(0) == GUIDE_MARKER_2 && distance.at(0) < 55.0)
                    {
                        auto_key = "stop";
                        distance.at(0) = 1000.0;
                        right_pwm = PWM_RIGHT;
                        phase += 1;
                    }
                else if(ids.at(0) == target_ID)
                {
                    if(abs(pixels_off.at(0)) > 50)
                    {
                        keep_center(0);
                        auto_key = "forward";
                    }
                }
            }
            else
            {
                auto_key = "forward";
            }
        }
        // Phase 11 : Aim and fire at target 4
        else if(phase == 11)
        {
            std::cout << "Entered phase 11" << std::endl;

            std::cout << "elapsed time = " << (cv::getTickCount() - elapsed_time)/cv::getTickFrequency() << std::endl;
            if((cv::getTickCount() - elapsed_time)/cv::getTickFrequency() > 1.5)
            {
                std::cout << "Reset elapsed time" << std::endl;
                elapsed_time = cv::getTickCount();
            }

            if(ids.size() > 0)
            {
                if(pixels_off.size() > 0)
                std::cout << "Phase 11, pixels_off = " << pixels_off.at(0) << std::endl;

                if(abs(pixels_off.at(0)) < 50 && ids.at(0) == target_ID)
                {
                    std::cout << "Firing" << std::endl;
                    auto_key = "fire";
                    phase += 1;
                    target_ID = END_MARKER;
                }
                else if((cv::getTickCount() - elapsed_time)/cv::getTickFrequency() > AIM_DELAY)
                {
                    auto_key = "rotate left";
                }
            }
            else if((cv::getTickCount() - elapsed_time)/cv::getTickFrequency() > AIM_DELAY)
            {
                auto_key = "rotate left";
            }
        }
        // Phase 12 : Drive backwards until certain distance is reached
        else if(phase == 12)
        {
            std::cout << "Entered phase 12" << std::endl;
            auto_key = "reverse";
            right_pwm = PWM_RIGHT - 10;

            target_ID = TARGET_4;

            if(ids.size() > 0)
            {
                if(ids.at(0) == target_ID && distance.at(0) > 70.0)
                {
                    auto_key = "stop";
                    std::cout << "Target marker stop" << std::endl;
                    distance.at(0) = 1000.0;
                    right_pwm = PWM_RIGHT;
                    phase += 1;
                }
                else if(ids.at(0) == GUIDE_MARKER_2 && distance.at(0) > 105.0)
                    {
                        std::cout << "Guide marker stop" << std::endl;
                        auto_key = "stop";
                        distance.at(0) = 1000.0;
                        right_pwm = PWM_RIGHT;
                        phase += 1;
                    }
                else if(ids.at(0) == target_ID)
                {
                    if(abs(pixels_off.at(0)) > 50)
                    {
                        //keep_center(0);
                        auto_key = "reverse";
                    }
                }
            }
            else
            {
                auto_key = "reverse";
            }
        }
        // Phase 13 : Align with the END marker
        else if(phase == 13)
        {
            std::cout << "Entered phase 13" << std::endl;
            auto_key = "stop";

            target_ID = END_MARKER;

            std::cout << "elapsed time = " << (cv::getTickCount() - elapsed_time)/cv::getTickFrequency() << std::endl;
            if((cv::getTickCount() - elapsed_time)/cv::getTickFrequency() > TURN_DELAY)
            {
                std::cout << "Reset elapsed time" << std::endl;
                elapsed_time = cv::getTickCount();
            }

            if((cv::getTickCount() - elapsed_time)/cv::getTickFrequency() < 0.05)
                auto_key = "left_reverse";
            else
                auto_key = "stop";

            if(ids.size() > 0)
            {
                if(ids.size() > 1 && pixels_off.size() > 1)
                {
                    if((ids.at(0) == target_ID) && (pixels_off.at(0) > 60))
                    {
                        auto_key = "stop";
                        distance.at(0) = 1000.0;
                        right_pwm = PWM_RIGHT;
                        phase += 1;
                    }
                    else if((ids.at(1) == target_ID) && (pixels_off.at(1) > 60))
                    {
                        auto_key = "stop";
                        distance.at(0) = 1000.0;
                        right_pwm = PWM_RIGHT;
                        phase += 1;
                    }
                }
                else if((ids.at(0) == target_ID) && (pixels_off.at(0) > 60))
                {
                    auto_key = "stop";
                    distance.at(0) = 1000.0;
                    right_pwm = PWM_RIGHT;
                    phase += 1;
                }
            }
        }
        // Phase 14 : Drive to the finish
        else if(phase == 14)
        {
            std::cout << "Entered phase 14" << std::endl;
            target_ID = END_MARKER;
            right_pwm = PWM_RIGHT - 40;

            auto_key = "forward";
            if(ids.size() > 0)
            {
                if(ids.at(0) == target_ID && distance.at(0) < 20.0)
                {
                    auto_key = "stop";
                    distance.at(0) = 1000.0;
                    right_pwm = PWM_RIGHT;
                    phase += 1;
                }
                else if(ids.at(0) == GUIDE_MARKER_2 && distance.at(0) < 20.0)
                    {
                        auto_key = "stop";
                        distance.at(0) = 1000.0;
                        right_pwm = PWM_RIGHT;
                        phase += 1;
                    }
                else if(ids.at(0) == target_ID)
                {
                    if(abs(pixels_off.at(0)) > 50)
                    {
                        keep_center(0);
                        auto_key = "forward";
                    }
                }
            }
            else
            {
                auto_key = "forward";
            }
        }
        else
        {
            select_action();
        }


        }
}

void CGuidance::keep_center(int ID_index)
{
    std::cout << "Entered keep_center()" << std::endl;
    std::cout << "pixels off = " << pixels_off.at(0) << std::endl;
    if(pixels_off.at(ID_index) < -TOLERANCE && ((cv::getTickCount() - elapsed_time)/cv::getTickFrequency() > 0.2))
    {
        if(right_pwm < 250)
        {
            std::cout<< "Increased right_pwm" << std::endl;
            elapsed_time = cv::getTickCount();
            right_pwm += 5;
        }
    }
    else if(pixels_off.at(ID_index) > TOLERANCE && ((cv::getTickCount() - elapsed_time)/cv::getTickFrequency() > 0.2))
    {
        if(right_pwm > 100)
        {
            std::cout << "Decreased right_pwm" << std::endl;
            elapsed_time = cv::getTickCount();
            right_pwm -= 5;
        }

    }
}

void CGuidance::select_action()
{
    std::cout << std::endl << "Select the next test :";
    std::cin >> phase;
    std::cout << std::endl;
}
