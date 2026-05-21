# morpheus-motorcyclist-savior 🏍️🪂

> **Automated Emergency Parachute Control & Rider Safety System for Motorcycle Riders**

morpheus-motorcyclist-savior is an Arduino-based emergency safety system (`motoros_emergency`) designed to lift the rider and their seat completely out of the danger zone during a critical event (such as a speed wobble or crash). It deploys a braking canopy to lift the rider to an optimal, speed-calculated altitude while providing immediate, controlled deceleration.





<img width="1536" height="1024" alt="902FC882-FF6E-4233-A3A7-01AD279A8287 (1)" src="https://github.com/user-attachments/assets/fd0742df-eb60-4088-b036-42649493fb45" />

<img width="1462" height="1076" alt="10A04CB2-1024-4255-8624-A96728258E32" src="https://github.com/user-attachments/assets/38a82976-ff66-418e-a78e-313537273666" />



---

## 🏗️ 1. Computational Statics and Core Principles

The entire operation of the system, deployment trajectory, and launch energy are dynamically governed by two critical baseline variables:

1. **Motorcycle Speed (Km/h):** Directly determines the required flight rope length and the longitudinal tilt angle of the parachute deployment tube.
2. **Rider Weight (Kg):** Combined with the fixed mechanical mass of the ejectable seat base, this establishes the total system mass used for inertia and braking calculations.

> 🛠️ **Developer Note:** The current software code serves as a functional foundation. Exact angles, pressure wastegate curves, and microsecond timings can be fine-tuned via the **1-to-9 developer tuning scale (multipliers)** located in `Morpheus_Motorcycle_SaviorConfig.h` based on data analysis and physical testing.

### 1/1. Dynamic Parachute Deployment Tube Positioning (Pitch)
The speed of the motorcycle directly dictates the physical forward tilt angle of the parachute deployment tube prior to launch:
* **Low Speed (below 50 km/h):** The deployment tube stands completely **vertical (0° / 90°)**.
* **High Speed:** The tube dynamically tilts forward into the oncoming wind up to a **maximum angle of 45°** to compensate for horizontal kinetic momentum.

### 1/2. Altitude Tracking and Brake Correlation
The sequence is initiated via a **foot-operated trigger mechanism** or, upon request, via an **automated emergency override loop**. The system transmits a signal to the core hub, calculating the required launch energy and target altitude.
* **Physical Altitude Sensing:** As the canopy is deployed, the high-tensile rope physical strips/snaps through **break-away sensors (staged pulse encoders)**.
* **Winch Block Point:** When the number of real-time pulses from the break-away sensors correlates perfectly with the pre-calculated speed/mass profile, the system instantly **locks the winch spool**. This fixes the precise altitude position of the parachute.

### 1/3. Signal-in-Rope: Activating the "Air-Harpoon" via CO2 / Air Cartridge
A **dedicated signal core cable** runs inside the center of the unreeling rope. The exact millisecond the winch brake locks, the final electrical command is sent down this cable: a valve triggers (or ruptures) a **CO2 or compressed air cartridge** inside the canopy, forcefully blossoming the deployment system into an immediate **"Air-Harpoon" (lég-szigony)**.

---

## 🚨 2. Ejection and Deceleration Sequence

Once the Air-Harpoon bites into the airflow, the physical chain reaction unfolds as follows:

1. **Rope Tension:** The high-tensile cord is anchored underneath the seat, running right between the rider's legs.
2. **Separation:** The massive kinetic jerk instantly detaches the ejection seat assembly (along with the rider) from the motorcycle frame.
3. **Deceleration & Lift:** A combined lifting and braking effect takes over. This controlled kinetic management lasts for **approximately one-third (1/3)** of the total deployed altitude distance.

⚠️ **Project Scope Limit:** *The core objective of this system ends here. The system has successfully removed the operator from the bike, cleared the danger zone, and brought them to a safe altitude matched to their initial speed. From this point onward, secondary landing parachutes (which are outside the scope of this specific project) can take over to ensure a safe descent to the ground.*

---

## 🛡️ 3. Safety Principle and AI Readiness (Lateral Tuning)

The software design strictly prioritizes **rider safety** and explicit environmental awareness.

* **Why there is no automated lateral shifting:** Without live situational data, the system **never makes autonomous lateral decisions**. Blindingly tilting the deployment tube to the right could inadvertently throw the rider into a ravine, concrete barriers, or oncoming traffic. Therefore, the lateral angle default is locked strictly at **0 degrees (centered)**.
* **AI-Ready Infrastructure:** The source code includes a protected override channel (`aiExternalLateralOverride`). If a future environment-scanning camera or AI diagnostic module is integrated—and verifies a clear, safe ditch or soft ground area—it can modulate the lateral actuator to guide the trajectory away from the asphalt.

---

## 🛠️ Hardware Pin Mapping (Updated Baseline)


| Peripheral | Arduino PIN | Function |
| :--- | :---: | :--- |
| `footTrigger` | **2** | Foot-operated trigger pedal / switch (Rider's intent) |
| `autoEnable` | **3** | Automated emergency mode toggle switch (Upon request) |
| `mainLaunchValve` | **4** | Main pneumatic valve for the parachute deployment tube |
| `lateralActuator` | **5** | Tube LATERAL tilt mechanism actuator (For AI override) |
| `trimActuator` | **6** | Tube LONGITUDINAL tilt mechanism actuator (Pitch: 0° to 45°) |
| `winchBrake` | **7** | Heavy-duty mechanical brake to lock the spool |
| `pressureWastegate` | **8** | Deployment tube pressure relief valve (PWM) |
| `internalRopeSignal` | **9** | Signal core cable in rope (Opens the CO2 / Air cartridge) |
| `breakAwaySensorPin` | **10** | Break-away loop inputs for altitude tracking |

---

<img width="1334" height="750" alt="MMS" src="https://github.com/user-attachments/assets/6bd8a7b9-daeb-4681-bd6c-61bf08ada909" />


---

## 📜 4. Licensing

This project is licensed under the **GNU General Public License v3.0 (GPLv3)**.
