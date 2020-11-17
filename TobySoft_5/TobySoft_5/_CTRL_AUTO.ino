int middleDistance, leftDistance, rightDistance;

#define autoPilotDecisionInterval 40000
unsigned long lastPilotDecision = 0; // micros() timestamp of last autoPilotDecision

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ///////////////                                                                                     //
// END GLOBAL VARS                                                                                     //
// ///////////////                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Auto-Pilot functions                                                                                //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void autoControl()
{
    // check RF switch for AutoPilot toggle
    if ((dataFromTransmitter[2] == 0) && (micros() - lastAutoPilotToggle > toggleDebounce))
    {
        toggleAutoPilot();
        return;
    }

    if ((micros() - lastPilotDecision > autoPilotDecisionInterval))
    {
        switch (BLOCKED_DRIVE_COUNT)
        {
        case 0: // Try forward
            driveMotors(1);
            break;
        case 1: // Try right
            driveMotors(3);
            break;
        case 2: // Try left
            driveMotors(2);
            break;
        case 3: // Try reverse
            driveMotors(-1);
            break;
        case 4: // reseting count for now... maybe cleaner sensor data arrives on next loop?
            driveMotors(0);
            BLOCKED_DRIVE_COUNT = 0;
            // TODO: consider limiting resets with a counter
            break;
        default:
            // TODO: robo sleep. no valid paths.
            break;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Toggle function                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void toggleAutoPilot()
{
    playAutoToggleSound();
    lastAutoPilotToggle = micros();

    if (AUTOPILOT_ON)
    {
        driveMotors(0); // stop
        displayLogo();
        AUTOPILOT_ON = false;
        return;
    }
    else
    {
        driveMotors(0); // stop
        displayUltrasonicHeader();
        AUTOPILOT_ON = true;
        return;
    }
}
