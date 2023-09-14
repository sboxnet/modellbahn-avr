/*
 * IncFile1.h
 *
 * Created: 14.09.2023 10:04:20
 *  Author: balagi
 */ 


#ifndef DCC_DEC_H_
#define DCC_DEC_H_

#define DCC_DEC_STATE_OFF      0
#define DCC_DEC_STATE_FIRST    1
#define DCC_DEC_STATE_PREAMBLE 2
#define DCC_DEC_STATE_STARTBIT 3
#define DCC_DEC_STATE_BIT_H1   4
#define DCC_DEC_STATE_BIT_H2   5
#define DCC_DEC_STATE_STARTSTOPBIT 6

struct dcc_dec_t {
	uint8_t state;
	uint8_t preamble;
	uint8_t buf[10];
	uint8_t bufsize;
	uint8_t bits;
	uint8_t bitbuf;
	uint8_t lasthbit;
	uint8_t xor;
	uint8_t cutout;
};


void dcc_dec_init(struct dcc_dec_t* dccdec);
void dcc_dec_start(void);
void dcc_dec_stop(void);
void dcc_dec_parse_packet(struct dcc_dec_t* dccdec);
void dcc_dec_halfbit(struct dcc_dec_t* dccdec, uint8_t hb);


#endif /* DCC_DEC_H_ */