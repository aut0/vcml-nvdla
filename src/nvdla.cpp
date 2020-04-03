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

#include "nvdla.h"

namespace xonx {

    nvdla::nvdla(const sc_core::sc_module_name &nm) :
        peripheral(nm),
        m_nvdla(nullptr),
        m_sram_adapter("SRAM_IF_BUS_ADAPTER"),
        m_dbb_adapter("DBB_IF_BUS_ADAPTER"),
        m_config_adapter("CONFIG_IF_BUS_ADAPTER"),
        SRAM_IF("NVDLA_SRAM_IF"),
        DBB_IF("NVDLA_DBB_IF"),
        CONFIG_IF("NVDLA_CONFIG_IF"),
        IRQ("NVDLA_IRQ")
    {
        m_nvdla = new scsim::cmod::NV_nvdla("nvdla_core");
        m_nvdla->nvdla_core2cvsram_axi4.bind(m_sram_adapter.IN);
        m_sram_adapter.OUT.bind(SRAM_IF);
        m_nvdla->nvdla_core2dbb_axi4.bind(m_dbb_adapter.IN);
        m_dbb_adapter.OUT.bind(DBB_IF);
        CONFIG_IF.bind(m_config_adapter.IN);
        m_config_adapter.OUT.bind(m_nvdla->nvdla_host_master_if);
        m_nvdla->nvdla_intr.bind(IRQ);
    }

    nvdla::~nvdla() {
        if (m_nvdla)
            delete m_nvdla;
    }
}