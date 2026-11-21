# DC Motor Speed Control – Implementation Plan

## Status (2026-11-21): all three gates approved

| Milestone | Model | Result | Report |
|---|---|---|---|
| M1 Open loop and calibration | `DCMotor_M1_OpenLoop.slx` | ✔ Gate 1: Enc_Kcal = 0.8284 → ±100 % reads +2185 / −2215 rpm | `M1_Calibration.md` |
| M2 Plant ID and PI design | `DCMotor_M2_PISim.slx` | ✔ Gate 2: P1D plant (98.1 % validation fit), PI Kp = 0.01617 / Ki = 0.3778, tests pass | `M2_PITuning.md` |
| M3 Closed loop on hardware | `DCMotor_M3_ClosedLoopHW.slx` | ✔ Gate 3 (run2, PI + friction FF): ess ≤ 0.5 rpm, settling ≤ 0.21 s, breakaway overshoot 10.5 rpm | `M3_ClosedLoopResults.md` |

Deviation from the original plan: friction compensation (`SpeedController/FrictionFF`) was added after M3 run1 at the user's request, and the M2 simulation gained a matching friction model.

Hardware: Arduino Uno · Pololu #4841 (4.4:1 25D HP 12 V, 48 CPR encoder) · BTS7960M driver · ACS712-5A on A3
Tooling: MATLAB R2026b, Simulink Support Package for Arduino Hardware 26.2.4, Simulink/MATLAB Agentic Toolkit (model_read / model_edit / model_check / model_test, building/simulating/testing skills)

## Key facts and design decisions

| Item | Value |
|---|---|
| Encoder resolution (output shaft, x4 quadrature) | 48 × 4.4 = **211.2 counts/rev** |
| Speed at 2200 rpm | ≈ 7 744 counts/s (interrupt load is fine for the Uno) |
| Base sample time | **Ts = 10 ms** (resolution ≈ 28 rpm/count/sample → low-pass filter in the RPM subsystem) |
| Speed formula | `rpm = K_cal × Δcount / Ts / 211.2 × 60`, then 1st-order low-pass (cut-off tuned in M1) |
| Encoder calibration | Assumption: **100 % PWM = 2200 rpm**. `K_cal = 2200 / rpm_raw(+100 %)` (checked against −100 %), stored in `DCMotorParams.m` |
| Actuator command | Duty cycle in % (−100 … +100) → `DC7960MDriver` reference subsystem (unchanged) |
| Current sensor | ACS712-5A: 185 mV/A, 2.5 V offset; A3, 10-bit ADC. Logged and zero-calibrated in M1 only, not used in the control loop |
| Controller | Discrete **PID Controller block, PI form**, output limits ±100 %, clamping anti-windup |
| Reference | Staircase +1500 → −1500 rpm, **−150 rpm every 2 s** (20 steps, 42 s total) |
| Data acquisition | Monitor & Tune (external mode over serial), signals logged to SDI and saved as .mat |

### Reusable components (01_Implementation)
- `DC7960MDriver.slx` – already provided in `01_Implementation` by the user; used unchanged through a Subsystem Reference block.
- `EncoderToRPM.slx` – subsystem reference: Arduino Encoder block (pins 2/3) → counts → RPM (filtered).
- `SpeedController.slx` – subsystem reference: PI Controller block, used in both the simulation and the hardware model.
- `DCMotorParams.m` – all parameters (Ts, CPR, gear ratio, filter, ACS712 calibration, PI gains).

### Prerequisite found during exploration
The Arduino support package is installed, but its folders are **not on the MATLAB path** in the current session (`arduinolib` can't be found, so the driver's blocks don't resolve). Fix: add the `arduinolib` folder to the MATLAB path for this session.

---

## Milestone 1 – Open-loop hardware test and sensor calibration
**Model:** `DCMotor_M1_OpenLoop.slx`
1. Add `arduinolib` to the path; create `DCMotorParams.m`.
2. Build `EncoderToRPM.slx` (Encoder block on pins 2/3, Δcount, RPM scaling, calibration gain `K_cal`, low-pass). It also outputs the raw counts for calibration.
3. Top model: PWM profile `0 → +100 % → 0 → −100 % → 0` (about 4 s per segment, with 0 % pauses before reversal to protect the gearbox and driver) → `DC7960MDriver`; `EncoderToRPM`; A3 Analog Input → ACS712 current conversion; signals logged.
4. Configure for Arduino Uno (Ts = 10 ms, external mode, cache/codegen folders from the .prj); `model_check` plus a compile check.
5. Run on hardware, then evaluate the logs: RPM sign vs. PWM sign, steady-state speed in both directions, noise level / filter cut-off, current-sensor zero offset at 0 %.
6. **Encoder calibration:** with 100 % PWM = 2200 rpm as the reference, compute `K_cal` from the steady-state +100 % segment (and check it against −100 %), write it into `DCMotorParams.m`, and re-run to confirm that ±100 % reads ±2200 rpm (±2 %).

**Gate 1:** the motor runs in both directions at 100 %, positive PWM gives positive RPM, calibrated speed reads ±2200 rpm (±2 %) at ±100 %, RPM noise is acceptable, and calibration values are in `DCMotorParams.m`. Results go in `02_SupportingArtifacts/M1_Calibration.md` with plots. **After your approval → save the M1 model.**

## Milestone 2 – Plant identification and PI design in simulation
**Model:** `DCMotor_M2_PISim.slx`
1. Identify a 1st-order (+ delay) plant `duty % → rpm` from the M1 step data (System Identification Toolbox).
2. Build `SpeedController.slx` (PI Controller block, ±100 % saturation, clamping anti-windup).
3. Build the simulation model: staircase reference → SpeedController → identified plant + PWM quantisation/RPM-measurement model.
4. Tune the PI with `pidtune`, then verify with the staircase simulation; add `model_test` Gherkin tests for the controller (tracking, saturation, anti-windup on reversal).

**Gate 2:** in simulation, each 150 rpm step settles within ≈ 0.5 s with less than 10 % overshoot, the full +1500 → −1500 sweep tracks without windup, and the tests pass. Results go in `M2_PITuning.md`. **After your approval → save the M2 model.**

## Milestone 3 – Closed-loop speed control on hardware
**Model:** `DCMotor_M3_ClosedLoopHW.slx`
1. Top model: staircase reference (+1500 → −1500, −150 rpm / 2 s) → `SpeedController` → `DC7960MDriver`; feedback from `EncoderToRPM`. Current is still logged.
2. Deploy with Monitor & Tune, log the run, compare with the M2 simulation.
3. Retune on hardware if needed (gains are tunable parameters in external mode).

**Gate 3:** on the real motor, steady-state error stays within about ±30 rpm on each step, settling time is under 1 s, the zero crossing is smooth, and there is no sustained oscillation. Results go in `M3_ClosedLoopResults.md`. **After your approval → save the final M3 model.**

---

## Working agreements
- Models and .m/.mat files go in `01_Implementation`; .md files and plots go in `02_SupportingArtifacts`. All new files are added to the project. No git commits – you commit manually.
- Hardware runs: I build and start the Monitor & Tune run from MATLAB only after you confirm the rig is powered, the motor is clamped and the shaft is clear.
- None of the models or data in the earlier `DCMotorControl` project are reused (clean reproduction).
