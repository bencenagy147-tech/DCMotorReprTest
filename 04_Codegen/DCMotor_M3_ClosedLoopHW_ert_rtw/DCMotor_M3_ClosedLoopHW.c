/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: DCMotor_M3_ClosedLoopHW.c
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
#include "MW_PWM.h"
#include "MW_arduino_digitalio.h"
#include "MW_ArduinoEncoder.h"
#include "MW_AnalogIn.h"
#include <math.h>
#include "rt_nonfinite.h"
#include "DCMotor_M3_ClosedLoopHW_private.h"
#include "rtwtypes.h"

/* Block signals (default storage) */
B_DCMotor_M3_ClosedLoopHW_T DCMotor_M3_ClosedLoopHW_B;

/* Block states (default storage) */
DW_DCMotor_M3_ClosedLoopHW_T DCMotor_M3_ClosedLoopHW_DW;

/* Real-time model */
static RT_MODEL_DCMotor_M3_ClosedLoo_T DCMotor_M3_ClosedLoopHW_M_;
RT_MODEL_DCMotor_M3_ClosedLoo_T *const DCMotor_M3_ClosedLoopHW_M =
  &DCMotor_M3_ClosedLoopHW_M_;
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
void DCMotor_M3_ClosedLoopHW_step(void)
{
  real_T rtb_IntegralGain;
  real_T rtb_PI_LowerLimit;
  real_T rtb_Sum;
  real_T rtb_duty_ff;
  real_T tmp;
  int32_T rtb_counts_0;
  real32_T rtb_Switch_k_idx_0;
  real32_T rtb_Switch_k_idx_1;
  uint16_T b_varargout_1;
  int8_T tmp_1;
  int8_T tmp_2;
  uint8_T tmp_0;

  /* DigitalClock: '<S5>/Time' incorporates:
   *  DigitalClock: '<Root>/Time'
   */
  DCMotor_M3_ClosedLoopHW_B.Diff =
    ((DCMotor_M3_ClosedLoopHW_M->Timing.clockTick0) * 0.01);

  /* Bias: '<S5>/StartLevel' incorporates:
   *  DigitalClock: '<S5>/Time'
   *  Gain: '<S5>/PerPeriod'
   *  Gain: '<S5>/StepSize'
   *  Rounding: '<S5>/StepIndex'
   */
  rtb_IntegralGain = floor(1.0 / DCMotor_M3_ClosedLoopHW_P.Ref_Period *
    DCMotor_M3_ClosedLoopHW_B.Diff) * -DCMotor_M3_ClosedLoopHW_P.Ref_Step +
    DCMotor_M3_ClosedLoopHW_P.Ref_Start;

  /* Saturate: '<S5>/RefLimit' */
  if (rtb_IntegralGain > DCMotor_M3_ClosedLoopHW_P.Ref_Start) {
    rtb_IntegralGain = DCMotor_M3_ClosedLoopHW_P.Ref_Start;
  } else if (rtb_IntegralGain < DCMotor_M3_ClosedLoopHW_P.Ref_Min) {
    rtb_IntegralGain = DCMotor_M3_ClosedLoopHW_P.Ref_Min;
  }

  /* Product: '<Root>/RefStopGate' incorporates:
   *  Constant: '<S6>/Constant'
   *  RelationalOperator: '<S6>/Compare'
   *  Saturate: '<S5>/RefLimit'
   */
  DCMotor_M3_ClosedLoopHW_B.rpm_ref = (real_T)(DCMotor_M3_ClosedLoopHW_B.Diff <
    DCMotor_M3_ClosedLoopHW_P.Ref_StopTime) * rtb_IntegralGain;

  /* Signum: '<S8>/RefSign' */
  if (rtIsNaN(DCMotor_M3_ClosedLoopHW_B.rpm_ref)) {
    rtb_duty_ff = (rtNaN);
  } else if (DCMotor_M3_ClosedLoopHW_B.rpm_ref < 0.0) {
    rtb_duty_ff = -1.0;
  } else {
    rtb_duty_ff = (DCMotor_M3_ClosedLoopHW_B.rpm_ref > 0.0);
  }

  /* End of Signum: '<S8>/RefSign' */

  /* DiscreteTransferFcn: '<S3>/RPM_LPF' */
  DCMotor_M3_ClosedLoopHW_B.rpm = DCMotor_M3_ClosedLoopHW_P.RPM_LPF_Num *
    DCMotor_M3_ClosedLoopHW_DW.RPM_LPF_states;

  /* Gain: '<S8>/FF_Enable' incorporates:
   *  Abs: '<S8>/AbsSpeed'
   *  Constant: '<S10>/Constant'
   *  Gain: '<S8>/CoulombFF'
   *  Gain: '<S8>/StictionBoost'
   *  Product: '<S8>/StictionDir'
   *  RelationalOperator: '<S10>/Compare'
   *  Sum: '<S8>/FFSum'
   */
  rtb_duty_ff = ((real_T)(fabs(DCMotor_M3_ClosedLoopHW_B.rpm) <
    DCMotor_M3_ClosedLoopHW_P.Stick_RPM) * rtb_duty_ff *
                 DCMotor_M3_ClosedLoopHW_P.Duty_StictionBoost +
                 DCMotor_M3_ClosedLoopHW_P.Duty_Coulomb * rtb_duty_ff) *
    DCMotor_M3_ClosedLoopHW_P.FF_Enable;

  /* Sum: '<S4>/PI_UpperLimit' incorporates:
   *  Constant: '<S4>/DutyMax'
   */
  DCMotor_M3_ClosedLoopHW_B.Diff = DCMotor_M3_ClosedLoopHW_P.Duty_Max -
    rtb_duty_ff;

  /* Sum: '<S4>/SpeedError' */
  rtb_IntegralGain = DCMotor_M3_ClosedLoopHW_B.rpm_ref -
    DCMotor_M3_ClosedLoopHW_B.rpm;

  /* Sum: '<S58>/Sum' incorporates:
   *  DiscreteIntegrator: '<S48>/Integrator'
   *  Gain: '<S53>/Proportional Gain'
   */
  rtb_Sum = DCMotor_M3_ClosedLoopHW_P.Kp_Speed * rtb_IntegralGain +
    DCMotor_M3_ClosedLoopHW_DW.Integrator_DSTATE;

  /* Sum: '<S4>/PI_LowerLimit' incorporates:
   *  Constant: '<S4>/DutyMin'
   */
  rtb_PI_LowerLimit = -DCMotor_M3_ClosedLoopHW_P.Duty_Max - rtb_duty_ff;

  /* Switch: '<S56>/Switch2' incorporates:
   *  RelationalOperator: '<S56>/LowerRelop1'
   *  RelationalOperator: '<S56>/UpperRelop'
   *  Switch: '<S56>/Switch'
   */
  if (rtb_Sum > DCMotor_M3_ClosedLoopHW_B.Diff) {
    tmp = DCMotor_M3_ClosedLoopHW_B.Diff;
  } else if (rtb_Sum < rtb_PI_LowerLimit) {
    /* Switch: '<S56>/Switch' */
    tmp = rtb_PI_LowerLimit;
  } else {
    tmp = rtb_Sum;
  }

  /* Sum: '<S4>/DutyTotal' incorporates:
   *  Switch: '<S56>/Switch2'
   */
  DCMotor_M3_ClosedLoopHW_B.duty_cmd = tmp + rtb_duty_ff;

  /* Saturate: '<S4>/DutyLimit' */
  if (DCMotor_M3_ClosedLoopHW_B.duty_cmd > DCMotor_M3_ClosedLoopHW_P.Duty_Max) {
    /* Sum: '<S4>/DutyTotal' incorporates:
     *  Saturate: '<S4>/DutyLimit'
     */
    DCMotor_M3_ClosedLoopHW_B.duty_cmd = DCMotor_M3_ClosedLoopHW_P.Duty_Max;
  } else if (DCMotor_M3_ClosedLoopHW_B.duty_cmd <
             -DCMotor_M3_ClosedLoopHW_P.Duty_Max) {
    /* Sum: '<S4>/DutyTotal' incorporates:
     *  Saturate: '<S4>/DutyLimit'
     */
    DCMotor_M3_ClosedLoopHW_B.duty_cmd = -DCMotor_M3_ClosedLoopHW_P.Duty_Max;
  }

  /* End of Saturate: '<S4>/DutyLimit' */

  /* Switch: '<S1>/Switch' incorporates:
   *  Constant: '<S1>/Constant4'
   *  Constant: '<S1>/Constant5'
   */
  if (DCMotor_M3_ClosedLoopHW_B.duty_cmd >
      DCMotor_M3_ClosedLoopHW_P.Switch_Threshold) {
    /* Gain: '<S1>/Gain' */
    rtb_Switch_k_idx_0 = (real32_T)(DCMotor_M3_ClosedLoopHW_P.Gain_Gain *
      DCMotor_M3_ClosedLoopHW_B.duty_cmd);

    /* Saturate: '<S1>/Saturation' */
    if (rtb_Switch_k_idx_0 > DCMotor_M3_ClosedLoopHW_P.Saturation_UpperSat) {
      rtb_Switch_k_idx_0 = DCMotor_M3_ClosedLoopHW_P.Saturation_UpperSat;
    } else if (rtb_Switch_k_idx_0 <
               DCMotor_M3_ClosedLoopHW_P.Saturation_LowerSat) {
      rtb_Switch_k_idx_0 = DCMotor_M3_ClosedLoopHW_P.Saturation_LowerSat;
    }

    /* End of Saturate: '<S1>/Saturation' */
    rtb_Switch_k_idx_1 = DCMotor_M3_ClosedLoopHW_P.Constant4_Value;
  } else {
    rtb_Switch_k_idx_0 = DCMotor_M3_ClosedLoopHW_P.Constant5_Value;

    /* Gain: '<S1>/Gain1' incorporates:
     *  Constant: '<S1>/Constant5'
     */
    rtb_Switch_k_idx_1 = (real32_T)(DCMotor_M3_ClosedLoopHW_P.Gain1_Gain *
      DCMotor_M3_ClosedLoopHW_B.duty_cmd);

    /* Saturate: '<S1>/Saturation1' */
    if (rtb_Switch_k_idx_1 > DCMotor_M3_ClosedLoopHW_P.Saturation1_UpperSat) {
      rtb_Switch_k_idx_1 = DCMotor_M3_ClosedLoopHW_P.Saturation1_UpperSat;
    } else if (rtb_Switch_k_idx_1 <
               DCMotor_M3_ClosedLoopHW_P.Saturation1_LowerSat) {
      rtb_Switch_k_idx_1 = DCMotor_M3_ClosedLoopHW_P.Saturation1_LowerSat;
    }

    /* End of Saturate: '<S1>/Saturation1' */
  }

  /* End of Switch: '<S1>/Switch' */

  /* MATLABSystem: '<S1>/PWM' */
  DCMotor_M3_ClosedLoopHW_DW.obj_b.PWMDriverObj.MW_PWM_HANDLE = MW_PWM_GetHandle
    (10UL);
  if (rtb_Switch_k_idx_1 <= 255.0F) {
    rtb_duty_ff = rtb_Switch_k_idx_1;
  } else {
    rtb_duty_ff = 255.0;
  }

  if (!(rtb_duty_ff >= 0.0)) {
    rtb_duty_ff = 0.0;
  }

  MW_PWM_SetDutyCycle
    (DCMotor_M3_ClosedLoopHW_DW.obj_b.PWMDriverObj.MW_PWM_HANDLE, rtb_duty_ff);

  /* End of MATLABSystem: '<S1>/PWM' */

  /* MATLABSystem: '<S1>/PWM1' */
  DCMotor_M3_ClosedLoopHW_DW.obj_ln.PWMDriverObj.MW_PWM_HANDLE =
    MW_PWM_GetHandle(11UL);
  if (rtb_Switch_k_idx_0 <= 255.0F) {
    rtb_duty_ff = rtb_Switch_k_idx_0;
  } else {
    rtb_duty_ff = 255.0;
  }

  if (!(rtb_duty_ff >= 0.0)) {
    rtb_duty_ff = 0.0;
  }

  MW_PWM_SetDutyCycle
    (DCMotor_M3_ClosedLoopHW_DW.obj_ln.PWMDriverObj.MW_PWM_HANDLE, rtb_duty_ff);

  /* End of MATLABSystem: '<S1>/PWM1' */

  /* DiscreteTransferFcn: '<S2>/Cur_LPF' */
  DCMotor_M3_ClosedLoopHW_B.current_A = DCMotor_M3_ClosedLoopHW_P.Cur_LPF_Num *
    DCMotor_M3_ClosedLoopHW_DW.Cur_LPF_states;

  /* Switch: '<S40>/Switch' incorporates:
   *  RelationalOperator: '<S40>/u_GTE_up'
   */
  if (!(rtb_Sum >= DCMotor_M3_ClosedLoopHW_B.Diff)) {
    /* Switch: '<S40>/Switch1' incorporates:
     *  RelationalOperator: '<S40>/u_GT_lo'
     */
    if (rtb_Sum > rtb_PI_LowerLimit) {
      DCMotor_M3_ClosedLoopHW_B.Diff = rtb_Sum;
    } else {
      DCMotor_M3_ClosedLoopHW_B.Diff = rtb_PI_LowerLimit;
    }

    /* End of Switch: '<S40>/Switch1' */
  }

  /* End of Switch: '<S40>/Switch' */

  /* Sum: '<S40>/Diff' */
  DCMotor_M3_ClosedLoopHW_B.Diff = rtb_Sum - DCMotor_M3_ClosedLoopHW_B.Diff;

  /* Gain: '<S45>/Integral Gain' */
  rtb_IntegralGain *= DCMotor_M3_ClosedLoopHW_P.Ki_Speed;

  /* MATLABSystem: '<S1>/Digital Output' incorporates:
   *  Constant: '<S1>/Constant2'
   */
  rtb_Switch_k_idx_0 = rt_roundf_snf(DCMotor_M3_ClosedLoopHW_P.Constant2_Value);
  if (rtb_Switch_k_idx_0 < 256.0F) {
    if (rtb_Switch_k_idx_0 >= 0.0F) {
      tmp_0 = (uint8_T)rtb_Switch_k_idx_0;
    } else {
      tmp_0 = 0U;
    }
  } else {
    tmp_0 = MAX_uint8_T;
  }

  writeDigitalPin(8, tmp_0);

  /* End of MATLABSystem: '<S1>/Digital Output' */

  /* MATLABSystem: '<S1>/Digital Output1' */
  if (rtb_Switch_k_idx_0 < 256.0F) {
    if (rtb_Switch_k_idx_0 >= 0.0F) {
      tmp_0 = (uint8_T)rtb_Switch_k_idx_0;
    } else {
      tmp_0 = 0U;
    }
  } else {
    tmp_0 = MAX_uint8_T;
  }

  writeDigitalPin(9, tmp_0);

  /* End of MATLABSystem: '<S1>/Digital Output1' */

  /* MATLABSystem: '<S3>/Encoder' */
  if (DCMotor_M3_ClosedLoopHW_DW.obj_j.TunablePropsChanged) {
    DCMotor_M3_ClosedLoopHW_DW.obj_j.TunablePropsChanged = false;
  }

  MW_EncoderRead(DCMotor_M3_ClosedLoopHW_DW.obj_j.Index, &rtb_counts_0);

  /* Gain: '<S3>/CountsToRPM' incorporates:
   *  DataTypeConversion: '<S3>/ToDouble'
   *  MATLABSystem: '<S3>/Encoder'
   *  Sum: '<S7>/Diff'
   *  UnitDelay: '<S7>/UD'
   *
   * Block description for '<S7>/Diff':
   *
   *  Add in CPU
   *
   * Block description for '<S7>/UD':
   *
   *  Store in Global RAM
   */
  DCMotor_M3_ClosedLoopHW_B.rpm_raw = (real_T)(rtb_counts_0 -
    DCMotor_M3_ClosedLoopHW_DW.UD_DSTATE) *
    DCMotor_M3_ClosedLoopHW_P.Enc_RPMGain;

  /* MATLABSystem: '<S2>/AnalogInput_A3' */
  DCMotor_M3_ClosedLoopHW_DW.obj_e.AnalogInDriverObj.MW_ANALOGIN_HANDLE =
    MW_AnalogIn_GetHandle(17UL);
  MW_AnalogInSingle_ReadResult
    (DCMotor_M3_ClosedLoopHW_DW.obj_e.AnalogInDriverObj.MW_ANALOGIN_HANDLE,
     &b_varargout_1, MW_ANALOGIN_UINT16);

  /* Update for DiscreteTransferFcn: '<S3>/RPM_LPF' */
  DCMotor_M3_ClosedLoopHW_DW.RPM_LPF_states = (DCMotor_M3_ClosedLoopHW_B.rpm_raw
    - DCMotor_M3_ClosedLoopHW_P.RPM_LPF_Den[1L] *
    DCMotor_M3_ClosedLoopHW_DW.RPM_LPF_states) /
    DCMotor_M3_ClosedLoopHW_P.RPM_LPF_Den[0];

  /* Switch: '<S37>/Switch1' incorporates:
   *  Constant: '<S37>/Clamping_zero'
   *  Constant: '<S37>/Constant'
   *  Constant: '<S37>/Constant2'
   *  RelationalOperator: '<S37>/fix for DT propagation issue'
   */
  if (DCMotor_M3_ClosedLoopHW_B.Diff >
      DCMotor_M3_ClosedLoopHW_P.Clamping_zero_Value) {
    tmp_1 = DCMotor_M3_ClosedLoopHW_P.Constant_Value;
  } else {
    tmp_1 = DCMotor_M3_ClosedLoopHW_P.Constant2_Value_m;
  }

  /* Switch: '<S37>/Switch2' incorporates:
   *  Constant: '<S37>/Clamping_zero'
   *  Constant: '<S37>/Constant3'
   *  Constant: '<S37>/Constant4'
   *  RelationalOperator: '<S37>/fix for DT propagation issue1'
   */
  if (rtb_IntegralGain > DCMotor_M3_ClosedLoopHW_P.Clamping_zero_Value) {
    tmp_2 = DCMotor_M3_ClosedLoopHW_P.Constant3_Value;
  } else {
    tmp_2 = DCMotor_M3_ClosedLoopHW_P.Constant4_Value_c;
  }

  /* Switch: '<S37>/Switch' incorporates:
   *  Constant: '<S37>/Clamping_zero'
   *  Constant: '<S37>/Constant1'
   *  Logic: '<S37>/AND3'
   *  RelationalOperator: '<S37>/Equal1'
   *  RelationalOperator: '<S37>/Relational Operator'
   *  Switch: '<S37>/Switch1'
   *  Switch: '<S37>/Switch2'
   */
  if ((DCMotor_M3_ClosedLoopHW_P.Clamping_zero_Value !=
       DCMotor_M3_ClosedLoopHW_B.Diff) && (tmp_1 == tmp_2)) {
    rtb_IntegralGain = DCMotor_M3_ClosedLoopHW_P.Constant1_Value;
  }

  /* Update for DiscreteIntegrator: '<S48>/Integrator' incorporates:
   *  Switch: '<S37>/Switch'
   */
  DCMotor_M3_ClosedLoopHW_DW.Integrator_DSTATE +=
    DCMotor_M3_ClosedLoopHW_P.Integrator_gainval * rtb_IntegralGain;

  /* Update for DiscreteTransferFcn: '<S2>/Cur_LPF' incorporates:
   *  Constant: '<S2>/ZeroOffsetV'
   *  DataTypeConversion: '<S2>/ToDouble'
   *  Gain: '<S2>/ADCToVolt'
   *  Gain: '<S2>/VoltToAmp'
   *  MATLABSystem: '<S2>/AnalogInput_A3'
   *  Sum: '<S2>/RemoveOffset'
   * */
  DCMotor_M3_ClosedLoopHW_DW.Cur_LPF_states =
    ((DCMotor_M3_ClosedLoopHW_P.Cur_VRef / DCMotor_M3_ClosedLoopHW_P.Cur_ADCMax *
      (real_T)b_varargout_1 - DCMotor_M3_ClosedLoopHW_P.Cur_OffsetV) *
     (DCMotor_M3_ClosedLoopHW_P.Cur_Dir / DCMotor_M3_ClosedLoopHW_P.Cur_Sens) -
     DCMotor_M3_ClosedLoopHW_P.Cur_LPF_Den[1L] *
     DCMotor_M3_ClosedLoopHW_DW.Cur_LPF_states) /
    DCMotor_M3_ClosedLoopHW_P.Cur_LPF_Den[0];

  /* Update for UnitDelay: '<S7>/UD' incorporates:
   *  MATLABSystem: '<S3>/Encoder'
   *
   * Block description for '<S7>/UD':
   *
   *  Store in Global RAM
   */
  DCMotor_M3_ClosedLoopHW_DW.UD_DSTATE = rtb_counts_0;

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The resolution of this integer timer is 0.01, which is the step size
   * of the task. Size of "clockTick0" ensures timer will not overflow during the
   * application lifespan selected.
   */
  DCMotor_M3_ClosedLoopHW_M->Timing.clockTick0++;
}

/* Model initialize function */
void DCMotor_M3_ClosedLoopHW_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));
  rtmSetTFinal(DCMotor_M3_ClosedLoopHW_M, 44.0);

  /* External mode info */
  DCMotor_M3_ClosedLoopHW_M->Sizes.checksums[0] = (3879912588U);
  DCMotor_M3_ClosedLoopHW_M->Sizes.checksums[1] = (2463801624U);
  DCMotor_M3_ClosedLoopHW_M->Sizes.checksums[2] = (4143224891U);
  DCMotor_M3_ClosedLoopHW_M->Sizes.checksums[3] = (804559939U);

  {
    static const sysRanDType rtAlwaysEnabled = SUBSYS_RAN_BC_ENABLE;
    static RTWExtModeInfo rt_ExtModeInfo;
    static const sysRanDType *systemRan[15];
    DCMotor_M3_ClosedLoopHW_M->extModeInfo = (&rt_ExtModeInfo);
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
    systemRan[9] = &rtAlwaysEnabled;
    systemRan[10] = &rtAlwaysEnabled;
    systemRan[11] = &rtAlwaysEnabled;
    systemRan[12] = &rtAlwaysEnabled;
    systemRan[13] = &rtAlwaysEnabled;
    systemRan[14] = &rtAlwaysEnabled;
    rteiSetModelMappingInfoPtr(DCMotor_M3_ClosedLoopHW_M->extModeInfo,
      &DCMotor_M3_ClosedLoopHW_M->SpecialInfo.mappingInfo);
    rteiSetChecksumsPtr(DCMotor_M3_ClosedLoopHW_M->extModeInfo,
                        DCMotor_M3_ClosedLoopHW_M->Sizes.checksums);
    rteiSetTFinalTicks(DCMotor_M3_ClosedLoopHW_M->extModeInfo, 4400);
  }

  /* InitializeConditions for DiscreteTransferFcn: '<S3>/RPM_LPF' */
  DCMotor_M3_ClosedLoopHW_DW.RPM_LPF_states =
    DCMotor_M3_ClosedLoopHW_P.RPM_LPF_InitialStates;

  /* InitializeConditions for DiscreteIntegrator: '<S48>/Integrator' */
  DCMotor_M3_ClosedLoopHW_DW.Integrator_DSTATE =
    DCMotor_M3_ClosedLoopHW_P.Integrator_IC;

  /* InitializeConditions for DiscreteTransferFcn: '<S2>/Cur_LPF' */
  DCMotor_M3_ClosedLoopHW_DW.Cur_LPF_states =
    DCMotor_M3_ClosedLoopHW_P.Cur_LPF_InitialStates;

  /* InitializeConditions for UnitDelay: '<S7>/UD'
   *
   * Block description for '<S7>/UD':
   *
   *  Store in Global RAM
   */
  DCMotor_M3_ClosedLoopHW_DW.UD_DSTATE =
    DCMotor_M3_ClosedLoopHW_P.DeltaCount_ICPrevInput;

  /* Start for MATLABSystem: '<S1>/PWM' */
  DCMotor_M3_ClosedLoopHW_DW.obj_b.matlabCodegenIsDeleted = false;
  DCMotor_M3_ClosedLoopHW_DW.obj_b.isInitialized = 1L;
  DCMotor_M3_ClosedLoopHW_DW.obj_b.PWMDriverObj.MW_PWM_HANDLE = MW_PWM_Open(10UL,
    0.0, 0.0);
  DCMotor_M3_ClosedLoopHW_DW.obj_b.isSetupComplete = true;

  /* Start for MATLABSystem: '<S1>/PWM1' */
  DCMotor_M3_ClosedLoopHW_DW.obj_ln.matlabCodegenIsDeleted = false;
  DCMotor_M3_ClosedLoopHW_DW.obj_ln.isInitialized = 1L;
  DCMotor_M3_ClosedLoopHW_DW.obj_ln.PWMDriverObj.MW_PWM_HANDLE = MW_PWM_Open
    (11UL, 0.0, 0.0);
  DCMotor_M3_ClosedLoopHW_DW.obj_ln.isSetupComplete = true;

  /* Start for MATLABSystem: '<S1>/Digital Output' */
  DCMotor_M3_ClosedLoopHW_DW.obj_l.matlabCodegenIsDeleted = false;
  DCMotor_M3_ClosedLoopHW_DW.obj_l.isInitialized = 1L;
  digitalIOSetup(8, 1);
  DCMotor_M3_ClosedLoopHW_DW.obj_l.isSetupComplete = true;

  /* Start for MATLABSystem: '<S1>/Digital Output1' */
  DCMotor_M3_ClosedLoopHW_DW.obj.matlabCodegenIsDeleted = false;
  DCMotor_M3_ClosedLoopHW_DW.obj.isInitialized = 1L;
  digitalIOSetup(9, 1);
  DCMotor_M3_ClosedLoopHW_DW.obj.isSetupComplete = true;

  /* Start for MATLABSystem: '<S3>/Encoder' */
  DCMotor_M3_ClosedLoopHW_DW.obj_j.Index = 0U;
  DCMotor_M3_ClosedLoopHW_DW.obj_j.matlabCodegenIsDeleted = false;
  DCMotor_M3_ClosedLoopHW_DW.obj_j.isInitialized = 1L;
  MW_EncoderSetup(2UL, 3UL, &DCMotor_M3_ClosedLoopHW_DW.obj_j.Index);
  DCMotor_M3_ClosedLoopHW_DW.obj_j.isSetupComplete = true;
  DCMotor_M3_ClosedLoopHW_DW.obj_j.TunablePropsChanged = false;

  /* InitializeConditions for MATLABSystem: '<S3>/Encoder' */
  MW_EncoderReset(DCMotor_M3_ClosedLoopHW_DW.obj_j.Index);

  /* Start for MATLABSystem: '<S2>/AnalogInput_A3' */
  DCMotor_M3_ClosedLoopHW_DW.obj_e.matlabCodegenIsDeleted = false;
  DCMotor_M3_ClosedLoopHW_DW.obj_e.isInitialized = 1L;
  DCMotor_M3_ClosedLoopHW_DW.obj_e.AnalogInDriverObj.MW_ANALOGIN_HANDLE =
    MW_AnalogInSingle_Open(17UL);
  DCMotor_M3_ClosedLoopHW_DW.obj_e.isSetupComplete = true;
}

/* Model terminate function */
void DCMotor_M3_ClosedLoopHW_terminate(void)
{
  /* Terminate for MATLABSystem: '<S1>/PWM' */
  if (!DCMotor_M3_ClosedLoopHW_DW.obj_b.matlabCodegenIsDeleted) {
    DCMotor_M3_ClosedLoopHW_DW.obj_b.matlabCodegenIsDeleted = true;
    if ((DCMotor_M3_ClosedLoopHW_DW.obj_b.isInitialized == 1L) &&
        DCMotor_M3_ClosedLoopHW_DW.obj_b.isSetupComplete) {
      DCMotor_M3_ClosedLoopHW_DW.obj_b.PWMDriverObj.MW_PWM_HANDLE =
        MW_PWM_GetHandle(10UL);
      MW_PWM_SetDutyCycle
        (DCMotor_M3_ClosedLoopHW_DW.obj_b.PWMDriverObj.MW_PWM_HANDLE, 0.0);
      DCMotor_M3_ClosedLoopHW_DW.obj_b.PWMDriverObj.MW_PWM_HANDLE =
        MW_PWM_GetHandle(10UL);
      MW_PWM_Close(DCMotor_M3_ClosedLoopHW_DW.obj_b.PWMDriverObj.MW_PWM_HANDLE);
    }
  }

  /* End of Terminate for MATLABSystem: '<S1>/PWM' */

  /* Terminate for MATLABSystem: '<S1>/PWM1' */
  if (!DCMotor_M3_ClosedLoopHW_DW.obj_ln.matlabCodegenIsDeleted) {
    DCMotor_M3_ClosedLoopHW_DW.obj_ln.matlabCodegenIsDeleted = true;
    if ((DCMotor_M3_ClosedLoopHW_DW.obj_ln.isInitialized == 1L) &&
        DCMotor_M3_ClosedLoopHW_DW.obj_ln.isSetupComplete) {
      DCMotor_M3_ClosedLoopHW_DW.obj_ln.PWMDriverObj.MW_PWM_HANDLE =
        MW_PWM_GetHandle(11UL);
      MW_PWM_SetDutyCycle
        (DCMotor_M3_ClosedLoopHW_DW.obj_ln.PWMDriverObj.MW_PWM_HANDLE, 0.0);
      DCMotor_M3_ClosedLoopHW_DW.obj_ln.PWMDriverObj.MW_PWM_HANDLE =
        MW_PWM_GetHandle(11UL);
      MW_PWM_Close(DCMotor_M3_ClosedLoopHW_DW.obj_ln.PWMDriverObj.MW_PWM_HANDLE);
    }
  }

  /* End of Terminate for MATLABSystem: '<S1>/PWM1' */

  /* Terminate for MATLABSystem: '<S1>/Digital Output' */
  if (!DCMotor_M3_ClosedLoopHW_DW.obj_l.matlabCodegenIsDeleted) {
    DCMotor_M3_ClosedLoopHW_DW.obj_l.matlabCodegenIsDeleted = true;
  }

  /* End of Terminate for MATLABSystem: '<S1>/Digital Output' */

  /* Terminate for MATLABSystem: '<S1>/Digital Output1' */
  if (!DCMotor_M3_ClosedLoopHW_DW.obj.matlabCodegenIsDeleted) {
    DCMotor_M3_ClosedLoopHW_DW.obj.matlabCodegenIsDeleted = true;
  }

  /* End of Terminate for MATLABSystem: '<S1>/Digital Output1' */

  /* Terminate for MATLABSystem: '<S3>/Encoder' */
  if (!DCMotor_M3_ClosedLoopHW_DW.obj_j.matlabCodegenIsDeleted) {
    DCMotor_M3_ClosedLoopHW_DW.obj_j.matlabCodegenIsDeleted = true;
    if ((DCMotor_M3_ClosedLoopHW_DW.obj_j.isInitialized == 1L) &&
        DCMotor_M3_ClosedLoopHW_DW.obj_j.isSetupComplete) {
      MW_EncoderRelease();
    }
  }

  /* End of Terminate for MATLABSystem: '<S3>/Encoder' */

  /* Terminate for MATLABSystem: '<S2>/AnalogInput_A3' */
  if (!DCMotor_M3_ClosedLoopHW_DW.obj_e.matlabCodegenIsDeleted) {
    DCMotor_M3_ClosedLoopHW_DW.obj_e.matlabCodegenIsDeleted = true;
    if ((DCMotor_M3_ClosedLoopHW_DW.obj_e.isInitialized == 1L) &&
        DCMotor_M3_ClosedLoopHW_DW.obj_e.isSetupComplete) {
      DCMotor_M3_ClosedLoopHW_DW.obj_e.AnalogInDriverObj.MW_ANALOGIN_HANDLE =
        MW_AnalogIn_GetHandle(17UL);
      MW_AnalogIn_Close
        (DCMotor_M3_ClosedLoopHW_DW.obj_e.AnalogInDriverObj.MW_ANALOGIN_HANDLE);
    }
  }

  /* End of Terminate for MATLABSystem: '<S2>/AnalogInput_A3' */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
