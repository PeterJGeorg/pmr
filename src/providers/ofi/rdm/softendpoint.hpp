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

#ifndef pMR_PROVIDERS_OFI_RDM_SOFTENDPOINT_H
#define pMR_PROVIDERS_OFI_RDM_SOFTENDPOINT_H

#include <cstdint>
#include <vector>
extern "C" {
#include <rdma/fabric.h>
}
#include "../common/completionqueue.hpp"
#include "../common/endpoint.hpp"
#include "../common/memoryaddress.hpp"
#include "../common/memoryregion.hpp"
#include "../common/message.hpp"
#include "../common/rma.hpp"

namespace pMR
{
    namespace OFI
    {
        class SoftEndpoint
        {
        public:
            SoftEndpoint(Domain &domain, Info &info);
            SoftEndpoint(SoftEndpoint const &) = delete;
            SoftEndpoint(SoftEndpoint &&) = delete;
            SoftEndpoint &operator=(SoftEndpoint const &) = delete;
            SoftEndpoint &operator=(SoftEndpoint &&) = delete;
            ~SoftEndpoint() = default;

            std::vector<std::uint8_t> getAddress() const;
            void connect(std::vector<std::uint8_t> const &addr);

            fi_context *getSendContext();
            fi_context *getRecvContext();

            void postSend(MemoryRegion &memoryRegion);
            void postSend(
                MemoryRegion &memoryRegion, std::size_t const sizeByte);
            void postSend();
            void postSend(Message &message);
            void postRecv(MemoryRegion &memoryRegion);
            void postRecv();
            void postRecv(Message &message);

            void postWrite(MemoryRegion &memoryRegion, MemoryAddress &target);
            void postWrite(MemoryRegion &memoryRegion, MemoryAddress &target,
                std::size_t const sizeByte);
            void postWrite(RMA &message);

            void pollSend();
            void pollRecv();

        private:
            Endpoint mEndpoint;
            AddressVector mAddressVector;
            CompletionQueueContext mCompletionQueue;
            fi_context mSendContext = {};
            fi_context mRecvContext = {};
            fi_addr_t mPeerAddress = 0;
            std::size_t mInjectSize = 0;
            std::uint64_t checkInjectSize(std::size_t size) const;
        };
    }
}
#endif // pMR_PROVIDERS_OFI_RDM_SOFTENDPOINT_H
