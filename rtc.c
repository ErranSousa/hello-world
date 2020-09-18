//
#include <stm32l0xx.h>
//#include <stm32l0xx_hal.h>
//#include <stm32l0xx_hal_rcc.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "main.h"
#include "rtc.h"
#include "uart_comms.h"

extern RTC_HandleTypeDef hrtc;

uint32_t sys_tickCount;

void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
}


/**
  ******************************************************************************
  * @brief Place the PDS Controller into STOP mode
  * @param number of seconds to remain in STOP
  * @retval None
	******************************************************************************
  */

/**
  ******************************************************************************
  * @brief Place the PDS Controller into STOP mode
  * @param number of seconds to remain in STOP
  * @retval None
	******************************************************************************
  */
uint32_t SleepUntilRTCTime(uint32_t wakeSeconds)
{
		return 1;
}		//___  SleepUntilRTCTime ___//





/**********************************************************************************************************
 Functions that will eventually go into their own files
 **********************************************************************************************************/
 
/**
  ******************************************************************************
  * @brief Sets the Internal RTCC Date and Time
  * @param None
  * @retval None
	******************************************************************************
  */		 
void RTCCInt_SetDateTime(_Bool getFromUser, RTC_TimeTypeDef *Time, RTC_DateTypeDef *Date)
{
		RTC_TimeTypeDef RTC_TimeStructure = {0,0,0,0,0,0,0,0};
		RTC_DateTypeDef RTC_DateStructure = {0,0,0,0};
	
		HAL_StatusTypeDef rtcDateStatus;
		HAL_StatusTypeDef rtcTimeStatus;
		struct tm theTime = {0,0,0,0,0,0,0,0,0};
		
		int32_t yy,mm,dd,hh,mn,ss;
		int16_t count = 0;
		int32_t yeartemp;
		char dateString[64]; //long enough to cover almost all screwups..
	
		if ( getFromUser == true )
		{
			/* Read the current internal RTCC Date/Time */ 
			uint32_t secs = 0;
			secs = RTCCInt_ReadDateTime(&theTime);
			//! \todo Implement something to check secs
		
			theTime.tm_year -= 100;	 
			theTime.tm_mon++;				// account for zero indexing of month....
	
			printf("\n\r\n\rCurrent date and time [");
	
			if ( theTime.tm_year < 70 )
				yeartemp = 2000;
			else
				yeartemp = 1900;
		
			yeartemp += theTime.tm_year;
			printf("%d/", yeartemp);
	
			if (theTime.tm_mon < 10)
				printf("0%d/", theTime.tm_mon);
			else
				printf("%d/", theTime.tm_mon);

			if (theTime.tm_mday < 10)
				printf("0%d ", theTime.tm_mday);
			else
				printf("%d ", theTime.tm_mday);

			if (theTime.tm_hour < 10)
				printf("0%d:", theTime.tm_hour);
			else
				printf("%d:", theTime.tm_hour);

			if (theTime.tm_min < 10)
				printf("0%d:", theTime.tm_min);
			else
				printf("%d:", theTime.tm_min);

			if (theTime.tm_sec < 10)
				printf("0%d]: ", theTime.tm_sec);
			else
				printf("%d]\n\rEnter the new date [yyyy/mm/dd hh:mm:ss]: ",theTime.tm_sec);
		
			uartGetString(LOPWRUART1,dateString,64);

			mm = dd = yy = hh = mn = ss = 0;
			count = (sscanf (dateString, "%d/%d/%d %d:%d:%d", &yy, &mm, &dd, &hh, &mn, &ss) );
			
			if (count != 6)	// Check for correct number of entries
			{
				uartPutString(LOPWRUART1,"\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a");    
				uartPutString(LOPWRUART1,"\a  WARNING!..... TIME NOT CHANGED!     Incorrect format entry");
				return;
			}
		
			//	Check for dopey numbers

			if ((yy < 1970) || (yy > 2050))	// do nothing
			{	
				uartPutString(LOPWRUART1,"\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a");
				uartPutString(LOPWRUART1,"\a  WARNING!..... TIME NOT CHANGED!     Invalid year entry");
				return;
			}
		
			if ( yy < 2000 )
			{
				RTC_DateStructure.Year = (uint8_t)(yy - 1900);
			}
			else
			{
				RTC_DateStructure.Year = (uint8_t)(yy - 2000);
			}
		
			if ((mm < 1) || (mm > 12))		// do nothing
			{	
				uartPutString(LOPWRUART1,"\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a");    
				uartPutString(LOPWRUART1,"\a  WARNING!..... TIME NOT CHANGED!     Invalid month entry");
				return;
			}
			RTC_DateStructure.Month = (uint8_t)mm;
		
			if ((dd < 1) || (dd > 31))		// do nothing
			{	
				uartPutString(LOPWRUART1,"\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a");    
				uartPutString(LOPWRUART1,"\a  WARNING!..... TIME NOT CHANGED!     Invalid day entry");
				return;
			}
			RTC_DateStructure.Date = (uint8_t)dd;
		
			if ((hh < 0 ) || (hh > 23))		// do nothing
			{	
				uartPutString(LOPWRUART1,"\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a");    
				uartPutString(LOPWRUART1,"\a  WARNING!..... TIME NOT CHANGED!     Invalid hour entry");
				return;
			}
			RTC_TimeStructure.Hours = (uint8_t)hh;
		
			if ((mn < 0) || (mn > 59))		// do nothing
			{	
				uartPutString(LOPWRUART1,"\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a");    
				uartPutString(LOPWRUART1,"\a  WARNING!..... TIME NOT CHANGED!     Invalid minute entry");
				return;
			}
			RTC_TimeStructure.Minutes = (uint8_t)mn;
		
			if ((ss < 0 ) || (ss > 59))		// do nothing
			{	
				uartPutString(LOPWRUART1,"\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a");    
				uartPutString(LOPWRUART1,"\a  WARNING!..... TIME NOT CHANGED!     Invalid seconds entry");
				return;
			}
			RTC_TimeStructure.Seconds = (uint8_t)ss;
		}
		else
		{
			RTC_DateStructure.Year = (uint8_t)(Date->Year);
			RTC_DateStructure.Month = (uint8_t)(Date->Month);
			RTC_DateStructure.Date = (uint8_t)(Date->Date);
			RTC_TimeStructure.Hours = (uint8_t)(Time->Hours);
			RTC_TimeStructure.Minutes = (uint8_t)(Time->Minutes);
			RTC_TimeStructure.Seconds = (uint8_t)(Time->Seconds);
		}
		
		rtcDateStatus = HAL_RTC_SetDate(&hrtc,&RTC_DateStructure,RTC_FORMAT_BIN);
		rtcTimeStatus = HAL_RTC_SetTime(&hrtc,&RTC_TimeStructure,RTC_FORMAT_BIN);
		
		if ( rtcDateStatus != HAL_OK || rtcTimeStatus != HAL_OK )
		{
			// TODO: We need to determine how to handle HAL errors setting/reading RTC
			printf("\n Unable to Set Internal RealTime Clock \n");
		}
		
}		//___  RTCCInt_SetDateTime() ___//
 
 
/**
  ******************************************************************************
  * @brief Reads the Internal RTCC Date and Time and puts it into theTime Structure
  * @param Time Structure
  * @retval Seconds elapsed since epoch (January 1, 1970)
	******************************************************************************
  */		
uint32_t RTCCInt_ReadDateTime(struct tm *theTime)
{
		RTC_TimeTypeDef RTC_Time = {0,0,0,0,0,0,0,0};
		RTC_DateTypeDef RTC_Date = {0,0,0,0};
		
		HAL_StatusTypeDef timeStatus = HAL_OK;
		HAL_StatusTypeDef dateStatus = HAL_OK;
		
		time_t rtcSecs = 0;	

		timeStatus = HAL_RTC_GetTime(&hrtc,&RTC_Time,RTC_FORMAT_BIN);
		dateStatus = HAL_RTC_GetDate(&hrtc,&RTC_Date,RTC_FORMAT_BIN);
		
		if ( timeStatus != HAL_OK || dateStatus != HAL_OK )
		{
			// TODO: We need to determine how to handle HAL errors setting/reading RTC
			printf("\n Error[RDT]:HAL_RTC_GetDate/Time() returned NOT OK");
		}
		
		DBG(printf("\n[RDT]yy:%d, mm:%d\n",RTC_Date.Year,RTC_Date.Month);)
	
		theTime->tm_year = RTC_Date.Year + 100;
		theTime->tm_mon = RTC_Date.Month - 1;
		theTime->tm_mday = RTC_Date.Date;
		theTime->tm_hour = RTC_Time.Hours;
		theTime->tm_min = RTC_Time.Minutes;
		theTime->tm_sec = RTC_Time.Seconds;
	
		DBG(printf("\n[RTCCIntRDT]%04d/%02d/%02d %02d:%02d:%02d\n",
						theTime->tm_year,theTime->tm_mon,theTime->tm_mday,
						theTime->tm_hour,theTime->tm_min,theTime->tm_sec);)
	
		rtcSecs = mktime(theTime);
//	rtcSecs = RTC_ToEpoch(&RTC_Time, &RTC_Date);
	
		return rtcSecs;

}		//___  RTCCInt_ReadDateTime() ___//


/**
  ******************************************************************************
  * @brief Read the internal RTC sub-seconds
  * @param none
  * @retval None
	******************************************************************************
  */
void RTCCInt_ReadSubSeconds(void)
{
		RTC_TimeTypeDef RTC_TimeStructure = {0,0,0,0,0,0,0,0};
		RTC_DateTypeDef RTC_DateStructure = {0,0,0,0};
		HAL_StatusTypeDef timeStatus = HAL_OK;
		HAL_StatusTypeDef dateStatus = HAL_OK;
		
		timeStatus = HAL_RTC_GetTime(&hrtc,&RTC_TimeStructure,RTC_FORMAT_BIN);
		dateStatus = HAL_RTC_GetDate(&hrtc,&RTC_DateStructure,RTC_FORMAT_BIN);
		
		if ( timeStatus != HAL_OK || dateStatus != HAL_OK )
		{
			// TODO: We need to determine how to handle HAL errors setting/reading RTC
			printf("\n ERROR[RSS]:HAL_RTC_GetDate/Time() returned NOT OK");
		}
	
		printf("\n\rSubSeconds: %d\n\rSecondFraction: %d\r\nCalculated: %d msecs",
						RTC_TimeStructure.SubSeconds,RTC_TimeStructure.SecondFraction,
						(RTC_TimeStructure.SubSeconds*1000)/(RTC_TimeStructure.SecondFraction+1));
	
}		//___  RTCCInt_ReadSubSeconds() ___//

 
/**
  ******************************************************************************
  * @brief Calibrates the RTC crystal
  * @param Number of pulses to skip
  * @retval None
	******************************************************************************
  */
void RTCCInt_Cal(uint32_t numPulses)
{
		printf("\tSkipping %0.3d Pulses", numPulses);
		HAL_RTCEx_SetSmoothCalib(&hrtc,RTC_SMOOTHCALIB_PERIOD_32SEC,RTC_SMOOTHCALIB_PLUSPULSES_RESET,numPulses);
	
}		//___  RTCCInt_Cal() ___//


/**
  ******************************************************************************
  * @brief Reads the STM32 Internal RTCC CALR Register
  * @param None
  * @retval None
	******************************************************************************
  */
void RTCCInt_Read_CALR(void)
{
		uint32_t regCALR = RTC->CALR;	
		printf("\r\n\r\nCALR Register: 0x%X",regCALR);
	
}		//___  RTCCInt_Read_CALR() ___//


/**
  ******************************************************************************
  * @brief Sets up an elapsed timer based on the Internal RTCC.
  * @param Pointer to RTCCInt_ElapsedTimerTypeDef Structure
  * @retval None
	* @note The RTCC Internal Elapsed timer has a granularity of 1/256 seconds
	* @note The effective resolution of the Elapsed timer is +/-4msec
	* @note This will fail if the Internal RTCC is reset between the Setup and Read
	******************************************************************************
  */
void RTCCInt_ElapsedTimer_Reset(RTCCInt_ElapsedTimerTypeDef *RTCCInt_ElapsedTimerStruct)
{
		RTC_TimeTypeDef RTC_TimeStructure = {0,0,0,0,0,0,0,0};
		RTC_DateTypeDef RTC_DateStructure = {0,0,0,0};
		HAL_StatusTypeDef timeStatus = HAL_OK;	
		HAL_StatusTypeDef dateStatus = HAL_OK;
		
		struct tm theTime = {0,0,0,0,0,0,0,0,0};
		time_t rtcSecs = 0;
		uint32_t rtcSubSeconds = 0;

		timeStatus = HAL_RTC_GetTime(&hrtc,&RTC_TimeStructure,RTC_FORMAT_BIN);
		dateStatus = HAL_RTC_GetDate(&hrtc,&RTC_DateStructure,RTC_FORMAT_BIN);
		
		if ( timeStatus != HAL_OK || dateStatus != HAL_OK )
		{
			// TODO: We need to determine how to handle HAL errors setting/reading RTC
			printf("\n ERROR[ETS]:HAL_RTC_GetDate/Time() returned NOT OK");
		}
	
		theTime.tm_year = RTC_DateStructure.Year + 100;
		theTime.tm_mon = RTC_DateStructure.Month - 1;
		theTime.tm_mday = RTC_DateStructure.Date;
		theTime.tm_hour = RTC_TimeStructure.Hours;
		theTime.tm_min = RTC_TimeStructure.Minutes;
		theTime.tm_sec = RTC_TimeStructure.Seconds;

		rtcSecs = mktime(&theTime);
		rtcSubSeconds = ((RTC_TimeStructure.SecondFraction-RTC_TimeStructure.SubSeconds)*1000)/(RTC_TimeStructure.SecondFraction+1);
	
		RTCCInt_ElapsedTimerStruct->epochSecsRef = rtcSecs;
		RTCCInt_ElapsedTimerStruct->msecsRef = rtcSubSeconds;
		
		//printf("\n\r\n\rMinutes: %d\n\rSeconds Ref: %d\n\rSubseconds Ref: %d",RTC_TimeStructure.Minutes,RTC_TimeStructure.Seconds,RTC_TimeStructure.SubSeconds);
	
}		//___  RTCCInt_ElapsedTimer_Reset() ___//


/**
  ******************************************************************************
  * @brief Read the elapsed time in milliseconds
  * @param Pointer to RTCCInt_ElapsedTimerTypeDef Structure
  * @retval Elapsed time in milliseconds
	* @note The RTCC Internal Elapsed timer has a granularity of 1/256 seconds
	* @note The effective resolution of the Elapsed timer is +/-4msec
	* @note This will fail if the Internal RTCC is reset between the Setup and Read
	******************************************************************************
  */
uint32_t RTCCInt_ElapsedTimer_Read(RTCCInt_ElapsedTimerTypeDef *RTCCInt_ElapsedTimerStruct)
{
		RTC_TimeTypeDef RTC_TimeStructure = {0,0,0,0,0,0,0,0};
		RTC_DateTypeDef RTC_DateStructure= {0,0,0,0};
		HAL_StatusTypeDef dateStatus = HAL_OK;
		HAL_StatusTypeDef timeStatus = HAL_OK;

		struct tm theTime = {0,0,0,0,0,0,0,0,0};
		time_t rtcSecs = 0;
		uint32_t rtcSubSeconds = 0;
		uint32_t rtcMsecsElapsed = 0;

		timeStatus = HAL_RTC_GetTime(&hrtc,&RTC_TimeStructure,RTC_FORMAT_BIN);
		dateStatus = HAL_RTC_GetDate(&hrtc,&RTC_DateStructure,RTC_FORMAT_BIN);
		
		if ( timeStatus != HAL_OK || dateStatus != HAL_OK )
		{
			// TODO: We should handle this error case
			DBG(printf("\n ERROR[ETR]:HAL_RTC_GetDate/Time() returned NOT OK");)
		}
	
		theTime.tm_year = RTC_DateStructure.Year + 100;
		theTime.tm_mon = RTC_DateStructure.Month - 1;
		theTime.tm_mday = RTC_DateStructure.Date;
		theTime.tm_hour = RTC_TimeStructure.Hours;
		theTime.tm_min = RTC_TimeStructure.Minutes;
		theTime.tm_sec = RTC_TimeStructure.Seconds;
	
		rtcSecs = mktime(&theTime);
		rtcSubSeconds = ((RTC_TimeStructure.SecondFraction-RTC_TimeStructure.SubSeconds)*1000)/(RTC_TimeStructure.SecondFraction+1);
	
		rtcMsecsElapsed = rtcSubSeconds + (rtcSecs-RTCCInt_ElapsedTimerStruct->epochSecsRef)*1000 - RTCCInt_ElapsedTimerStruct->msecsRef;
	
/***Deprecated*** RTCCInt_ElapsedTimerStruct->msecs = rtcMsecsElapsed; */

//	printf("\n\r\n\rSeconds Ref: %d\n\rMilliSeconds Ref: %d\n\rSeconds Stop: %d\n\rMilliSeconds Stop: %d",
//				RTCCInt_ElapsedTimerStruct->epochSecsRef,RTCCInt_ElapsedTimerStruct->msecsRef,rtcSecs,rtcSubSeconds);

//	printf("\n\r\n\rMinutes: %d\n\rSeconds Ref: %d\n\rSubseconds Ref: %d",
//				RTC_TimeStructure.Minutes,RTC_TimeStructure.Seconds,RTC_TimeStructure.SubSeconds);
	
		return rtcMsecsElapsed;
	
}		//___  RTCCInt_ElapsedTimer_Read() ___//


/**
  ******************************************************************************
	* @verbatim
		This Test Routine show the usage of the Internal RTCC Elapsed Timer.
		Two timer structures are setup.  The first timer is read after the user input
		delay in milliseconds.  The second timer is read after twice the user input
		delay in milliseconds.  Finally, the result of both timers is printed to
		the screen.
	*	@endverbatim
  * @brief Test the internal RTCC Elapsed Timer
  * @param msecs number of milliseconds elapsed since timer start
  * @retval none
	******************************************************************************
  */
void RTCCInt_ElapsedTimer_Test(int32_t msecs)
{	
		uint32_t mSecsTimer1, mSecsTimer2;
	
		//Structures for RTC based Timer1 and Timer2
		RTCCInt_ElapsedTimerTypeDef RTCCTimerStruct_timer1 = {0,0};
		RTCCInt_ElapsedTimerTypeDef RTCCTimerStruct_timer2 = {0,0};
	
		//Setup and clear structs for Timer1 and Timer2
		RTCCInt_ElapsedTimer_Reset(&RTCCTimerStruct_timer1);
		RTCCInt_ElapsedTimer_Reset(&RTCCTimerStruct_timer2);
	
		HAL_Delay(msecs);		//Delay for the user input time	
		mSecsTimer1 = RTCCInt_ElapsedTimer_Read(&RTCCTimerStruct_timer1);		//Read Timer1
		
		HAL_Delay(msecs);		//Delay for the user input time... again
		mSecsTimer2 = RTCCInt_ElapsedTimer_Read(&RTCCTimerStruct_timer2);		//Read Timer2
		
		printf("\n\r\n\rTimer1 Elapsed Time: %d msecs",mSecsTimer1);
		printf("\n\rTimer2 Elapsed Time: %d msecs",mSecsTimer2);
	
}		//___  RTCCInt_ElapsedTimer_Test() ___//
 

/**
  ******************************************************************************
  * @brief 
  * @param none
  * @retval tickCount the number of clock ticks
	******************************************************************************
	*/
uint32_t SYS_GetTick(void)
{	
		return sys_tickCount;

}		//___  SYS_GetTick() ___//


/**
  ******************************************************************************
  * @brief Increment the tick counter
  * @param none
  * @retval none
	******************************************************************************
	*/
void SYS_IncTick(void)
{
		sys_tickCount++;

}		//___  SYS_IncTick() ___//


/**
  ******************************************************************************
  * @brief 
  * @param SYS_elapsedTimerStruct a pointer to an ElapsedTimerStruct
  * @retval none
	******************************************************************************
	*/
void SYS_ElapsedTimer_Reset(SYS_ElapsedTimerTypeDef *SYS_ElapsedTimerStruct)
{	
		SYS_ElapsedTimerStruct->ticksRef = SYS_GetTick();
		SYS_ElapsedTimerStruct->ticksNow = 0;
		SYS_ElapsedTimerStruct->ticksElapsed = 0;

}		//___  SYS_ElapsedTimer_Setup() ___//


/**
  ******************************************************************************
  * @brief Reads the Elapsed Timer
  * @param SYS_ElapsedTimerStruct a pointer to an ElapsedTimerStruct
  * @retval ticksNow the number of clock ticks
	******************************************************************************
  */
uint32_t SYS_ElapsedTimer_Read(SYS_ElapsedTimerTypeDef *SYS_ElapsedTimerStruct)
{	
		uint32_t ticksNow = 0;
	
		ticksNow = SYS_GetTick();
		SYS_ElapsedTimerStruct->ticksNow = ticksNow;
		SYS_ElapsedTimerStruct->ticksElapsed = ticksNow - SYS_ElapsedTimerStruct->ticksRef;
	
		return SYS_ElapsedTimerStruct->ticksElapsed;

}		//___  SYS_ElapsedTimer_Read() ___//


/**
  ******************************************************************************
	* @verbatim
		This Test Routine show the usage of the System Elapsed Timer.
		Two timer structures are setup.  The first timer is read after the user input
		delay in milliseconds.  The second timer is read after twice the user input
		delay in milliseconds.  Finally, the result of both timers is printed to
		the screen.
	*	@endverbatim
  * @brief Tests the System Elapsed Timer based on SysTick()
  * @param msecs number of milliseconds elapsed since timer start
  * @retval none
	******************************************************************************
  */
void SYS_ElapsedTimer_Test(int32_t msecs)
{
		uint32_t mSecsTimer1 = 0;
		uint32_t mSecsTimer2 = 0;
	
		//Structures for RTC based Timer1 and Timer2
		SYS_ElapsedTimerTypeDef SYSTimerStruct_timer1 = {0,0,0};
		SYS_ElapsedTimerTypeDef SYSTimerStruct_timer2 = {0,0,0};
	
		SYS_ElapsedTimer_Reset(&SYSTimerStruct_timer1);	// Initialize structs for Timer1 and Timer2
		SYS_ElapsedTimer_Reset(&SYSTimerStruct_timer2);
	
		HAL_Delay(msecs);		//Delay for the user input time	
		mSecsTimer1 = SYS_ElapsedTimer_Read(&SYSTimerStruct_timer1);	//Read Timer1
	
		HAL_Delay(msecs);		//Delay for the user input time... again	
		mSecsTimer2 = SYS_ElapsedTimer_Read(&SYSTimerStruct_timer2);	//Read Timer2

		printf("\n\r\n\rTimer1 Elapsed Time: %d msecs",SYSTimerStruct_timer1.ticksElapsed);
		printf("\n\rTimer2 Elapsed Time: %d msecs",SYSTimerStruct_timer2.ticksElapsed);
	
}		//___  SYS_ElapsedTimer_Test() ___//


/**
  ******************************************************************************
  * @brief Dislays the current system time
  * @param none
  * @retval none
	******************************************************************************
  */
void RTCCInt_DisplayDateTime(void)
{
		struct tm theTime = {0,0,0,0,0,0,0,0,0};
		time_t secs = 0;
		int32_t yeartemp;
	
		/* Read the current internal RTCC Date/Time */ 
		secs = RTCCInt_ReadDateTime(&theTime);
		//! \todo Implement something to check secs
		
		theTime.tm_year -= 100;	 
		theTime.tm_mon++;				// account for zero indexing of month....
	
		printf("\n  System clock: ");
	
		if ( theTime.tm_year < 70 ) {
			yeartemp = 2000;
		} 
		else {
			yeartemp = 1900;
		}
		yeartemp += theTime.tm_year;
		printf("%d/", yeartemp);
	
		if ( theTime.tm_mon < 10 ) {
			printf("0%d/", theTime.tm_mon);
		} 
		else {
			printf("%d/", theTime.tm_mon);
		}
		
		if ( theTime.tm_mday < 10 ) {
			printf("0%d ", theTime.tm_mday);
		} 
		else {
			printf("%d ", theTime.tm_mday);
		}
		
		if ( theTime.tm_hour < 10 ) {
			printf("0%d:", theTime.tm_hour);
		} 
		else {
			printf("%d:", theTime.tm_hour);
		}
		
		if ( theTime.tm_min < 10 ) {
			printf("0%d:", theTime.tm_min);
		} 
		else {
			printf("%d:", theTime.tm_min);
		}
		
		if ( theTime.tm_sec < 10 ) {
			printf("0%d ", theTime.tm_sec);
		} 
		else {
			printf("%d",theTime.tm_sec);
		}
		
}		//___  RTCCInt_DisplayDateTime() ___//


#ifdef TESTING
/**
  ******************************************************************************
  * @brief Convert a Popeye hour stamp (5 hex characters string) to date/time
  * @param None
  * @retval None
	******************************************************************************
  */
void Hex2Time(void)
{
		uint32_t secs,hours,year;
		char hourString[64]; //long enough to cover almost all screwups..
	
		RTC_TimeTypeDef RTC_Time = {0,0,0,0,0,0,0,0};
		RTC_DateTypeDef RTC_Date = {0,0,0,0};
	
		printf("\n\n  Enter Popeye modem.dat file hour stamp [XXXXX] =>: ");
	
		uartGetString(LOPWRUART1,hourString,64);
	
		sscanf(hourString, "%x", &hours);
		secs = hours * 3600;
		DBG(printf("\n  hours:[%x]%u; secs:%u\n\r", hours,hours,secs);)
	
		if ( secs == 0 )
		{
				printf("\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a");
				printf("\a	WARNING!...	Invalid Entry");
				return;
		}		
		
		RTC_FromEpoch(secs, &RTC_Time, &RTC_Date);
		
		year = (RTC_Date.Year >= 70) ? RTC_Date.Year + 1900 : RTC_Date.Year + 2000;
		printf("\n  Converted date & time: %04d/%02d/%02d %02d:%02d:%02d\n",
					year,RTC_Date.Month,RTC_Date.Date,RTC_Time.Hours,RTC_Time.Minutes,RTC_Time.Seconds);
		
}		//___  Hex2Time() ___//


/**
  ******************************************************************************
  * @brief Convert date/time to Popeye hour stamp (5 hex characters string)
  * @param None
  * @retval None
	******************************************************************************
  */
void Time2Hex(void)
{

		RTC_TimeTypeDef RTC_Time = {0,0,0,0,0,0,0,0};
		RTC_DateTypeDef RTC_Date = {0,0,0,0};
		
		int32_t yy,mm,dd,hh,mn,ss;
		int16_t count = 0;
		
		char dateString[64]; //long enough to cover almost all screwups..
	
		uint32_t secs = 0;
		
		printf("\n  Enter a date & time [yyyy/mm/dd hh:mm:ss] =>: ");
	  uartGetString(LOPWRUART1,dateString,64);

		mm = dd = yy = hh = mn = ss = 0;
		count = (sscanf (dateString, "%d/%d/%d %d:%d:%d", &yy, &mm, &dd, &hh, &mn, &ss) );
	
		if (count != 6)	// Check for correct number of entries
		{
			uartPutString(LOPWRUART1,"\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a");    
			uartPutString(LOPWRUART1,"\a  WARNING!..... Incorrect format entry");
			return;
		}
		
		//	Check for dopey numbers

		if ((yy > 2050) || ( yy < 1970))	// do nothing
		{	
			uartPutString(LOPWRUART1,"\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a");
			uartPutString(LOPWRUART1,"\a  WARNING!..... Invalid year entry");
			return;
		}
		RTC_Date.Year = (yy < 2000) ? yy - 1900 : yy - 2000;
		
		if (mm > 12)	// do nothing
		{	
			uartPutString(LOPWRUART1,"\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a");    
			uartPutString(LOPWRUART1,"\a  WARNING!..... Invalid month entry");
			return;
		}
		RTC_Date.Month = mm;
		
		if (dd > 31)	// do nothing
		{	
			uartPutString(LOPWRUART1,"\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a");    
			uartPutString(LOPWRUART1,"\a  WARNING!..... Invalid day entry");
			return;
		}
		RTC_Date.Date = dd;
		
		if (hh > 24)	// do nothing
		{	
			uartPutString(LOPWRUART1,"\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a");    
			uartPutString(LOPWRUART1,"\a  WARNING!..... Invalid hour entry");
			return;
		}
		RTC_Time.Hours = hh;
		
		if (mn > 59)	// do nothing
		{	
			uartPutString(LOPWRUART1,"\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a");    
			uartPutString(LOPWRUART1,"\a  WARNING!..... Invalid minute entry");
			return;
		}
		RTC_Time.Minutes = mn;
		
		if (ss > 59)	// do nothing
		{	
			uartPutString(LOPWRUART1,"\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a\a");    
			uartPutString(LOPWRUART1,"\a  WARNING!..... Invalid seconds entry");
			return;
		}
		RTC_Time.Seconds = ss;
		
		secs = RTC_ToEpoch(&RTC_Time, &RTC_Date);
		printf ("\n  Popeye modem.dat file hour stamp:  %X", secs/3600);
		printf ("\n  Popeye  test.dat file hour stamp:  %u", secs/3600);
    printf ("\n  Popeye  test.dat file secs stamp:  %u", secs);
		
}		//___  Time2Hex() ___//

#endif

