/*
 * prng_core.h
 *
 *  Created on: Apr 3, 2019
 *      Author: Toan
 */

#ifndef SRC_PRNG_CORE_H_
#define SRC_PRNG_CORE_H_

#include "chu_init.h"
class PRNGCore {
public:

	/**
	 * register map
	 */
	enum {
		PRNG_REG = 0
	};

	/**
	 * constructor
	 *
	 */
	PRNGCore(uint32_t core_base_addr);
	/**
	 * destructor not used
	 */
	~PRNGCore();

	/**
	 * write a 32-bit word
	 * @param data 32-bit data
	 */
	void write(uint32_t data);

	/**
	 * write a it at a specific position
	 *
	 */
	void write(int bit_value, int bit_pos);

	/**
	 * read a 32-bit word
	 * @return 32-bit read data word
	 * @note unused bits return 0's
	 */
	uint32_t read();

	/**
	 * read a bit at a specific position
	 * @param bit_pos bit position
	 *
	 */
	int read(int bit_pos);

private:
	uint32_t base_addr;
	uint32_t wr_data;

};

#endif /* SRC_PRNG_CORE_H_ */
