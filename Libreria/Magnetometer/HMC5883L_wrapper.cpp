

/*
 * Include Files
 *
 */
#if defined(MATLAB_MEX_FILE)
#include "tmwtypes.h"
#include "simstruc_types.h"
#else
#include "rtwtypes.h"
#endif

/* %%%-SFUNWIZ_wrapper_includes_Changes_BEGIN --- EDIT HERE TO _END */
#ifndef MATLAB_MEX_FILE

#include <Arduino.h>
#include <Math.h>
#include "C:\Users\Francescodario\Desktop\Libreria - v0.4\lib\Wire.h";
#include "C:\Users\Francescodario\Desktop\Libreria - v0.4\lib\Wire.cpp";
#include "C:\Users\Francescodario\Desktop\Libreria - v0.4\lib\twi.h";
#include "C:\Users\Francescodario\Desktop\Libreria - v0.4\lib\twi.c";

#define HMC5883L_Address       0x1E
#define ConfigurationRegisterA 0x00
#define ConfigurationRegisterB 0x01
#define ModeRegister           0x02
#define DataRegisterBegin      0x03
#define Measurement_Continuous 0x00
#define Measurement_SingleShot 0x01
#define Measurement_Idle 0x03
#define ErrorCode_1 "Entered scale was not valid, valid gauss values are: 0.88, 1.3, 1.9, 2.5, 4.0, 4.7, 5.6, 8.1"
#define ErrorCode_1_Num 1

float m_Scale;
float norm;

# endif
/* %%%-SFUNWIZ_wrapper_includes_Changes_END --- EDIT HERE TO _BEGIN */
#define u_width 
#define y_width 1
/*
 * Create external references here.  
 *
 */
/* %%%-SFUNWIZ_wrapper_externs_Changes_BEGIN --- EDIT HERE TO _END */
#ifndef MATLAB_MEX_FILE

int SetScale(float gauss)
{
	uint8_t regValue = 0x00;
	if(gauss == 0.88)
	{
		regValue = 0x00;
		m_Scale = 0.73;
	}
	else if(gauss == 1.3)
	{
		regValue = 0x01;
		m_Scale = 0.92;
	}
	else if(gauss == 1.9)
	{
		regValue = 0x02;
		m_Scale = 1.22;
	}
	else if(gauss == 2.5)
	{
		regValue = 0x03;
		m_Scale = 1.52;
	}
	else if(gauss == 4.0)
	{
		regValue = 0x04;
		m_Scale = 2.27;
	}
	else if(gauss == 4.7)
	{
		regValue = 0x05;
		m_Scale = 2.56;
	}
	else if(gauss == 5.6)
	{
		regValue = 0x06;
		m_Scale = 3.03;
	}
	else if(gauss == 8.1)
	{
		regValue = 0x07;
		m_Scale = 4.35;
	}
	else
		return ErrorCode_1_Num;
	
	// Setting is in the top 3 bits of the register.
	regValue = regValue << 5;
  	Wire.beginTransmission(HMC5883L_Address);
  	Wire.write(ConfigurationRegisterB);
  	Wire.write(regValue);
  	Wire.endTransmission();
}

# endif
/* %%%-SFUNWIZ_wrapper_externs_Changes_END --- EDIT HERE TO _BEGIN */

/*
 * Output functions
 *
 */
extern "C" void HMC5883L_Outputs_wrapper(real_T *X_Axis,
			real_T *Y_Axis,
			real_T *Z_Axis,
			const real_T *xD)
{
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_BEGIN --- EDIT HERE TO _END */
if (xD[0]==1) {    
  # ifndef MATLAB_MEX_FILE
    int x,y,z; //triple axis data
    Wire.beginTransmission(HMC5883L_Address);
    Wire.write(DataRegisterBegin); //select register 3, X MSB register
    Wire.endTransmission();
    Wire.requestFrom(HMC5883L_Address, 6);
     if(6<=Wire.available()){
       x = Wire.read()<<8; //X msb
       x |= Wire.read(); //X lsb
       z = Wire.read()<<8; //Z msb
       z |= Wire.read(); //Z lsb
       y = Wire.read()<<8; //Y msb
       y |= Wire.read(); //Y lsb
        }
    X_Axis[0]=x*m_Scale;
    Y_Axis[0]=y*m_Scale;
    Z_Axis[0]=z*m_Scale;
    norm  = sqrt(pow(X_Axis[0],2) + pow(Y_Axis[0],2) + pow(Z_Axis[0],2));
    X_Axis[0] = X_Axis[0]/norm;
    Y_Axis[0] = Y_Axis[0]/norm;
    Z_Axis[0] = Z_Axis[0]/norm;
  # endif
}
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_END --- EDIT HERE TO _BEGIN */
}

/*
  * Updates function
  *
  */
extern "C" void HMC5883L_Update_wrapper(const real_T *X_Axis,
			const real_T *Y_Axis,
			const real_T *Z_Axis,
			real_T *xD)
{
  /* %%%-SFUNWIZ_wrapper_Update_Changes_BEGIN --- EDIT HERE TO _END */
if (xD[0]!=1) {
  # ifndef MATLAB_MEX_FILE
      Wire.begin();
    //Put the HMC5883 IC into the correct operating mode
      Wire.beginTransmission(HMC5883L_Address); //open communication with HMC5883
      Wire.write(ModeRegister); //select mode register
      Wire.write(Measurement_Continuous); //continuous measurement mode  
      Wire.endTransmission();
      SetScale(1.3);                      // Set compass scale
  # endif   
  // initialization done 
  xD[0]=1;
}
/* %%%-SFUNWIZ_wrapper_Update_Changes_END --- EDIT HERE TO _BEGIN */
}
