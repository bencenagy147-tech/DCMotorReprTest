/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: DCMotor_M1_OpenLoop_types.h
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

#ifndef DCMotor_M1_OpenLoop_types_h_
#define DCMotor_M1_OpenLoop_types_h_
#include "rtwtypes.h"
#include "MW_SVD.h"
#ifndef struct_tag_hXyY5vc1gIe0XgRqnotTuD
#define struct_tag_hXyY5vc1gIe0XgRqnotTuD

struct tag_hXyY5vc1gIe0XgRqnotTuD
{
  int16_T __dummy;
};

#endif                                 /* struct_tag_hXyY5vc1gIe0XgRqnotTuD */

#ifndef typedef_b_arduinodriver_ArduinoDigita_T
#define typedef_b_arduinodriver_ArduinoDigita_T

typedef struct tag_hXyY5vc1gIe0XgRqnotTuD b_arduinodriver_ArduinoDigita_T;

#endif                             /* typedef_b_arduinodriver_ArduinoDigita_T */

#ifndef struct_tag_klXnNqPzYCMAAeCXx5caDG
#define struct_tag_klXnNqPzYCMAAeCXx5caDG

struct tag_klXnNqPzYCMAAeCXx5caDG
{
  boolean_T matlabCodegenIsDeleted;
  int32_T isInitialized;
  boolean_T isSetupComplete;
  b_arduinodriver_ArduinoDigita_T DigitalIODriverObj;
};

#endif                                 /* struct_tag_klXnNqPzYCMAAeCXx5caDG */

#ifndef typedef_codertarget_arduinobase_block_T
#define typedef_codertarget_arduinobase_block_T

typedef struct tag_klXnNqPzYCMAAeCXx5caDG codertarget_arduinobase_block_T;

#endif                             /* typedef_codertarget_arduinobase_block_T */

#ifndef struct_tag_UvJx03atA3spPs6mvx8QqE
#define struct_tag_UvJx03atA3spPs6mvx8QqE

struct tag_UvJx03atA3spPs6mvx8QqE
{
  MW_Handle_Type MW_PWM_HANDLE;
};

#endif                                 /* struct_tag_UvJx03atA3spPs6mvx8QqE */

#ifndef typedef_e_matlabshared_ioclient_perip_T
#define typedef_e_matlabshared_ioclient_perip_T

typedef struct tag_UvJx03atA3spPs6mvx8QqE e_matlabshared_ioclient_perip_T;

#endif                             /* typedef_e_matlabshared_ioclient_perip_T */

#ifndef struct_tag_98oXr1FLAUvDVbfqcQVTfD
#define struct_tag_98oXr1FLAUvDVbfqcQVTfD

struct tag_98oXr1FLAUvDVbfqcQVTfD
{
  boolean_T matlabCodegenIsDeleted;
  int32_T isInitialized;
  boolean_T isSetupComplete;
  e_matlabshared_ioclient_perip_T PWMDriverObj;
};

#endif                                 /* struct_tag_98oXr1FLAUvDVbfqcQVTfD */

#ifndef typedef_codertarget_arduinobase_inter_T
#define typedef_codertarget_arduinobase_inter_T

typedef struct tag_98oXr1FLAUvDVbfqcQVTfD codertarget_arduinobase_inter_T;

#endif                             /* typedef_codertarget_arduinobase_inter_T */

#ifndef struct_tag_mVifzvLPn0StcnJENmjEHE
#define struct_tag_mVifzvLPn0StcnJENmjEHE

struct tag_mVifzvLPn0StcnJENmjEHE
{
  MW_Handle_Type MW_ANALOGIN_HANDLE;
};

#endif                                 /* struct_tag_mVifzvLPn0StcnJENmjEHE */

#ifndef typedef_e_arduinodriver_ArduinoAnalog_T
#define typedef_e_arduinodriver_ArduinoAnalog_T

typedef struct tag_mVifzvLPn0StcnJENmjEHE e_arduinodriver_ArduinoAnalog_T;

#endif                             /* typedef_e_arduinodriver_ArduinoAnalog_T */

#ifndef struct_tag_vVzUTixqlt3OpD5jelgrr
#define struct_tag_vVzUTixqlt3OpD5jelgrr

struct tag_vVzUTixqlt3OpD5jelgrr
{
  boolean_T matlabCodegenIsDeleted;
  int32_T isInitialized;
  boolean_T isSetupComplete;
  e_arduinodriver_ArduinoAnalog_T AnalogInDriverObj;
};

#endif                                 /* struct_tag_vVzUTixqlt3OpD5jelgrr */

#ifndef typedef_codertarget_arduinobase_int_f_T
#define typedef_codertarget_arduinobase_int_f_T

typedef struct tag_vVzUTixqlt3OpD5jelgrr codertarget_arduinobase_int_f_T;

#endif                             /* typedef_codertarget_arduinobase_int_f_T */

#ifndef struct_tag_OjC5VBvSFhNHBpBmWJLoXH
#define struct_tag_OjC5VBvSFhNHBpBmWJLoXH

struct tag_OjC5VBvSFhNHBpBmWJLoXH
{
  boolean_T matlabCodegenIsDeleted;
  int32_T isInitialized;
  boolean_T isSetupComplete;
  boolean_T TunablePropsChanged;
  uint8_T Index;
};

#endif                                 /* struct_tag_OjC5VBvSFhNHBpBmWJLoXH */

#ifndef typedef_codertarget_arduinobase_in_fo_T
#define typedef_codertarget_arduinobase_in_fo_T

typedef struct tag_OjC5VBvSFhNHBpBmWJLoXH codertarget_arduinobase_in_fo_T;

#endif                             /* typedef_codertarget_arduinobase_in_fo_T */

/* Parameters (default storage) */
typedef struct P_DCMotor_M1_OpenLoop_T_ P_DCMotor_M1_OpenLoop_T;

/* Forward declaration for rtModel */
typedef struct tag_RTM_DCMotor_M1_OpenLoop_T RT_MODEL_DCMotor_M1_OpenLoop_T;

#endif                                 /* DCMotor_M1_OpenLoop_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
