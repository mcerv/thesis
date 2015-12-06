
#ifndef __DEFINES_H_INCLUDED__
#define __DEFINES_H_INCLUDED__

#define PSA_OK 0
#define PSA_FAULT -1

//input registers
#define COSYLAB_CONTROL 1
#define COSYLAB_CONTROL_READBACK 96
#define COSYLAB_DEBUG_REG 98
#define COSYLAB_STATUS_REG 65
#define COSYLAB_DATA_TRANSFER_REQUEST 3


#define PSA_ARRAY_LEN 156 // --pulse array length


#define PSA_RAW_DATA_DELAY_LOW 6    //delay of raw data during USB transfer
#define PSA_RAW_DATA_DELAY_HIGH 7
#define PSA_INPUT_PARAM_MIN_SAM_OVER_THRES0 10
#define PSA_INPUT_PARAM_LO_THRES0 11
#define PSA_INPUT_PARAM_HI_THRES0 12
#define PSA_INPUT_PARAM_HI_THRES1 13
#define PSA_INPUT_PARAM_GAMMA_THRES 14
#define PSA_INPUT_PARAM_WIDTH_CUT0 15
#define PSA_INPUT_PARAM_SWAP_POLARITY 16
#define PSA_PULSE_ARRAY 100 // -- address 100 - 255
#define PSA_CONTROL 17 // -- start/stop PSA, signal parser, ..
#define PSA_CONTROL_2 18 // --
#define PSA_CONTROL_3 19
#define PSA_INPUT_PARAM_AREA_CUT0 2
#define PSA_INPUT_PARAM_AMPL_CUT 3
#define PSA_INPUT_PARAM_SLOPE_CUT 4
#define PSA_CONTROL_4 5
#define GEEL_TIMEOUT 100
#define PSA_USB_FIFO_TRANSFER_LENGTH 101
#define PSA_RAW_FIFO_FULL_THRESHOLD0 102
#define PSA_RAW_FIFO_FULL_THRESHOLD1 103
#define PSA_BASELINE_VALUE 104

#define PSA_INPUT_PARAM_WIDTH_CUT1 6
#define PSA_INPUT_PARAM_AREA_CUT1 7
#define PSA_CONTROL_5 8
#define PSA_CONTROL_6 9
#define PSA_INPUT_PARAM_AREA_HI_CUT0 125
#define PSA_INPUT_PARAM_AREA_HI_CUT1 126
#define PSA_INPUT_PARAM_WIDTH_HI_CUT0 127
#define PSA_INPUT_PARAM_WIDTH_HI_CUT1 128
#define PSA_INPUT_PARAM_AMPL_HI_CUT 129
#define PSA_INPUT_PARAM_SLOPE_HI_CUT 130
#define PSA_EDGE_TRIGDOUBLECNT0 131
#define PSA_EDGE_TRIGDOUBLECNT1 132
#define PSA_EDGE_TRIGDOUBLECNT2 133
#define PSA_EDGE_TRIGDOUBLECNT3 134
#define PSA_EDGE_TRIGCNT0	135
#define PSA_EDGE_TRIGCNT1	136
#define PSA_EDGE_TRIGCNT2	137
#define PSA_EDGE_TRIGCNT3	138
#define PSA_EDGE_FSMTRIGCNT0 139
#define PSA_EDGE_FSMTRIGCNT1 140
#define PSA_EDGE_FSMTRIGCNT2 141
#define PSA_EDGE_FSMTRIGCNT3 142
#define PSA_CLEAN_AREA_THRES 143
#define PSA_DEBUG_AREA0 144
#define PSA_DEBUG_AREA1 145
#define PSA_DEBUG_VALID 146



//control register bits
#define PSA_ENABLE 0
#define PSA_RESET_RESULTS 1
#define PSA_PARSER_ENABLE 2
#define PSA_PARSER_PARSE 3
#define PSA_RAW_SIG_HANDLER_ENABLE 4
#define PSA_DDR_TEST_WORD_ENABLE 5    // used also for histogramming enable
#define PSA_USB_RAW_TRANSFER 6
#define PSA_USB_RAW_CONSTANT 7
//control_2 register bits. here 0 - 8, in FPGA 8-15
#define PSA_INPUT_SELECT 0   //enable - real signals on chA. disable - parsed signals.
#define PSA_REQUEST_RAW_DATA_TRANSFER 1 //request raw pulse transfer - bit 9 in fpga
#define PSA_HISTOGRAMMING_ENABLE 2   //enable histogramming - bit 10 in fpga
#define PSA_AUTOMATIC_RESET_ENABLE 3 //enable automatic reset of PSA after a pulse - bit 11 in fpga
#define PSA_TEST_DATATAKING_MODE_ENABLE 4 //enable test datataking mode - bit 12 in fpga
#define PSA_MANUAL_SINGLE_TRIGGER 5 //send a single trigger. - bit 13 in fpga
#define PSA_BASELINE_RECOVER_ENABLE 6 //enable baseline recover during pulse - bit 14 in fpga
#define PSA_BASELINE_FILTER_ENABLE 7  //enable baseline filtering - bit 15 in fpga
//control_3 register bits. here 0 - 8, in FPGA 16 - 23
#define PSA_BASELINE_DECIMATE_LO 0    //4-sample moving average. which samples?
#define PSA_BASELINE_DECIMATE_HI 1    //00: sample 0, -2, -4, -6. 01: sample 0, -4, -8, -12, ... //bits 16 and 17 in fpga
#define PSA_HIST_AREA_SCALE_0 2       // scale the area histogram (default 4)
#define PSA_HIST_AREA_SCALE_1 3       // bits 18, 19 and 20 in fpga
#define PSA_HIST_AREA_SCALE_2 4       // scaling factor for the pulse area histogram
#define PSA_HIST_AREA_SCALE_3 5      //  scaling factor for the pulse area histogram (0 for shortest pulses)
#define PSA_HIST_WIDTH_SCALE_0 6    // scaling facotr for the pulse width histogram (0 for shortest pulses)
#define PSA_HIST_WIDTH_SCALE_1 7    // scaling facotr for the pulse width histogram (0 for shortest pulses)
//control_4 register bits. here 0 - 8, in FPGA 24 - 31
#define PSA_HIST_WIDTH_SCALE_2 0    // scaling facotr for the pulse width histogram (0 for shortest pulses)
#define PSA_OPERATION_MODE   1      // operation modes:   0 - normal, short - 51 ns buffers.
#define GEEL_ENABLE 2          //                    1 - normal, long - 800 ns buffers.
#define GEEL_RESET 3                                    //                   Geel trigger
#define PSA_USBFIFO_DELAY_WRITE 4
#define PSA_ANALOG_READOUT_ENABLE 5
#define PSA_RAW_FIFO_FULL_THRES 6    //1024 for 0 and 1023 for 1
#define PSA_FSM_TRANSFER_ENABLE 7    //1 to enable all transfer functionalities.
//control_5 register bits. here 0 - 7, in FPGA 32 - 39
#define PSA_FWVH_SELECT_0 0
#define PSA_FWVH_SELECT_1 1
#define PSA_EVAL_ACC_LOGIC_0 2
#define PSA_EVAL_ACC_LOGIC_1 3
#define PSA_EVAL_ACC_LOGIC_2 4
#define PSA_EVAL_ACC_LOGIC_3 5
#define PSA_EVAL_ACC_LOGIC_4 6

//status register bits
#define PSA_TEST_DATATAKING_MODE_BUFFER_FULL 0 //when the raw signal buffer during data taking is full.
#define PSA_HIST_DATA_CORRUPTED_FLAG 1 // set if the histogram data headers arent AAAA or DEAD
#define PSA_ANALOG_READOUT_FULL_FLAG 2

//readback registers
#define PSA_CLASSIFICATION 20
#define PSA_CNT_ALL0 21
#define PSA_CNT_ALL1 22
#define PSA_CNT_ALL2 23
#define PSA_CNT_ALL3 24
#define PSA_CNT_GAMMA0 25
#define PSA_CNT_GAMMA1 26
#define PSA_CNT_GAMMA2 27
#define PSA_CNT_GAMMA3 28
#define PSA_CNT_ALPHA0 29
#define PSA_CNT_ALPHA1 30
#define PSA_CNT_ALPHA2 31
#define PSA_CNT_ALPHA3 32
#define PSA_CONTROL_READBACK 33
#define PSA_CONTROL_2_READBACK 45
#define PSA_CONTROL_3_READBACK 48
#define PSA_CONTROL_4_READBACK 84
#define PSA_CONTROL_5_READBACK 89
#define PSA_CONTROL_6_READBACK 90
#define PSA_CNT_PROTO0 85
#define PSA_CNT_PROTO1 86
#define PSA_CNT_PROTO2 87
#define PSA_CNT_PROTO3 88


//counters
#define PSA_TIMESTAMP0 72
#define PSA_TIMESTAMP1 73
#define PSA_TIMESTAMP2 74
#define PSA_TIMESTAMP3 75
#define PSA_TIMESTAMP4 76
#define PSA_TIMESTAMP5 77
#define PSA_TIMESTAMP6 78
#define PSA_TIMESTAMP7 79
#define PSA_CNT_VALID0 80
#define PSA_CNT_VALID1 81
#define PSA_CNT_VALID2 82
#define PSA_CNT_VALID3 83

//GEEL counters
#define GEEL_CNT_ALL0		110
#define GEEL_CNT_ALL1		111
#define GEEL_CNT_ALL2		112
#define GEEL_CNT_ALL3		113
#define GEEL_CNT_FIRST0	114
#define GEEL_CNT_FIRST1	115
#define GEEL_CNT_FIRST2	116
#define GEEL_CNT_FIRST3	117
#define GEEL_CNT_SECOND0	118
#define GEEL_CNT_SECOND1	119
#define GEEL_CNT_SECOND2	120
#define GEEL_CNT_SECOND3	121


//high threshold 0?
#define PSA_INPUT_P_READBACK 34
#define PSA_STATUS 35
#define PSA_PARSE_CHKSUM 36
#define PSA_DEBUG_TRIGCNT 37
#define PSA_DEBUG_FWHM_WIDTH 38
#define PSA_DEBUG_FWHM_HALF_MAX 39
#define PSA_DEBUG_PREPROC_LEN_OVERFLOW  40
#define PSA_DEBUG_PREPROC_MAX_AMPLITUDE 41
#define PSA_DEBUG_PREPROC_PULSE_LEN 42
#define PSA_DEBUG_DDR_BURST_COUNTER 43
#define PSA_RAW_HANDLER_DEBUG_UPTIME 44
//PSA_CONTROL_2_READBACK = 45
#define PSA_TRIGFREQ_LOW 46
#define PSA_TRIGFREQ_HIGH 47


//Histogram conversion factors
//#define NBINS 60                          //fwhm bins
#define NBINS 256                          //fwhm bins
#define NBINS_AREA 256                    //area bins
#define NBINS_EXTAREA 4096                    //area bins
#define NBINS_MAXAMPL 256                    // maxampl bins
#define NBINS_FWHM_SLOPE_X 64
// #define NBINS_FWHM_SLOPE_Y 16
#define NBINS_FWHM_SLOPE_Y 64
// #define NBINS_FWHM_SLOPE 1024
#define NBINS_FWHM_SLOPE 4096
#define NBINS_GEEL_TRIGGER 1024
#define SAMPLING_RATE (double)0.2                     //sampling rate
#define SAMPLING_RATE_GEEL (double)0.8                     //sampling rate
//#define AREA_HIST_SCALING_FACTOR 4        //set before the acquisition starts
//#define PS_INPUT_MV 100                   //set before the acquisition starts
//#define ADC_TO_MV (double)1.0/(double)128.0*(double)PS_INPUT_MV.0/(double)AREA_HIST_SCALING_FACTOR // 1/128*100*4
//#define AREA_TO_PVS (double)ADC_TO_MV*SAMPLING_RATE   //mV * ns = pVs
#define ENERGY_SCALING_FACTOR_PVS_TO_MEV (double)5.115/(double)535.0   //area of 535 pVs = energy 5.115 MeV
//#define AREA_TO_MEV (double)AREA_TO_PVS*ENERGY_SCALING_FACTOR_PVS_TO_MEV
//for 100mV input, AREA_TO_MV = 1/128*100*4*0.2*5.115/535.0 = 0.005975467

#define MAX_USB_DATA_LEN_BUF 260000

//time to wait in microseconds
#define PS_LONG_WAIT  1000000
#define PS_SHORT_WAIT 100

//when cropping the pulse data from 1000samples to 156 samples,
//this constant tells us where to start from
#define PSA_PARSER_PULSE_START 420

#endif //ifndef
