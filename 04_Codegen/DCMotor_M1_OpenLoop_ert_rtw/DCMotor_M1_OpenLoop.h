/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: DCMotor_M1_OpenLoop.h
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

#ifndef DCMotor_M1_OpenLoop_h_
#define DCMotor_M1_OpenLoop_h_
#ifndef DCMotor_M1_OpenLoop_COMMON_INCLUDES_
#define DCMotor_M1_OpenLoop_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_extmode.h"
#include "sysran_types.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                /* DCMotor_M1_OpenLoop_COMMON_INCLUDES_ */

#include "DCMotor_M1_OpenLoop_types.h"
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
  real_T duty_cmd;                     /* '<Root>/DutyLimit' */
  real_T rpm;                          /* '<S3>/RPM_LPF' */
  real_T current_A;                    /* '<S2>/Cur_LPF' */
  real_T sensor_V;                     /* '<S2>/ADCToVolt' */
  real_T rpm_raw;                      /* '<S3>/CountsToRPM' */
  int32_T counts;                      /* '<S3>/Encoder' */
} B_DCMotor_M1_OpenLoop_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  codertarget_arduinobase_block_T obj; /* '<S1>/Digital Output1' */
  codertarget_arduinobase_block_T obj_l;/* '<S1>/Digital Output' */
  codertarget_arduinobase_inter_T obj_ln;/* '<S1>/PWM1' */
  codertarget_arduinobase_inter_T obj_b;/* '<S1>/PWM' */
  codertarget_arduinobase_int_f_T obj_e;/* '<S2>/AnalogInput_A3' */
  codertarget_arduinobase_in_fo_T obj_j;/* '<S3>/Encoder' */
  real_T RPM_LPF_states;               /* '<S3>/RPM_LPF' */
  real_T Cur_LPF_states;               /* '<S2>/Cur_LPF' */
  int32_T UD_DSTATE;                   /* '<S4>/UD' */
} DW_DCMotor_M1_OpenLoop_T;

/* Parameters (default storage) */
struct P_DCMotor_M1_OpenLoop_T_ {
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
  real_T Enc_RPMGain;                  /* Variable: Enc_RPMGain
                                        * Referenced by: '<S3>/CountsToRPM'
                                        */
  real_T OL_Duty[6];                   /* Variable: OL_Duty
                                        * Referenced by: '<Root>/OL_DutyProfile'
                                        */
  real_T OL_Scale;                     /* Variable: OL_Scale
                                        * Referenced by: '<Root>/OL_Scale'
                                        */
  real_T OL_Time[6];                   /* Variable: OL_Time
                                        * Referenced by: '<Root>/OL_DutyProfile'
                                        */
  real_T RPM_LPF_Den[2];               /* Variable: RPM_LPF_Den
                                        * Referenced by: '<S3>/RPM_LPF'
                                        */
  real_T RPM_LPF_Num;                  /* Variable: RPM_LPF_Num
                                        * Referenced by: '<S3>/RPM_LPF'
                                        */
  int32_T DeltaCount_ICPrevInput;      /* Mask Parameter: DeltaCount_ICPrevInput
                                        * Referenced by: '<S4>/UD'
                                        */
  real_T Gain_Gain;                    /* Expression: 255/100
                                        * Referenced by: '<S1>/Gain'
                                        */
  real_T Gain1_Gain;                   /* Expression: -255/100
                                        * Referenced by: '<S1>/Gain1'
                                        */
  real_T DutyLimit_UpperSat;           /* Expression: 100
                                        * Referenced by: '<Root>/DutyLimit'
                                        */
  real_T DutyLimit_LowerSat;           /* Expression: -100
                                        * Referenced by: '<Root>/DutyLimit'
                                        */
  real_T Switch_Threshold;             /* Expression: 0
                                        * Referenced by: '<S1>/Switch'
                                        */
  real_T RPM_LPF_InitialStates;        /* Expression: 0
                                        * Referenced by: '<S3>/RPM_LPF'
                                        */
  real_T Cur_LPF_InitialStates;        /* Expression: 0
                                        * Referenced by: '<S2>/Cur_LPF'
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
};

/* Real-time Model Data Structure */
struct tag_RTM_DCMotor_M1_OpenLoop_T {
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
extern P_DCMotor_M1_OpenLoop_T DCMotor_M1_OpenLoop_P;

/* Block signals (default storage) */
extern B_DCMotor_M1_OpenLoop_T DCMotor_M1_OpenLoop_B;

/* Block states (default storage) */
extern DW_DCMotor_M1_OpenLoop_T DCMotor_M1_OpenLoop_DW;

/* Model entry point functions */
extern void DCMotor_M1_OpenLoop_initialize(void);
extern void DCMotor_M1_OpenLoop_step(void);
extern void DCMotor_M1_OpenLoop_terminate(void);

/* Real-time Model object */
extern RT_MODEL_DCMotor_M1_OpenLoop_T *const DCMotor_M1_OpenLoop_M;
extern volatile boolean_T stopRequested;
extern volatile boolean_T runModel;

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
 * '<Root>' : 'DCMotor_M1_OpenLoop'
 * '<S1>'   : 'DCMotor_M1_OpenLoop/BTS7960_Driver'
 * '<S2>'   : 'DCMotor_M1_OpenLoop/CurrentSensor'
 * '<S3>'   : 'DCMotor_M1_OpenLoop/EncoderToRPM'
 * '<S4>'   : 'DCMotor_M1_OpenLoop/EncoderToRPM/DeltaCount'
 */
#endif                                 /* DCMotor_M1_OpenLoop_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
