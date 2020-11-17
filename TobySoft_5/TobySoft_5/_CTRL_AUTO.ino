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

        if ((advancedFunctionStart != 0) || (checkFowardSafety() == false))
        {
            tryEvade();
        }
        else
        {
            clearAdvancedFunctions();
            driveMotors(1); // go forward
        }
    }
}

void tryEvade()
{
    Serial.println("NEW EVADE!");

    switch (evadeTryCount)
    {
    case 0: // Try left Clearing Search
        Serial.println("evade: FCL");
        findClearLeft();
        break;
    case 1: // Try backing to the left
        Serial.println("evade: BTL");
        backLeftEvade();
        break;
    case 2: // Try right Clearing Search
        Serial.println("evade: FCR");
        findClearRight();
        break;
    case 3: // Try backing to the right
        Serial.println("evade: BTR");
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
