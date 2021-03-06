/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Manual RF Control functions                                                                         //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void manualControl()
{
    // Transmitter data re: joystick button
    if ((dataFromTransmitter[2] == 1) && (millis() - lastAutoPilotToggle > toggleDebounce))
    {
        lastAutoPilotToggle = millis();
        toggleAutoPilot();
        return;
    }

    // Transmitter data re: toggle switch
    //// TODO: move to options menu
    if ((dataFromTransmitter[3] == 1) && (millis() - lastSafeModeToggle > toggleDebounce))
    {
        //lastAutoPilotToggle = millis();
        //toggleSafeMode();
        //return
    }

    if ((dataFromTransmitter[0] < 100) && (dataFromTransmitter[0] > 80) && (dataFromTransmitter[1] < 100) && (dataFromTransmitter[1] > 80))
    {
        driveMotors(0, 0); // stop
        return;
    }

    if ((dataFromTransmitter[0] == -1) && (dataFromTransmitter[1] == -1))
    {
        driveMotors(0, 0); // stop
        return;
    }

    int xDiff = 0;
    if (dataFromTransmitter[0] < 90)
    {
        xDiff = 90 - dataFromTransmitter[0];
    }
    else
    {
        xDiff = dataFromTransmitter[0] - 90;
    }
    int yDiff = 0;
    if (dataFromTransmitter[1] < 90)
    {
        yDiff = 90 - dataFromTransmitter[1];
    }
    else
    {
        yDiff = dataFromTransmitter[1] - 90;
    }

    if (xDiff >= yDiff) // determine whether joystick displacement is more vertical or horizontal
    {                   // turning (xDiff) takes precedence over forward/reverse (yDiff) in case of equality
        if (dataFromTransmitter[0] < 90)
        {
            driveMotors(3, getPercentSpeed(dataFromTransmitter[0])); // right
        }
        else
        {
            driveMotors(2, getPercentSpeed(dataFromTransmitter[0])); // left
        }
    }
    else
    {
        if (dataFromTransmitter[1] < 90)
        {
            driveMotors(1, getPercentSpeed(dataFromTransmitter[1])); //forward
        }
        else
        {
            driveMotors(-1, getPercentSpeed(dataFromTransmitter[1])); //reverse
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Instruction Filter function                                                                         //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void driveMotors(int direction, int percentSpeed)
{
    // Basic Drive functions
    switch (direction)
    {
    case 0: // Stop
        stopDrive(0);
        break;
    case 1: // Forward
        forwardDrive(((float)percentSpeed / 100) * maxSpeed);
        break;
    case -1: // Reverse
        reverseDrive(((float)percentSpeed / 100) * maxSpeed);
        break;
    case 2: // Left
        leftDrive(((float)percentSpeed / 100) * maxSpeed);
        break;
    case 3: // Right
        rightDrive(((float)percentSpeed / 100) * maxSpeed);
        break;
    default:
        break;
    }
}

int getPercentSpeed(int joystickAngle)
{
    if (joystickAngle > 90)
    {
        return (joystickAngle - 90) + 10;
    }
    else if (joystickAngle < 90)
    {
        return (90 - joystickAngle) + 10;
    }
}
