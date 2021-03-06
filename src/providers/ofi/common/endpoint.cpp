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

#include "endpoint.hpp"
#include <stdexcept>
extern "C" {
#include <rdma/fi_cm.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_errno.h>
}

pMR::OFI::Endpoint::Endpoint(Domain &domain, Info &info)
{
    if(fi_endpoint(domain.get(), info.get(), &mEndpoint, &mContext))
    {
        throw std::runtime_error("pMR: Unable to open endpoint.");
    }
}

pMR::OFI::Endpoint::~Endpoint()
{
    if(mEndpoint)
    {
        fi_close(&mEndpoint->fid);
    }
}

fid_ep *pMR::OFI::Endpoint::get()
{
    return mEndpoint;
}

fid_ep const *pMR::OFI::Endpoint::get() const
{
    return mEndpoint;
}

void pMR::OFI::Endpoint::bind(AddressVector &av)
{
    if(fi_ep_bind(mEndpoint, &av.get()->fid, 0))
    {
        throw std::runtime_error("pMR: Unable to bind AV to endpoint.");
    }
}

void pMR::OFI::Endpoint::bind(CompletionQueue &queue, std::uint64_t flags)
{
    if(fi_ep_bind(mEndpoint, &queue.get()->fid, {flags}))
    {
        throw std::runtime_error("pMR: Unable to bind CQ to endpoint.");
    }
}

void pMR::OFI::Endpoint::bind(EventQueue &queue)
{
    if(fi_ep_bind(mEndpoint, &queue.get()->fid, 0))
    {
        throw std::runtime_error("pMR: Unable to bind EQ to endpoint.");
    }
}

void pMR::OFI::Endpoint::enable()
{
    if(fi_enable(mEndpoint))
    {
        throw std::runtime_error("pMR: Unable to enable endpoint.");
    }
}

void pMR::OFI::Endpoint::connect(std::vector<std::uint8_t> const &address)
{
    if(fi_connect(mEndpoint, address.data(), NULL, 0))
    {
        throw std::runtime_error("pMR: Unable to connect to endpoint.");
    }
}

void pMR::OFI::Endpoint::accept()
{
    if(fi_accept(mEndpoint, NULL, 0))
    {
        throw std::runtime_error("pMR: Unable to accept connection.");
    }
}

std::vector<std::uint8_t> pMR::OFI::Endpoint::getAddress() const
{
    std::vector<std::uint8_t> address;
    std::size_t size = 0;

    if(fi_getname(&mEndpoint->fid, NULL, &size) != -FI_ETOOSMALL)
    {
        throw std::runtime_error("pMR: Unable to detect size of address.");
    }

    address.resize({size});

    if(fi_getname(&mEndpoint->fid, address.data(), &size))
    {
        throw std::runtime_error("pMR: Unable to get address.");
    }

    return address;
}

std::vector<std::uint8_t> pMR::OFI::Endpoint::getPeerAddress() const
{
    std::vector<std::uint8_t> address;
    std::size_t size = 0;

    if(fi_getpeer(mEndpoint, NULL, &size) != -FI_ETOOSMALL)
    {
        throw std::runtime_error("pMR: Unable to detect size of address.");
    }

    address.resize({size});

    if(fi_getpeer(mEndpoint, address.data(), &size))
    {
        throw std::runtime_error("pMR: Unable to get address.");
    }

    return address;
}

void pMR::OFI::Endpoint::setAddress(std::vector<std::uint8_t> address)
{
    if(fi_setname(&mEndpoint->fid, address.data(), {address.size()}))
    {
        throw std::runtime_error("pMR: Unable to set address.");
    }
}
