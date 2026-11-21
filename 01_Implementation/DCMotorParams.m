%DCMOTORPARAMS Parameters for the Pololu #4841 speed-control project.
%   Hardware: Arduino Uno, Pololu #4841 (4.4:1 25D HP 12 V, 48 CPR encoder),
%   BTS7960M driver (DC7960MDriver.slx), ACS712-5A current sensor on A3.
%   Run before simulating/building (called from the models' InitFcn).

%% Timing
Ts = 0.01;                          % Base sample time [s]

%% Motor data (Pololu #4841 product page)
Motor_NoLoadRPM   = 2200;           % No-load speed at 12 V, output shaft [rpm]
Motor_GearRatio   = (22*24)/(12*10);% Exact gear ratio = 4.4
Enc_CPR_Motor     = 48;             % Counts per motor rev (x4 quadrature)
Enc_CountsPerRev  = Enc_CPR_Motor*Motor_GearRatio;  % 211.2 counts / output rev

%% Encoder -> RPM (EncoderToRPM.slx)
Enc_PinA = 2;                       % Encoder channel A (INT0)
Enc_PinB = 3;                       % Encoder channel B (INT1)
Enc_Dir  = 1;                       % Direction sign: M1 run1 confirmed +PWM -> +rpm
% Calibration gain (M1 run1): uncalibrated +100 % = 2629.0 rpm, -100 % = -2682.4 rpm.
% Assumption 100 % PWM = 2200 rpm, applied to the mean of both directions:
% 2200 / 2655.7 = 0.8284  ->  +100 % = 2178 rpm, -100 % = -2222 rpm
Enc_Kcal = 0.8284;
Enc_RPMGain = Enc_Dir*Enc_Kcal*60/(Ts*Enc_CountsPerRev);   % [rpm per count per sample]

% First-order low-pass on the speed signal: y(k) = a*u(k) + (1-a)*y(k-1)
RPM_LPF_Tau = 0.04;                 % Filter time constant [s]
RPM_LPF_a   = Ts/(RPM_LPF_Tau + Ts);
RPM_LPF_Num = RPM_LPF_a;
RPM_LPF_Den = [1, RPM_LPF_a - 1];

%% Current sensor ACS712-5A (CurrentSensor.slx)
Cur_Pin       = 'A3';               % Analog input pin (documentation only)
Cur_ADCMax    = 1023;               % 10-bit ADC full scale
Cur_VRef      = 5.0;                % ADC reference voltage [V]
Cur_Sens      = 0.185;              % Sensitivity [V/A]
Cur_OffsetV   = 2.5643;             % Zero-current output [V], M1 run1 (mean at 0 % PWM)
Cur_Dir       = -1;                 % Sign so that +PWM -> +current (M1 run1: sensor reversed)
Cur_LPF_Tau   = 0.05;               % Current filter time constant [s]
Cur_LPF_a     = Ts/(Cur_LPF_Tau + Ts);
Cur_LPF_Num   = Cur_LPF_a;
Cur_LPF_Den   = [1, Cur_LPF_a - 1];

%% M1 open-loop PWM profile [%] (1-D lookup, previous-value hold)
OL_Time   = [0   2   6   9   13  16];   % Breakpoints [s]
OL_Duty   = [0 100   0 -100   0   0];   % Duty cycle [%]
OL_Scale  = 1;                          % Tunable amplitude scale (Monitor & Tune)
OL_StopTime = 18;                       % Run length [s]

%% Identified plant duty [%] -> rpm (identify_DCMotorPlant.m, M1 run2, P1D)
%  Validation fit on M1 run1: 98.1 %
Plant_K   = 21.948;                 % Static gain [rpm/%]
Plant_Tau = 0.0242;                 % Mechanical time constant [s]
Plant_Td  = 0.0043;                 % Dead time [s]
Plant_d   = absorbDelay(c2d(tf(Plant_K, [Plant_Tau 1], 'InputDelay', Plant_Td), Ts, 'zoh'));
[Plant_Num, Plant_Den] = tfdata(Plant_d, 'v');
Duty_Quant = 100/255;               % PWM resolution of the driver (8-bit) [%]

%% Speed reference staircase (SpeedReference.slx)
Ref_Start    = 1500;                % First level [rpm]
Ref_Step     = 150;                 % Step size [rpm]
Ref_Period   = 2;                   % Hold time per level [s]
Ref_Min      = -1500;               % Last level [rpm]
Ref_StopTime = Ref_Period*((Ref_Start - Ref_Min)/Ref_Step + 1);   % 42 s
M3_StopHold  = 2;                   % Hardware: reference forced to 0 rpm after the staircase [s]
M3_StopTime  = Ref_StopTime + M3_StopHold;                        % 44 s

%% Speed PI controller (SpeedController.slx, tuned in design_SpeedPI.m)
Duty_Max = 100;                     % Controller output limit [%]
% design_SpeedPI.m: pidtune, wc = 8 rad/s, PM 65 deg, GM 15.3 dB
% M2 sim: max OS 6.4 %, max settling (+/-5 %) 0.28 s on 150 rpm steps
Kp_Speed = 0.01617;                 % Proportional gain [%/rpm]
Ki_Speed = 0.3778;                  % Integral gain [%/(rpm*s)]

%% Friction compensation feedforward (SpeedController.slx/FrictionFF), fitted on M3 run1
%  Steady-state duty per level: duty = 0.04525*ref + 1.024*sign(ref)  (max residual 0.65 %)
%  Breakaway from standstill observed at about -14.6 % duty (-150 rpm step at t = 22 s)
%  ff = FF_Enable*( Duty_Coulomb*sign(ref) + Duty_StictionBoost*sign(ref)*(|rpm_meas| < Stick_RPM) )
%  The PI output limits are Duty_Max - ff / -Duty_Max - ff (external limits), so
%  PI + ff stays within +/-Duty_Max and clamping anti-windup still works.
%  A velocity feedforward (0.04525 %/rpm) was evaluated and dropped: in simulation it
%  raised the overshoot on normal 150 rpm steps from 9 to 60-87 rpm (P part adds on
%  top of a correct feedforward while the measured speed lags ~40 ms).
%  Boost sweep with friction model (verify_FrictionFF.m): 0 % -> 154 rpm breakaway
%  overshoot, 5 % -> 65, 7 % -> 61, 9 % -> 48 (settles 0.09 s), 11 % -> 51, 15 % -> 102.
FF_Enable          = 1;             % 1 = friction compensation on, 0 = pure PI (tunable)
Duty_Coulomb       = 1.024;         % Kinetic friction offset [%]
Duty_StictionBoost = 9.0;           % Extra duty while at standstill [%]
Stick_RPM          = 5;             % Standstill threshold on measured speed [rpm]

%% Friction model for simulation only (DCMotor_M2_PISim/MotorFriction)
Sim_FrictionEnable = 1;
Sim_DutyBreakaway  = 14.0;          % Duty needed to start from standstill [%]
Sim_StickRPM       = 5;             % Below this the motor is considered stuck [rpm]
