//  Copyright 2016 Peter Georg
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.

#include "units.hpp"
#include <stdexcept>
extern "C" {
#include <psm2.h>
}
#include "error.hpp"

std::uint32_t pMR::PSM2::getNumberOfUnits()
{
    std::uint32_t devices;

    auto err = psm2_ep_num_devunits(&devices);
    if(err)
    {
        throw std::runtime_error(
            "pMR: Unable to get number of devices: " + toString(err));
    }

    return {devices};
}
