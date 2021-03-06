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

#include "info.hpp"
#include <cstring>
#include <stdexcept>
extern "C" {
#include <rdma/fi_errno.h>
}
#include "../ofi.hpp"
#include "hints.hpp"

pMR::OFI::Info::Info(std::string const &address)
{
    int returnValue;
    if(!address.empty())
    {
        returnValue =
            fi_getinfo(FI_VERSION(cVersionMajor, cVersionMinor), address.data(),
                NULL, FI_SOURCE | FI_NUMERICHOST, Hints().get(), &mInfo);
    }
    else
    {
        returnValue = fi_getinfo(FI_VERSION(cVersionMajor, cVersionMinor),
            nullptr, NULL, 0, Hints().get(), &mInfo);
    }

    if(returnValue)
    {
        switch(returnValue)
        {
            case -FI_ENOMEM:
            {
                throw std::runtime_error(
                    "pMR: Unable to get fi_info. Insufficient memory.");
            }
            case -FI_ENODATA:
            {
                throw std::runtime_error(
                    "pMR: Unable to get fi_info. No provider found.");
            }
            default:
            {
                throw std::runtime_error("pMR: Unable to get fi_info.");
            }
        }
    }

    queryStruct();
}

pMR::OFI::Info::Info(fi_info *info) : mInfo(info)
{
    queryStruct();
}

// Copies only the currently set provider
pMR::OFI::Info::Info(Info const &other)
{
    mInfo = fi_dupinfo(other.get());

    queryStruct();
}

pMR::OFI::Info::Info(Info &&other)
    : mInfos(std::move(other.mInfos)), mProvider(std::move(other.mProvider))
{
    mInfo = other.mInfo;
    other.mInfo = nullptr;
}

pMR::OFI::Info::~Info()
{
    if(mInfo)
    {
        fi_freeinfo(mInfo);
    }
}

fi_info *pMR::OFI::Info::get()
{
    return mInfos.at({mProvider});
}

fi_info const *pMR::OFI::Info::get() const
{
    return mInfos.at({mProvider});
}

std::size_t pMR::OFI::Info::numProviders() const
{
    return {mInfos.size()};
}

std::size_t pMR::OFI::Info::getProvider() const
{
    return {mProvider};
}

void pMR::OFI::Info::setProvider(std::size_t num)
{
    if(num >= numProviders())
    {
        throw std::logic_error("pMR: Desired provider does not exist.");
    }

    mProvider = {num};
}

std::string pMR::OFI::Info::getName() const
{
    return getFabric()->name;
}

std::string pMR::OFI::Info::getProviderName() const
{
    return getFabric()->prov_name;
}

std::string pMR::OFI::Info::getProtocol() const
{
    return fi_tostr(mInfos.at(mProvider)->ep_attr, FI_TYPE_PROTOCOL);
}

std::string pMR::OFI::Info::getStruct() const
{
    return fi_tostr(mInfos.at(mProvider), FI_TYPE_INFO);
}

bool pMR::OFI::Info::checkProvider() const
{
#ifdef OFI_RMA_TARGET_RX
    if(!(mInfos.at(mProvider)->rx_attr->mode & FI_RX_CQ_DATA))
    {
        return {false};
    }
#endif // OFI_RMA_TARGET_RX

    return {true};
}

void pMR::OFI::Info::searchProvider(std::string const &providerName)
{
    do
    {
        if(checkProvider() && getProviderName() == providerName)
        {
            break;
        }

        operator++();
    } while(true);
}

fi_fabric_attr *pMR::OFI::Info::getFabric()
{
    return mInfos.at({mProvider})->fabric_attr;
}

fi_fabric_attr const *pMR::OFI::Info::getFabric() const
{
    return mInfos.at({mProvider})->fabric_attr;
}

std::size_t pMR::OFI::Info::injectSize() const
{
    return {mInfos.at({mProvider})->tx_attr->inject_size};
}

std::size_t pMR::OFI::Info::maxSize() const
{
    return {mInfos.at({mProvider})->ep_attr->max_msg_size};
}

std::size_t pMR::OFI::Info::getContextSize() const
{
    return {std::min(getTransmitContextSize(), getReceiveContextSize())};
}

std::size_t pMR::OFI::Info::getTransmitContextSize() const
{
    return {mInfos.at({mProvider})->tx_attr->size};
}

std::size_t pMR::OFI::Info::getReceiveContextSize() const
{
    return {mInfos.at({mProvider})->rx_attr->size};
}

void pMR::OFI::Info::setSourceAddress(std::vector<std::uint8_t> const &address)
{
    if(mInfos.at({mProvider})->src_addrlen != address.size())
    {
        std::free(mInfos.at({mProvider})->src_addr);
        mInfos.at({mProvider})->src_addr = std::calloc(1, {address.size()});
        mInfos.at({mProvider})->src_addrlen = {address.size()};
    }
    std::copy(address.begin(), address.end(),
        static_cast<std::uint8_t *>(mInfos.at({mProvider})->src_addr));
}

void pMR::OFI::Info::setDestinationAddress(
    std::vector<std::uint8_t> const &address)
{
    if(mInfos.at({mProvider})->dest_addrlen != address.size())
    {
        if(mInfos.at({mProvider})->dest_addr)
        {
            free(mInfos.at({mProvider})->dest_addr);
        }
        mInfos.at({mProvider})->dest_addr = calloc(1, {address.size()});
        mInfos.at({mProvider})->dest_addrlen = {address.size()};
    }

    std::copy(address.begin(), address.end(),
        static_cast<std::uint8_t *>(mInfos.at(mProvider)->dest_addr));
}

pMR::OFI::Info &pMR::OFI::Info::operator++()
{
    setProvider({getProvider() + 1});
    return *this;
}

pMR::OFI::Info &pMR::OFI::Info::operator--()
{
    setProvider({getProvider() - 1});
    return *this;
}

void pMR::OFI::Info::queryStruct()
{
    for(auto info = mInfo; info; info = info->next)
    {
        mInfos.push_back(info);
    }
}

pMR::OFI::Info pMR::OFI::getProvider(
    std::string const &providerName, std::string const &address)
{
    Info info(address);
    info.searchProvider(providerName);

    return info;
}
