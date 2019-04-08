/*
 * Simon_Says_main.cpp
 *
 *  Created on: Apr 3, 2019
 *      Author: Toan
 */
#include "chu_init.h"
#include "gpio_cores.h"
#include "xadc_core.h"
#include "sseg_core.h"
#include "spi_core.h"
#include "i2c_core.h"
#include "ps2_core.h"
#include "ddfs_core.h"
#include "adsr_core.h"
#include "prng_core.h"

/*Function used to read switches from Basys 3*/
void readSwitches();
/*Function used to set the PRNG seed based on 8 LSB switches*/
void setPRNG();
/*Function called from displayLevel used to display a particular level*/
void displaySeg (uint8_t *ptn_array);
/*Used to display the state of the game: begn pass fail end*/
void displayState (uint8_t *ptn_array);
/*Generate a random number using PRNG*/
uint8_t randomNum();
/*Used to display the current level and sequence generated*/
void displayLevel();
/*Function  used to initialize data structures*/
int init();
/*Function used to display end infinitely when a player wins. Press center button to play again*/
void win();
/*used to keep track of a certain level, if player_lvl == game_lvl player pass else fail*/
int playRounds();

GpoCore led(get_slot_addr(BRIDGE_BASE, S2_LED));
GpiCore sw(get_slot_addr(BRIDGE_BASE, S3_SW));
PRNGCore prng (get_slot_addr(BRIDGE_BASE,S4_USER));
XadcCore adc(get_slot_addr(BRIDGE_BASE, S5_XDAC));
PwmCore pwm(get_slot_addr(BRIDGE_BASE, S6_PWM));
DebounceCore btn(get_slot_addr(BRIDGE_BASE, S7_BTN));
SsegCore sseg(get_slot_addr(BRIDGE_BASE, S8_SSEG));
SpiCore spi(get_slot_addr(BRIDGE_BASE, S9_SPI));
I2cCore adt7420(get_slot_addr(BRIDGE_BASE, S10_I2C));
Ps2Core ps2(get_slot_addr(BRIDGE_BASE, S11_PS2));
DdfsCore ddfs(get_slot_addr(BRIDGE_BASE, S12_DDFS));
AdsrCore adsr(get_slot_addr(BRIDGE_BASE, S13_ADSR), &ddfs);

/*Display the different sequences: Up, down, right, and left */
uint8_t right = 0xf9;
uint8_t left = 0xcf;
uint8_t up = 0xfe;
uint8_t down = 0xf7;

/*Display the messages of the game for different stages: begin, pass, fail, end*/
uint8_t bgn[4] ={0xab,0x90,sseg.h2s(14),sseg.h2s(11)};
uint8_t pass[4] ={sseg.h2s(5),sseg.h2s(5),sseg.h2s(10),0x8c};
uint8_t fail[4] = {0xc7,right,sseg.h2s(10),0x8e};
uint8_t end[4] = {0xff,sseg.h2s(13),0xab,sseg.h2s(14)};
uint8_t blank[4] = {0xff,0xff,0xff,0xff};

/*data structures to keep track of game*/
uint8_t lvl_array[15];	//sequence generated from PRNG
int lvl_array2[15];
int game_lvl = 1;		//current game level
int player_lvl = 0;		//keeps track of player's progress
unsigned int sw_arr[8];	//switch array used for PRNG


/*Timer delay for displaying sequence*/
int time = 0;

/**
 * Function reads in switches
 */
void readSwitches(){
	for (int i = 0; i < 8; i++){
		sw_arr[i] = sw.read(i);
	}
	//temporary array used to read switches for time delay
	int tmp[2];
	tmp[0] = sw.read(14);
	tmp[1] = sw.read(15);
	if(tmp[0] == 0 && tmp[1] == 0){
		time = 2000;
	}else if(tmp[1] == 0 && tmp[0] == 1){
		time = 1500;
	}else if(tmp[1] == 1 && tmp[0] == 0){
		time = 1000;
	}else{
		time = 500;
	}
	setPRNG();
}

/*Set PRNG using pins*/
void setPRNG(){
	for(int i=0; i<8; i++){
		prng.write(sw_arr[i],i);
	}
	prng.write(1,24);
}

/*Function used to write to seven segment display*/
void displaySeg (uint8_t *ptn_array){
	for(int i = 0; i < 4; i++){
		sseg.write_1ptn(*ptn_array,i);
		ptn_array++;
		sseg.set_dp(4);
	}
}

/*Function used to write to seven segment display*/
void displayState (uint8_t *ptn_array){
	for(int i = 0; i < 4; i++){
		sseg.write_1ptn(*ptn_array,i);
		ptn_array++;
		sseg.set_dp(0);
	}
}
/*Generate a random number from 0-3*/
/*0 is for Up, 1 is for down, 2 is for left, 3 is for right */
uint8_t randomNum(){
	prng.write(1,20);
	prng.write(0,28);
	int ret = (int) prng.read();
	return ret;
}

/*Display current level*/
void displayLevel(){
	uint8_t tmp[4] = {0xff,0xff,0xff,0xff};
	for(int i=0; i<game_lvl;i++){
		if(game_lvl <10){
			tmp[2] = sseg.h2s(i+1);
			tmp[3] = sseg.h2s(0);
		}else{
			tmp[2] = sseg.h2s((i+1)%10);
			tmp[3] = sseg.h2s(1);
		}
		//Display Up
		if(lvl_array[i] == 0){
			lvl_array2[i] = 0;
			tmp[0] = up;
			tmp[1] = up;
		//Display Down
		}else if (lvl_array[i] == 1){
			lvl_array2[i] = 1;
			tmp[0] = down;
			tmp[1] = down;
		//Display left
		}else if (lvl_array[i] == 2){
			lvl_array2[i] = 2;
			tmp[0] = 0xff;
			tmp[1] = left;
		}else if(lvl_array[i] == 3){
			lvl_array2[i] = 3;
			tmp[0] = right;
			tmp[1] = 0xff;
		}
		displaySeg(tmp);
		sleep_ms(time);
	}
	tmp[0] = 0xff;
	tmp[1] = 0xff;
	displaySeg(tmp);
}

/*Initialize any values needed */
int init(){
	player_lvl = 0;
	game_lvl = 1;
	do{
		displayState(bgn);
		sleep_ms(200);
		displayState(blank);
		sleep_ms(200);
	}while(btn.read_db(4) != 1);
	readSwitches();
	prng.write(1,28);
	prng.write(1,20);

	for(int i=0;i<15;i++){
		int rand = randomNum();
		lvl_array[i] = rand;
	}
	return 1;
}

/*If player win, display End infinitely*/
void win(){
	do{
		displayState(end);
		sleep_ms(200);
		displayState(blank);
		sleep_ms(200);
	}while(btn.read_db(4) != 1);
}


/*Function used to control player's sequence verses game's sequence*/
int playRounds(){
	player_lvl = 0;
	bool passed = false;
	int pressed = 10;
	while(passed == false){
		while(btn.read_db(0) != 1 && btn.read_db(1) != 1 && btn.read_db(2) != 1 && btn.read_db(3) !=1);
		//up button
		if(btn.read_db(0) == 1) {
			uart.disp("Up button\n\r");
			pressed = 0;
		//Right button
		}if(btn.read_db(1) == 1){
			uart.disp("right button\n\r");
			pressed = 3;
		//Down button
		}if(btn.read_db(2) == 1){
			uart.disp("down button\n\r");
			pressed = 1;
		//left button
		}if(btn.read_db(3) == 1){
			uart.disp("left button\n\r");
			pressed = 2;
		}
		if(pressed != lvl_array2[player_lvl]){
			displayState(fail);
			sleep_ms(2000);
			break;
		}else if(pressed == lvl_array2[player_lvl]){
			player_lvl++;
			sleep_ms(200);
			if(player_lvl == game_lvl){
				passed = true;
			}
		}
	}
	if(passed == true){
		return 1;
	}
	return 0;
}


int main(){
	int a = 0;
	int passed = 10;
	while(1){
		a = init();
		if(a == 1){
			while(game_lvl < 16){
				displayLevel();
				passed = playRounds();
				if(passed == 0){
					game_lvl = 1;
					player_lvl = 0;
					break;
				}else if(passed == 1){
					displayState(pass);
					sleep_ms(2000);
					game_lvl++;
				}
			}//end game lvl loop

			//FAILED
			if(game_lvl != 15){
				for(int i=0; i<5;i++){
				displayState(end);
				sleep_ms(200);
				displayState(blank);
				sleep_ms(200);
				}
				continue;
			}else{
				win();
			}
		}
	}
	return (0);
}
