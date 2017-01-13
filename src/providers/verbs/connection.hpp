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

#ifndef pMR_PROVIDERS_VERBS_CONNECTION_H
#define pMR_PROVIDERS_VERBS_CONNECTION_H

#include <cstdint>
extern "C" {
#include <infiniband/verbs.h>
}
#include "config.hpp"
#include "completionqueue.hpp"
#include "connectionaddress.hpp"
#include "context.hpp"
#include "memoryaddress.hpp"
#include "memoryregion.hpp"
#include "protectiondomain.hpp"
#include "queuepair.hpp"
#include "scattergather.hpp"

namespace pMR
{
    namespace verbs
    {
        class Connection
        {
        public:
            Connection(Target const &target, Device const &device,
                std::uint8_t const portNumber = 1);
            Connection(Connection const &) = delete;
            Connection(Connection &&) = delete;
            Connection &operator=(Connection const &) = delete;
            Connection &operator=(Connection &&) = delete;
            ~Connection() = default;
            Context &getContext();
            Context const &getContext() const;
            ProtectionDomain &getProtectionDomain();
            ProtectionDomain const &getProtectionDomain() const;

#ifdef VERBS_RDMA
            void setLocalMemoryAddress(MemoryRegion const &);
            void postRecvAddressToActive();
            void postSendAddressToPassive();
            void postRecvToPassive();
            void postWriteToActive(
                MemoryRegion const &memoryRegion, std::uint32_t const sizeByte);
#else
            void postRecvToActive();
            void postSendToPassive();
            void postRecvToPassive(MemoryRegion const &memoryRegion);
            void postSendToActive(
                MemoryRegion const &memoryRegion, std::uint32_t const sizeByte);
#endif // VERBS_RDMA

            void pollActiveCompletionQueue();
            void pollPassiveCompletionQueue();

        private:
            Context mContext;
            ProtectionDomain mProtectionDomain;
#ifdef VERBS_RDMA
            alignas(alignment) MemoryAddress mLocalMemoryAddress;
            alignas(alignment) MemoryAddress mRemoteMemoryAddress;
            MemoryRegion mSendLocalAddress;
            MemoryRegion mRecvRemoteAddress;
#endif // VERBS_RDMA
            CompletionQueue mActiveCompletionQueue;
            CompletionQueue mPassiveCompletionQueue;
            QueuePair mActiveQueuePair;
            QueuePair mPassiveQueuePair;

            void postSendRequest(QueuePair &queuePair,
                ScatterGatherElement &scatterGatherElement);
            void postRecvRequest(QueuePair &queuePair,
                ScatterGatherElement &scatterGatherElement);
#ifdef VERBS_RDMA
            void postWriteRequest(QueuePair &queuePair,
                ScatterGatherElement &scatterGatherElement);
#endif // VERBS_RDMA
        };
    }
}
#endif // pMR_PROVIDERS_VERBS_CONNECTION_H
