//=====================================================================================================================
// Sgitheach clocks are built and documented with an Open Source philosophy in mind. All the source files
// including circuit diagrams, eagle board files, software and design files are provided under a Creative Commons
// ShareAlike 4.0 International license.
//
// More specifically;
// i) You may share, copy and redistribute the material in any medium or format,
// ii) You may remix, transform and build upon the material presented herein,
// iii) You MUST give appropriate credit, provide a link to the license and indicate if changes have been made.
// iv) This license is for NON-COMMERCIAL use only, you may not use the material for commercial gain.
// v) If you remix, transform, 'improve', modify or build upon the material presented herein, you must distribute your
// contributions under the same license as the original.
//
// You may not apply legal terms or technological measures that legally restrict others from doing anything the
// license permits.
//
// For further information, please see the following URL:
//
// http://creativecommons.org/licenses/bync-sa/4.0/
//
// or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
//
// In the event that there is any conflict between this file and the License given above, then the License shall take
// priority.
//=====================================================================================================================
//
// Harwell and Haar Dekatron Clocks
//
// Copyright (c) 2019 Grahame Marsh
//
//---------------------------------------------------------------------------------------------------------------------
//
// ATMEGA324PA - SAM3X8C shared interface
//
//=====================================================================================================================

#ifndef SLAVE_H_INCLUDED
#define SLAVE_H_INCLUDED

#include <asf.h>

// #define DEKATRON_DEBUG

#define DEKATRON_SLAVE_ADDRESS		0x10

#define TWI_SLAVE_RX_BUFFER			140
#define TWI_SLAVE_TX_BUFFER			TWI_SLAVE_RX_BUFFER

#define DATA_PACKET_LENGTH			(TWI_SLAVE_RX_BUFFER - 3)		// allow for cmd, length and CRC)

#define INITIAL_CRC					0xff

// this value is specific to the ATMEGA324PA with the largest boot size
#define BOOT_TEXT_START 0x7000

#ifndef SPM_PAGESIZE
	#define	SPM_PAGESIZE	128
#endif

// Dekatron V7 indicator usage with GC12/4B

enum
{
	DOT_IDLE,			// 0
	DOT_TIME,			// 1
	DOT_DATE,			// 2
	DOT_LONGITUDE,		// 3
	DOT_LATITUDE,		// 4
	DOT_ELEVATION,		// 5
	DOT_FACE,			// 6
	DOT_PAIR,			// 7
	DOT_VOLUME,			// 8
	DOT_UTC,			// 9
	DOT_SUN,			// 10
	DOT_SYSTEM			// 11
};

// for a 10 cathode dekatron as V7 

// system = idle
// sun = pair + time etc

/*
Other Notes:

Time shows the Local Time
Date shows the Local Date
Time+UTC shows UTC time
Date+UTC shows UTC date
Long shows the Longitude
Lat shows the Latitude
Elev shows the Elevation
Sun+Time shows the Sunrise
Sun+Date shows the Sunset
Sun+Long shows the Sun's Azimuth
Sun+Elev shows the Sun's Elevation
Face current face number or face number entry
Pair current pair number or pair number entry
Vol current chime volume or beep volume
System shows internal readings

System+Time shows hardware version
System+Date shows firmware version
System+Longitude shows Temperature
System+Latitude shows LDR
System+Elevation shows GPS data
System+Face shows WiFi data

*/

// Dekatron Effects
// value must correspond to effect_xxxx in constants.ini
enum
{
	EFFECT_NONE,			// no effect
	EFFECT_CATHODE,			// 1 move to a cathode and go idle (same as DEKATRON command)
	EFFECT_IDLE,			// 2 stop any effect leaving dot where it is
	EFFECT_IDLE_ALL,		// 3 ... all tubes
	EFFECT_MOVE,			// 4 move dot and go idle
	EFFECT_ONCE,			// 5 spin the tube once and go idle
	EFFECT_PENDULUM,		// 6 pendulum
	EFFECT_SHOW,			// 7 show a position
	EFFECT_SPIN,			// 8 continuously spin
	EFFECT_TWO,				// 9 show two dots
	EFFECT_STAR,			// 10 star effect
	
	EFFECT_ZERO,			// 11 zero one tube and go idle
	EFFECT_ZERO_ALL			// 12 ... all tubes
};


// buffer for the slave to send data to the master
// empty (command only) packet
COMPILER_PACK_SET(1)
typedef struct
{
	uint8_t cmd;
	uint8_t len;
	uint8_t	crc;
} empty_packet_t;

// boolean value
COMPILER_PACK_SET(1)
typedef struct
{
	uint8_t cmd;
	uint8_t len;
	bool value;
	uint8_t	crc;
} bool_packet_t;

// uint8_t value
COMPILER_PACK_SET(1)
typedef struct
{
	uint8_t cmd;
	uint8_t len;
	uint8_t	value;
	uint8_t	crc;
} uint8_packet_t;

// uint16_t value
COMPILER_PACK_SET(1)
typedef struct
{
	uint8_t cmd;
	uint8_t len;
	uint16_t value;
	uint8_t	crc;
} uint16_packet_t;

// boot memory page
COMPILER_PACK_SET(1)
typedef struct
{
	uint8_t cmd;
	uint8_t len;
	uint16_t addr;
	uint8_t	values[SPM_PAGESIZE];
	uint8_t	crc;
} boot_mem_packet_t;

// idle - use uint8_t for tube value

// move values
COMPILER_PACK_SET(1)
typedef struct
{
	uint8_t tube;
	uint8_t pos;
	uint8_t dir;
	uint16_t speed;
} move_packet_t;

COMPILER_PACK_SET(1)
typedef struct
{
	uint8_t pos[6];
	uint8_t dir;
	uint16_t speed;
} move6_packet_t;

COMPILER_PACK_SET(1)
typedef struct
{
	uint8_t pos[7];
	uint8_t dir;
	uint16_t speed;
} move7_packet_t;

// once - use uint8_t for tube value

// once values
COMPILER_PACK_SET(1)
typedef struct
{
	uint8_t tube;
	uint8_t dir;
	uint16_t speed;
} once_packet_t;

// pendulum - use uint8_t for tube

// pendulum values
COMPILER_PACK_SET(1)
typedef struct
{
	uint8_t tube;
	uint8_t start;
	uint8_t type;
	uint16_t sweep;
} pendulum_packet_t;

// show values
COMPILER_PACK_SET(1)
typedef struct
{
	uint8_t tube;
	uint8_t pos;
	uint8_t swing;
	uint16_t speed;
} show_packet_t;

// spin - use uint8_t for tube

// spin values
COMPILER_PACK_SET(1)
typedef struct
{
	uint8_t tube;
	uint8_t start;
	uint8_t dir;
	uint16_t speed;
} spin_packet_t;

// Two - oscillate tube between two values
// zero values
COMPILER_PACK_SET(1)
typedef struct
{
	uint8_t tube;
	uint8_t pos1;
	uint8_t pos2;
	uint16_t pause;
} two_packet_t;

// Bright equispaced dots forming a star or snowflake
typedef struct 
{
	uint8_t tube;
	uint8_t start;
	uint8_t count;
//	uint8_t dir;
//	uint16_t speed;
} star_packet_t;
 
// zero values
COMPILER_PACK_SET(1)
typedef struct
{
	uint8_t tube;
	uint8_t dir;
	uint16_t speed;
} zero_packet_t;

// bootloader data packet
typedef struct  
{
	uint16_t	addr;
	uint8_t		page[SPM_PAGESIZE];
} boot_packet_t;

enum
{
// probe command
	TWI_CMD_PROBE,					// 0 of course

// bootloader stuff must be second so other commands can be added later
	TWI_CMD_BOOTLOADER,				// bool			bootloader running

//--- debug if used
	TWI_CMD_DEKATRON_DEBUG,

//--- bootloader commands
	TWI_CMD_BOOT_FORCE,				// force entry into bootloader mode
	TWI_CMD_BOOT_STATE,				// get bootloader status
	TWI_CMD_BOOT_REPORT,			// get bootloader report
	TWI_CMD_BOOT_ADDRESS,			// send an address
	TWI_CMD_BOOT_READ,				// read a page of flash memory
	TWI_CMD_BOOT_WRITE,				// write a page of flash memory
	TWI_CMD_BOOT_ERASE,				// erase a single page
	TWI_CMD_BOOT_ERASE_ALL,			// erase all memory
	TWI_CMD_BOOT_CHECKSUM,			// evaluate and keep the flash checksum
	TWI_CMD_BOOT_JUMP0,				// jump to address 0
	TWI_CMD_BOOT_RESERVED1,
	TWI_CMD_BOOT_RESERVED2,
	TWI_CMD_BOOT_RESERVED3,
	TWI_CMD_BOOT_RESERVED4,

//--- read report data commands
	TWI_CMD_BASIC_REPORT,			// packet		basic data to configure the start up
	TWI_CMD_INIT_REPORT,			// packet		confirmation that init was a success
	TWI_CMD_SYSTEM_REPORT,			// packet		system report
	TWI_CMD_REPORT_RESERVED1,
	TWI_CMD_REPORT_RESERVED2,
	TWI_CMD_REPORT_RESERVED3,
	TWI_CMD_REPORT_RESERVED4,

//--- write commands
	TWI_CMD_START,					// start up dekatron driver
	TWI_CMD_RESTART,				// restart after a flyback start up
	TWI_CMD_LED_SET,				// bool send required LED state	
	TWI_CMD_DRIVER,					// send driver default control values
	TWI_CMD_TUBE_SET,				// set tube number for further work (set _GET commands)
	
	TWI_CMD_IDLE,					// kill job and halt
	TWI_CMD_ALL_IDLE,				// kill all jobs, don't move the dekatrons
	
	TWI_CMD_MOVE,					// move dekatron to given position and idle
	TWI_CMD_MOVE_SNAP,				// move to a cathode and halt
	TWI_CMD_MOVE_SNAP6,				// move V1 - V6 to a cathode and idle
	TWI_CMD_MOVE_SNAP7,				// move V1 - V7 to a cathode and idle
	TWI_CMD_ONCE,					// spin once and halt
	TWI_CMD_PENDULUM,				// continuous pendulum 
	TWI_CMD_SHOW,					// continuous show/highlight a position
	TWI_CMD_SPIN,					// continuous spinner
	TWI_CMD_ZERO,					// zero and halt
	TWI_CMD_ALL_ZERO,				// kill all jobs, zero all dekatrons
	TWI_CMD_ALL,					// instructions for all tubes
	TWI_CMD_TWO,					// oscillate between positions
	TWI_CMD_STAR,					// bright equispaced dots forming a star
	
	TWI_CMD_SAVE,					// save job state to buffer, zero and go idle
	TWI_CMD_RESTORE,				// restore job states

//--- read commands

// these take no arguments (length = 1)
	TWI_CMD_42_GET,					// uint8_t		communications test value
	TWI_CMD_VERSION_GET,			// uint16_t		return firmware version number
	TWI_CMD_LED_GET,				// bool			return LED status
	TWI_CMD_TYPE_GET,				// uint8_t		dekatron type setting
	TWI_CMD_TEST_GET,				// bool			test jumper setting
	TWI_CMD_SLAVE_STATE,			// uint8_t		slave state 
	
// these take one argument (length = 2)
	TWI_CMD_TUBE_GET,				// get tube position
	TWI_CMD_TUBE_GET_SNAP,			// move to nearest cathode and return position
	TWI_CMD_TUBE_JOB,				// get tube job code
};



enum
{
	TWI_SLAVE_STATE_UNDEFINED,		// 0 WTF
	TWI_SLAVE_STATE_INIT,			// 1 initialisation phase
	TWI_SLAVE_STATE_TESTING,		// 2 testing
	TWI_SLAVE_STATE_ERRORS,			// 3 errors found during testing
	TWI_SLAVE_STATE_READY,			// 4 ready for normal operation
	TWI_SLAVE_STATE_CRC,			// 5 CRC error found
	TWI_SLAVE_STATE_TIMEOUT,		// 6 Timeout error
	TWI_SLAVE_STATE_DEMO,			// 7 in demonstration mode
	TWI_SLAVE_STATE_BOOTLOADER		// 8 in bootloader mode
};

enum
{
	DEKATRON_STATE_INIT,			// 0
	DEKATRON_STATE_TESTING,			// 1
	DEKATRON_STATE_ZERO_FAILED,		// 2
	DEKATRON_STATE_COUNT_FAILED,	// 3
	DEKATRON_STATE_READY			// 4
};

enum
{
	CLOCK_TYPE_UNDEFINED,
	CLOCK_TYPE_HARWELL,
	CLOCK_TYPE_HAAR
};

// decode jumpers to type of dekatrons in use
enum
{
					// jumpers     0 1 2		
	DEKATRON_TYPE_RESERVED2,	// 1 1 1
	DEKATRON_TYPE_RESERVED1,	// 0 1 1
	DEKATRON_TYPE_RESERVED0,	// 1 0 1
	DEKATRON_TYPE_AMERICAN,		// 0 0 1
	DEKATRON_TYPE_SOVIET,		// 1 1 0
	DEKATRON_TYPE_BRITISH10,	// 0 1 0
	DEKATRON_TYPE_BRITISH12,	// 1 0 0 
	BOOTLOADER_FORCED			// 0 0 0
};

// jobs that can be assigned to each dekatron
// jobs are individual to each dekatron and are executed asynchronously
enum
{
	JOB_UNKNOWN,		// no command, converts to idle, slave internal use only
	JOB_IDLE,			// the dekatron goes idle
	JOB_MOVE,			// move to a new position then go idle
	JOB_ONCE,			// spin once and go idle
	JOB_PENDULUM,		// pendulum oscillator 
	JOB_SHOW,			// oscillate about a given position
	JOB_SPIN,			// continuous spinner
	JOB_SPINNERS,		// select multiple dekatrons and spin them
	JOB_TWO,			// oscillate between two positions on tube 7
	JOB_STAR,			// equispaced dots forming a star
	JOB_ZERO,			// seek zero then go idle
	JOB_STOP			// stop job, go to idle, slave internal use only
};

// when moving between do positions travel in this direction:
// this enum has the same values as the xxxx_HUES enum in bling.h
// to allow the same constants to be used
enum
{
	FORWARD_MOVE,		// clockwise, normal rotation, right step
	BACKWARD_MOVE,		// anticlockwise, left step
	SHORTEST_MOVE,		// right or left but shortest
	LONGEST_MOVE		// right or left but longest
};

// spinners, waves etc can spin left of right
enum
{ 
	SPIN_LEFT	= false,
	SPIN_RIGHT	= true
};

// pendulums can be linear speed or gravity (cosine) speed types
enum
{
	PENDULUM_GRAVITY	= 10,
	PENDULUM_LINEAR		= 20
};

// TWI Master Write

// Dekatron driver set up data

#define PENDULUM_DELAY_10_COUNT		16
#define PENDULUM_DELAY_12_COUNT		19

COMPILER_PACK_SET(1)
typedef struct
{
	uint8_t		cmd;			// returned command value
	uint8_t		len;			// packet length
	uint16_t	glow;			// glow transfer wait period uS
	uint16_t	count;			// step delay when counting cathodes
	uint8_t		move_dir;		// move job direction
	uint16_t	move_speed;		// move speed 
	uint8_t		once_dir;		// one spin direction
	uint16_t	once_speed;		// one spin speed
	uint8_t		pend_start;		// pendulum start position
	uint8_t		pend_type;		// pendulum type
	uint16_t	pend_sweep;		// one swing time mS
	uint16_t	show_swing;		// show sing half distance
	uint16_t	show_speed;		// show dot time
	uint8_t		spin_start;		// spinner start position
	uint8_t		spin_dir;		// spinner direction
	uint16_t	spin_speed;		// spinner speed
	uint8_t		star_start;		// dots start position
	uint8_t		star_count;		// number of dots
	uint8_t		zero_dir;		// zero job direction
	uint16_t	zero_speed;		// zero speed
	uint8_t		crc;			// CRC value
} dekatron_driver_packet_t;

#define static_assert _Static_assert

// sanity check - make sure this large data structure will fit inside the slave Rx buffer
static_assert(sizeof(dekatron_driver_packet_t) < TWI_SLAVE_RX_BUFFER, "dekatron_driver_packet_t too big");

// TWI Master Read - initialisation report
COMPILER_PACK_SET(1)
typedef struct
{
	uint8_t cmd;			// returned command value
	uint8_t len;			// packet length
	uint8_t count;			// number of dekatrons initialised
	uint8_t slave;			// slave state
	uint8_t	state[7];		// state of each dekatron
	uint8_t glowing[7];		// glowing cathode
	uint8_t cathodes[7];	// number of cathodes on the tube
	bool	zero[7];		// true if cathode zero is burning
	uint8_t crc;			// CRC value
} init_report_packet_t;

// TWI Master Read - basic data report packet
COMPILER_PACK_SET(1)
typedef struct
{
	uint8_t cmd;			// returned command value
	uint8_t len;			// packet length
	uint8_t	type;			// tube type jumpers
	uint8_t count;			// number of dekatrons installed
	bool	test;			// test jumper position
	uint8_t crc;			// CRC value
} basic_report_packet_t;

// TWI Master Read - system report packet
COMPILER_PACK_SET(1)
typedef struct
{
	uint8_t cmd;			// returned command value
	uint8_t len;			// packet length
	uint8_t	type;			// tube type jumpers
	uint8_t count;			// number of dekatrons installed
	uint8_t major;			// major version
	uint8_t minor;			// minor version
	uint8_t build;			// build version
	char	time[9];		// build time
	char	date[12];		// build date
	bool	led;			// LED state
	bool	test;			// test jumper state
	uint16_t wait;			// glow wait in uS
	uint8_t crc;			// CRC value
} system_report_packet_t;

#ifdef DEKATRON_DEBUG
// TWI Master Read - dekatron debug packet
COMPILER_PACK_SET(1)
typedef struct
{
	uint8_t cmd;			// returned command value
	uint8_t len;			// packet length
	uint16_t data[30];		// dekatron 10 data
	uint8_t crc;			// CRC value
} dekatron_debug_packet_t;
#endif

// general buffer for slave receiving data from the master
COMPILER_PACK_SET(1)
typedef struct
{
	uint8_t cmd;
	uint8_t len;
	union 
	{
// general purpose buffer
		uint8_t					data[DATA_PACKET_LENGTH];
// dekatron job data
		move_packet_t			move;
		move6_packet_t			move6;
		move7_packet_t			move7;
		once_packet_t			once;
		pendulum_packet_t		pend;
		show_packet_t			show;
		spin_packet_t			spin;
		two_packet_t			two;
		star_packet_t			star;
		zero_packet_t			zero;
		boot_packet_t			boot;	
	};
} rx_data_packet_t;

// sanity check - make sure this large data structure will fit inside the slave Rx buffer
static_assert(sizeof(rx_data_packet_t) < TWI_SLAVE_RX_BUFFER, "rx_data_packet_t is too big");

// general buffer for master sending data to the slave
// command is sent as the device address and the CRC is place after the data i.e. dat[len]

COMPILER_PACK_SET(1)
typedef struct
{
	uint8_t len;
	uint8_t dat[DATA_PACKET_LENGTH];
} tx_data_packet_t;

//---------------------------------------------------------------------------------------------------------------------

// Bootloader stuff

// variable in EEPROM to pass force the bootloader to run
#define BOOTLOADER_MAGIC				0x23051995
#define BOOTLOADER_MAGIC_LOCATION		0x0000			// uint32_t 0x0000 ... 0x0003

#define BOOTLOADER_CHECKSUM_LOCATION	0x0004			// uint8_t	0x0004


// the bootloader was entered for one of these reasons
enum
{
	BOOTLOADER_REASON_NONE,			// no reason found, run the main program
	BOOTLOADER_REASON_NO_PROGRAM,	// no flash program found
	BOOTLOADER_REASON_CHECKSUM,		// flash checksum failed
	BOOTLOADER_REASON_MAGIC,		// magic number was detected
	BOOTLOADER_REASON_JUMPERS		// board jumpers were set
};

// TWI Master Read - bootloader report packet
COMPILER_PACK_SET(1)
typedef struct
{
	uint8_t cmd;			// returned command value
	uint8_t len;			// packet length
	uint8_t major;			// major version
	uint8_t minor;			// minor version
	uint8_t build;			// build version
	char	time[9];		// build time
	char	date[12];		// build date
	bool	led;			// LED state
	bool	test;			// test jumper state
	uint8_t sig0;			// device signature in three bytes - should be 0x1e9511 for mega324pa
	uint8_t sig1;
	uint8_t sig2;
	uint8_t fuse_low;		// fuse settings should be 0xff 0xd9 0xff
	uint8_t fuse_high;
	uint8_t fuse_ext;
	uint8_t	page_size;		// SPM page size
	uint8_t type;			// setting of the type jumpers J0, J1, J2
	uint8_t	fchecksum;		// flash checksum
	uint8_t echecksum;		// eeprom checksum
	uint8_t	reason;			// reason why bootloader has been run
	uint8_t crc;			// CRC value
} boot_report_packet_t;


#endif



