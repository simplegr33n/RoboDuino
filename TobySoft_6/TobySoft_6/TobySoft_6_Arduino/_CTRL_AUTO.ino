int middleDistance, leftDistance, rightDistance;

#define autoPilotDecisionInterval 100000
unsigned long lastPilotDecision = 0; // micros() timestamp of last autoPilotDecision

int evadeTryCount = 0;
int evadeLoopCount = 0;

bool tryEvadeRight = false;

#define clearSailingAuto 100 // threshold of clearAutoSamples for moving forward
int clearAutoSamples = clearSailingAuto;

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
    if ((dataFromTransmitter[2] == 1) && (millis() - lastAutoPilotToggle > toggleDebounce))
    {
        toggleAutoPilot();
        return;
    }

    if ((micros() - lastPilotDecision > autoPilotDecisionInterval))
    {
        if (checkFowardSafety() == false)
        {
            clearAutoSamples = 0;
            tryEvade();
        }
        else
        {
            clearAutoSamples++;
            if (clearAutoSamples > clearSailingAuto * 2)
            {
                clearAutoSamples = clearSailingAuto + 1;
            }

            if ((advancedFunctionStart == 0) || (clearAutoSamples >= clearSailingAuto))
            {
                clearAdvancedFunctions();
                driveMotors(1, autoPilotSpeed); // go forward
            }
            else
            {
                tryEvade();
            }
        }
    }
}

void tryEvade()
{
    switch (evadeTryCount)
    {
    case 0: // Try left/right Clearing Search
//        if (tryEvadeRight)
//        {
            if (checkRightSafety())
            {
                Serial.println("evade: FCR");
                findClearRight();
                return;
            }
            if (checkLeftSafety())
            {
                Serial.println("evade: FCL");
                findClearLeft();
                return;
            }
            else
            {
                evadeTryCount++;
            }
//        }
//        else
//        {
//            if (checkLeftSafety())
//            {
//                Serial.println("evade: FCL");
//                findClearLeft();
//                return;
//            }
//            else if (checkRightSafety())
//            {
//                Serial.println("evade: FCR");
//                findClearRight();
//                return;
//            }
//            else
//            {
//                evadeTryCount++;
//            }
//        }

        break;
    case 1: // Try backing up
        Serial.println("evade: BU");
        backUpEvade();
        break;
    default:
        evadeLoopCount++;
        if (evadeLoopCount > 500)
        {
            stopDrive(0);
            evadeLoopCount = 0;
            evadeTryCount = 0;
            toggleAutoPilot();
            break;
        }
        else
        {
            evadeTryCount = 0;
            tryEvadeRight != tryEvadeRight;
        }
        break;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Toggle function                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void toggleAutoPilot()
{
    lastAutoPilotToggle = micros();

    if (AUTOPILOT_ON)
    {
        clearAdvancedFunctions();
        driveMotors(0, 0); // stop
        AUTOPILOT_ON = false;
        return;
    }
    else
    {
        driveMotors(0, 0); // stop
        AUTOPILOT_ON = true;
        return;
    }
}
