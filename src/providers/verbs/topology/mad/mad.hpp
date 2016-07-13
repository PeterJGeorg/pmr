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

#ifndef pMR_PROVIDERS_VERBS_TOPOLOGY_MAD_MAD_H
#define pMR_PROVIDERS_VERBS_TOPOLOGY_MAD_MAD_H

#include <cstdint>
#include <array>
extern "C"
{
#include <infiniband/verbs.h>
}
#include "../../context.hpp"
#include "../../portattributes.hpp"
#include "../../protectiondomain.hpp"
#include "../../completionqueue.hpp"
#include "../../memoryregion.hpp"
#include "../../verbs.hpp"
#include "queuepair.hpp"

namespace pMR { namespace verbs { namespace mad
{
    class MAD
    {
        public:
            MAD(Context&, std::uint8_t const portNumber);
            MAD(const MAD&) = delete;
            MAD(MAD&&) = delete;
            MAD& operator=(const MAD&) = delete;
            MAD& operator=(MAD&&) = delete;
            ~MAD() = default;
            void postRecvRequest();
            void postSendRequest();
            void query();
        protected:
            std::array<std::uint8_t, VerbsMADBlockSize> mSendMAD{};
            std::array<std::uint8_t, VerbsMADBlockGRHSize> mRecvMAD{};
            std::uint8_t const mPortNumber;
            PortAttributes mPortAttributes;
        private:
            ProtectionDomain mProtectionDomain;
            CompletionQueue mSendCompletionQueue;
            CompletionQueue mRecvCompletionQueue;
            QueuePair mQueuePair;
            MemoryRegion mRecvMemoryRegion;
    };
}}}
#endif // pMR_PROVIDERS_VERBS_TOPOLOGY_MAD_MAD_H
