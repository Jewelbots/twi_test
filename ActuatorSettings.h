/*
 * ActuatorSettings.h
 *
 *  Created on: Sep 20, 2012
 *      Author: a0866685
 *
 * Description: Use this file to use pre-tested auto-calibration values for
 * 					common actuators
 */

/**********************************************************
 * Instructions
 **********************************************************
 * To use auto-calibration results from RunAutoCal_xxx():
 * 	1) Set DEFAULTCONTROLSETTINGS = 1
 *  2) Set AUTOCALRESULTS_xxx = 1
 * 	3) Set all actuators in the "Actuator Enable List" to "0"
 * 	4) Select one actuator by changing the "0" to a "1"
 *
 * To use the lab-tested auto-calibration values below:
 *  1) Set DEFAULTCONTROLSETTINGS = 1
 *  2) Set AUTOCALRESULTS_xxx = 0
 * 	3) Set all actuators in the "Actuator Enable List" to "0"
 * 	4) Select one actuator by changing the "0" to a "1"
 *
 * Note: Only one actuator should have a "1" next to it
 **********************************************************/

#ifndef ACTUATORSETTINGS_H_
#define ACTUATORSETTINGS_H_


/***********************************************************
 * CONTROL REGISTER SETTINGS
 *
 * Set to 1 to use control register settings CTRL2 and CTRL3
 * 	below.
 * 0 = Use control register values in Haptics_Init()
 * 1 = Use CTRL2 and CTRL3 below
 *
 * @TODO - Select to use CTRL2 and CTRL3 values below
 ***********************************************************/
#define DEFAULTCONTROLSETTINGS	0	// Select control settings

/***********************************************************
 * AUTO-CALIBRATION RESULTS
 *
 * Set to 1 to use live auto-calibration results
 * 0 = Use pre-tested auto-calibration values below
 * 1 = Use Auto-cal results taken during RunAutoCal_ERM() or
 * 		RunAutoCal_LRA()
 *
 * @TODO - Select to use live auto-cal results or settings below
 ***********************************************************/
#define AUTOCALRESULTS_LRA	1		// Select auto-cal for LRA
#define AUTOCALRESULTS_ERM	0 		// Select auto-cal for ERM

/***********************************************************
 * LRA ACTUATOR ENABLE LIST
 * Select the LRA actuator on the EVM by setting it to 1
 *
 * @TODO - Choose model of on-board LRA (choose only one)
 ***********************************************************/
#define SEMCO1030 			0		// SEMCO 1030
#define SEMCO1036			0		// SEMCO 1036
#define SEMCO0934			0		// SEMCO 0934
#define SEMCO0850			0		// SEMCO 0850
#define AAC1036				1		// AAC 1036
#define AAC1411				0		// AAC ELV1411
#define PARTRON1030			0		// Partron 1030

/***********************************************************/

/***********************************************************
 * ERM ACTUATOR ENABLE LIST
 * Select the ERM actuator on the board by setting it to 1
 *
 * @TODO = Choose model of on-board ERM (choose only one)
 ***********************************************************/
#define SANYO2574 			1		// Sanyo NRS-2574i
#define JL2513				0		// Jinglong Z4TJB1512513
#define JL1992				0		// Jinglong Z4TH5B1241992
#define JL1030				0		// Jinglong	C1030B028F
#define AWA4342				0		// AWA GT-4342

/***********************************************************/








/***********************************************************
 *
 * 	DO NOT EDIT BELOW THIS LINE
 *
 ***********************************************************/

/* Begin setting actuator auto-calibration values  */

/***********************************************************
 * 	LRA - Begin LRA settings
 ***********************************************************/

// SEMCO 1030 - Last Modified:  11/29/2012
#if SEMCO1030	
#define LRA_AUTOCAL_COMP 		0x09					// Compensation Coef.
#define	LRA_AUTOCAL_BEMF 		0x79					// BEMF Coef.
#define LRA_AUTOCAL_FB 			0xB5					// Feedback Control
#define LRA_RATED_VOLTAGE		VoltageRMS_LRA_RV_1p5	// Rated voltage
#define LRA_OVERDRIVE_VOLTAGE	Voltage_3p0				// Overdrive voltage
#define LRA_CTRL2 				SampleTime_300us | BlankingTime_Medium | IDissTime_Medium 	// Control Register 2
#define LRA_CTRL3				LRADriveMode_Once | LRA_ClosedLoop								// Control Register 3
#endif


// SEMCO 1036 - Last Modified:	11/29/2012
#if SEMCO1036	
#define LRA_AUTOCAL_COMP 		0x07					// Compensation Coef.
#define	LRA_AUTOCAL_BEMF 		0x8C					// BEMF Coef.
#define LRA_AUTOCAL_FB 			0xB5					// Feedback Control
#define LRA_RATED_VOLTAGE		VoltageRMS_LRA_RV_2p0	// Rated voltage
#define LRA_OVERDRIVE_VOLTAGE	Voltage_3p6				// Overdrive voltage
#define LRA_CTRL2 				SampleTime_300us | BlankingTime_Short | IDissTime_Short 		// Control Register 2
#define LRA_CTRL3				LRADriveMode_Once | LRA_ClosedLoop								// Control Register 3
#endif


// SEMCO 0934 - Last Modified:	11/29/2012
#if SEMCO0934	
#define LRA_AUTOCAL_COMP 		0x08					// Compensation Coef.
#define	LRA_AUTOCAL_BEMF 		0x82					// BEMF Coef.
#define LRA_AUTOCAL_FB 			0xB5					// Feedback Control
#define LRA_RATED_VOLTAGE		VoltageRMS_LRA_RV_2p0	// Rated voltage
#define LRA_OVERDRIVE_VOLTAGE	Voltage_3p6				// Overdrive voltage
#define LRA_CTRL2 				SampleTime_300us | BlankingTime_Short | IDissTime_Short 		// Control Register 2
#define LRA_CTRL3				LRADriveMode_Once | LRA_ClosedLoop								// Control Register 3
#endif


// SEMCO 0850 - Last Modified:	11/29/2012
#if SEMCO0850
#define LRA_AUTOCAL_COMP 		0x08					// Compensation Coef.
#define	LRA_AUTOCAL_BEMF 		0xB9					// BEMF Coef.
#define LRA_AUTOCAL_FB 			0xB5					// Feedback Control
#define LRA_RATED_VOLTAGE		VoltageRMS_LRA_RV_2p0	// Rated voltage
#define LRA_OVERDRIVE_VOLTAGE	Voltage_3p6				// Overdrive voltage
#define LRA_CTRL2 				SampleTime_300us | BlankingTime_Short | IDissTime_Short 		// Control Register 2
#define LRA_CTRL3				LRADriveMode_Once | LRA_ClosedLoop								// Control Register 3
#endif


// AAC 1036 - Last Modified: 	12/21/2012
#if AAC1036		
#define LRA_AUTOCAL_COMP 		0x06					// Compensation Coef.
#define	LRA_AUTOCAL_BEMF 		0xF2					// BEMF Coef.
#define LRA_AUTOCAL_FB 			0xA7					// Feedback Control
#define LRA_RATED_VOLTAGE		VoltageRMS_LRA_RV_2p0	// Rated voltage
#define LRA_OVERDRIVE_VOLTAGE	Voltage_3p6				// Overdrive voltage
#define LRA_CTRL2 				SampleTime_300us | BlankingTime_Short | IDissTime_Short 		// Control Register 2
#define LRA_CTRL3				LRADriveMode_Once | LRA_ClosedLoop								// Control Register 3
#endif


// AAC ELV1411 - Last Modified: 11/29/2012
#if AAC1411
#define LRA_AUTOCAL_COMP 		0x07					// Compensation Coef.
#define	LRA_AUTOCAL_BEMF 		0x6F					// BEMF Coef.
#define LRA_AUTOCAL_FB 			0xB4					// Feedback Control
#define LRA_RATED_VOLTAGE		VoltageRMS_LRA_RV_2p0	// Rated voltage
#define LRA_OVERDRIVE_VOLTAGE	Voltage_3p6				// Overdrive voltage
#define LRA_CTRL2 				SampleTime_300us | BlankingTime_Short | IDissTime_Short 		// Control Register 2
#define LRA_CTRL3				LRADriveMode_Once | LRA_ClosedLoop								// Control Register 3
#endif


// Partron 1030 - Last Modified: 11/29/2012
#if PARTRON1030
#define LRA_AUTOCAL_COMP 		0x09					// Compensation Coef.
#define	LRA_AUTOCAL_BEMF 		0xB7					// BEMF Coef.
#define LRA_AUTOCAL_FB 			0xB7					// Feedback Control
#define LRA_RATED_VOLTAGE		VoltageRMS_LRA_RV_1p5	// Rated voltage
#define LRA_OVERDRIVE_VOLTAGE	Voltage_3p0				// Overdrive voltage
#define LRA_CTRL2 				SampleTime_300us | BlankingTime_Short | IDissTime_Short 		// Control Register 2
#define LRA_CTRL3				LRADriveMode_Once | LRA_ClosedLoop								// Control Register 3
#endif


/***********************************************************
 * 	ERM - Begin ERM settings
 ***********************************************************/

// Sanyo NRS2574 - Last Modified 11/30/2012
#if SANYO2574
#define ERM_AUTOCAL_COMP		0x0D					// Compensation Coef.
#define ERM_AUTOCAL_BEMF		0x6E					// BEMF Coef.
#define ERM_AUTOCAL_FB			0x36					// Feedback Control
#define ERM_RATED_VOLTAGE		Voltage_ERM_RV_CL_1p3	// Rated voltage closed-loop
#define ERM_OVERDRIVE_VOLTAGE	Voltage_ERM_OD_CL_3p3	// Overdrive voltage closed-loop
#define ERM_OVERDRIVE_OL		Voltage_3p3				// Overdrive voltage open-loop
#define ERM_CTRL3				ERM_ClosedLoop			// Control Register 3
#endif


// Jinlong Z4TJB1512513 - Last Modified 11/28/2012
#if JL2513
#define ERM_AUTOCAL_COMP		0x04			// Compensation Coef.
#define ERM_AUTOCAL_BEMF		0x8A			// BEMF Coef.
#define ERM_AUTOCAL_FB			0x36			// Feedback Control
#define ERM_RATED_VOLTAGE		Voltage_3p0		// Rated voltage
#define ERM_OVERDRIVE_VOLTAGE	Voltage_5p0		// Overdrive voltage
#define ERM_CTRL3				ERM_ClosedLoop	// Control Register 3
#endif


// Jinlong Z4TH5B1241992 - Last Modified 11/28/2012
#if JL1992
#define ERM_AUTOCAL_COMP		0x03			// Compensation Coef.
#define ERM_AUTOCAL_BEMF		0x9E			// BEMF Coef.
#define ERM_AUTOCAL_FB			0x35			// Feedback Control
#define ERM_RATED_VOLTAGE		Voltage_3p0		// Rated voltage
#define ERM_OVERDRIVE_VOLTAGE	Voltage_5p0		// Overdrive voltage
#define ERM_CTRL3				ERM_ClosedLoop	// Control Register 3
#endif


// Jinlong C1030B028F - Last Modified 11/28/2012
#if JL1030
#define ERM_AUTOCAL_COMP		0x03			// Compensation Coef.
#define ERM_AUTOCAL_BEMF		0x4B			// BEMF Coef.
#define ERM_AUTOCAL_FB			0x34			// Feedback Control
#define ERM_RATED_VOLTAGE		Voltage_3p0		// Rated voltage
#define ERM_OVERDRIVE_VOLTAGE	Voltage_5p0		// Overdrive voltage
#define ERM_CTRL3				ERM_ClosedLoop	// Control Register 3
#endif


// AWA GT-4342 - Last Modified 11/28/2012
#if AWA4342
#define ERM_AUTOCAL_COMP		0x0B			// Compensation Coef.
#define ERM_AUTOCAL_BEMF		0x7B			// BEMF Coef.
#define ERM_AUTOCAL_FB			0x36			// Feedback Control
#define ERM_RATED_VOLTAGE		Voltage_2p7		// Rated voltage
#define ERM_OVERDRIVE_VOLTAGE	Voltage_5p0		// Overdrive voltage
#define ERM_CTRL3				ERM_ClosedLoop	// Control Register 3
#endif


#endif /* ACTUATORSETTINGS_H_ */
