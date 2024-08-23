// Copyright 2023 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Alessandro Ottaviano <aottaviano@iis.ee.ethz.ch>
// Alberto Schiaffino 
#include "dif/clint.h"
#include "dif/uart.h"
#include "dif/dma.h"
#include "params.h"
#include "regs/cheshire.h"
#include "util.h"
#include "car_util.h"

#define TEST_VECTOR 1
#include "test-spatzBenchmarks-dp-fdotp_M4096_vector.h"
// #include "test-spatzBenchmarks-dp-fdotp_M128.h"

int diyprintf(char *str, int size) {
    // char str[] = "Hello World!\r\n";
    uint32_t rtc_freq = *reg32(&__base_regs, CHESHIRE_RTC_FREQ_REG_OFFSET);
    uint64_t reset_freq = clint_get_core_freq(rtc_freq, 2500);
    uart_init(&__base_uart, reset_freq, 115200);
    uart_write_str(&__base_uart, str, size);
    uart_write_flush(&__base_uart);
    return 0;
}

void prepare_spatzd_boot () {

	// Write entry point into boot address
	volatile uintptr_t bootaddr_addr = (uintptr_t)(CAR_FP_CLUSTER_PERIPHS_BASE_ADDR(car_spatz_cluster1) + 0x58);
	writew(ELF_BOOT_ADDR, bootaddr_addr);




	// Send IRQ
	volatile uintptr_t mbox_addr = (uintptr_t)CAR_MBOX_BASE_ADDR;
	writew(1, mbox_addr + 0x20c);
    writew(1, mbox_addr + 0x204);
    writew(1, mbox_addr + 0x30c);
    writew(1, mbox_addr + 0x304);


}

uint32_t poll_spatzd_corestatus () {

	volatile uintptr_t status_addr = (uintptr_t)(CAR_SOC_CTRL_BASE_ADDR(car_soc_ctrl) + CARFIELD_SPATZ_CLUSTER1_BUSY_REG_OFFSET);
	
	while (((uint32_t)readw(status_addr)) == 1){

	}



	return (uint32_t)readw(CAR_FP_CLUSTER_PERIPHS_BASE_ADDR(car_spatz_cluster1) + 0x60); //EOC register
}

uint32_t spatzd_offloader_blocking () {

	uint32_t ret = 0;

	// Load binary payload
	#if TEST_VECTOR==0
		load_binary();
	#else
		for(int i=0; i<ELF_BOOT_SIZE/4; i++) {
			writew(payload[i], ELF_START_ADDR + i*4);
			//(*(volatile unsigned int *)(uint64_t)(ELF_START_ADDR + i*4)) = payload[i] ;
		}
		if(readw(ELF_BOOT_ADDR) != 0x00001197) {
			char str[] = "Wrong Boot Address\n";
			diyprintf(str, sizeof(str));
		}
	#endif

	

	fence();

	// Select bootmode, write entry point, write launch signal
	prepare_spatzd_boot();

	// Poll status register
	volatile uint32_t corestatus = poll_spatzd_corestatus();


	// Check core status. Return spatzd exit code to signal an error in the execution.
	if ((corestatus & 0xFFFFFFFE) != 0) {
	    ret = corestatus >> 1;
	}
	return ret;
}


int main(void)
{

	// Init the HW
	// Spatz Island
    car_enable_domain(CAR_SPATZ_CL1_RST);

	// char str[] = "1\n";
	// diyprintf(str, sizeof(str));
	//we need to offload file
	
	// Here we assume that the offloader has to poll a status register to catch the end of
	// computation of the Spatzd Island. Therefore, the offloading is blocking.
	uint32_t ret = spatzd_offloader_blocking();

	// str[0] = '7';
	// diyprintf(str, sizeof(str));
	return ret;
}
