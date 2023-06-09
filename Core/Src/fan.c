#include "fan.h"
#include "main.h"
#include "tim.h"
#include "run.h"
#include "delay.h"
#include "adc.h"




void FAN_CCW_RUN(void)
{
   FAN_CW_SetLow();
   FAN_CCW_SetHigh(); //PA6
  
}
 
void FAN_Stop(void)
{
    FAN_CCW_SetLow(); //brake
    FAN_CCW_SetLow(); //PA6
    
}
void ShutDown_AllFunction(void)
{
	
	PLASMA_SetLow(); //
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic ON 
	PTC_SetLow();
	FAN_Stop();



}




//"杀毒" 
void SterIlization(uint8_t sel)
{
    if(sel==0){//open 
		 
		 PLASMA_SetHigh();
		

	}
	else{ //close

		PLASMA_SetLow();
	

	}



}
//PTC
void Dry_Function(uint8_t sel)
{
   if(sel ==0){

  

		PTC_SetHigh();

   }
   else{

       PTC_SetLow();

   }

}
//FAN 
void Fan_Function(uint8_t sel)
{

	if(sel ==0){
	
	  FAN_CCW_RUN();
	
	}
	else{
	
		FAN_Stop() ;
	
	}



}

