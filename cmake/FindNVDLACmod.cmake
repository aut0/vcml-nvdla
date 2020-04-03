 ##############################################################################
 #                                                                            #
 # Copyright 2020 Lukas Jünger                                                #
 #                                                                            #
 # Licensed under the Apache License, Version 2.0 (the "License");            #
 # you may not use this file except in compliance with the License.           #
 # You may obtain a copy of the License at                                    #
 #                                                                            #
 #     http://www.apache.org/licenses/LICENSE-2.0                             #
 #                                                                            #
 # Unless required by applicable law or agreed to in writing, software        #
 # distributed under the License is distributed on an "AS IS" BASIS,          #
 # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   #
 # See the License for the specific language governing permissions and        #
 # limitations under the License.                                             #
 #                                                                            #
 ##############################################################################

find_path(NVDLA_CMOD_INCLUDE_DIR
    NAMES NV_nvdla.h
    HINTS deps/nvdla_hw/outdir/nv_full/cmod/release/include
)

find_library(NVDLA_CMOD_LIBRARY
    NAMES nvdla_cmod
    HINTS deps/nvdla_hw/outdir/nv_full/cmod/release/lib
)

find_file(NVDLA_HW_VERSION_FILE
    NAMES VERSION
    HINTS deps/nvdla_hw/
)

FILE(READ ${NVDLA_HW_VERSION_FILE} NVDLA_HW_VERSION)

set(NVDLA_CMOD_LIBRARIES ${NVDLA_CMOD_LIBRARY})
set(NVDLA_INCLUDE_DIRS ${NVDLA_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(NVDLACmod
    REQUIRED_VARS NVDLA_CMOD_LIBRARY NVDLA_CMOD_INCLUDE_DIR 
    VERSION_VAR NVDLA_HW_VERSION)

mark_as_advanced(NVDLA_CMOD_INCLUDE_DIR NVDLA_CMOD_LIBRARY)
