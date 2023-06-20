#include "run.h"
#include <stdio.h>
#include "usart.h"
#include "dht11.h"
#include "fan.h"
#include "tim.h"
#include "cmd_link.h"
#include "buzzer.h"
#include "flash.h"
#include "execute.h"
#include "adc.h"
#include "self_check.h"

RUN_T run_t; 

static void Single_Power_ReceiveCmd(uint8_t cmd);
static void Single_Command_ReceiveCmd(uint8_t cmd); 
static void Fan_ContinueRun_OneMinute_Fun(void);

volatile uint8_t run_state =0;

uint8_t no_buzzer_sound_dry_off;



/**********************************************************************
*
*Function Name:void Decode_RunCmd(void)
*Function: receive usart touchkey of command 
*Input Ref:NO
*Return Ref:NO
*
**********************************************************************/
void Decode_RunCmd(void)
{
  uint16_t count_total_times;
//  uint8_t count_rem_times_one,count_rem_times_two;
  uint8_t cmdType_1=inputCmd[0],cmdType_2 = inputCmd[1];

    
  switch(cmdType_1){
  
      case 'P': //power on and off
        
      	Single_Power_ReceiveCmd(cmdType_2);  
           
      break;
      

	  case 'W': //wifi-function
	      if(run_t.gPower_On==POWER_ON){
	      if(cmdType_2==1){//long press key that power key
              //fast blink led for link to tencent cloud
            
			
	      
			   
		   }
		 }
       
	   break;
        
      case 'C':
           if(run_t.gPower_On==POWER_ON){
              Single_Command_ReceiveCmd(cmdType_2); //Single_ReceiveCmd(cmdType_2); 
              #if DEBUG
	        	printf("rx_dp=%d\n",cmdType_2);
	         #endif 
              
           }
     
         
      break;

	  case 'M': //set up temperature value
	  	if(run_t.gPower_On==POWER_ON){
              
             run_t.set_temperature_value = cmdType_2;
		     #if DEBUG

			 printf("rx_ds_data\n");

			 #endif 
			   
         }
	  break;

	  case 'T': //set up tiemr timing
		  if(run_t.gPower_On==POWER_ON){
           
	
			   
         }
	  break;

	  case 'O': //works how long times minute ?
          if(run_t.gPower_On==POWER_ON){

		   
           
        }
	  break;

	  case 'R': //remaining time minutes value ?
         
      break;

	  
	  case 'Z' ://buzzer sound 
	    if(run_t.gPower_On==POWER_ON){

		    if(cmdType_2== 'Z'){//turn off AI
		       run_t.buzzer_sound_flag = 1;
			    
			}
			 
		
		}
     
	    break;
 	
}   
}
/**********************************************************************
	*
	*Functin Name: void Single_ReceiveCmd(uint8_t cmd)
	*Function : resolver is by usart port receive data  from display panle  
	*Input Ref:  usart receive data
	*Return Ref: NO
	*
**********************************************************************/
static void Single_Power_ReceiveCmd(uint8_t cmd)
{
  
  static uint8_t  first_power_on_buzzer=0xff,set_power_on_buzzer;
    static uint8_t  first_power_off_buzzer=0xff,set_power_off_buzzer;
    switch(cmd){

    case 0x00: //power off
       set_power_on_buzzer++;
         if(first_power_off_buzzer != set_power_off_buzzer){
		   first_power_off_buzzer=set_power_off_buzzer;
			Buzzer_KeySound();

       	}
        printf("poff\n");
		run_t.buzzer_sound_flag=0;
		//Answering_Signal_USART1_Handler(COMMAND_ID,ANSWER_POWER_OFF);
	   run_t.RunCommand_Label= POWER_OFF;

 

    cmd = 0xff;
    break;

    case 0x01: // power on
       set_power_off_buzzer ++ ;
        if(first_power_on_buzzer != set_power_on_buzzer){
		   first_power_on_buzzer=set_power_on_buzzer;
           Buzzer_KeySound();
        }
		run_t.buzzer_sound_flag=0;
		  printf("pon\n");
        //Answering_Signal_USART1_Handler(COMMAND_ID,ANSWER_POWER_ON);
		run_t.RunCommand_Label= POWER_ON;
		

		 
	 cmd=0xff;  
     break;

    

     default:

     break;

    }

}
/**********************************************************************
	*
	*Functin Name: void Single_ReceiveCmd(uint8_t cmd)
	*Function : resolver is by usart port receive data  from display panle  
	*Input Ref:  usart receive data
	*Return Ref: NO
	*
**********************************************************************/
static void Single_Command_ReceiveCmd(uint8_t cmd)
{

    
	switch(cmd){

	    case DRY_ON_NO_BUZZER:

	      no_buzzer_sound_dry_off=1;

       case DRY_ON:
         run_t.gDry = 1;
		 run_t.gFan_by_key_turn_off=0;
       	 run_t.gFan =1;
		 if(no_buzzer_sound_dry_off==0){
		  	    Buzzer_KeySound();
		  }
		  else no_buzzer_sound_dry_off=0;
		
		   
		 
       break;

	   case DRY_OFF_NO_BUZZER :

	         no_buzzer_sound_dry_off=1;

	  case DRY_OFF:
 			run_t.gDry = 0;
			
			if(no_buzzer_sound_dry_off==0){
				Buzzer_KeySound();
            }
			else no_buzzer_sound_dry_off=0;
		
			
		
			   
       break;

       case PLASMA_ON:
       		run_t.gPlasma=1;
			run_t.gFan_by_key_turn_off=0;
       	    run_t.gFan =1;
			Buzzer_KeySound();
	  
	    
       break;

       case PLASMA_OFF:
           run_t.gPlasma=0;
		   Buzzer_KeySound();
	
	   
       break;

       case  FAN_ON:
           run_t.gFan_by_key_turn_off = 0;
		   run_t.gFan_continueRun=0;
	       run_t.gFan =1;
		   Buzzer_KeySound();
      break;

	   case FAN_OFF:
	   	   run_t.gFan_by_key_turn_off = 1;
		   run_t.gFan_continueRun=1;
	   	   run_t.gFan =0;
		   run_t.gPlasma=0;
	       run_t.gDry = 0;
		   Buzzer_KeySound();
	    

	   break;


      default :
        cmd =0;

      break; 


    }



}
/**********************************************************************
	*
	*Functin Name: void Single_ReceiveCmd(uint8_t cmd)
	*Function : resolver is by usart port receive data  from display panle  
	*Input Ref:  usart receive data
	*Return Ref: NO
	*
**********************************************************************/
void SystemReset(void)
{
    if(run_t.gPower_On ==POWER_ON){
	
		
		__set_PRIMASK(1) ;
		HAL_NVIC_SystemReset();
    }

}

/**********************************************************************
	*
	*Functin Name: void  RunCommand_MainBoard_Fun(void)
	*Function : be check key of value 
	*Input Ref:  key of value
	*Return Ref: NO
	*
**********************************************************************/
void RunCommand_MainBoard_Fun(void)
{

  static uint8_t power_off_flag=0,power_off_fan;
 
    if(run_t.buzzer_sound_flag == 1){
	 	run_t.buzzer_sound_flag = 0;
	    Buzzer_KeySound();

	 }
  
   switch(run_t.RunCommand_Label){

	case POWER_ON: //1
	   run_t.power_on_send_data_flag=0;
	 
        run_t.gPower_On = POWER_ON;
		run_t.gTimer_10s=0;
		run_t.gTimer_continuce_works_time=0;
		run_t.interval_time_stop_run=0;
		 run_t.fan_warning =0;
		 run_t.ptc_warning =0;
		 run_t.gTimer_ptc_adc_times=0;
		 run_t.open_ptc_detected_flag=0;
    
     	SetPowerOn_ForDoing();

		
	run_t.RunCommand_Label= UPDATE_TO_PANEL_DATA;

	break;

    case POWER_OFF: //2
	     run_t.gPower_On = POWER_OFF;
		 run_t.power_on_send_data_flag=0;
	     run_t.gTimer_continuce_works_time=0;
		 run_t.interval_time_stop_run=0;
		 run_t.fan_warning =0;
		 run_t.ptc_warning =0;

        power_off_flag=0;
		if(power_off_fan==0){
             
		    power_off_fan++;
			run_t.gFan_counter =0;
		    run_t.gFan_continueRun =0;


		}
		else{
			run_t.gFan_counter =0;
			run_t.gFan_continueRun =1;
		}
	
	
	   run_t.RunCommand_Label= FAN_CONTINUCE_RUN_ONE_MINUTE;
	 break;

	case UPDATE_TO_PANEL_DATA: //3

		switch(run_t.interval_time_stop_run){

            case 0 :

	        

		    switch(run_state){

			    case 0:
				if(run_t.gTimer_senddata_panel >40 && run_t.gPower_On==POWER_ON){ //300ms
				run_t.gTimer_senddata_panel=0;

				ActionEvent_Handler();
			

				}
                run_state =1;
				break;

				case 1:

				if((run_t.gTimer_10s>34 && run_t.gPower_On == POWER_ON)|| run_t.power_on_send_data_flag < 2){
					run_t.power_on_send_data_flag ++ ;
					run_t.gTimer_10s=0;
					Update_DHT11_Value();
					HAL_Delay(2);
				}
				 run_state =2;

				break;

				case 2:

				if(run_t.gTimer_ptc_adc_times > 64){ //3 minutes 120s
					run_t.gTimer_ptc_adc_times=0;
					Get_PTC_Temperature_Voltage(ADC_CHANNEL_1,5);
					//run_t.ptc_temp_voltage=200;
					Judge_PTC_Temperature_Value();


				}
				 run_state =3;
                break;

			    case 3:
				
				if(run_t.gTimer_fan_adc_times > 72 && run_t.gFan_by_key_turn_off==0){ //2 minute 180s
					run_t.gTimer_fan_adc_times =0;

					//Self_CheckFan_Handler(ADC_CHANNEL_0,5);
					Get_Fan_Adc_Fun(ADC_CHANNEL_0,5);
					HAL_Delay(10);

				}
                  run_state =4;
				break;


				case 4:

				if(run_t.gTimer_continuce_works_time > 7200){
			     run_t.gTimer_continuce_works_time =0;
		         run_t.interval_time_stop_run =1;
			     run_t.gFan_continueRun =1;
				 run_t.gFan_counter=0;
			    }

				if(run_t.interval_time_stop_run ==0)
				run_state =5;
				break;

				case 5:

				  if(run_t.gFan_continueRun ==1 && run_t.interval_time_stop_run ==0){

					if(run_t.gFan_counter < 60){

					   FAN_CCW_RUN();
					}       

					if(run_t.gFan_counter > 59){

					run_t.gFan_counter=0;

					run_t.gFan_continueRun++;
					FAN_Stop();
					}

				  }
               if(run_t.interval_time_stop_run ==0) run_state =6;

				break;

			   case  6:
               if(run_t.fan_warning ==1 && run_t.gPower_On == POWER_ON){

                   if(run_t.gTimer_fan_adc_times > 1 ){
				   run_t.gTimer_fan_adc_times=0;

				   	#if DEBUG
					 printf("fan_warning\n");

					#endif
			       HAL_Delay(200);
			       Buzzer_KeySound();
			       HAL_Delay(100);
				   Buzzer_KeySound();
			       HAL_Delay(100);
				   Buzzer_KeySound();
			       HAL_Delay(100);
				   Buzzer_KeySound();
			       HAL_Delay(100);
				   SendWifiCmd_To_Order(FAN_WARNING);
				  
                   	}
			   	}
			   


			   if(run_t.ptc_warning==1 && run_t.gPower_On == POWER_ON){

			     if(run_t.gTimer_ptc_adc_times > 2){
				 	run_t.gTimer_ptc_adc_times=0;
			   	    
 					#if DEBUG
					 printf("ptc_warning\n");

					#endif 
			         Buzzer_KeySound();
                    HAL_Delay(200);
			       Buzzer_KeySound();
			       HAL_Delay(100);
				   Buzzer_KeySound();
			       HAL_Delay(100);
				   Buzzer_KeySound();
			       HAL_Delay(100);
				   Buzzer_KeySound();
			       HAL_Delay(100);
				   SendWifiCmd_To_Order(PTC_WARNING);
			     }

			   }
			    run_state =0;
			   break;

			}
		break;

		case 1:
			    if(run_t.interval_time_stop_run ==1){

				if(run_t.gTimer_continuce_works_time < 10){
					PLASMA_SetLow(); //
					HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic Off 
					PTC_SetLow(); 
				}
			
				if(run_t.gTimer_continuce_works_time > 600){
					run_t.gTimer_continuce_works_time=0;
					run_t.interval_time_stop_run =0;
				    run_state =0;


				}

				if(run_t.gFan_continueRun ==1){

					if(run_t.gFan_counter < 60){

					   FAN_CCW_RUN();
					}       

					if(run_t.gFan_counter > 59){

					run_t.gFan_counter=0;

					run_t.gFan_continueRun++;
					FAN_Stop();
					}

				}
			    }
				else{

					run_t.gTimer_continuce_works_time=0;
					run_t.interval_time_stop_run =0;
				    run_state =0;

				}

		break;
		}
    break;

	case  FAN_CONTINUCE_RUN_ONE_MINUTE:

	   if(power_off_flag==0){
		 	power_off_flag++;
			SetPowerOff_ForDoing();
          //  Answering_Signal_USART1_Handler(COMMAND_ID,ANSWER_POWER_OFF);
     
		 }
         
	     Fan_ContinueRun_OneMinute_Fun();

	break;
	}
	
}

static void Fan_ContinueRun_OneMinute_Fun(void)
{
	
	if(run_t.gFan_continueRun ==1 && run_t.gPower_On ==POWER_OFF){
          
		if(run_t.gFan_counter < 60){

		    FAN_CCW_RUN();
		}       
    else if(run_t.gFan_counter > 59){

		run_t.gFan_counter=0;

		run_t.gFan_continueRun++;
		FAN_Stop();
		}
	  }



}



