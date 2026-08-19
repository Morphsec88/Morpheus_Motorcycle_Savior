
This is a theoretical project; we assume no responsibility for its technical implementation or any potential consequences.

https://github.com/user-attachments/assets/6eabb30b-16ba-471a-8871-94f3af6c2db0

# morpheus-motorcyclist-savior 

> **Automated Emergency Parachute Control & Rider Safety System for Motorcycle Riders**

morpheus-motorcyclist-savior is an Arduino-based emergency safety system (`motoros_emergency`) designed to lift the rider and their seat completely out of the danger zone during a critical event (such as a speed wobble or crash). It deploys a braking canopy to lift the rider to an optimal, speed-calculated altitude while providing immediate, controlled deceleration.





<img width="1536" height="1024" alt="902FC882-FF6E-4233-A3A7-01AD279A8287 (1)" src="https://github.com/user-attachments/assets/fd0742df-eb60-4088-b036-42649493fb45" />

<img width="1462" height="1076" alt="10A04CB2-1024-4255-8624-A96728258E32" src="https://github.com/user-attachments/assets/38a82976-ff66-418e-a78e-313537273666" />

# TECHNICAL SPECIFICATION AND SYSTEM DESIGN
**Project:** Morpheus Motorcycle Savior (MMS) – Active Ballistic Rider Protection System  
**Document Version:** 2.2 (Dynamic Ascent Trajectory Baseline)

## 1. Computational Statics and Trajectory Control

The core operating principle of the system relies on converting horizontal kinetic momentum into vertical lift via a guided, curved constraint path (dynamic pendulum-lift effect). The deployment trajectory, capsule activation altitude, and braking point are dynamically governed by two critical baseline variables:

* **Motorcycle Speed (v [km/h]):** Directly determines the longitudinal tilt angle (Pitch) of the deployment tube and the target deployment altitude. Higher speeds require a shallower, more elongated trajectory to ensure optimal, controlled deceleration.
* **Rider Weight (m [kg]):** Combined with the fixed mechanical mass of the ejectable seat base, this establishes the total system mass. The central processing unit uses this value to calculate the apex of the ballistic trajectory and the mechanical load distribution during the ascent phase.

*Developer Note: Exact timings, pressure wastegate curves, and microsecond adjustments can be fine-tuned via the 1-to-9 developer tuning scale (multipliers) located in `Morpheus_Motorcycle_SaviorConfig.h` based on data analysis and physical testing.*

### 1.1. Dynamic Ballistic Deployment Tube Positioning (Pitch)

The speed of the motorcycle directly dictates the physical forward tilt angle of the deployment tube prior to launch, ensuring the parachute capsule is projected into the ideal focal point of the calculated pendulum arc:

* **Low Speed (below 50 km/h):** The deployment tube stands completely vertical (0° relative to the perpendicular axis). Because horizontal momentum is minimal, the system optimizes for pure vertical lift.
* **High Speed (above 50 km/h):** The tube dynamically tilts forward into the oncoming wind up to a maximum angle of 45°. This projects the capsule ahead of the rider, establishing the fixed anchor point around which the rider will swing upward using their own forward velocity.

### 1.2. Altitude Tracking and Dual-Point Activation Logic

The sequence is initiated via a dedicated dual-point safety trigger mechanism or, upon request, via an automated emergency override loop. To prevent accidental deployment, the manual input requires two separate safety switches to be activated simultaneously. 

Operational readiness requires that operators undergo comprehensive training at a certified training facility using specialized simulation vehicles. This training is mandatory to master system handling, adapt to the unique physical forces of the pendulum ascent, and build the necessary sensory familiarity and confidence.

Upon successful dual-activation, the core hub processes the real-time speed/mass profile to calculate required launch energy and target altitude.

* **Trajectory Tracking:** As the ballistic capsule is projected, it draws out the high-tensile rope. This rope strips through break-away sensors (staged pulse encoders), generating real-time pulse streams to measure deployed length and altitude.
* **Winch Block Point:** When the number of real-time pulses correlates perfectly with the pre-calculated profile, the system instantly locks the mechanical winch spool (`winchBrake`), establishing the fixed pivot point for the pendulum arc.

### 1.3. Signal-in-Rope: Activating the "Air-Harpoon" via Controlled Sub-Explosion

A dedicated signal core cable runs inside the center of the unreeling rope (`internalRopeSignal`). The exact millisecond the winch brake locks, the final electrical command is sent down this cable:

* **Capsule Detonation:** The signal triggers a localized, controlled pressure release (CO2, compressed air, or pyrotechnic charge) inside the flying capsule at the variable, pre-calculated altitude.
* **Canopy Blossoming:** The capsule forcefully ruptures, deploying the canopy instantly into the airflow. This creates the "Air-Harpoon" (lég-szigony) effect, anchoring the system firmly within the air mass.

---

## 2. Ejection and Pendulum Ascent Sequence

Once the Air-Harpoon bites into the airflow, the physical chain reaction converts horizontal velocity into smooth, vertical lift:

* **Curved Constraint Path:** Driven by their initial forward momentum, the rider swings forward and upward on the taut high-tensile cord, transforming linear kinetic energy into potential energy.
* **Separation:** The cord is anchored beneath the seat, running directly between the rider's legs. As the rope reaches full tension, the upward force vector cleanly detaches the ejection seat assembly from the motorcycle frame.
* **Deceleration & Lift:** Instead of a harsh kinetic jerk, the rider experiences a controlled, glider-like ascent. Aerodynamic drag and gravity progressively manage the kinetic energy, a process that lasts for approximately one-third (1/3) of the total deployed trajectory distance.

*Project Scope Limit: The core objective of this system ends once the operator is established on a safe trajectory, cleared from the danger zone, and decelerated at a safe altitude. Secondary landing parachutes (which are outside the scope of this specific project) handle the subsequent descent to the ground.*

---

## 3. Safety Principle and AI Readiness (Lateral Tuning)

The software design strictly prioritizes deterministic safety and explicit environmental awareness.

* **No Autonomous Lateral Shifting:** Without live situational data, the system never makes autonomous lateral decisions. Blindingly tilting the deployment tube could throw the rider into roadside obstacles. Therefore, the lateral angle default is locked strictly at 0 degrees (centered along the motorcycle's longitudinal axis).
* **AI-Ready Infrastructure:** The source code includes a protected override channel (`aiExternalLateralOverride`). If a future environment-scanning camera or AI diagnostic module is integrated—and verifies a clear, safe ditch or soft ground area—it can modulate the lateral actuator at the moment of launch to guide the pendulum arc away from the asphalt.

---

## 4. Hardware Pin Mapping (Updated Baseline)


| Peripheral | Arduino PIN | Function |
| :--- | :---: | :--- |
| `safetyTriggerA` | 2 | Primary input of the dual-point manual trigger system |
| `safetyTriggerB` | 3 | Secondary input of the dual-point manual trigger system (Simultaneous) |
| `mainLaunchValve` | 4 | Main pneumatic/ballistic valve for the capsule deployment tube |
| `lateralActuator` | 5 | Tube LATERAL tilt mechanism actuator (Reserved for AI override) |
| `trimActuator` | 6 | Tube LONGITUDINAL tilt mechanism actuator (Pitch: 0° to 45°) |
| `winchBrake` | 7 | Heavy-duty mechanical brake to lock the spool |
| `pressureWastegate` | 8 | Deployment tube pressure relief valve (PWM) |
| `internalRopeSignal` | 9 | Signal core cable in rope (Triggers capsule explosion/opening) |
| `breakAwaySensorPin` | 10 | Staged pulse encoder input for real-time altitude 
