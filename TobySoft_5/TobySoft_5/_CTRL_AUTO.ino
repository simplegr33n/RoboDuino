int middleDistance, leftDistance, rightDistance;

#define autoPilotDecisionInterval 40000
unsigned long lastPilotDecision = 0; // micros() timestamp of last autoPilotDecision

int evadeTryCount = 0;
int evadeLoopCount = 0;

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
    if ((dataFromTransmitter[2] == 1) && (micros() - lastAutoPilotToggle > toggleDebounce))
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
        case 99: // Give up
            stopDrive(0);
            playSleepSound();
            break;
        default: // Try evade
            tryEvade();
            break;
        }
    }
}

void tryEvade()
{
    switch (evadeTryCount)
    {
    case 0: // Try a 180 to the left
        leftDegrees(180);
        break;
    case 1: // Try a 180 to the right
        rightDegrees(180);
        break;
    case 2: // Try left Clearing Search
        findClearLeft();
        break;
    case 3: // Try right Clearing Search
        findClearRight();
        break;
    case 4: // Try backing to the left
        backLeftEvade();
        break;
    case 5: // Try backing to the right
        backRightEvade();
        break;
    default:
        evadeLoopCount++;
        if (evadeLoopCount > 99)
        {
            BLOCKED_DRIVE_COUNT = 99;
        }
        break;
    }

    evadeTryCount++;
    if (evadeTryCount > 5) // reset BLOCKED_DRIVE_COUNT until evade tries exhausted
    {
        evadeTryCount = 0;
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
        cancelAdvancedFunctions();
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
