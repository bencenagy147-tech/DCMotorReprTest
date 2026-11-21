/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: DCMotor_M1_OpenLoop_data.c
 *
 * Code generated for Simulink model 'DCMotor_M1_OpenLoop'.
 *
 * Model version                  : 1.3
 * Simulink Coder version         : 26.2 (R2026b) 22-May-2026
 * C/C++ source code generated on : Sat Nov 21 16:10:56 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Atmel->AVR
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "DCMotor_M1_OpenLoop.h"

/* Block parameters (default storage) */
P_DCMotor_M1_OpenLoop_T DCMotor_M1_OpenLoop_P = {
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

  /* Variable: Enc_RPMGain
   * Referenced by: '<S3>/CountsToRPM'
   */
  23.534090909090907,

  /* Variable: OL_Duty
   * Referenced by: '<Root>/OL_DutyProfile'
   */
  { 0.0, 100.0, 0.0, -100.0, 0.0, 0.0 },

  /* Variable: OL_Scale
   * Referenced by: '<Root>/OL_Scale'
   */
  1.0,

  /* Variable: OL_Time
   * Referenced by: '<Root>/OL_DutyProfile'
   */
  { 0.0, 2.0, 6.0, 9.0, 13.0, 16.0 },

  /* Variable: RPM_LPF_Den
   * Referenced by: '<S3>/RPM_LPF'
   */
  { 1.0, -0.8 },

  /* Variable: RPM_LPF_Num
   * Referenced by: '<S3>/RPM_LPF'
   */
  0.19999999999999998,

  /* Mask Parameter: DeltaCount_ICPrevInput
   * Referenced by: '<S4>/UD'
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

  /* Expression: 100
   * Referenced by: '<Root>/DutyLimit'
   */
  100.0,

  /* Expression: -100
   * Referenced by: '<Root>/DutyLimit'
   */
  -100.0,

  /* Expression: 0
   * Referenced by: '<S1>/Switch'
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S3>/RPM_LPF'
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S2>/Cur_LPF'
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
  1.0F
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
