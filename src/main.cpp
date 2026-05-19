#include <Arduino.h>
#include "AeroAnchorConfig.h"

VehicleAndRiderProfile profile;
DeveloperTuning tuning;
HardwarePins pins;

float currentSpeedKmh = 0.0;     
int breakAwayCounter = 0;       

// BIZTONSÁGI ALAPELV: A mi rendszerünk vakon nem hoz oldalirányú döntést!
// Ez a változó alapból 0.0 (közép). Egy későbbi, környezetet látó AI diagnosztika 
// felülbírálhatja ezt az értéket, ha igazoltan tiszta és biztonságos az árok.
float aiExternalLateralOverride = 0.0; 

// ============================================================================
// 1. HARDVER-ABSZTRAKCIÓS MEGVALÓSÍTÁS
// ============================================================================
void setTubePitchAngle(float degrees) {
    Serial.print("[HARDVER] Katapultcső hosszirányú dőlésszöge: ");
    Serial.print(degrees);
    Serial.println(" fok (menetirány szerint előre).");
}

void setTubeYawAngle(float degrees) {
    // 5-ös PIN: Oldalirányú mozgató szervó/aktuátor
    Serial.print("[HARDVER] Katapultcső oldalirányú szöge: ");
    Serial.print(degrees);
    Serial.println(" fok (0 = szigorúan középen).");
}

void lockWinchSpool() {
    digitalWrite(pins.winchBrake, HIGH);
    Serial.println("[HARDVER] !!! CSÉVÉLÉS BLOKKOLVA !!! (Mechanikus fék zárva)");
}

void detonateAirHarpoon() {
    digitalWrite(pins.internalRopeSignal, HIGH);
    delay(50); 
    digitalWrite(pins.internalRopeSignal, LOW);
    Serial.println("[HARDVER] !!! LÉG-SZIGONY FELROBBANTVA !!! (Ernyő kinyílt)");
}

void IRAM_ATTR onBreakAwaySensorTrip() {
    breakAwayCounter++;
}

// ============================================================================
// 2. MATEMATIKAI ÉS STATIKAI SZÁMÍTÁSOK (1-9 SKÁLA ALAPJÁN)
// ============================================================================

// 1/1. Cső előredőlése a sebesség alapján (50 km/h felett dől előre max 45°-ig)
float calculateTubePitch(float speedKmh) {
    if (speedKmh <= profile.speedThresholdKmH) return 0.0; // Teljesen függőleges
    
    float sensitivityFactor = tuning.tiltSensitivity / 5.0; // 1-9 skála szorzója
    float targetAngle = (speedKmh - profile.speedThresholdKmH) * 0.3 * sensitivityFactor;
    
    if (targetAngle > 45.0) targetAngle = 45.0; // Gépészeti maximum korlát
    return targetAngle;
}

// Oldalirányú szög kiszámítása
float calculateTubeYaw() {
    // Biztonsági okokból alapértelmezetten szigorúan középen (0.0) tartjuk a csövet.
    // Ha a jövőben egy külső AI szoftver (ami látja a környezetet/szakadékot) küld adatot, 
    // az 1-9 skála segítségével az az érték modulálható.
    float lateralFactor = tuning.lateralSensitivity / 5.0;
    return aiExternalLateralOverride * lateralFactor;
}

int calculateTargetPulses(float speedKmh) {
    float totalMass = profile.driverMassKg + profile.ejectorSeatMassKg;
    float altitudeFactor = tuning.altitudeMultiplier / 5.0; 
    int targetPulses = (int)(((speedKmh * 0.4) + (totalMass * 0.1)) * altitudeFactor);
    if (targetPulses < 10) targetPulses = 10;   
    if (targetPulses > 100) targetPulses = 100; 
    return targetPulses;
}

int calculateWastegateDuty(float speedKmh) {
    if (speedKmh <= profile.speedThresholdKmH) return 255; 
    float forceFactor = tuning.catapultForceMulti / 5.0;
    int duty = (int)(((200.0 - speedKmh) / 150.0) * 255 / forceFactor);
    if (duty < 0) duty = 0;     
    if (duty > 255) duty = 255; 
    return duty;
}

// ============================================================================
// 3. A KRITIKUS MENTÉSI SZEKVENCIA
// ============================================================================
void executeRescueSequence(float speedKmh) {
    Serial.println("\n--- VÉSZHELYZETI INDÍTÁS: MENTÉSI TENGELY AKTIVÁLÁSA ---");
    
    // 1. POZICIONÁLÁS: Előredőlés és a biztonságos oldalirány beállítása
    float pitchAngle = calculateTubePitch(speedKmh);
    setTubePitchAngle(pitchAngle);
    
    float yawAngle = calculateTubeYaw();
    setTubeYawAngle(yawAngle); // Alapból ez 0 fok (egyenesen előre lő), kivéve ha külső AI felülbírálja
    
    int wastegateDuty = calculateWastegateDuty(speedKmh);
    analogWrite(pins.pressureWastegate, wastegateDuty); 
    
    int targetPulses = calculateTargetPulses(speedKmh);

    // 2. KILÖVÉS (A vezető akaratából)
    digitalWrite(pins.mainLaunchValve, HIGH); 
    delay(25); 
    digitalWrite(pins.mainLaunchValve, LOW);
    Serial.println("[HARDVER] Katapult fő pneumatikus szelep elsütve.");

    // 3. MAGASSÁGKÖVETÉS (1/2. fázis)
    breakAwayCounter = 0; 
    while (breakAwayCounter < targetPulses) {
        delayMicroseconds(10); 
    }

    // 4. CSÉVÉLÉS BLOKKOLÁSA
    lockWinchSpool();

    // 5. LÉG-SZIGONY ROBBANTÁSA (1/3. fázis)
    detonateAirHarpoon();
    
    Serial.println("[RENDSZER] Biztonságos mentési fázis lefutott.\n");
}

// ============================================================================
// 4. ARDUINO ALAPFÜGGVÉNYEK
// ============================================================================
void setup() {
    Serial.begin(115200);
    
    pinMode(pins.mainLaunchValve, OUTPUT);
    pinMode(pins.winchBrake, OUTPUT);
    pinMode(pins.internalRopeSignal, OUTPUT);
    pinMode(pins.lateralActuator, OUTPUT); 
    
    pinMode(pins.manualTrigger, INPUT_PULLUP);
    pinMode(pins.autoEnable, INPUT_PULLUP);
    
    pinMode(pins.breakAwaySensorPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pins.breakAwaySensorPin), onBreakAwaySensorTrip, RISING);

    Serial.println("AeroAnchor Rendszermag készenlétben.");
}

void loop() {
    currentSpeedKmh = 90.0; // Szimulált tesztsebesség

    // Alapértelmezett biztonsági állapot: a környezet ismerete nélkül NEM térünk ki oldalra (0.0)
    aiExternalLateralOverride = 0.0; 

    if (digitalRead(pins.manualTrigger) == LOW) {
        executeRescueSequence(currentSpeedKmh);
        
        while (true) { 
            delay(1000); 
        }
    }
    
    delay(50); 
}