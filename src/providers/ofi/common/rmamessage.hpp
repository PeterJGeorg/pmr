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

#ifndef pMR_PROVIDERS_OFI_COMMON_RMAMESSAGE_H
#define pMR_PROVIDERS_OFI_COMMON_RMAMESSAGE_H

#include <cstdint>
extern "C" {
#include <rdma/fi_rma.h>
}
#include "memoryaddress.hpp"
#include "memoryregion.hpp"

namespace pMR
{
    namespace ofi
    {
        class RMAMessage
        {
        public:
            RMAMessage(MemoryRegion &, MemoryAddress const &remoteMemoryAddress,
                fi_context *context, fi_addr_t address = 0);
            RMAMessage(MemoryRegion &, std::size_t const sizeByte,
                MemoryAddress const &remoteMemoryAddress, fi_context *context,
                fi_addr_t address = 0);
            ~RMAMessage() = default;
            fi_msg_rma *get();
            fi_msg_rma const *get() const;
            std::size_t getLength() const;

        private:
            struct iovec mIOV = {};
            struct fi_rma_iov mRMAIOV = {};
            fi_msg_rma mRMAMessage = {};
        };
    }
}
#endif // pMR_PROVIDERS_OFI_COMMON_RMAMESSAGE_H
