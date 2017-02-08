/*
 * sfuntmpl_basic.c: Basic 'C' template for a level 2 S-function.
 *
 *  -------------------------------------------------------------------------
 *  | See matlabroot/simulink/src/sfuntmpl_doc.c for a more detailed template |
 *  -------------------------------------------------------------------------
 *
 * Copyright 1990-2001 The MathWorks, Inc.
 * $Revision: 1.26 $
 */


/*
 * You must specify the S_FUNCTION_NAME as the name of your S-function
 * (i.e. replace sfuntmpl_basic with the name of your S-function).
 */

#define S_FUNCTION_NAME buffer_workvector
#define S_FUNCTION_LEVEL 2

/*
 * Need to include simstruc.h for the definition of the SimStruct and
 * its associated macro definitions.
 */
#include "simstruc.h"
#include <stdio.h>





/*====================*
 * S-function methods *
 *====================*/

/* Function: mdlInitializeSizes ===============================================
 * Abstract:
 *    The sizes information is used by Simulink to determine the S-function
 *    block's characteristics (number of inputs, outputs, states, etc.).
 */

#define ST1_IDX 0
#define ST1_PARAM(S) ssGetSFcnParam(S, ST1_IDX)

#define ST2_IDX 1
#define ST2_PARAM(S) ssGetSFcnParam(S, ST2_IDX)

#define ST3_IDX 2
#define ST3_PARAM(S) ssGetSFcnParam(S, ST3_IDX)

#define NPARAMS 3

extern void UpdateBuffer(real_T* rptr, int_T* iptr, real_T* u) ;

#define MDL_CHECK_PARAMETERS
#if defined(MDL_CHECK_PARAMETERS) && defined(MATLAB_MEX_FILE)
  /* Function: mdlCheckParameters =============================================
   * Abstract:
   *    Validate our parameters to verify they are okay.
   */
  static void mdlCheckParameters(SimStruct *S)
  {
	  {      
          if ( *mxGetPr(ST1_PARAM(S)) <= 0 ) 
		  {
              ssSetErrorStatus(S,"parameter to S-function "
                               "\" Size of buffer \" must be greater than zero");
              return;
          }
		  if ( *mxGetPr(ST2_PARAM(S)) < 0 ) 
		  {
              ssSetErrorStatus(S,"parameter to S-function "
                               "\"Initial values \" must be greater than or equal to zero");
              return;
          }
		  if ( *mxGetPr(ST3_PARAM(S)) > *mxGetPr(ST1_PARAM(S)) ) 
		  {
              ssSetErrorStatus(S,"parameter to S-function "
                               "\"Number of output channels \" must be less than or equal to buffer size");
              return;
          }
	  }
  } 
  
#endif /* MDL_CHECK_PARAMETERS */

static void mdlInitializeSizes(SimStruct *S)
{
    
	int_T i = 0 ;
		
	ssSetNumSFcnParams(S, NPARAMS);  /* Number of expected parameters */
	
	#if defined(MATLAB_MEX_FILE)
    if (ssGetNumSFcnParams(S) == ssGetSFcnParamsCount(S)) 
	{
        mdlCheckParameters(S);
        if (ssGetErrorStatus(S) != NULL)
		{
            return;
        }
    } 
	else 
	{
        return; /* Parameter mismatch will be reported by Simulink */
    }
	#endif
	
	ssSetSFcnParamTunable(S,ST1_IDX, 0); // parameter is non tunable
	ssSetSFcnParamTunable(S,ST2_IDX, 0);
	ssSetSFcnParamTunable(S,ST3_IDX, 0);
    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);

    if (!ssSetNumInputPorts(S, 1)) return;
    ssSetInputPortWidth(S, 0, 1);
	
    ssSetInputPortRequiredContiguous(S, 0, true); /*direct input signal access*/
	
    /*
     * Set direct feedthrough flag (1=yes, 0=no).
     * A port has direct feedthrough if the input is used in either
     * the mdlOutputs or mdlGetTimeOfNextVarHit functions.
     * See matlabroot/simulink/src/sfuntmpl_directfeed.txt.
     */
    ssSetInputPortDirectFeedThrough(S, 0, 1);
	

    if (!ssSetNumOutputPorts(S, 1)) return;
    ssSetOutputPortWidth(S, 0, (int)(*mxGetPr(ST3_PARAM(S))));
	

    ssSetNumSampleTimes(S, 1);

    ssSetNumRWork(S, 50); //using work vectors 
    ssSetNumIWork(S, 4);
    ssSetNumPWork(S, 0);
    ssSetNumModes(S, 0);
    ssSetNumNonsampledZCs(S, 0);

    ssSetOptions(S, 0);
}



/* Function: mdlInitializeSampleTimes =========================================
 * Abstract:
 *    This function is used to specify the sample time(s) for your
 *    S-function. You must register the same number of sample times as
 *    specified in ssSetNumSampleTimes.
 */
static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
    ssSetOffsetTime(S, 0, 0.0);

}



#define MDL_INITIALIZE_CONDITIONS   /* Change to #undef to remove function */
#if defined(MDL_INITIALIZE_CONDITIONS)
  /* Function: mdlInitializeConditions ========================================
   * Abstract:
   *    In this function, you should initialize the continuous and discrete
   *    states for your S-function block.  The initial states are placed
   *    in the state vector, ssGetContStates(S) or ssGetRealDiscStates(S).
   *    You can also perform any other initialization activities that your
   *    S-function may require. Note, this routine will be called at the
   *    start of simulation and if it is present in an enabled subsystem
   *    configured to reset states, it will be call when the enabled subsystem
   *    restarts execution to reset the states.
   */
  static void mdlInitializeConditions(SimStruct *S)
  {
	  int i = 0 ;
	  real_T* rptr = ssGetRWork(S) ;
	  int_T* iptr = ssGetIWork(S) ;
	  for(i = 0; i<50; i++)
		  rptr[i] = (*mxGetPr(ST2_PARAM(S))) ;
	  iptr[0] = (int)(*mxGetPr(ST1_PARAM(S))) ; //max buffer size
	  iptr[1] = (int)(*mxGetPr(ST3_PARAM(S))) ; //max output width
	  iptr[2] = 0 ; //buffer_top
	  iptr[3] = 0 ; //stack top


  }
#endif /* MDL_INITIALIZE_CONDITIONS */



#define MDL_START  /* Change to #undef to remove function */
#if defined(MDL_START) 
  /* Function: mdlStart =======================================================
   * Abstract:
   *    This function is called once at start of model execution. If you
   *    have states that should be initialized once, this is the place
   *    to do it.
   */
  static void mdlStart(SimStruct *S)
  {
  }
#endif /*  MDL_START */



/* Function: mdlOutputs =======================================================
 * Abstract:
 *    In this function, you compute the outputs of your S-function
 *    block. Generally outputs are placed in the output vector, ssGetY(S).
 */
static void mdlOutputs(SimStruct *S, int_T tid)
{
	real_T* rptr = ssGetRWork(S) ;
	int_T* iptr = ssGetIWork(S) ;
    real_T *u = (real_T*) ssGetInputPortSignal(S,0);
    real_T       *y = ssGetOutputPortSignal(S,0);

	int_T i = 0 ;
	UpdateBuffer(rptr, iptr, u) ;
	for(i=0; i <iptr[1]; i++)
	{	
		if((iptr[2]) < 0) 
			iptr[2] = iptr[0]-1 ;
		y[i] = rptr[iptr[2]] ;			
		iptr[2]-- ;
	}

}



#define MDL_UPDATE  /* Change to #undef to remove function */
#if defined(MDL_UPDATE)
  /* Function: mdlUpdate ======================================================
   * Abstract:
   *    This function is called once for every major integration time step.
   *    Discrete states are typically updated here, but this function is useful
   *    for performing any tasks that should only take place once per
   *    integration step.
   */
  static void mdlUpdate(SimStruct *S, int_T tid)
  {
  }
#endif /* MDL_UPDATE */



#define MDL_DERIVATIVES  /* Change to #undef to remove function */
#if defined(MDL_DERIVATIVES)
  /* Function: mdlDerivatives =================================================
   * Abstract:
   *    In this function, you compute the S-function block's derivatives.
   *    The derivatives are placed in the derivative vector, ssGetdX(S).
   */
  static void mdlDerivatives(SimStruct *S)
  {
  }
#endif /* MDL_DERIVATIVES */



/* Function: mdlTerminate =====================================================
 * Abstract:
 *    In this function, you should perform any actions that are necessary
 *    at the termination of a simulation.  For example, if memory was
 *    allocated in mdlStart, this is the place to free it.
 */
static void mdlTerminate(SimStruct *S)
{
	
}

#define MDL_RTW  
#if defined(MDL_RTW) && defined(MATLAB_MEX_FILE)
  /* Function: mdlRTW =========================================================*/

static void mdlRTW(SimStruct *S)
{
  real_T bsize =mxGetPr(ST1_PARAM(S))[0] ;
  real_T ival = mxGetPr(ST2_PARAM(S))[0] ;
  real_T osize = mxGetPr(ST3_PARAM(S))[0] ;
  
  if (!ssWriteRTWWorkVect(S, "RWork", 1,   "buffer_real", 50)) return ;
  if (!ssWriteRTWWorkVect(S, "IWork", 1,   "buffer_int", 4)) return ;   

  if(!ssWriteRTWParamSettings(S, 3,SSWRITE_VALUE_NUM, "BufSize", bsize,
                                      SSWRITE_VALUE_NUM, "InitValue", ival,
                                      SSWRITE_VALUE_NUM, "OutSize", osize) )
									  return;
}   
  
#endif /* MDL_RTW */



/*=============================*
 * Required S-function trailer *
 *=============================*/

#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif
