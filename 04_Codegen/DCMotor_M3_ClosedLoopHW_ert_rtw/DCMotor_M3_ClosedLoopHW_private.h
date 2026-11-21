/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: DCMotor_M3_ClosedLoopHW_private.h
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

#ifndef DCMotor_M3_ClosedLoopHW_private_h_
#define DCMotor_M3_ClosedLoopHW_private_h_
#include "rtwtypes.h"
#include "multiword_types.h"
#include "DCMotor_M3_ClosedLoopHW_types.h"

/* Private macros used by the generated code to access rtModel */
#ifndef rtmSetTFinal
#define rtmSetTFinal(rtm, val)         ((rtm)->Timing.tFinal = (val))
#endif

extern real32_T rt_roundf_snf(real32_T u);

#endif                                 /* DCMotor_M3_ClosedLoopHW_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
