#define autoPilotDecisionInterval 100 // millis
unsigned long lastPilotDecision = 0;  // micros() timestamp of last autoPilotDecision

int autoPilotSpeed = 75; // 0-255, constant speed in autoPilot for now
int autoPilotTurnSpeed = 160;
#define halfTurn160 1250 // millis duration for a 180 degree turn at 160 speed

int DRIVE_PATH;
unsigned long DRIVE_PATH_START;
float DRIVE_PATH_PROPORTION; // proportion of a full advance/reverse unit or portion of a 180degree turn

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ///////////////                                                                                     //
// END GLOBAL VARS                                                                                     //
// ///////////////                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Auto-Pilot functions                                                                                //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void autoControl()
{
    // check RF switch for AutoPilot toggle
    if ((dataFromTransmitter[2] == 1) && (millis() - lastAutoPilotToggle > toggleDebounce))
    {
        toggleAutoPilot();
        return;
    }

    if ((millis() - lastPilotDecision > autoPilotDecisionInterval))
    {
        getDistances(); // A re-call (within main loop) to getDistances before acting, for more precise read of world state

        if (DRIVE_PATH == 0)
        {
            autoChoosePath(); // Choose your fate, robot.
        }
        else
        {
            autoPursuePath();
        }
    }
}

void autoChoosePath()
{
    stopDrive(0);
    DRIVE_PATH = random(1, 11);
    DRIVE_PATH_PROPORTION = (float)random(1, 10) / 10;
}

void autoPursuePath()
{
    if (DRIVE_PATH < 6)
    {
        // try forward
        autoDriveForward();
    }
    else if (DRIVE_PATH == 6 || DRIVE_PATH == 8)
    {
        // try right
        autoDriveRight();
    }
    else if (DRIVE_PATH == 7 || DRIVE_PATH == 9)
    {
        // try left
        autoDriveLeft();
    }
    else if (DRIVE_PATH > 9)
    {
        //try reverse... maybe stop first.
        autoDriveBack();
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Advanced Driving functions                                                                          //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void autoDriveForward()
{
    if (checkForwardSafety() == false)
    {
        clearDrivePath();
    }

    if (DRIVE_PATH_START == 0)
    {
        DRIVE_PATH_START = millis();
    }
    else
    {
        if (millis() - DRIVE_PATH_START < ((int)(5000 * DRIVE_PATH_PROPORTION)))
        {
            forwardDrive(autoPilotSpeed);
            return;
        }

        clearDrivePath();
    }
}

void autoDriveBack()
{
    if (checkReverseSafety() == false)
    {
        clearDrivePath();
    }

    if (DRIVE_PATH_START == 0)
    {
        DRIVE_PATH_START = millis();
    }
    else
    {
        if (millis() - DRIVE_PATH_START < ((int)(1000 * DRIVE_PATH_PROPORTION)))
        {
            reverseDrive(autoPilotSpeed);
            return;
        }

        clearDrivePath();
    }
}

void autoDriveLeft()
{
    if (checkLeftSafety() == false)
    {
        clearDrivePath();
    }

    if (DRIVE_PATH_START == 0)
    {
        DRIVE_PATH_START = millis();
    }
    else
    {
        if (millis() - DRIVE_PATH_START < ((int)(halfTurn160 * DRIVE_PATH_PROPORTION)))
        {
            leftDrive(autoPilotTurnSpeed);
        }
        else
        {
            clearDrivePath();
        }
    }
}

void autoDriveRight()
{
    if (checkRightSafety() == false)
    {
        clearDrivePath();
    }

    if (DRIVE_PATH_START == 0)
    {
        DRIVE_PATH_START = millis();
    }
    else
    {
        if (millis() - DRIVE_PATH_START < ((int)(halfTurn160 * DRIVE_PATH_PROPORTION)))
        {
            rightDrive(autoPilotTurnSpeed);
        }
        else
        {
            clearDrivePath();
        }
    }
}

// Clear relevant path variables to allow for a new path choice and pursuit
void clearDrivePath()
{
    stopDrive(0);
    DRIVE_PATH = 0;
    DRIVE_PATH_START = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Toggle Autopilot function                                                                           //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void toggleAutoPilot()
{
    lastAutoPilotToggle = millis();

    if (AUTOPILOT_ON)
    {
        clearDrivePath();
        driveMotors(0, 0); // stop
        AUTOPILOT_ON = false;
        turnOnWhiteHeadlights();
        return;
    }
    else
    {
        driveMotors(0, 0); // stop
        AUTOPILOT_ON = true;
        turnOnGreenHeadlights();
        return;
    }
}
