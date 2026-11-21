/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: ert_main.c
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
#include "rtwtypes.h"
#include "ext_mode.h"                  /* External mode header file */
#include "xcp.h"
#include "MW_target_hardware_resources.h"

volatile int IsrOverrun = 0;
static boolean_T OverrunFlag = 0;
void rt_OneStep(void)
{
  extmodeSimulationTime_T extmodeTime = (extmodeSimulationTime_T)0;
  extmodeErrorCode_T extmodeError = EXTMODE_SUCCESS;

  /* Check for overrun. Protect OverrunFlag against preemption */
  if (OverrunFlag++) {
    IsrOverrun = 1;
    OverrunFlag--;
    return;
  }

#ifndef _MW_ARDUINO_LOOP_

  sei();

#endif;

  extmodeTime = (extmodeSimulationTime_T)
    (((DCMotor_M1_OpenLoop_M->Timing.clockTick0 * 1) + 0));
  DCMotor_M1_OpenLoop_step();

  /* Get model outputs here */
  extmodeError = extmodeEvent((extmodeEventId_T)(0), extmodeTime);
  if (extmodeError != EXTMODE_SUCCESS) {
    /* Code to handle external mode event errors may be added here */
  }

#ifndef _MW_ARDUINO_LOOP_

  cli();

#endif;

  OverrunFlag--;
}

extern void rtIOStreamResync();
volatile boolean_T stopRequested;
volatile boolean_T runModel;
int main(void)
{
  float modelBaseRate = 0.01;
  float systemClock = 0;
  extmodeErrorCode_T errorCode = EXTMODE_SUCCESS;

  /* Initialize variables */
  stopRequested = false;
  runModel = false;
  init();
  MW_Arduino_Init();
  rtmSetErrorStatus(DCMotor_M1_OpenLoop_M, 0);

  /* Parse External Mode command line arguments */
  errorCode = extmodeParseArgs(0, (const char_T**)NULL);
  if (errorCode != EXTMODE_SUCCESS) {
    return errorCode;
  }

  DCMotor_M1_OpenLoop_initialize();
  cli();
  sei ();

  /* External Mode initialization */
  errorCode = extmodeInit(DCMotor_M1_OpenLoop_M->extModeInfo,
    (extmodeSimulationTime_T *)rteiGetPtrTFinalTicks
    (DCMotor_M1_OpenLoop_M->extModeInfo));
  if (errorCode != EXTMODE_SUCCESS) {
    return errorCode;
  }

  if (errorCode == EXTMODE_SUCCESS) {
    /* Wait until a Start or Stop Request has been received from the Host */
    extmodeWaitForHostRequest(EXTMODE_WAIT_FOREVER);
    if (extmodeStopRequested()) {
      rtmSetStopRequested(DCMotor_M1_OpenLoop_M, true);
    }
  }

  cli();
  configureArduinoAVRTimer();
  runModel =
    !extmodeSimulationComplete() && !extmodeStopRequested()&&
    !rtmGetStopRequested(DCMotor_M1_OpenLoop_M);

#ifndef _MW_ARDUINO_LOOP_

  sei();

#endif;

  XcpStatus lastXcpState = xcpStatusGet();
  sei ();
  while (runModel) {
    /* Run External Mode background activities */
    errorCode = extmodeBackgroundRun();
    if (errorCode != EXTMODE_SUCCESS && errorCode != EXTMODE_EMPTY) {
      /* Code to handle External Mode background task errors
         may be added here */
    }

    stopRequested = !(
                      !extmodeSimulationComplete() && !extmodeStopRequested()&&
                      !rtmGetStopRequested(DCMotor_M1_OpenLoop_M));
    runModel = !(stopRequested);
    if (stopRequested)
      disable_rt_OneStep();
    if (lastXcpState==XCP_CONNECTED && xcpStatusGet()==XCP_DISCONNECTED)
      rtIOStreamResync();
    lastXcpState = xcpStatusGet();
    MW_Arduino_Loop();
    MW_Modbus_Slave_Poll();
  }

  /* Terminate model */
  DCMotor_M1_OpenLoop_terminate();

  /* External Mode reset */
  extmodeReset();
  cli();
  return 0;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
