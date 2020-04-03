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

#ifndef NVDLA_VCML_H
#define NVDLA_VCML_H

#include "common.h"
#include "NV_nvdla.h"

namespace xonx {
class nvdla : public vcml::peripheral {
private:
    scsim::cmod::NV_nvdla *m_nvdla;
    vcml::bus_width_adapter<32,64> m_sram_adapter;
    vcml::bus_width_adapter<32,64> m_dbb_adapter;
    vcml::bus_width_adapter<64,32> m_config_adapter;
public:
    tlm::tlm_initiator_socket<64> SRAM_IF;
    tlm::tlm_initiator_socket<64> DBB_IF;
    tlm::tlm_target_socket<64>    CONFIG_IF;
    vcml::out_port<bool> IRQ;

    nvdla() = delete;
    nvdla(const nvdla&) = delete;

    nvdla(const sc_core::sc_module_name& nm);
    ~nvdla();
};
}
#endif //NVDLA_VCML_H
