/*
 * prng.cpp
 *
 *  Created on: Apr 3, 2019
 *      Author: Toan
 */

#include "prng_core.h"

PRNGCore::PRNGCore(uint32_t core_base_addr){
	base_addr = core_base_addr;
	wr_data = 0;
}

PRNGCore::~PRNGCore(){

}

void PRNGCore::write(uint32_t data){
	wr_data = data;
	io_write(base_addr, PRNG_REG,wr_data);
}

void PRNGCore::write(int bit_value, int bit_pos){
	bit_write(wr_data, bit_pos, bit_value);
	io_write(base_addr, PRNG_REG, wr_data);
}
uint32_t PRNGCore::read(){
	return (io_read(base_addr, PRNG_REG));
}

int PRNGCore::read(int bit_pos){
	uint32_t rd_data = io_read(base_addr, PRNG_REG);
	return ((int) bit_read(rd_data, bit_pos));
}




