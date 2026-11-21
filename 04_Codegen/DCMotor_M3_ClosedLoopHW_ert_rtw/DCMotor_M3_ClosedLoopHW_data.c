/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: DCMotor_M3_ClosedLoopHW_data.c
 *
 * Code generated for Simulink model 'DCMotor_M3_ClosedLoopHW'.
 *
 * Model version                  : 1.6
 * Simulink Coder version         : 26.2 (R2026b) 22-May-2026
 * C/C++ source code generated on : Sat Nov 21 16:36:12 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Atmel->AVR
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "DCMotor_M3_ClosedLoopHW.h"

/* Block parameters (default storage) */
P_DCMotor_M3_ClosedLoopHW_T DCMotor_M3_ClosedLoopHW_P = {
  /* Variable: Cur_ADCMax
   * Referenced by: '<S2>/ADCToVolt'
   */
  1023.0,

  /* Variable: Cur_Dir
   * Referenced by: '<S2>/VoltToAmp'
   */
  -1.0,

  /* Variable: Cur_LPF_Den
   * Referenced by: '<S2>/Cur_LPF'
   */
  { 1.0, -0.83333333333333337 },

  /* Variable: Cur_LPF_Num
   * Referenced by: '<S2>/Cur_LPF'
   */
  0.16666666666666666,

  /* Variable: Cur_OffsetV
   * Referenced by: '<S2>/ZeroOffsetV'
   */
  2.5643,

  /* Variable: Cur_Sens
   * Referenced by: '<S2>/VoltToAmp'
   */
  0.185,

  /* Variable: Cur_VRef
   * Referenced by: '<S2>/ADCToVolt'
   */
  5.0,

  /* Variable: Duty_Coulomb
   * Referenced by: '<S8>/CoulombFF'
   */
  1.024,

  /* Variable: Duty_Max
   * Referenced by:
   *   '<S4>/DutyMax'
   *   '<S4>/DutyMin'
   *   '<S4>/DutyLimit'
   */
  100.0,

  /* Variable: Duty_StictionBoost
   * Referenced by: '<S8>/StictionBoost'
   */
  9.0,

  /* Variable: Enc_RPMGain
   * Referenced by: '<S3>/CountsToRPM'
   */
  23.534090909090907,

  /* Variable: FF_Enable
   * Referenced by: '<S8>/FF_Enable'
   */
  1.0,

  /* Variable: Ki_Speed
   * Referenced by: '<S45>/Integral Gain'
   */
  0.3778,

  /* Variable: Kp_Speed
   * Referenced by: '<S53>/Proportional Gain'
   */
  0.01617,

  /* Variable: RPM_LPF_Den
   * Referenced by: '<S3>/RPM_LPF'
   */
  { 1.0, -0.8 },

  /* Variable: RPM_LPF_Num
   * Referenced by: '<S3>/RPM_LPF'
   */
  0.19999999999999998,

  /* Variable: Ref_Min
   * Referenced by: '<S5>/RefLimit'
   */
  -1500.0,

  /* Variable: Ref_Period
   * Referenced by: '<S5>/PerPeriod'
   */
  2.0,

  /* Variable: Ref_Start
   * Referenced by:
   *   '<S5>/StartLevel'
   *   '<S5>/RefLimit'
   */
  1500.0,

  /* Variable: Ref_Step
   * Referenced by: '<S5>/StepSize'
   */
  150.0,

  /* Variable: Ref_StopTime
   * Referenced by: '<S6>/Constant'
   */
  42.0,

  /* Variable: Stick_RPM
   * Referenced by: '<S10>/Constant'
   */
  5.0,

  /* Mask Parameter: DeltaCount_ICPrevInput
   * Referenced by: '<S7>/UD'
   */
  0L,

  /* Expression: 255/100
   * Referenced by: '<S1>/Gain'
   */
  2.55,

  /* Expression: -255/100
   * Referenced by: '<S1>/Gain1'
   */
  -2.55,

  /* Expression: 0
   * Referenced by: '<S37>/Constant1'
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S3>/RPM_LPF'
   */
  0.0,

  /* Computed Parameter: Integrator_gainval
   * Referenced by: '<S48>/Integrator'
   */
  0.01,

  /* Expression: 0
   * Referenced by: '<S48>/Integrator'
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S1>/Switch'
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S2>/Cur_LPF'
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S37>/Clamping_zero'
   */
  0.0,

  /* Computed Parameter: Saturation_UpperSat
   * Referenced by: '<S1>/Saturation'
   */
  255.0F,

  /* Computed Parameter: Saturation_LowerSat
   * Referenced by: '<S1>/Saturation'
   */
  0.0F,

  /* Computed Parameter: Constant4_Value
   * Referenced by: '<S1>/Constant4'
   */
  0.0F,

  /* Computed Parameter: Constant5_Value
   * Referenced by: '<S1>/Constant5'
   */
  0.0F,

  /* Computed Parameter: Saturation1_UpperSat
   * Referenced by: '<S1>/Saturation1'
   */
  255.0F,

  /* Computed Parameter: Saturation1_LowerSat
   * Referenced by: '<S1>/Saturation1'
   */
  0.0F,

  /* Computed Parameter: Constant2_Value
   * Referenced by: '<S1>/Constant2'
   */
  1.0F,

  /* Computed Parameter: Constant_Value
   * Referenced by: '<S37>/Constant'
   */
  1,

  /* Computed Parameter: Constant2_Value_m
   * Referenced by: '<S37>/Constant2'
   */
  -1,

  /* Computed Parameter: Constant3_Value
   * Referenced by: '<S37>/Constant3'
   */
  1,

  /* Computed Parameter: Constant4_Value_c
   * Referenced by: '<S37>/Constant4'
   */
  -1
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
