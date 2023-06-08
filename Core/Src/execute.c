#include "execute.h"
#include "dht11.h"
#include "fan.h"
#include "tim.h"
#include "cmd_link.h"
#include "run.h"
#include "wifi_fun.h"
#include "esp8266.h"
#include "buzzer.h"



void SetPowerOn_ForDoing(void)
{

  
   run_t.gPower_On=POWER_ON;
	run_t.open_fan_works_flag++;
     //FAN_CCW_RUN();
	run_t.gFan = 1;
   run_t.gDry = 1;
	run_t.gPlasma =1;       //"杀菌"
	run_t.gUltrasonic = 1; // "驱虫"
		// Fan_Run_Fun();
	 //    PLASMA_SetHigh(); //
	 //    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);//ultrasnoic ON 
	 //    PTC_SetHigh(); //bug is be detect 2023.04.30
    
   
	
   
	
 }

void SetPowerOff_ForDoing(void)
{

   
	
	run_t.gPower_On = POWER_OFF;

 
    run_t.gFan = 0;
    run_t.gDry = 0;
	run_t.gPlasma =0;       //"杀菌"
	run_t.gUltrasonic = 0; // "驱虫"
	
	
	PLASMA_SetLow(); //
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic Off 
	PTC_SetLow();
	 //input ac power be detect fan default 
	FAN_Stop();

}



void ActionEvent_Handler(void)
{
   if(run_t.ptc_too_heat_value ==0){
	if(run_t.gDry == 1){
	
        PTC_SetHigh();
	    run_t.fan_start_shut_off_flag=1;
	}
	else{
	   PTC_SetLow();
	   run_t.fan_start_shut_off_flag=0;

	}
   }
    
	//kill
	if(run_t.gPlasma == 1){
	     PLASMA_SetHigh();
		 run_t.fan_start_shut_off_flag=1;
	}
	else{

		PLASMA_SetLow();
		run_t.fan_start_shut_off_flag=0;
	}

    if(run_t.fan_detect_malfuntion== 0){
		if(run_t.set_wind_speed_value==0){
			 run_t.fan_set_level = 1;
		     SetLevel_Fan_PWMA(99);
	    }
		else{
			 run_t.fan_set_level = 2;

		     SetLevel_Fan_PWMA(100);

		}
    }
	//driver bug
	if(run_t.gUltrasonic ==1){
	
	   HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);//ultrasnoic ON 
	}
	else{
	  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic off

	}
}

