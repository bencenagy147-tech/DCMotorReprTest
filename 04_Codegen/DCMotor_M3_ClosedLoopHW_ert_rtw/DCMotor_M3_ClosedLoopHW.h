/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: DCMotor_M3_ClosedLoopHW.h
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

#ifndef DCMotor_M3_ClosedLoopHW_h_
#define DCMotor_M3_ClosedLoopHW_h_
#ifndef DCMotor_M3_ClosedLoopHW_COMMON_INCLUDES_
#define DCMotor_M3_ClosedLoopHW_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_extmode.h"
#include "sysran_types.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                            /* DCMotor_M3_ClosedLoopHW_COMMON_INCLUDES_ */

#include "DCMotor_M3_ClosedLoopHW_types.h"
#include "rt_nonfinite.h"
#include "rtGetNaN.h"
#include "MW_target_hardware_resources.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetFinalTime
#define rtmGetFinalTime(rtm)           ((rtm)->Timing.tFinal)
#endif

#ifndef rtmGetRTWExtModeInfo
#define rtmGetRTWExtModeInfo(rtm)      ((rtm)->extModeInfo)
#endif

#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetStopRequested
#define rtmGetStopRequested(rtm)       ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequested
#define rtmSetStopRequested(rtm, val)  ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStopRequestedPtr
#define rtmGetStopRequestedPtr(rtm)    (&((rtm)->Timing.stopRequestedFlag))
#endif

#ifndef rtmGetTFinal
#define rtmGetTFinal(rtm)              ((rtm)->Timing.tFinal)
#endif

#ifndef rtmGetTPtr
#define rtmGetTPtr(rtm)                (&)
#endif

/* Block signals (default storage) */
typedef struct {
  real_T rpm_ref;                      /* '<Root>/RefStopGate' */
  real_T rpm;                          /* '<S3>/RPM_LPF' */
  real_T duty_cmd;                     /* '<S4>/DutyLimit' */
  real_T current_A;                    /* '<S2>/Cur_LPF' */
  real_T rpm_raw;                      /* '<S3>/CountsToRPM' */
  real_T Diff;                         /* '<S40>/Diff' */
} B_DCMotor_M3_ClosedLoopHW_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  codertarget_arduinobase_block_T obj; /* '<S1>/Digital Output1' */
  codertarget_arduinobase_block_T obj_l;/* '<S1>/Digital Output' */
  codertarget_arduinobase_inter_T obj_ln;/* '<S1>/PWM1' */
  codertarget_arduinobase_inter_T obj_b;/* '<S1>/PWM' */
  codertarget_arduinobase_int_j_T obj_e;/* '<S2>/AnalogInput_A3' */
  codertarget_arduinobase_in_jp_T obj_j;/* '<S3>/Encoder' */
  real_T RPM_LPF_states;               /* '<S3>/RPM_LPF' */
  real_T Integrator_DSTATE;            /* '<S48>/Integrator' */
  real_T Cur_LPF_states;               /* '<S2>/Cur_LPF' */
  int32_T UD_DSTATE;                   /* '<S7>/UD' */
} DW_DCMotor_M3_ClosedLoopHW_T;

/* Parameters (default storage) */
struct P_DCMotor_M3_ClosedLoopHW_T_ {
  real_T Cur_ADCMax;                   /* Variable: Cur_ADCMax
                                        * Referenced by: '<S2>/ADCToVolt'
                                        */
  real_T Cur_Dir;                      /* Variable: Cur_Dir
                                        * Referenced by: '<S2>/VoltToAmp'
                                        */
  real_T Cur_LPF_Den[2];               /* Variable: Cur_LPF_Den
                                        * Referenced by: '<S2>/Cur_LPF'
                                        */
  real_T Cur_LPF_Num;                  /* Variable: Cur_LPF_Num
                                        * Referenced by: '<S2>/Cur_LPF'
                                        */
  real_T Cur_OffsetV;                  /* Variable: Cur_OffsetV
                                        * Referenced by: '<S2>/ZeroOffsetV'
                                        */
  real_T Cur_Sens;                     /* Variable: Cur_Sens
                                        * Referenced by: '<S2>/VoltToAmp'
                                        */
  real_T Cur_VRef;                     /* Variable: Cur_VRef
                                        * Referenced by: '<S2>/ADCToVolt'
                                        */
  real_T Duty_Coulomb;                 /* Variable: Duty_Coulomb
                                        * Referenced by: '<S8>/CoulombFF'
                                        */
  real_T Duty_Max;                     /* Variable: Duty_Max
                                        * Referenced by:
                                        *   '<S4>/DutyMax'
                                        *   '<S4>/DutyMin'
                                        *   '<S4>/DutyLimit'
                                        */
  real_T Duty_StictionBoost;           /* Variable: Duty_StictionBoost
                                        * Referenced by: '<S8>/StictionBoost'
                                        */
  real_T Enc_RPMGain;                  /* Variable: Enc_RPMGain
                                        * Referenced by: '<S3>/CountsToRPM'
                                        */
  real_T FF_Enable;                    /* Variable: FF_Enable
                                        * Referenced by: '<S8>/FF_Enable'
                                        */
  real_T Ki_Speed;                     /* Variable: Ki_Speed
                                        * Referenced by: '<S45>/Integral Gain'
                                        */
  real_T Kp_Speed;                     /* Variable: Kp_Speed
                                        * Referenced by: '<S53>/Proportional Gain'
                                        */
  real_T RPM_LPF_Den[2];               /* Variable: RPM_LPF_Den
                                        * Referenced by: '<S3>/RPM_LPF'
                                        */
  real_T RPM_LPF_Num;                  /* Variable: RPM_LPF_Num
                                        * Referenced by: '<S3>/RPM_LPF'
                                        */
  real_T Ref_Min;                      /* Variable: Ref_Min
                                        * Referenced by: '<S5>/RefLimit'
                                        */
  real_T Ref_Period;                   /* Variable: Ref_Period
                                        * Referenced by: '<S5>/PerPeriod'
                                        */
  real_T Ref_Start;                    /* Variable: Ref_Start
                                        * Referenced by:
                                        *   '<S5>/StartLevel'
                                        *   '<S5>/RefLimit'
                                        */
  real_T Ref_Step;                     /* Variable: Ref_Step
                                        * Referenced by: '<S5>/StepSize'
                                        */
  real_T Ref_StopTime;                 /* Variable: Ref_StopTime
                                        * Referenced by: '<S6>/Constant'
                                        */
  real_T Stick_RPM;                    /* Variable: Stick_RPM
                                        * Referenced by: '<S10>/Constant'
                                        */
  int32_T DeltaCount_ICPrevInput;      /* Mask Parameter: DeltaCount_ICPrevInput
                                        * Referenced by: '<S7>/UD'
                                        */
  real_T Gain_Gain;                    /* Expression: 255/100
                                        * Referenced by: '<S1>/Gain'
                                        */
  real_T Gain1_Gain;                   /* Expression: -255/100
                                        * Referenced by: '<S1>/Gain1'
                                        */
  real_T Constant1_Value;              /* Expression: 0
                                        * Referenced by: '<S37>/Constant1'
                                        */
  real_T RPM_LPF_InitialStates;        /* Expression: 0
                                        * Referenced by: '<S3>/RPM_LPF'
                                        */
  real_T Integrator_gainval;           /* Computed Parameter: Integrator_gainval
                                        * Referenced by: '<S48>/Integrator'
                                        */
  real_T Integrator_IC;                /* Expression: 0
                                        * Referenced by: '<S48>/Integrator'
                                        */
  real_T Switch_Threshold;             /* Expression: 0
                                        * Referenced by: '<S1>/Switch'
                                        */
  real_T Cur_LPF_InitialStates;        /* Expression: 0
                                        * Referenced by: '<S2>/Cur_LPF'
                                        */
  real_T Clamping_zero_Value;          /* Expression: 0
                                        * Referenced by: '<S37>/Clamping_zero'
                                        */
  real32_T Saturation_UpperSat;       /* Computed Parameter: Saturation_UpperSat
                                       * Referenced by: '<S1>/Saturation'
                                       */
  real32_T Saturation_LowerSat;       /* Computed Parameter: Saturation_LowerSat
                                       * Referenced by: '<S1>/Saturation'
                                       */
  real32_T Constant4_Value;            /* Computed Parameter: Constant4_Value
                                        * Referenced by: '<S1>/Constant4'
                                        */
  real32_T Constant5_Value;            /* Computed Parameter: Constant5_Value
                                        * Referenced by: '<S1>/Constant5'
                                        */
  real32_T Saturation1_UpperSat;     /* Computed Parameter: Saturation1_UpperSat
                                      * Referenced by: '<S1>/Saturation1'
                                      */
  real32_T Saturation1_LowerSat;     /* Computed Parameter: Saturation1_LowerSat
                                      * Referenced by: '<S1>/Saturation1'
                                      */
  real32_T Constant2_Value;            /* Computed Parameter: Constant2_Value
                                        * Referenced by: '<S1>/Constant2'
                                        */
  int8_T Constant_Value;               /* Computed Parameter: Constant_Value
                                        * Referenced by: '<S37>/Constant'
                                        */
  int8_T Constant2_Value_m;            /* Computed Parameter: Constant2_Value_m
                                        * Referenced by: '<S37>/Constant2'
                                        */
  int8_T Constant3_Value;              /* Computed Parameter: Constant3_Value
                                        * Referenced by: '<S37>/Constant3'
                                        */
  int8_T Constant4_Value_c;            /* Computed Parameter: Constant4_Value_c
                                        * Referenced by: '<S37>/Constant4'
                                        */
};

/* Real-time Model Data Structure */
struct tag_RTM_DCMotor_M3_ClosedLoop_T {
  const char_T *errorStatus;
  RTWExtModeInfo *extModeInfo;

  /*
   * Sizes:
   * The following substructure contains sizes information
   * for many of the model attributes such as inputs, outputs,
   * dwork, sample times, etc.
   */
  struct {
    uint32_T checksums[4];
  } Sizes;

  /*
   * SpecialInfo:
   * The following substructure contains special information
   * related to other components that are dependent on RTW.
   */
  struct {
    const void *mappingInfo;
  } SpecialInfo;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    uint32_T clockTick0;
    time_T tFinal;
    boolean_T stopRequestedFlag;
  } Timing;
};

/* Block parameters (default storage) */
extern P_DCMotor_M3_ClosedLoopHW_T DCMotor_M3_ClosedLoopHW_P;

/* Block signals (default storage) */
extern B_DCMotor_M3_ClosedLoopHW_T DCMotor_M3_ClosedLoopHW_B;

/* Block states (default storage) */
extern DW_DCMotor_M3_ClosedLoopHW_T DCMotor_M3_ClosedLoopHW_DW;

/* Model entry point functions */
extern void DCMotor_M3_ClosedLoopHW_initialize(void);
extern void DCMotor_M3_ClosedLoopHW_step(void);
extern void DCMotor_M3_ClosedLoopHW_terminate(void);

/* Real-time Model object */
extern RT_MODEL_DCMotor_M3_ClosedLoo_T *const DCMotor_M3_ClosedLoopHW_M;
extern volatile boolean_T stopRequested;
extern volatile boolean_T runModel;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S56>/Data Type Duplicate' : Unused code path elimination
 * Block '<S56>/Data Type Propagation' : Unused code path elimination
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'DCMotor_M3_ClosedLoopHW'
 * '<S1>'   : 'DCMotor_M3_ClosedLoopHW/BTS7960_Driver'
 * '<S2>'   : 'DCMotor_M3_ClosedLoopHW/CurrentSensor'
 * '<S3>'   : 'DCMotor_M3_ClosedLoopHW/EncoderToRPM'
 * '<S4>'   : 'DCMotor_M3_ClosedLoopHW/SpeedController'
 * '<S5>'   : 'DCMotor_M3_ClosedLoopHW/SpeedReference'
 * '<S6>'   : 'DCMotor_M3_ClosedLoopHW/StaircaseActive'
 * '<S7>'   : 'DCMotor_M3_ClosedLoopHW/EncoderToRPM/DeltaCount'
 * '<S8>'   : 'DCMotor_M3_ClosedLoopHW/SpeedController/FrictionFF'
 * '<S9>'   : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed'
 * '<S10>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/FrictionFF/AtStandstill'
 * '<S11>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/Anti-windup'
 * '<S12>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/D Gain'
 * '<S13>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/External Derivative'
 * '<S14>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/Filter'
 * '<S15>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/Filter ICs'
 * '<S16>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/I Gain'
 * '<S17>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/Ideal P Gain'
 * '<S18>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/Ideal P Gain Fdbk'
 * '<S19>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/Integrator'
 * '<S20>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/Integrator ICs'
 * '<S21>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/N Copy'
 * '<S22>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/N Gain'
 * '<S23>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/P Copy'
 * '<S24>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/Parallel P Gain'
 * '<S25>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/Reset Signal'
 * '<S26>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/Saturation'
 * '<S27>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/Saturation Fdbk'
 * '<S28>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/Sum'
 * '<S29>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/Sum Fdbk'
 * '<S30>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/Tracking Mode'
 * '<S31>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/Tracking Mode Sum'
 * '<S32>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/Tsamp - Integral'
 * '<S33>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/Tsamp - Ngain'
 * '<S34>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/postSat Signal'
 * '<S35>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/preInt Signal'
 * '<S36>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/preSat Signal'
 * '<S37>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/Anti-windup/Disc. Clamping Parallel'
 * '<S38>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/Anti-windup/Disc. Clamping Parallel/Dead Zone'
 * '<S39>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/Anti-windup/Disc. Clamping Parallel/Dead Zone/External'
 * '<S40>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/Anti-windup/Disc. Clamping Parallel/Dead Zone/External/Dead Zone Dynamic'
 * '<S41>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/D Gain/Disabled'
 * '<S42>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/External Derivative/Disabled'
 * '<S43>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/Filter/Disabled'
 * '<S44>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/Filter ICs/Disabled'
 * '<S45>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/I Gain/Internal Parameters'
 * '<S46>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/Ideal P Gain/Passthrough'
 * '<S47>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/Ideal P Gain Fdbk/Disabled'
 * '<S48>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/Integrator/Discrete'
 * '<S49>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/Integrator ICs/Internal IC'
 * '<S50>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/N Copy/Disabled wSignal Specification'
 * '<S51>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/N Gain/Disabled'
 * '<S52>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/P Copy/Disabled'
 * '<S53>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/Parallel P Gain/Internal Parameters'
 * '<S54>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/Reset Signal/Disabled'
 * '<S55>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/Saturation/External'
 * '<S56>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/Saturation/External/Saturation Dynamic'
 * '<S57>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/Saturation Fdbk/Disabled'
 * '<S58>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/Sum/Sum_PI'
 * '<S59>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/Sum Fdbk/Disabled'
 * '<S60>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/Tracking Mode/Disabled'
 * '<S61>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/Tracking Mode Sum/Passthrough'
 * '<S62>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/Tsamp - Integral/TsSignalSpecification'
 * '<S63>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/Tsamp - Ngain/Passthrough'
 * '<S64>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/postSat Signal/Forward_Path'
 * '<S65>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/preInt Signal/Internal PreInt'
 * '<S66>'  : 'DCMotor_M3_ClosedLoopHW/SpeedController/PI_Speed/preSat Signal/Forward_Path'
 */
#endif                                 /* DCMotor_M3_ClosedLoopHW_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
