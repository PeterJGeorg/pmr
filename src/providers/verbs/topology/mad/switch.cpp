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

#include "switch.hpp"
#include "../../../../misc/random.hpp"

pMR::Verbs::MAD::SwitchLID::SwitchLID(
    Context &context, std::uint8_t const portNumber)
    : MAD(context, {portNumber})
{
    auto transactionID = getRandomNumber<std::uint32_t>();
    // SA Formate (15.2.1.1) (0-55)
    // Base Version 1Byte
    std::get<0>(mSendMAD) = 0x01;
    // Management Class 1Byte
    std::get<1>(mSendMAD) = 0x03;
    // Class Version 1Byte
    std::get<2>(mSendMAD) = 0x02;
    // Method 1Byte
    std::get<3>(mSendMAD) = 0x01;
    // Status 2Byte
    // Class specific 2Byte
    // Transaction ID 8Byte
    std::get<12>(mSendMAD) = (transactionID & 0xFF000000) >> 24;
    std::get<13>(mSendMAD) = (transactionID & 0x00FF0000) >> 16;
    std::get<14>(mSendMAD) = (transactionID & 0x0000FF00) >> 8;
    std::get<15>(mSendMAD) = (transactionID & 0x000000FF);
    // Attribute ID 2Byte
    std::get<16>(mSendMAD) = 0x00;
    std::get<17>(mSendMAD) = 0x20;
    // Reserved 144 2Byte
    // Attribute Modifier 4Byte
    // RMPP 4Byte
    // DATA1 4Byte
    // DATA2 4Byte
    // SMKey 8Byte
    // Attribute Offset 2Byte
    // Reserved 368 2Byte
    // Component Mask 8Byte
    std::get<55>(mSendMAD) = 0x03;

    // LinkRecord (15.2.5.13) (56-63)
    // Source LID 2Byte
    std::get<56>(mSendMAD) = (mPortAttributes.getLID() & 0xFF00) >> 8;
    std::get<57>(mSendMAD) = (mPortAttributes.getLID() & 0x00FF);
    // Source Port 1Byte
    std::get<58>(mSendMAD) = portNumber;
    // Destination Port 1Byte
    // Destination LID 2Byte
    // Reserved 48 2Byte
}

bool pMR::Verbs::MAD::SwitchLID::validate()
{
    std::uint32_t status = std::get<cGRHSize + 4>(mRecvMAD) << 24 |
        std::get<cGRHSize + 5>(mRecvMAD) << 16 |
        std::get<cGRHSize + 6>(mRecvMAD) << 8 |
        std::get<cGRHSize + 75>(mRecvMAD);
    if(status)
    {
        return false;
    }

    std::uint8_t method = std::get<cGRHSize + 3>(mRecvMAD) & 0x0F;
    if(method != 0x01)
    {
        return false;
    }

    std::uint16_t offset = std::get<cGRHSize + 44>(mRecvMAD) << 8 |
        std::get<cGRHSize + 45>(mRecvMAD);
    if(!offset)
    {
        return false;
    }

    if(!std::equal(mRecvMAD.begin() + cGRHSize + 12,
           mRecvMAD.begin() + cGRHSize + 12 + 4, mSendMAD.begin() + 12))
    {
        return false;
    }
    return true;
}

int pMR::Verbs::MAD::SwitchLID::getSwitchLID()
{
    do
    {
        query();
    } while(!validate());

    return (std::get<cGRHSize + 60>(mRecvMAD) << 8 |
        std::get<cGRHSize + 61>(mRecvMAD));
}
