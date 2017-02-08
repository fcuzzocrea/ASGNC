

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
#include "C:\Users\Francescodario\Desktop\Libreria - v0.4\lib\Wire.h";
#include "C:\Users\Francescodario\Desktop\Libreria - v0.4\lib\Wire.cpp";
#include "C:\Users\Francescodario\Desktop\Libreria - v0.4\lib\twi.h";
#include "C:\Users\Francescodario\Desktop\Libreria - v0.4\lib\twi.c";

#define MPU6050_I2C_ADDRESS        0x68
#define MPU6050_PWR_MGMT_1         0x6B   // R/W
#define MPU6050_ACCEL_XOUT_H       0x3B   // R  
#define accelerometer_sensitivity  16.384
#define gyroscope_sensitivity      131

typedef union accel_t_gyro_union
{
  struct
  {
    unsigned char x_accel_h;
    unsigned char x_accel_l;
    unsigned char y_accel_h;
    unsigned char y_accel_l;
    unsigned char z_accel_h;
    unsigned char z_accel_l;
    unsigned char t_h;
    unsigned char t_l;
    unsigned char x_gyro_h;
    unsigned char x_gyro_l;
    unsigned char y_gyro_h;
    unsigned char y_gyro_l;
    unsigned char z_gyro_h;
    unsigned char z_gyro_l;
  } reg;
  struct
  {
    int x_accel;
    int y_accel;
    int z_accel;
    int temperature;
    int x_gyro;
    int y_gyro;
    int z_gyro;
  } value;
};

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

int MPU6050_read(int start, unsigned char *buffer, int size)
{
  int i, n, error;
 
  Wire.beginTransmission(MPU6050_I2C_ADDRESS);
  n = Wire.write(start);
  if (n != 1)
    return (-10);
 
  n = Wire.endTransmission();    // hold the I2C-bus
  if (n != 0)
    return (n);
 
  // Third parameter is true: relase I2C-bus after data is read.
  Wire.requestFrom(MPU6050_I2C_ADDRESS, size);
  i = 0;
  while(Wire.available() && i<size)
  {
    buffer[i++]=Wire.read();
  }
  if ( i != size)
    return (-11);
 
  return (0);  // return : no error
}

int MPU6050_write(int start, const unsigned char *pData, int size)
{
  int n, error;
 
  Wire.beginTransmission(MPU6050_I2C_ADDRESS);
  n = Wire.write(start);        // write the start address
  if (n != 1)
    return (-20);
 
  n = Wire.write(pData, size);  // write data bytes
  if (n != size)
    return (-21);
 
  error = Wire.endTransmission(); // release the I2C-bus
  if (error != 0)
    return (error);
 
  return (0);         // return : no error
}

int MPU6050_write_reg(int reg, unsigned char data)
{
  int error;
 
  error = MPU6050_write(reg, &data, 1);
 
  return (error);
}

# endif
/* %%%-SFUNWIZ_wrapper_externs_Changes_END --- EDIT HERE TO _BEGIN */

/*
 * Output functions
 *
 */
extern "C" void MPU6050_Outputs_wrapper(real_T *acc_X,
			real_T *acc_Y,
			real_T *acc_Z,
			real_T *gyro_X,
			real_T *gyro_Y,
			real_T *gyro_Z,
			const real_T *xD)
{
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_BEGIN --- EDIT HERE TO _END */
if (xD[0]==1) {    
  # ifndef MATLAB_MEX_FILE
    int error;
    accel_t_gyro_union accel_t_gyro;
    error = MPU6050_read (MPU6050_ACCEL_XOUT_H, (unsigned char *) &accel_t_gyro, sizeof(accel_t_gyro));
    Serial.println(error,DEC);
    unsigned char swap;
    #define SWAP(x,y) swap = x; x = y; y = swap
    SWAP (accel_t_gyro.reg.x_accel_h, accel_t_gyro.reg.x_accel_l);
    SWAP (accel_t_gyro.reg.y_accel_h, accel_t_gyro.reg.y_accel_l);
    SWAP (accel_t_gyro.reg.z_accel_h, accel_t_gyro.reg.z_accel_l);
    SWAP (accel_t_gyro.reg.t_h, accel_t_gyro.reg.t_l);
    SWAP (accel_t_gyro.reg.x_gyro_h, accel_t_gyro.reg.x_gyro_l);
    SWAP (accel_t_gyro.reg.y_gyro_h, accel_t_gyro.reg.y_gyro_l);
    SWAP (accel_t_gyro.reg.z_gyro_h, accel_t_gyro.reg.z_gyro_l);
    acc_X[0] = accel_t_gyro.value.x_accel/accelerometer_sensitivity;
    acc_Y[0] = accel_t_gyro.value.y_accel/accelerometer_sensitivity;
    acc_Z[0] = accel_t_gyro.value.z_accel/accelerometer_sensitivity;
    gyro_X[0] = accel_t_gyro.value.x_gyro/gyroscope_sensitivity;
    gyro_Y[0] = accel_t_gyro.value.y_gyro/gyroscope_sensitivity;
    gyro_Z[0] = accel_t_gyro.value.z_gyro/gyroscope_sensitivity;
  # endif
}
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_END --- EDIT HERE TO _BEGIN */
}

/*
  * Updates function
  *
  */
extern "C" void MPU6050_Update_wrapper(const real_T *acc_X,
			const real_T *acc_Y,
			const real_T *acc_Z,
			const real_T *gyro_X,
			const real_T *gyro_Y,
			const real_T *gyro_Z,
			real_T *xD)
{
  /* %%%-SFUNWIZ_wrapper_Update_Changes_BEGIN --- EDIT HERE TO _END */
if (xD[0]!=1) {
    
  # ifndef MATLAB_MEX_FILE
    int error;
    unsigned char c;
    //Serial.begin(9600);
    Wire.begin();
    error = MPU6050_read (MPU6050_PWR_MGMT_1, &c, 1);
    MPU6050_write_reg (MPU6050_PWR_MGMT_1, 0);
  # endif   
  // initialization done 
  xD[0]=1;
}
/* %%%-SFUNWIZ_wrapper_Update_Changes_END --- EDIT HERE TO _BEGIN */
}
