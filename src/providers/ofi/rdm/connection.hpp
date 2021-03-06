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

#ifndef pMR_PROVIDERS_OFI_RDM_CONNECTION_H
#define pMR_PROVIDERS_OFI_RDM_CONNECTION_H

#include <cstdint>
#include <config.hpp>
#include "../common/domain.hpp"
#include "../common/fabric.hpp"
#include "../common/memoryaddress.hpp"
#include "../common/memoryregion.hpp"
#include "softendpoint.hpp"

namespace pMR
{
    class Target;

    namespace OFI
    {
        class Connection
        {
        public:
            Connection(Target const &target, Info info);
            Connection(Connection const &) = delete;
            Connection(Connection &&) = delete;
            Connection &operator=(Connection const &) = delete;
            Connection &operator=(Connection &&) = delete;
            ~Connection() = default;

            Domain &getDomain();
            Domain const &getDomain() const;
            void checkMessageSize(std::size_t const size) const;

            void postSendToActive(
                MemoryRegion &memoryRegion, std::size_t const sizeByte);
            void postSendToPassive();
            void postRecvToActive();
            void postRecvToPassive(MemoryRegion &memoryRegion);
            void postRecvToPassive();

            void pollActiveSend();
            void pollActiveRecv();
            void pollPassiveSend();
            void pollPassiveRecv();

#ifdef OFI_RMA
            void setLocalTargetMemoryAddress(MemoryRegion const &);
#ifdef OFI_RMA_CONTROL
            void postWriteAddressToPassive();
#else
            void postSendAddressToPassive();
            void postRecvAddressToActive();
#endif // OFI_RMA_CONTROL
            void postWriteToActive(
                MemoryRegion &memoryRegion, std::size_t const sizeByte);
#endif // OFI_RMA

        private:
            Fabric mFabric;
            Domain mDomain;
            SoftEndpoint mActiveEndpoint;
            SoftEndpoint mPassiveEndpoint;
#ifdef OFI_RMA
            alignas(cAlignment) MemoryAddress mLocalTargetMemoryAddress;
            alignas(cAlignment) MemoryAddress mRemoteTargetMemoryAddress;
            MemoryRegion mLocalTargetMemoryRegion;
            MemoryRegion mRemoteTargetMemoryRegion;
#ifdef OFI_RMA_CONTROL
            MemoryAddress mRemoteMemoryAddress;
#ifdef OFI_RMA_EVENT
            Counter mCounter;
#endif // OFI_RMA_EVENT
#endif // OFI_RMA_CONTROL
#endif // OFI_RMA
        };
    }
}
#endif // pMR_PROVIDERS_OFI_RDM_CONNECTION_H
