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

        if (checkFowardSafety() == false)
        {
            Serial.println("evade");
            tryEvade();
        }
        else
        {
            Serial.println("drive");
            driveMotors(1); // go forward
        }
    }
}

void tryEvade()
{
    if (advancedFunctionStart != 0) // evade still in progress
    {       
        return;
    }

    Serial.println("NEW EVADE!");

    switch (evadeTryCount)
    {
    case 0: // Try left Clearing Search
        findClearLeft();
        break;
    case 1: // Try backing to the left
        backLeftEvade();
        break;
    case 2: // Try right Clearing Search
        findClearRight();
        break;
    case 3: // Try backing to the right
        backRightEvade();
        break;
    default:
        evadeLoopCount++;
        if (evadeLoopCount > 99)
        {
            stopDrive(0);
            playSleepSound();
            delay(7000); // delays are bad.. but so is getting here
            break;
        }
        else
        {
            evadeTryCount = 0;
        }
        break;
    }

    evadeTryCount++;
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
        clearAdvancedFunctions();
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
