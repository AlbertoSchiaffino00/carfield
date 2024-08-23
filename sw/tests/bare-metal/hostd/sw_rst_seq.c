// Copyright 2023 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Robert Balas <balasr@iis.ee.ethz.ch>
// Alessandro Ottaviano <aottaviano@iis.ee.ethz.ch>

// Basic testing of warm resets. This test is executed only from LLC/SPM since it tests SW reset on
// shared L2 and hyperbus memory controller.

#include <stdint.h>
#include "params.h"
#include "regs/cheshire.h"
#include "regs/soc_ctrl.h"
#include "regs/safety_soc_ctrl.h"
#include "regs/spatz_cluster_peripheral.h"
#include "util.h"
#include "car_util.h"
#include "car_memory_map.h"

int diyprintf(char *str, int size) {
    // char str[] = "Hello World!\r\n";
    uint32_t rtc_freq = *reg32(&__base_regs, CHESHIRE_RTC_FREQ_REG_OFFSET);
    uint64_t reset_freq = clint_get_core_freq(rtc_freq, 2500);
    uart_init(&__base_uart, reset_freq, 115200);
    uart_write_str(&__base_uart, str, size);
    uart_write_flush(&__base_uart);
    return 0;
}


int main(void)
{
    // Spatz Island
    car_enable_domain(CAR_SPATZ_CL1_RST);
    car_enable_domain(CAR_SPATZ_CL2_RST);
    // Safety Island

    // We write a bunch of bytes to the safety island's boot register and check
    // that it reads the reset value after a warm reset. Note that we can't use
    // the safety island's scratchpad for that since it is not resetable
    uint64_t magic = 0xcafebeef;


    // // Spatz CL1
    writew(magic, CAR_FP_CLUSTER_PERIPHS_BASE_ADDR(car_spatz_cluster1) +
		      SPATZ_CLUSTER_PERIPHERAL_CLUSTER_BOOT_CONTROL_REG_OFFSET);
    if (readw(CAR_FP_CLUSTER_PERIPHS_BASE_ADDR(car_spatz_cluster1) +
	      SPATZ_CLUSTER_PERIPHERAL_CLUSTER_BOOT_CONTROL_REG_OFFSET) !=
	magic){
        return EFPCLNOACCES;
    }else{
        char str[] = "Wrote to SPATZ CLUSTER 1 boot control\n";
        diyprintf(str, sizeof(str));
    } 

    magic +=1;
    // // Spatz CL2
    writew(magic, CAR_FP_CLUSTER_PERIPHS_BASE_ADDR(car_spatz_cluster2) +
		      SPATZ_CLUSTER_PERIPHERAL_CLUSTER_BOOT_CONTROL_REG_OFFSET);
    if (readw(CAR_FP_CLUSTER_PERIPHS_BASE_ADDR(car_spatz_cluster2) +
	      SPATZ_CLUSTER_PERIPHERAL_CLUSTER_BOOT_CONTROL_REG_OFFSET) !=
	magic){
        return EFPCLNOACCES;
    }else{
        char str[] = "Wrote to SPATZ CLUSTER 2 boot control\n";
        diyprintf(str, sizeof(str));
    } 

    car_reset_domain(CAR_SPATZ_CL1_RST);
    car_reset_domain(CAR_SPATZ_CL2_RST);

    if (readw(CAR_FP_CLUSTER_PERIPHS_BASE_ADDR(car_spatz_cluster1) +
	      SPATZ_CLUSTER_PERIPHERAL_CLUSTER_BOOT_CONTROL_REG_OFFSET) != 0){
        return EFPCLNOACCES;
    }else{
        char str[] = "Reset SPATZ CLUSTER 1\n";
        diyprintf(str, sizeof(str));
    }

    if (readw(CAR_FP_CLUSTER_PERIPHS_BASE_ADDR(car_spatz_cluster2) +
	      SPATZ_CLUSTER_PERIPHERAL_CLUSTER_BOOT_CONTROL_REG_OFFSET) != 0){
        return EFPCLNOACCES;
    }else{
        char str[] = "Reset SPATZ CLUSTER 2\n";
        diyprintf(str, sizeof(str));
    }
	// return EFPCLNOACCES;

    // // L2 Reset
    // // Memory doesn't have a reset so this needs to be checked manually
    // car_reset_domain(CAR_L2_RST);

    // Security Island
    // We can't access anything so this needs to be checked manually In secure boot mode, the
    // security island can't be software resetted, which is why we disable this for now
    // car_reset_domain(CAR_SECURITY_RST);

    return 0;
}
