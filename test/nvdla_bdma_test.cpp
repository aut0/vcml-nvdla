/******************************************************************************
 *                                                                            *
 * Copyright 2020 Lukas Jünger                                                *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the "License");            *
 * you may not use this file except in compliance with the License.           *
 * You may obtain a copy of the License at                                    *
 *                                                                            *
 *     http://www.apache.org/licenses/LICENSE-2.0                             *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an "AS IS" BASIS,          *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 *                                                                            *
 ******************************************************************************/

#include "vcml.h"
#include "gtest/gtest.h"
#include "../include/nvdla.h"

#define EXPECT_OK(tlmcall) EXPECT_EQ(tlmcall, TLM_OK_RESPONSE)

using namespace ::vcml;
using namespace ::sc_core;

class test_harness : public component {
public:
    master_socket CONFIG_OUT;
    vcml::generic::memory SRAM;
    vcml::generic::memory RAM;

    in_port<bool> IRQ_IN;

    SC_HAS_PROCESS(test_harness);

    test_harness(const sc_module_name &nm) :
        component(nm),
        CONFIG_OUT("CONFIG_OUT"),
        SRAM("SRAM",0x10000000),
        RAM("RAM", 0x10000000),
        IRQ_IN("IRQ_IN") {
        SC_THREAD(run);
    }


    void run() {
        run_test();
        sc_stop();
    }

    void run_test() {
        // This can be used to enable NVDLA logging
        //sc_core::sc_report_handler::set_log_file_name("sc.log");
        //sc_core::sc_report_handler::set_verbosity_level(sc_core::SC_DEBUG);

        // write configuration data to dla
        struct dla_reg {
            uint32_t offset;
            uint32_t value;
        };

        struct dla_reg config_regs[] = {
            {0x4000, 0x01000000},   // CFG_SRC_ADDR_LOW
            {0x4004, 0},            // CFG_SRC_ADDR_HIGH
            {0x4008, 0x02000000},   // CFG_DST_ADDR_LOW
            {0x400c, 0},            // CFG_DST_ADDR_HIGH

            {0x4010, 7},     // CFG_LINE
            {0x4018, 7},     // CFG_LINE_REPEAT
            {0x401c, 0x100}, // CFG_SRC_LINE
            {0x4020, 0x100}, // CFG_DST_LINE
            {0x4024, 0},     // CFG_SURF_REPEAT
            {0x4028, 0x800}, // CFG_SRC_SURF
            {0x402c, 0x800}, // CFG_DST_SURF
            {0x4014, 3},     // CFG_CMD
            {0x4030, 1},     //CFG_OP
            {0x4034, 1},     //CFG_LAUNCH0
        };
        for (dla_reg &reg : config_regs) {
            EXPECT_OK(CONFIG_OUT.write(reg.offset, &reg.value, 4));
            sc_core::wait(sc_core::SC_ZERO_TIME);
        }
        u64 intr_status;
        EXPECT_OK(CONFIG_OUT.read(0xc, &intr_status, 4));
        EXPECT_EQ(intr_status, 0);
        EXPECT_FALSE(IRQ_IN.read());

        sc_core::wait(100, sc_core::SC_NS);

        EXPECT_OK(CONFIG_OUT.read(0xc, &intr_status, 4));
        EXPECT_EQ(intr_status>>6, 0x1);
        EXPECT_TRUE(IRQ_IN.read());
    }

};

TEST(nvdlaforvcml, nvdlaforvcml) {
    test_harness harness("HARNESS");
    xonx::nvdla nvdla("NVDLA");

    sc_signal<bool> irq_signal;

    harness.CLOCK.stub(100 * MHz);
    harness.RESET.stub();

    nvdla.CLOCK.stub(100 * MHz);
    nvdla.RESET.stub();

    harness.RAM.CLOCK.stub(100 * MHz);
    harness.RAM.RESET.stub();
    harness.SRAM.CLOCK.stub(100 * MHz);
    harness.SRAM.RESET.stub();

    harness.CONFIG_OUT.bind(nvdla.CONFIG_IF);
    nvdla.SRAM_IF.bind(harness.SRAM.IN);
    nvdla.DBB_IF.bind(harness.RAM.IN);

    harness.IRQ_IN.bind(irq_signal);
    nvdla.IRQ.bind(irq_signal);

    u32 test_data[0x200];
    for(u32 i=0; i < sizeof(test_data)/sizeof(test_data[0]); i++) {
        test_data[i] = 0xABCD0000 + i;
    }
    harness.RAM.write(vcml::range(0x01000000, 0x01000800), test_data, SBI_NONE);

    sc_start();

    u32 check_data[0x200];
    harness.RAM.read(vcml::range(0x02000000, 0x02000800), check_data, SBI_NONE);

   for(u32 i=0; i < sizeof(check_data)/sizeof(check_data[0]); i++) {
       ASSERT_EQ(check_data[i], test_data[i]);
    }

    ASSERT_EQ(sc_get_status(), SC_STOPPED);
}

extern "C" int sc_main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}