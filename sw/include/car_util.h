// Copyright 2023 ETH Zurich and University of Bologna.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Robert Balas <balasr@iis.ee.ethz.ch>
// Alessandro Ottaviano <aottaviano@iis.ee.ethz.ch>

#ifndef __CAR_UTIL_H
#define __CAR_UTIL_H

#include "util.h"
#include "car_memory_map.h"
#include "regs/soc_ctrl.h"
#include "io.h"

// for the calculation check safety island top
#define SAFETY_ISLAND_BOOT_ADDR_RSVAL (CAR_SAFETY_ISLAND_PERIPHS_BASE_ADDR + 0x1080)

enum car_isolation_status { CAR_ISOLATE_DISABLE = 0, CAR_ISOLATE_ENABLE = 1 };

enum car_rst_status { CAR_RST_ASSERT = 1, CAR_RST_RELEASE = 0 };

enum car_clk {
    CAR_HOST_CLK     = 0,
    CAR_PERIPH_CLK   = 1,
    CAR_SAFETY_CLK   = 2,
    CAR_SECURITY_CLK = 3,
    CAR_PULP_CLK     = 4,
    CAR_SPATZ_CLK    = 5,
    CAR_L2_CLK       = 6,
};

enum car_rst {
    CAR_PERIPH_RST   = 0,
    CAR_SAFETY_RST   = 1,
    CAR_SECURITY_RST = 2,
    CAR_PULP_RST     = 3,
    CAR_SPATZ_RST    = 4,
    CAR_L2_RST       = 5,
};

// these do not exist so we set a dummy value
#define CARFIELD_L2_ISOLATE_REG_OFFSET -1
#define CARFIELD_L2_ISOLATE_STATUS_REG_OFFSET -1

#define CARFIELD_HOST_CLK_EN_REG_OFFSET -1
#define CARFIELD_HOST_CLK_SEL_REG_OFFSET -1
#define CARFIELD_HOST_CLK_DIV_VALUE_REG_OFFSET -1

// generate register offset for reset domains from autogenerated soc_ctrl.h
#define X(NAME)                                                                \
    static inline uint32_t car_get_##NAME##_offset(enum car_rst rst)           \
    {                                                                          \
        switch (rst) {                                                         \
        case CAR_PERIPH_RST:                                                   \
            return CARFIELD_PERIPH_##NAME##_REG_OFFSET;                        \
        case CAR_SAFETY_RST:                                                   \
            return CARFIELD_SAFETY_ISLAND_##NAME##_REG_OFFSET;                 \
        case CAR_SECURITY_RST:                                                 \
            return CARFIELD_SECURITY_ISLAND_##NAME##_REG_OFFSET;               \
        case CAR_PULP_RST:                                                     \
            return CARFIELD_PULP_CLUSTER_##NAME##_REG_OFFSET;                  \
        case CAR_SPATZ_RST:                                                    \
            return CARFIELD_SPATZ_CLUSTER_##NAME##_REG_OFFSET;                 \
        case CAR_L2_RST:                                                       \
            return CARFIELD_L2_##NAME##_REG_OFFSET;                            \
        default:                                                               \
            return -1;                                                         \
        }                                                                      \
    }

X(RST);
X(ISOLATE);
X(ISOLATE_STATUS);
#undef X

// generate register offset for clock domains from autogenerated soc_ctrl.h
#define X(NAME)                                                                \
    static inline uint32_t car_get_##NAME##_offset(enum car_clk clk)           \
    {                                                                          \
        switch (clk) {                                                         \
        case CAR_HOST_CLK:                                                     \
            return CARFIELD_HOST_##NAME##_REG_OFFSET;                          \
        case CAR_PERIPH_CLK:                                                   \
            return CARFIELD_PERIPH_##NAME##_REG_OFFSET;                        \
        case CAR_SAFETY_CLK:                                                   \
            return CARFIELD_SAFETY_ISLAND_##NAME##_REG_OFFSET;                 \
        case CAR_SECURITY_CLK:                                                 \
            return CARFIELD_SECURITY_ISLAND_##NAME##_REG_OFFSET;               \
        case CAR_SPATZ_CLK:                                                    \
            return CARFIELD_SPATZ_CLUSTER_##NAME##_REG_OFFSET;                 \
        case CAR_L2_CLK:                                                       \
            return CARFIELD_L2_##NAME##_REG_OFFSET;                            \
        default:                                                               \
            return -1;                                                         \
        }                                                                      \
    }

X(CLK_EN);
X(CLK_SEL);
X(CLK_DIV_VALUE);
#undef X

static inline enum car_clk car_clkd_from_rstd(enum car_rst rst)
{
    switch (rst) {
    case CAR_PERIPH_RST:
        return CAR_PERIPH_CLK;
    case CAR_SAFETY_RST:
        return CAR_SAFETY_CLK;
    case CAR_SECURITY_RST:
        return CAR_SECURITY_CLK;
    case CAR_PULP_RST:
        return CAR_PULP_CLK;
    case CAR_SPATZ_RST:
        return CAR_SPATZ_CLK;
    case CAR_L2_RST:
        return CAR_L2_CLK;
    }
}

void car_set_isolate(enum car_rst rst, enum car_isolation_status status)
{
    writew(status, CAR_SOC_CTRL_BASE_ADDR + car_get_ISOLATE_offset(rst));
    fence();
    while (readw(CAR_SOC_CTRL_BASE_ADDR + car_get_ISOLATE_STATUS_offset(rst)) !=
           status)
        ;
}

void car_enable_clk(enum car_clk clk)
{
    writew(1, CAR_SOC_CTRL_BASE_ADDR + car_get_CLK_EN_offset(clk));
    fence();
}

void car_disable_clk(enum car_clk clk)
{
    writew(0, CAR_SOC_CTRL_BASE_ADDR + car_get_CLK_EN_offset(clk));
    fence();
}

void car_set_rst(enum car_rst rst, enum car_rst_status status)
{
    writew(status, CAR_SOC_CTRL_BASE_ADDR + car_get_RST_offset(rst));
    fence();
}

void car_reset_domain(enum car_rst rst)
{
    car_set_isolate(rst, CAR_ISOLATE_ENABLE);
    car_disable_clk(car_clkd_from_rstd(rst));

    car_set_rst(rst, CAR_RST_ASSERT);
    for (volatile int i = 0; i < 16; i++)
        ;
    car_set_rst(rst, CAR_RST_RELEASE);

    car_enable_clk(car_clkd_from_rstd(rst));
    car_set_isolate(rst, CAR_ISOLATE_DISABLE);
}

#endif
