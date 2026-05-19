#ifndef AERO_ANCHOR_CONFIG_H
#define AERO_ANCHOR_CONFIG_H

struct DeveloperTuning {
    int tiltSensitivity   = 5;  // 1-9 skála: Cső dőlésszög érzékenység
    int catapultForceMulti = 5;  // 1-9 skála: Katapulterő szorzó
    int altitudeMultiplier = 5;  // 1-9 skála: Magassági kötélhossz szorzó
};

struct VehicleAndRiderProfile {
    float driverMassKg             = 85.0;  // Vezető súlya
    float ejectorSeatMassKg        = 15.0;  // Ülés fix tömege
    const float chassisSlackM      = 1.2;   // Alap-holtjáték
    float speedThresholdKmH        = 50.0;  // Határsebesség
};

struct HardwarePins {
    const int manualTrigger      = 2;   // Indítógomb
    const int autoEnable         = 3;   // Automata kapcsoló
    const int mainLaunchValve    = 4;   // Fő pneumatikus szelep
    const int trimActuator       = 6;   // Cső állító mechanika
    const int winchBrake         = 7;   // Csörlőfék
    const int pressureWastegate  = 8;   // Oldalszelep (PWM)
    const int internalRopeSignal = 9;   // Kötél belső jelkábel
    const int breakAwaySensorPin = 10;  // Szakadó-szenzorok
};

#endif