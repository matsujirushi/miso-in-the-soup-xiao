#ifndef VIBRATIONMOTORCLASS_HPP
#define VIBRATIONMOTORCLASS_HPP

class VibrationMotorClass
{
private:
    int forwardPin;
    int reversePin;

public:
    VibrationMotorClass(int forwardPin, int reversePin)
        : forwardPin{forwardPin}, reversePin{reversePin}
    {
    }

    void begin(void)
    {
        pinMode(forwardPin, OUTPUT);
        digitalWrite(forwardPin, LOW);
        pinMode(reversePin, OUTPUT);
        digitalWrite(reversePin, LOW);
    }

    void turnOn(bool forward = true)
    {
        digitalWrite(forwardPin, forward ? HIGH : LOW);
        digitalWrite(reversePin, !forward ? HIGH : LOW);
    }

    void turnOff(void)
    {
        digitalWrite(forwardPin, LOW);
        digitalWrite(reversePin, LOW);
    }
};

#endif // VIBRATIONMOTORCLASS_HPP
