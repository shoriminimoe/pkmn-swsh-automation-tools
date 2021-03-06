/*
Nintendo Switch Fightstick - Proof-of-Concept

Based on the LUFA library's Low-Level Joystick Demo
	(C) Dean Camera
Based on the HORI's Pokken Tournament Pro Pad design
	(C) HORI

This project implements a modified version of HORI's Pokken Tournament Pro Pad
USB descriptors to allow for the creation of custom controllers for the
Nintendo Switch. This also works to a limited degree on the PS3.

Since System Update v3.0.0, the Nintendo Switch recognizes the Pokken
Tournament Pro Pad as a Pro Controller. Physical design limitations prevent
the Pokken Controller from functioning at the same level as the Pro
Controller. However, by default most of the descriptors are there, with the
exception of Home and Capture. Descriptor modification allows us to unlock
these buttons for our use.
*/

#include "Joystick.h"

typedef enum {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	SPIN,
	POSITION,
	X,
	Y,
	A,
	B,
	L,
	R,
	PLUS,
	MINUS,
	HOME,
	NOTHING,
	TRIGGERS
} Buttons_t;

typedef struct {
	Buttons_t button;
	uint16_t duration;
} command; 

/*
https://bulbapedia.bulbagarden.net/wiki/List_of_Pok%C3%A9mon_by_base_Egg_cycles
 5 cycles =  700
10 cycles = 1400
15 cycles = 2100
20 cycles = 2800 (default)
25 cycles = 3500
30 cycles = 4200
35 cycles = 4900
40 cycles = 5600
*/
#ifndef EGG_CYCLES
#define EGG_CYCLES 20
#endif

#ifndef MAX_HATCHES
#define MAX_HATCHES 240
#endif

#define ONE_CYCLE_DURATION 135

#define CYCLE_DURATION ONE_CYCLE_DURATION * EGG_CYCLES

static const command step[] = {
	// Setup controller
	{ TRIGGERS,   5 },	{ NOTHING,  50 },
	{ TRIGGERS,   5 },	{ NOTHING,  50 },
	{ A,          5 },	{ NOTHING,  50 },
	// Open game
	{ HOME,       5 },	{ NOTHING,  50 },
	{ HOME,       5 },	{ NOTHING, 200 },

	/* ###### Pokemon slot 6 ###### */
	// teleport to daycare in wildarea
	{ X,          5 },	{ NOTHING,  100 }, //open menu
	{ A,          5 },	{ NOTHING,  100 },
	{ A,          5 },	{ NOTHING,  100 }, //you want to teleport here?
	{ A,          5 },	{ NOTHING,  200 }, //sure!
	// walk to daycare and get an egg
	{ DOWN,      40 }, //walk down to daycare
	{ LEFT,       5 }, //a little bit left
	{ A,          5 },	{ NOTHING,  75 }, // "Your pokemon was found holding an egg" or "Welcome to the daycare"
 	{ A,          5 },	{ NOTHING,  175 }, // Yes
	{ B,          5 },	{ NOTHING,  100 }, //you got it or exit if there is no egg
	{ A,          5 },	{ NOTHING,   50 }, // Add to your party
	{ UP,         5 }, // Turn away if there was no egg
	{ A,          5 },	{ NOTHING,  100 }, // please select a pokemon to swapt from your party
	{ UP,         5 }, //select correct pokemon slot
	{ A,          5 },	{ NOTHING,  150 }, //You sure want to put it here?
	{ A,          5 },	{ NOTHING,  150 }, //Yes!
	{ A,          5 },	{ NOTHING,    5 }, //take good care of it

	// start hatching
	{ UP,         5 },	{ NOTHING,    5 }, // Look away from daycare lady
	{ PLUS,       5 },	{ NOTHING,    5 }, //get on your bike
	{ UP,        20 },
	{ POSITION,  10 },
	{ UP,        20 },
	{ POSITION, 100 },
	{ SPIN,  CYCLE_DURATION }, //spin for X cycles
	// egg hatched?
	{ A,          5 },	{ NOTHING,  825 }, //Oh
	{ A,          5 },	{ NOTHING,  125 }, //"Pokemon" hatched from the egg
	{ B,          5 },	{ NOTHING,   10 },
	{ PLUS,       5 },	{ NOTHING,  100 }, //get off the bike

	// repeat
};

static const command system_sleep[] = {
	// Open sleep menu and press A
	{ HOME,     200 },	{ NOTHING,  10 },
	{ A,          5 },  { NOTHING,  10 },
};
// Main entry point.
int main(void) {
	// We'll start by performing hardware and peripheral setup.
	SetupHardware();
	// We'll then enable global interrupts for our use.
	GlobalInterruptEnable();
	// Once that's done, we'll enter an infinite loop.
	for (;;)
	{
		// We need to run our task to process and deliver data for our IN and OUT endpoints.
		HID_Task();
		// We also need to run the main USB management task.
		USB_USBTask();
	}
}

// Configures hardware and peripherals, such as the USB peripherals.
void SetupHardware(void) {
	// We need to disable watchdog if enabled by bootloader/fuses.
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	// We need to disable clock division before initializing the USB hardware.
	clock_prescale_set(clock_div_1);
	// We can then initialize our hardware and peripherals, including the USB stack.

	#ifdef ALERT_WHEN_DONE
	// Both PORTD and PORTB will be used for the optional LED flashing and buzzer.
	#warning LED and Buzzer functionality enabled. All pins on both PORTB and \
PORTD will toggle when printing is done.
	DDRD  = 0xFF; //Teensy uses PORTD
	PORTD =  0x0;
                  //We'll just flash all pins on both ports since the UNO R3
	DDRB  = 0xFF; //uses PORTB. Micro can use either or, but both give us 2 LEDs
	PORTB =  0x0; //The ATmega328P on the UNO will be resetting, so unplug it?
	#endif
	// The USB stack should be initialized last.
	USB_Init();
}

// Fired to indicate that the device is enumerating.
void EVENT_USB_Device_Connect(void) {
	// We can indicate that we're enumerating here (via status LEDs, sound, etc.).
}

// Fired to indicate that the device is no longer connected to a host.
void EVENT_USB_Device_Disconnect(void) {
	// We can indicate that our device is not ready (via status LEDs, sound, etc.).
}

// Fired when the host set the current configuration of the USB device after enumeration.
void EVENT_USB_Device_ConfigurationChanged(void) {
	bool ConfigSuccess = true;

	// We setup the HID report endpoints.
	ConfigSuccess &= Endpoint_ConfigureEndpoint(JOYSTICK_OUT_EPADDR, EP_TYPE_INTERRUPT, JOYSTICK_EPSIZE, 1);
	ConfigSuccess &= Endpoint_ConfigureEndpoint(JOYSTICK_IN_EPADDR, EP_TYPE_INTERRUPT, JOYSTICK_EPSIZE, 1);

	// We can read ConfigSuccess to indicate a success or failure at this point.
}

// Process control requests sent to the device from the USB host.
void EVENT_USB_Device_ControlRequest(void) {
	// We can handle two control requests: a GetReport and a SetReport.

	// Not used here, it looks like we don't receive control request from the Switch.
}

// Process and deliver data from IN and OUT endpoints.
void HID_Task(void) {
	// If the device isn't connected and properly configured, we can't do anything here.
	if (USB_DeviceState != DEVICE_STATE_Configured)
		return;

	// We'll start with the OUT endpoint.
	Endpoint_SelectEndpoint(JOYSTICK_OUT_EPADDR);
	// We'll check to see if we received something on the OUT endpoint.
	if (Endpoint_IsOUTReceived())
	{
		// If we did, and the packet has data, we'll react to it.
		if (Endpoint_IsReadWriteAllowed())
		{
			// We'll create a place to store our data received from the host.
			USB_JoystickReport_Output_t JoystickOutputData;
			// We'll then take in that data, setting it up in our storage.
			while(Endpoint_Read_Stream_LE(&JoystickOutputData, sizeof(JoystickOutputData), NULL) != ENDPOINT_RWSTREAM_NoError);
			// At this point, we can react to this data.

			// However, since we're not doing anything with this data, we abandon it.
		}
		// Regardless of whether we reacted to the data, we acknowledge an OUT packet on this endpoint.
		Endpoint_ClearOUT();
	}

	// We'll then move on to the IN endpoint.
	Endpoint_SelectEndpoint(JOYSTICK_IN_EPADDR);
	// We first check to see if the host is ready to accept data.
	if (Endpoint_IsINReady())
	{
		// We'll create an empty report.
		USB_JoystickReport_Input_t JoystickInputData;
		// We'll then populate this report with what we want to send to the host.
		GetNextReport(&JoystickInputData);
		// Once populated, we can output this data to the host. We do this by first writing the data to the control stream.
		while(Endpoint_Write_Stream_LE(&JoystickInputData, sizeof(JoystickInputData), NULL) != ENDPOINT_RWSTREAM_NoError);
		// We then send an IN packet on this endpoint.
		Endpoint_ClearIN();
	}
}

typedef enum {
	SYNC_CONTROLLER,
	SYNC_POSITION,
	BREATHE,
	PROCESS,
	CLEANUP,
	DONE
} State_t;
State_t state = SYNC_CONTROLLER;

#define ECHOES 2
int echoes = 0;
USB_JoystickReport_Input_t last_report;

int report_count = 0;
int xpos = 0;
int ypos = 0;
int bufindex = 0;
int duration_count = 0;
int hatch_count = 0;
int portsval = 0;
int cleanup_duration = 0;

// Prepare the next report for the host.
void GetNextReport(USB_JoystickReport_Input_t* const ReportData) {

	// Prepare an empty report
	memset(ReportData, 0, sizeof(USB_JoystickReport_Input_t));
	ReportData->LX = STICK_CENTER;
	ReportData->LY = STICK_CENTER;
	ReportData->RX = STICK_CENTER;
	ReportData->RY = STICK_CENTER;
	ReportData->HAT = HAT_CENTER;

	// Repeat ECHOES times the last report
	if (echoes > 0)
	{
		memcpy(ReportData, &last_report, sizeof(USB_JoystickReport_Input_t));
		echoes--;
		return;
	}

	// States and moves management
	switch (state)
	{

		case SYNC_CONTROLLER:
			state = BREATHE;
			break;

		case SYNC_POSITION:
			bufindex = 0;
			ReportData->Button = 0;
			ReportData->LX = STICK_CENTER;
			ReportData->LY = STICK_CENTER;
			ReportData->RX = STICK_CENTER;
			ReportData->RY = STICK_CENTER;
			ReportData->HAT = HAT_CENTER;
			state = BREATHE;
			break;

		case BREATHE:
			state = PROCESS;
			break;

		case PROCESS:
			switch (step[bufindex].button)
			{
				case UP:
					ReportData->LY = STICK_MIN;				
					break;
				case LEFT:
					ReportData->LX = STICK_MIN;				
					break;
				case DOWN:
					ReportData->LY = STICK_MAX;				
					break;
				case RIGHT:
					ReportData->LX = STICK_MAX;				
					break;
				case SPIN:
					ReportData->RX = STICK_MIN;
					ReportData->LX = STICK_MIN;
					break;
				case POSITION:
					ReportData->LY = STICK_MIN;
					ReportData->LX = STICK_MAX;
					break;
				case A:
					ReportData->Button |= SWITCH_A;
					break;
				case B:
					ReportData->Button |= SWITCH_B;
					break;
				case R:
					ReportData->Button |= SWITCH_R;
					break;
				case X:
					ReportData->Button |= SWITCH_X;
					break;
				case Y:
					ReportData->Button |= SWITCH_Y;
					break;
				case PLUS:
					ReportData->Button |= SWITCH_PLUS;
					break;
				case MINUS:
					ReportData->Button |= SWITCH_MINUS;
					break;
				case HOME:
					ReportData->Button |= SWITCH_HOME;
					break;
				case TRIGGERS:
					ReportData->Button |= SWITCH_L | SWITCH_R;
					break;
				default:
					ReportData->LX = STICK_CENTER;
					ReportData->LY = STICK_CENTER;
					ReportData->RX = STICK_CENTER;
					ReportData->RY = STICK_CENTER;
					ReportData->HAT = HAT_CENTER;
					break;
			}

			duration_count++;
			if (duration_count > step[bufindex].duration)
			{
				bufindex++;
				duration_count = 0;				
			}

			if (bufindex > (int)( sizeof(step) / sizeof(step[0])) - 1)
			{
				bufindex = 10;
				hatch_count++;
				duration_count = 0;
				state = BREATHE;
				ReportData->LX = STICK_CENTER;
				ReportData->LY = STICK_CENTER;
				ReportData->RX = STICK_CENTER;
				ReportData->RY = STICK_CENTER;
				ReportData->HAT = HAT_CENTER;
			}

			if (hatch_count >= MAX_HATCHES)
			{
				state = DONE;
				duration_count = 0;
				bufindex = 0;
			}
			break;

		case CLEANUP:
			// Put the system to sleep
			// FIXME This step doesn't work. The switch wakes up after going to
			// sleep.
			/*
			switch (system_sleep[bufindex].button)
			{
				case A:
					ReportData->Button |= SWITCH_A;
					break;
				case HOME:
					ReportData->Button |= SWITCH_HOME;
					break;
				default:
					ReportData->LX = STICK_CENTER;
					ReportData->LY = STICK_CENTER;
					ReportData->RX = STICK_CENTER;
					ReportData->RY = STICK_CENTER;
					ReportData->HAT = HAT_CENTER;
					break;
			}

			duration_count++;
			if (duration_count > system_sleep[bufindex].duration)
			{
				bufindex++;
				duration_count = 0;
			}

			if (bufindex > (int)( sizeof(system_sleep) / sizeof(system_sleep[0])) - 1)
			{
				state = DONE;
			}
			*/
			state = DONE;
			break;

		case DONE:
			#ifdef ALERT_WHEN_DONE
			portsval = ~portsval;
			PORTD = portsval; //flash LED(s) and sound buzzer if attached
			PORTB = portsval;
			_delay_ms(250);
			#endif
			return;
	}
	// Prepare to echo this report
	memcpy(&last_report, ReportData, sizeof(USB_JoystickReport_Input_t));
	echoes = ECHOES;
}
