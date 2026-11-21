/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: DCMotor_M1_OpenLoop.c
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
#include "MW_PWM.h"
#include "MW_arduino_digitalio.h"
#include "MW_AnalogIn.h"
#include "MW_ArduinoEncoder.h"
#include "DCMotor_M1_OpenLoop_private.h"
#include "rtwtypes.h"
#include <math.h>

/* Block signals (default storage) */
B_DCMotor_M1_OpenLoop_T DCMotor_M1_OpenLoop_B;

/* Block states (default storage) */
DW_DCMotor_M1_OpenLoop_T DCMotor_M1_OpenLoop_DW;

/* Real-time model */
static RT_MODEL_DCMotor_M1_OpenLoop_T DCMotor_M1_OpenLoop_M_;
RT_MODEL_DCMotor_M1_OpenLoop_T *const DCMotor_M1_OpenLoop_M =
  &DCMotor_M1_OpenLoop_M_;
uint32_T plook_u32d_bincka(real_T u, const real_T bp[], uint32_T maxIndex)
{
  uint32_T bpIndex;

  /* Prelookup - Index only
     Index Search method: 'binary'
     Extrapolation method: 'Clip'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'on'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u <= bp[0UL]) {
    bpIndex = 0UL;
  } else if (u < bp[maxIndex]) {
    bpIndex = binsearch_u32d(u, bp, maxIndex >> 1UL, maxIndex);
  } else {
    bpIndex = maxIndex;
  }

  return bpIndex;
}

uint32_T binsearch_u32d(real_T u, const real_T bp[], uint32_T startIndex,
  uint32_T maxIndex)
{
  uint32_T bpIdx;
  uint32_T bpIndex;
  uint32_T iRght;

  /* Binary Search */
  bpIdx = startIndex;
  bpIndex = 0UL;
  iRght = maxIndex;
  while (iRght - bpIndex > 1UL) {
    if (u < bp[bpIdx]) {
      iRght = bpIdx;
    } else {
      bpIndex = bpIdx;
    }

    bpIdx = (iRght + bpIndex) >> 1UL;
  }

  return bpIndex;
}

real32_T rt_roundf_snf(real32_T u)
{
  real32_T y;
  if ((real32_T)fabs(u) < 8.388608E+6F) {
    if (u >= 0.5F) {
      y = (real32_T)floor(u + 0.5F);
    } else if (u > -0.5F) {
      y = u * 0.0F;
    } else {
      y = (real32_T)ceil(u - 0.5F);
    }
  } else {
    y = u;
  }

  return y;
}

/* Model step function */
void DCMotor_M1_OpenLoop_step(void)
{
  real_T y;
  real32_T rtb_Switch_idx_0;
  real32_T rtb_Switch_idx_1;
  uint16_T b_varargout_1;
  uint8_T tmp;

  /* Gain: '<Root>/OL_Scale' incorporates:
   *  DigitalClock: '<Root>/Time'
   *  Lookup_n-D: '<Root>/OL_DutyProfile'
   */
  DCMotor_M1_OpenLoop_B.duty_cmd = DCMotor_M1_OpenLoop_P.OL_Scale *
    DCMotor_M1_OpenLoop_P.OL_Duty[plook_u32d_bincka
    (((DCMotor_M1_OpenLoop_M->Timing.clockTick0) * 0.01),
     DCMotor_M1_OpenLoop_P.OL_Time, 5UL)];

  /* Saturate: '<Root>/DutyLimit' */
  if (DCMotor_M1_OpenLoop_B.duty_cmd > DCMotor_M1_OpenLoop_P.DutyLimit_UpperSat)
  {
    /* Gain: '<Root>/OL_Scale' incorporates:
     *  Saturate: '<Root>/DutyLimit'
     */
    DCMotor_M1_OpenLoop_B.duty_cmd = DCMotor_M1_OpenLoop_P.DutyLimit_UpperSat;
  } else if (DCMotor_M1_OpenLoop_B.duty_cmd <
             DCMotor_M1_OpenLoop_P.DutyLimit_LowerSat) {
    /* Gain: '<Root>/OL_Scale' incorporates:
     *  Saturate: '<Root>/DutyLimit'
     */
    DCMotor_M1_OpenLoop_B.duty_cmd = DCMotor_M1_OpenLoop_P.DutyLimit_LowerSat;
  }

  /* End of Saturate: '<Root>/DutyLimit' */

  /* Switch: '<S1>/Switch' incorporates:
   *  Constant: '<S1>/Constant4'
   *  Constant: '<S1>/Constant5'
   */
  if (DCMotor_M1_OpenLoop_B.duty_cmd > DCMotor_M1_OpenLoop_P.Switch_Threshold) {
    /* Gain: '<S1>/Gain' */
    rtb_Switch_idx_0 = (real32_T)(DCMotor_M1_OpenLoop_P.Gain_Gain *
      DCMotor_M1_OpenLoop_B.duty_cmd);

    /* Saturate: '<S1>/Saturation' */
    if (rtb_Switch_idx_0 > DCMotor_M1_OpenLoop_P.Saturation_UpperSat) {
      rtb_Switch_idx_0 = DCMotor_M1_OpenLoop_P.Saturation_UpperSat;
    } else if (rtb_Switch_idx_0 < DCMotor_M1_OpenLoop_P.Saturation_LowerSat) {
      rtb_Switch_idx_0 = DCMotor_M1_OpenLoop_P.Saturation_LowerSat;
    }

    /* End of Saturate: '<S1>/Saturation' */
    rtb_Switch_idx_1 = DCMotor_M1_OpenLoop_P.Constant4_Value;
  } else {
    rtb_Switch_idx_0 = DCMotor_M1_OpenLoop_P.Constant5_Value;

    /* Gain: '<S1>/Gain1' incorporates:
     *  Constant: '<S1>/Constant5'
     */
    rtb_Switch_idx_1 = (real32_T)(DCMotor_M1_OpenLoop_P.Gain1_Gain *
      DCMotor_M1_OpenLoop_B.duty_cmd);

    /* Saturate: '<S1>/Saturation1' */
    if (rtb_Switch_idx_1 > DCMotor_M1_OpenLoop_P.Saturation1_UpperSat) {
      rtb_Switch_idx_1 = DCMotor_M1_OpenLoop_P.Saturation1_UpperSat;
    } else if (rtb_Switch_idx_1 < DCMotor_M1_OpenLoop_P.Saturation1_LowerSat) {
      rtb_Switch_idx_1 = DCMotor_M1_OpenLoop_P.Saturation1_LowerSat;
    }

    /* End of Saturate: '<S1>/Saturation1' */
  }

  /* End of Switch: '<S1>/Switch' */

  /* MATLABSystem: '<S1>/PWM' */
  DCMotor_M1_OpenLoop_DW.obj_b.PWMDriverObj.MW_PWM_HANDLE = MW_PWM_GetHandle
    (10UL);
  if (rtb_Switch_idx_1 <= 255.0F) {
    y = rtb_Switch_idx_1;
  } else {
    y = 255.0;
  }

  if (!(y >= 0.0)) {
    y = 0.0;
  }

  MW_PWM_SetDutyCycle(DCMotor_M1_OpenLoop_DW.obj_b.PWMDriverObj.MW_PWM_HANDLE, y);

  /* End of MATLABSystem: '<S1>/PWM' */

  /* MATLABSystem: '<S1>/PWM1' */
  DCMotor_M1_OpenLoop_DW.obj_ln.PWMDriverObj.MW_PWM_HANDLE = MW_PWM_GetHandle
    (11UL);
  if (rtb_Switch_idx_0 <= 255.0F) {
    y = rtb_Switch_idx_0;
  } else {
    y = 255.0;
  }

  if (!(y >= 0.0)) {
    y = 0.0;
  }

  MW_PWM_SetDutyCycle(DCMotor_M1_OpenLoop_DW.obj_ln.PWMDriverObj.MW_PWM_HANDLE,
                      y);

  /* End of MATLABSystem: '<S1>/PWM1' */

  /* DiscreteTransferFcn: '<S3>/RPM_LPF' */
  DCMotor_M1_OpenLoop_B.rpm = DCMotor_M1_OpenLoop_P.RPM_LPF_Num *
    DCMotor_M1_OpenLoop_DW.RPM_LPF_states;

  /* DiscreteTransferFcn: '<S2>/Cur_LPF' */
  DCMotor_M1_OpenLoop_B.current_A = DCMotor_M1_OpenLoop_P.Cur_LPF_Num *
    DCMotor_M1_OpenLoop_DW.Cur_LPF_states;

  /* MATLABSystem: '<S1>/Digital Output' incorporates:
   *  Constant: '<S1>/Constant2'
   */
  rtb_Switch_idx_0 = rt_roundf_snf(DCMotor_M1_OpenLoop_P.Constant2_Value);
  if (rtb_Switch_idx_0 < 256.0F) {
    if (rtb_Switch_idx_0 >= 0.0F) {
      tmp = (uint8_T)rtb_Switch_idx_0;
    } else {
      tmp = 0U;
    }
  } else {
    tmp = MAX_uint8_T;
  }

  writeDigitalPin(8, tmp);

  /* End of MATLABSystem: '<S1>/Digital Output' */

  /* MATLABSystem: '<S1>/Digital Output1' */
  if (rtb_Switch_idx_0 < 256.0F) {
    if (rtb_Switch_idx_0 >= 0.0F) {
      tmp = (uint8_T)rtb_Switch_idx_0;
    } else {
      tmp = 0U;
    }
  } else {
    tmp = MAX_uint8_T;
  }

  writeDigitalPin(9, tmp);

  /* End of MATLABSystem: '<S1>/Digital Output1' */

  /* MATLABSystem: '<S2>/AnalogInput_A3' */
  DCMotor_M1_OpenLoop_DW.obj_e.AnalogInDriverObj.MW_ANALOGIN_HANDLE =
    MW_AnalogIn_GetHandle(17UL);
  MW_AnalogInSingle_ReadResult
    (DCMotor_M1_OpenLoop_DW.obj_e.AnalogInDriverObj.MW_ANALOGIN_HANDLE,
     &b_varargout_1, MW_ANALOGIN_UINT16);

  /* Gain: '<S2>/ADCToVolt' incorporates:
   *  DataTypeConversion: '<S2>/ToDouble'
   *  MATLABSystem: '<S2>/AnalogInput_A3'
   * */
  DCMotor_M1_OpenLoop_B.sensor_V = DCMotor_M1_OpenLoop_P.Cur_VRef /
    DCMotor_M1_OpenLoop_P.Cur_ADCMax * (real_T)b_varargout_1;

  /* MATLABSystem: '<S3>/Encoder' */
  if (DCMotor_M1_OpenLoop_DW.obj_j.TunablePropsChanged) {
    DCMotor_M1_OpenLoop_DW.obj_j.TunablePropsChanged = false;
  }

  /* MATLABSystem: '<S3>/Encoder' */
  MW_EncoderRead(DCMotor_M1_OpenLoop_DW.obj_j.Index,
                 &DCMotor_M1_OpenLoop_B.counts);

  /* Gain: '<S3>/CountsToRPM' incorporates:
   *  DataTypeConversion: '<S3>/ToDouble'
   *  Sum: '<S4>/Diff'
   *  UnitDelay: '<S4>/UD'
   *
   * Block description for '<S4>/Diff':
   *
   *  Add in CPU
   *
   * Block description for '<S4>/UD':
   *
   *  Store in Global RAM
   */
  DCMotor_M1_OpenLoop_B.rpm_raw = (real_T)(DCMotor_M1_OpenLoop_B.counts -
    DCMotor_M1_OpenLoop_DW.UD_DSTATE) * DCMotor_M1_OpenLoop_P.Enc_RPMGain;

  /* Update for DiscreteTransferFcn: '<S3>/RPM_LPF' */
  DCMotor_M1_OpenLoop_DW.RPM_LPF_states = (DCMotor_M1_OpenLoop_B.rpm_raw -
    DCMotor_M1_OpenLoop_P.RPM_LPF_Den[1L] *
    DCMotor_M1_OpenLoop_DW.RPM_LPF_states) / DCMotor_M1_OpenLoop_P.RPM_LPF_Den[0];

  /* Update for DiscreteTransferFcn: '<S2>/Cur_LPF' incorporates:
   *  Constant: '<S2>/ZeroOffsetV'
   *  Gain: '<S2>/VoltToAmp'
   *  Sum: '<S2>/RemoveOffset'
   */
  DCMotor_M1_OpenLoop_DW.Cur_LPF_states = (DCMotor_M1_OpenLoop_P.Cur_Dir /
    DCMotor_M1_OpenLoop_P.Cur_Sens * (DCMotor_M1_OpenLoop_B.sensor_V -
    DCMotor_M1_OpenLoop_P.Cur_OffsetV) - DCMotor_M1_OpenLoop_P.Cur_LPF_Den[1L] *
    DCMotor_M1_OpenLoop_DW.Cur_LPF_states) / DCMotor_M1_OpenLoop_P.Cur_LPF_Den[0];

  /* Update for UnitDelay: '<S4>/UD'
   *
   * Block description for '<S4>/UD':
   *
   *  Store in Global RAM
   */
  DCMotor_M1_OpenLoop_DW.UD_DSTATE = DCMotor_M1_OpenLoop_B.counts;

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The resolution of this integer timer is 0.01, which is the step size
   * of the task. Size of "clockTick0" ensures timer will not overflow during the
   * application lifespan selected.
   */
  DCMotor_M1_OpenLoop_M->Timing.clockTick0++;
}

/* Model initialize function */
void DCMotor_M1_OpenLoop_initialize(void)
{
  /* Registration code */
  rtmSetTFinal(DCMotor_M1_OpenLoop_M, 18.0);

  /* External mode info */
  DCMotor_M1_OpenLoop_M->Sizes.checksums[0] = (3022273519U);
  DCMotor_M1_OpenLoop_M->Sizes.checksums[1] = (963550913U);
  DCMotor_M1_OpenLoop_M->Sizes.checksums[2] = (2898776778U);
  DCMotor_M1_OpenLoop_M->Sizes.checksums[3] = (2196679872U);

  {
    static const sysRanDType rtAlwaysEnabled = SUBSYS_RAN_BC_ENABLE;
    static RTWExtModeInfo rt_ExtModeInfo;
    static const sysRanDType *systemRan[9];
    DCMotor_M1_OpenLoop_M->extModeInfo = (&rt_ExtModeInfo);
    rteiSetSubSystemActiveVectorAddresses(&rt_ExtModeInfo, systemRan);
    systemRan[0] = &rtAlwaysEnabled;
    systemRan[1] = &rtAlwaysEnabled;
    systemRan[2] = &rtAlwaysEnabled;
    systemRan[3] = &rtAlwaysEnabled;
    systemRan[4] = &rtAlwaysEnabled;
    systemRan[5] = &rtAlwaysEnabled;
    systemRan[6] = &rtAlwaysEnabled;
    systemRan[7] = &rtAlwaysEnabled;
    systemRan[8] = &rtAlwaysEnabled;
    rteiSetModelMappingInfoPtr(DCMotor_M1_OpenLoop_M->extModeInfo,
      &DCMotor_M1_OpenLoop_M->SpecialInfo.mappingInfo);
    rteiSetChecksumsPtr(DCMotor_M1_OpenLoop_M->extModeInfo,
                        DCMotor_M1_OpenLoop_M->Sizes.checksums);
    rteiSetTFinalTicks(DCMotor_M1_OpenLoop_M->extModeInfo, 1800);
  }

  /* InitializeConditions for DiscreteTransferFcn: '<S3>/RPM_LPF' */
  DCMotor_M1_OpenLoop_DW.RPM_LPF_states =
    DCMotor_M1_OpenLoop_P.RPM_LPF_InitialStates;

  /* InitializeConditions for DiscreteTransferFcn: '<S2>/Cur_LPF' */
  DCMotor_M1_OpenLoop_DW.Cur_LPF_states =
    DCMotor_M1_OpenLoop_P.Cur_LPF_InitialStates;

  /* InitializeConditions for UnitDelay: '<S4>/UD'
   *
   * Block description for '<S4>/UD':
   *
   *  Store in Global RAM
   */
  DCMotor_M1_OpenLoop_DW.UD_DSTATE =
    DCMotor_M1_OpenLoop_P.DeltaCount_ICPrevInput;

  /* Start for MATLABSystem: '<S1>/PWM' */
  DCMotor_M1_OpenLoop_DW.obj_b.matlabCodegenIsDeleted = false;
  DCMotor_M1_OpenLoop_DW.obj_b.isInitialized = 1L;
  DCMotor_M1_OpenLoop_DW.obj_b.PWMDriverObj.MW_PWM_HANDLE = MW_PWM_Open(10UL,
    0.0, 0.0);
  DCMotor_M1_OpenLoop_DW.obj_b.isSetupComplete = true;

  /* Start for MATLABSystem: '<S1>/PWM1' */
  DCMotor_M1_OpenLoop_DW.obj_ln.matlabCodegenIsDeleted = false;
  DCMotor_M1_OpenLoop_DW.obj_ln.isInitialized = 1L;
  DCMotor_M1_OpenLoop_DW.obj_ln.PWMDriverObj.MW_PWM_HANDLE = MW_PWM_Open(11UL,
    0.0, 0.0);
  DCMotor_M1_OpenLoop_DW.obj_ln.isSetupComplete = true;

  /* Start for MATLABSystem: '<S1>/Digital Output' */
  DCMotor_M1_OpenLoop_DW.obj_l.matlabCodegenIsDeleted = false;
  DCMotor_M1_OpenLoop_DW.obj_l.isInitialized = 1L;
  digitalIOSetup(8, 1);
  DCMotor_M1_OpenLoop_DW.obj_l.isSetupComplete = true;

  /* Start for MATLABSystem: '<S1>/Digital Output1' */
  DCMotor_M1_OpenLoop_DW.obj.matlabCodegenIsDeleted = false;
  DCMotor_M1_OpenLoop_DW.obj.isInitialized = 1L;
  digitalIOSetup(9, 1);
  DCMotor_M1_OpenLoop_DW.obj.isSetupComplete = true;

  /* Start for MATLABSystem: '<S2>/AnalogInput_A3' */
  DCMotor_M1_OpenLoop_DW.obj_e.matlabCodegenIsDeleted = false;
  DCMotor_M1_OpenLoop_DW.obj_e.isInitialized = 1L;
  DCMotor_M1_OpenLoop_DW.obj_e.AnalogInDriverObj.MW_ANALOGIN_HANDLE =
    MW_AnalogInSingle_Open(17UL);
  DCMotor_M1_OpenLoop_DW.obj_e.isSetupComplete = true;

  /* Start for MATLABSystem: '<S3>/Encoder' */
  DCMotor_M1_OpenLoop_DW.obj_j.Index = 0U;
  DCMotor_M1_OpenLoop_DW.obj_j.matlabCodegenIsDeleted = false;
  DCMotor_M1_OpenLoop_DW.obj_j.isInitialized = 1L;
  MW_EncoderSetup(2UL, 3UL, &DCMotor_M1_OpenLoop_DW.obj_j.Index);
  DCMotor_M1_OpenLoop_DW.obj_j.isSetupComplete = true;
  DCMotor_M1_OpenLoop_DW.obj_j.TunablePropsChanged = false;

  /* InitializeConditions for MATLABSystem: '<S3>/Encoder' */
  MW_EncoderReset(DCMotor_M1_OpenLoop_DW.obj_j.Index);
}

/* Model terminate function */
void DCMotor_M1_OpenLoop_terminate(void)
{
  /* Terminate for MATLABSystem: '<S1>/PWM' */
  if (!DCMotor_M1_OpenLoop_DW.obj_b.matlabCodegenIsDeleted) {
    DCMotor_M1_OpenLoop_DW.obj_b.matlabCodegenIsDeleted = true;
    if ((DCMotor_M1_OpenLoop_DW.obj_b.isInitialized == 1L) &&
        DCMotor_M1_OpenLoop_DW.obj_b.isSetupComplete) {
      DCMotor_M1_OpenLoop_DW.obj_b.PWMDriverObj.MW_PWM_HANDLE = MW_PWM_GetHandle
        (10UL);
      MW_PWM_SetDutyCycle
        (DCMotor_M1_OpenLoop_DW.obj_b.PWMDriverObj.MW_PWM_HANDLE, 0.0);
      DCMotor_M1_OpenLoop_DW.obj_b.PWMDriverObj.MW_PWM_HANDLE = MW_PWM_GetHandle
        (10UL);
      MW_PWM_Close(DCMotor_M1_OpenLoop_DW.obj_b.PWMDriverObj.MW_PWM_HANDLE);
    }
  }

  /* End of Terminate for MATLABSystem: '<S1>/PWM' */

  /* Terminate for MATLABSystem: '<S1>/PWM1' */
  if (!DCMotor_M1_OpenLoop_DW.obj_ln.matlabCodegenIsDeleted) {
    DCMotor_M1_OpenLoop_DW.obj_ln.matlabCodegenIsDeleted = true;
    if ((DCMotor_M1_OpenLoop_DW.obj_ln.isInitialized == 1L) &&
        DCMotor_M1_OpenLoop_DW.obj_ln.isSetupComplete) {
      DCMotor_M1_OpenLoop_DW.obj_ln.PWMDriverObj.MW_PWM_HANDLE =
        MW_PWM_GetHandle(11UL);
      MW_PWM_SetDutyCycle
        (DCMotor_M1_OpenLoop_DW.obj_ln.PWMDriverObj.MW_PWM_HANDLE, 0.0);
      DCMotor_M1_OpenLoop_DW.obj_ln.PWMDriverObj.MW_PWM_HANDLE =
        MW_PWM_GetHandle(11UL);
      MW_PWM_Close(DCMotor_M1_OpenLoop_DW.obj_ln.PWMDriverObj.MW_PWM_HANDLE);
    }
  }

  /* End of Terminate for MATLABSystem: '<S1>/PWM1' */

  /* Terminate for MATLABSystem: '<S1>/Digital Output' */
  if (!DCMotor_M1_OpenLoop_DW.obj_l.matlabCodegenIsDeleted) {
    DCMotor_M1_OpenLoop_DW.obj_l.matlabCodegenIsDeleted = true;
  }

  /* End of Terminate for MATLABSystem: '<S1>/Digital Output' */

  /* Terminate for MATLABSystem: '<S1>/Digital Output1' */
  if (!DCMotor_M1_OpenLoop_DW.obj.matlabCodegenIsDeleted) {
    DCMotor_M1_OpenLoop_DW.obj.matlabCodegenIsDeleted = true;
  }

  /* End of Terminate for MATLABSystem: '<S1>/Digital Output1' */

  /* Terminate for MATLABSystem: '<S2>/AnalogInput_A3' */
  if (!DCMotor_M1_OpenLoop_DW.obj_e.matlabCodegenIsDeleted) {
    DCMotor_M1_OpenLoop_DW.obj_e.matlabCodegenIsDeleted = true;
    if ((DCMotor_M1_OpenLoop_DW.obj_e.isInitialized == 1L) &&
        DCMotor_M1_OpenLoop_DW.obj_e.isSetupComplete) {
      DCMotor_M1_OpenLoop_DW.obj_e.AnalogInDriverObj.MW_ANALOGIN_HANDLE =
        MW_AnalogIn_GetHandle(17UL);
      MW_AnalogIn_Close
        (DCMotor_M1_OpenLoop_DW.obj_e.AnalogInDriverObj.MW_ANALOGIN_HANDLE);
    }
  }

  /* End of Terminate for MATLABSystem: '<S2>/AnalogInput_A3' */

  /* Terminate for MATLABSystem: '<S3>/Encoder' */
  if (!DCMotor_M1_OpenLoop_DW.obj_j.matlabCodegenIsDeleted) {
    DCMotor_M1_OpenLoop_DW.obj_j.matlabCodegenIsDeleted = true;
    if ((DCMotor_M1_OpenLoop_DW.obj_j.isInitialized == 1L) &&
        DCMotor_M1_OpenLoop_DW.obj_j.isSetupComplete) {
      MW_EncoderRelease();
    }
  }

  /* End of Terminate for MATLABSystem: '<S3>/Encoder' */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
