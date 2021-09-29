#ifndef Stepper_H
#define Stepper_H
#include <Arduino.h>

typedef struct{
  int id;
  int dirPin;
  int stepPin;
  int steps_per_revolution;
  double delaymic;
  bool resatablety;
  QueueHandle_t *queue;
}MotorConfig;

class Stepper
{
private:
    int dirPin , stepPin;
    
public:
    int stepsPerRevolution;
    int currentStep;
    int stepsRemain;
    int targetSteps;
    int lastPostion;
    double delayMic;
    int IdTag;
    bool resatable;
    QueueHandle_t *queue;
    Stepper(MotorConfig motor ){
        IdTag = motor.id;
        dirPin=motor.dirPin;
        stepPin=motor.stepPin;
        stepsPerRevolution=motor.steps_per_revolution;
        delayMic = motor.delaymic;
        resatable = motor.resatablety;
        queue = motor.queue;
        pinMode(stepPin, HIGH);
        pinMode(dirPin, HIGH);
    };
    // direction 1 = right , direction 0 = left
    void Init(int steps, int dir){
        targetSteps = steps;
        currentStep = 0;
        stepsRemain = steps;
        if (dir == 1)
        {
            digitalWrite(dirPin,HIGH);
        }
        else if (dir == 0)
        {
            digitalWrite(dirPin,LOW);
        }        
    };
    void GoStep(){
        if (currentStep == targetSteps && stepsRemain == 0)
        {
            lastPostion += currentStep;
            stepsRemain = 0;
            currentStep = 0;
            targetSteps = 0;
        }
        else
        {
            stepsRemain--;
            currentStep++;
        }
        digitalWrite(stepPin,HIGH);
        delayMicroseconds(delayMic);
        digitalWrite(stepPin,LOW);
        delayMicroseconds(delayMic);
    };

    void GoTarget(){
        while (currentStep != targetSteps && stepsRemain != 0)
        {
            digitalWrite(stepPin,HIGH);
            delayMicroseconds(delayMic);
            digitalWrite(stepPin,LOW);
            delayMicroseconds(delayMic);
            stepsRemain--;
            currentStep++;
        }
        if (currentStep == targetSteps && stepsRemain == 0)
        {
            lastPostion += currentStep;
            stepsRemain = 0;
            currentStep = 0;
            targetSteps = 0;
        }
    }


    double SetDelay(int velocity){
    
    double RPM = 0.00000000000000;
    switch (this->IdTag )
    {
    case 1:
        RPM = abs(1-(sqrt(1-(sq(velocity)/sq(c11)))))*12.86956521;
        break;
    case 2:
        RPM = abs(1-(sqrt(1-(sq(velocity)/sq(c11)))/60))*12.17391304;
        break;
    case 3:
        RPM = abs(1-(sqrt(1-(sq(velocity)/sq(c11)))/720))*12.15217391;
        break;
    case 4:
        RPM = 40;
        break;
    case 5:
        RPM = 50*(sqrt(1-(sq(velocity)/sq(c11))))/3.81;
        break;
    default:
        break;
    }
    return RPM/0.01875;
    }

};


#endif