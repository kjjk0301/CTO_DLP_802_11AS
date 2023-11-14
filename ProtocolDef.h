#define CAN_SFF_MASK 0x000007FFU /* standard frame format (SFF) */
#define CAN_EFF_MASK 0x1FFFFFFFU /* extended frame format (EFF) */
#define CAN_ERR_MASK 0x1FFFFFFFU /* omit EFF, RTR, ERR flags */

#define CANBMUID 			0x555
#define CAN_MASTER_DSPID 	0x551
#define CAN_SLAVE_DSPID 	0x552
#define SWITCH_BTL_ID 		0x630
#define CHIP_STATUS_ID 		0x730
//#define CHIP_STATUS_ID 		0x551


#define PCS_CAN_ID 			0x100  // HOST
//#define MBU_CAN_ID 			0x200  // BCU
#define MBU_CAN_ID 			0x301  // BCU
#define CT1_CAN_ID 			0x300  // BCU

#define MS_1    (0x4D)  // Single CPU


#define HEADER_CODE		0xFF
#define TAIL_CODE		0xFA
#define RTAIL_CODE		0xF0

#define CHECK_STAT		0x73
#define STAT_BTL		0x0B
#define STAT_APP		0x0A
