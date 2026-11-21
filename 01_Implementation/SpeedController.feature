# --- front-matter:toml ---
model = "DCMotor_M2_PISim.slx"
component = "DCMotor_M2_PISim/SpeedController"
[inputs]
Ref = "rpm_ref"
Meas = "rpm_meas"
[outputs]
Duty = "duty_cmd"
# --- end front-matter ---

Feature: Speed PI controller with friction compensation
  PI: Kp_Speed = 0.01617, Ki_Speed = 0.3778, Ts = 10 ms, output +/-100 %, clamping anti-windup
  with external limits (100 % - duty_ff, -100 % - duty_ff).
  Friction feedforward: duty_ff = sign(ref) * (Duty_Coulomb 1.024 % + Duty_StictionBoost 9 % while |rpm_meas| < 5 rpm).
  The PI scenarios use Ref = 0 (feedforward is then zero) and create the error through Meas.

Scenario: Zero error gives zero duty
  Reference and measurement are both zero, so neither the PI nor the feedforward acts.
  Given inputs
    * Ref = const(0)
    * Meas = const(0)
  When simulate for 3s in Normal mode
  Then outputs
    * NoDrift: Duty == [-0.001 .. 0.001]

Scenario: Proportional and integral action on constant error
  A constant 100 rpm error gives an immediate P part of about 1.6 % and an integral ramp of about 37.8 %/s.
  Given inputs
    * Ref = const(0)
    * Meas = const(-100)
  When simulate for 2s in Normal mode
  Then outputs
    * PositiveDuty: Duty > 0
    * ProportionalStart: Duty == [1.5 .. 2.5] when t < 20ms
    * IntegralAfterOneSecond: Duty == [36 .. 42] when t == 1s
    * WithinLimits: Duty <= 100

Scenario: Positive saturation at plus 100 percent
  A large positive error drives the output to the upper limit and no further.
  Given inputs
    * Ref = const(0)
    * Meas = const(-2000)
  When simulate for 3s in Normal mode
  Then outputs
    * UpperLimit: Duty <= 100
    * Saturated: Duty == [99.9 .. 100] when t > 1s

Scenario: Negative saturation at minus 100 percent
  A large negative error drives the output to the lower limit and no further.
  Given inputs
    * Ref = const(0)
    * Meas = const(2000)
  When simulate for 3s in Normal mode
  Then outputs
    * LowerLimit: Duty >= -100
    * Saturated: Duty == [-100 .. -99.9] when t > 1s

Scenario: Anti-windup allows fast reversal after saturation
  The output saturates at plus 100 % for 2 s; then the error reverses.
  With clamping the integrator stops at the limit, so the output turns negative within 100 ms.
  Given inputs
    * Ref = const(0)
    * Meas = step(-2000 -> 2000 @ 2s)
  When simulate for 4s in Normal mode
  Then outputs
    * PositiveBefore: Duty > 0 when t < 2s
    * NegativeQuickly: Duty < 0 when t > 2.1s
    * ReverseSaturated: Duty == [-100 .. -99.9] when t > 2.5s

Scenario: Coulomb feedforward while moving
  Zero error at 500 rpm: the motor is moving, so only the Coulomb term remains in the output.
  Given inputs
    * Ref = const(500)
    * Meas = const(500)
  When simulate for 2s in Normal mode
  Then outputs
    * CoulombOnly: Duty == [1.02 .. 1.03]

Scenario: Negative Coulomb feedforward while moving
  Zero error at -150 rpm: the Coulomb term follows the sign of the reference.
  Given inputs
    * Ref = const(-150)
    * Meas = const(-150)
  When simulate for 2s in Normal mode
  Then outputs
    * CoulombOnly: Duty == [-1.03 .. -1.02]

Scenario: Stiction boost at standstill positive
  Motor at standstill with a 150 rpm request: P part 2.4 % + Coulomb 1.0 % + boost 9 % at the first samples.
  Given inputs
    * Ref = const(150)
    * Meas = const(0)
  When simulate for 1s in Normal mode
  Then outputs
    * BoostedStart: Duty == [12 .. 13.5] when t < 20ms
    * Positive: Duty > 0

Scenario: Stiction boost at standstill negative
  Motor at standstill with a -150 rpm request: the boost acts in the negative direction.
  Given inputs
    * Ref = const(-150)
    * Meas = const(0)
  When simulate for 1s in Normal mode
  Then outputs
    * BoostedStart: Duty == [-13.5 .. -12] when t < 20ms
    * Negative: Duty < 0

Scenario: Total output limited with feedforward
  A large error together with the feedforward never exceeds plus 100 percent.
  Given inputs
    * Ref = const(2000)
    * Meas = const(0)
  When simulate for 3s in Normal mode
  Then outputs
    * UpperLimit: Duty <= 100
    * Saturated: Duty == [99.9 .. 100] when t > 1s

Scenario: Anti-windup with feedforward on full reversal
  Saturated at plus 100 percent with the feedforward active, then the reference reverses.
  Given inputs
    * Ref = step(2000 -> -2000 @ 2s)
    * Meas = const(0)
  When simulate for 4s in Normal mode
  Then outputs
    * NegativeQuickly: Duty < 0 when t > 2.1s
    * LowerLimit: Duty >= -100
    * ReverseSaturated: Duty == [-100 .. -99.9] when t > 2.5s
