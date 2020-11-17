/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Manual RF Control functions                                                                         //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void manualControl()
{
    // check RF switch for AutoPilot toggle
    if ((dataFromTransmitter[2] == 0) && (micros() - lastAutoPilotToggle > toggleDebounce))
    {
        toggleAutoPilot();
        return;
    }

    // check RF big red button for SafeMode toggle
    if ((dataFromTransmitter[3] == 1) && (micros() - lastSafeModeToggle > toggleDebounce))
    {
        toggleSafeMode();
    }

    if ((dataFromTransmitter[0] < 100) && (dataFromTransmitter[0] > 80) && (dataFromTransmitter[1] < 100) && (dataFromTransmitter[1] > 80))
    {
        driveMotors(0); // stop
        return;
    }

    if ((dataFromTransmitter[0] == -1) && (dataFromTransmitter[1] == -1))
    {
        driveMotors(0); // stop
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

    if (xDiff >= yDiff) // determine whether joystick is more vertically or horizontally displaced
    {                   // turning (xDiff) takes precedence over forward/reverse (yDiff) in case of equality
        if (dataFromTransmitter[0] < 90)
        {
            driveMotors(3); // right
        }
        else
        {
            driveMotors(2); // left
        }
    }
    else
    {
        if (dataFromTransmitter[1] < 90)
        {
            driveMotors(1); //forward
        }
        else
        {
            driveMotors(-1); //reverse
        }
    }
}
