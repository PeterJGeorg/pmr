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

#ifndef pMR_PROVIDERS_OFI_RDM_GLOBALENDPOINT_H
#define pMR_PROVIDERS_OFI_RDM_GLOBALENDPOINT_H

#include <cstdint>
#include <unordered_map>
#include <vector>
#include "thread.hpp"
#include "../ofi.hpp"
#include "../common/addressvector.hpp"
#include "../common/domain.hpp"
#include "../common/endpoint.hpp"
#include "../common/fabric.hpp"
#include "../common/info.hpp"

namespace pMR
{
    namespace ofi
    {
        class GlobalEndpoint
        {
        public:
            GlobalEndpoint(Info &info);
            GlobalEndpoint(GlobalEndpoint const &) = delete;
            GlobalEndpoint(GlobalEndpoint &&) = delete;
            GlobalEndpoint &operator=(GlobalEndpoint const &) = delete;
            GlobalEndpoint &operator=(GlobalEndpoint &&) = delete;
            ~GlobalEndpoint() = default;
            fid_ep *get();
            fid_ep const *get() const;
            Domain &getDomain();
            Domain const &getDomain() const;

            std::vector<std::uint8_t> getAddress() const;
            fi_addr_t addPeer(std::vector<std::uint8_t> const &addr);

            void bind(std::uint64_t const sendID, std::uint64_t const recvID);
            void unbind(std::uint64_t const sendID, std::uint64_t const recvID);

            void pollSend(std::uint64_t const iD);
            void pollRecv(std::uint64_t const iD);

            void checkMessageSize(std::size_t const size) const;
            std::uint64_t checkInjectSize(std::size_t size) const;
            bool checkEagerSize(std::size_t size) const;

        private:
            Fabric mFabric;
            Domain mDomain;
            Endpoint mEndpoint;
            AddressVector mAddressVector;

            CompletionQueueContext mSendCompletionQueue;
#if defined OFI_RMA && !defined OFI_RMA_EVENT
            CompletionQueueData mRecvCompletionQueue;
#else
            CompletionQueueContext mRecvCompletionQueue;
#endif // OFI_RMA && !OFI_RMA_EVENT
            thread::Mutex mSendCompletionQueueMutex;
            thread::Mutex mRecvCompletionQueueMutex;

            std::unordered_map<std::uint64_t, int> mSendCompletions;
            std::unordered_map<std::uint64_t, int> mRecvCompletions;
            thread::Mutex mSendCompletionsMutex;
            thread::Mutex mRecvCompletionsMutex;

            std::size_t mMaxSize = 0;
            std::size_t mInjectSize = 0;
            std::size_t mEagerSize = OFIRndvThreshold;

            void detectEagerSize();

            void bind(std::unordered_map<std::uint64_t, int> &map,
                std::uint64_t const iD);
            void unbind(std::unordered_map<std::uint64_t, int> &map,
                std::uint64_t const iD);

            bool checkCompletions(std::unordered_map<std::uint64_t, int> &map,
                std::uint64_t const iD);
            std::uint64_t retrieveCompletions(CompletionQueueContext &queue);
            std::uint64_t retrieveCompletions(CompletionQueueData &queue);

            template<typename T>
            void poll(T &queue, thread::Mutex &mutexQueue,
                std::unordered_map<std::uint64_t, int> &map,
                thread::Mutex &mutexMap, std::uint64_t const iD);
        };
    }
}

template<typename T>
void pMR::ofi::GlobalEndpoint::poll(T &queue, thread::Mutex &mutexQueue,
    std::unordered_map<std::uint64_t, int> &map, thread::Mutex &mutexMap,
    std::uint64_t const iD)
{
    {
        // Check whether completion already retrieved.
        thread::ScopedLock scopedLock(mutexMap);
        if(checkCompletions(map, iD))
        {
            return;
        }
    }
    {
        thread::ScopedLock scopedLock(mutexQueue);
        // For non serialized threading re-check retrieved completions.
        if(!thread::isSerialized())
        {
            thread::ScopedLock scopedLock(mutexMap);
            if(checkCompletions(map, iD))
            {
                return;
            }
        }

        while(true)
        {
            auto rID = decltype(iD){retrieveCompletions(queue)};

            if(rID == iD)
            {
                break;
            }

            {
                thread::ScopedLock scopedLock(mutexMap);
                try
                {
                    ++map.at({rID});
                }
                catch(std::exception const &e)
                {
                    throw std::runtime_error("pMR: Retrieved unknown ID.");
                }
            }
        }
    }
}
#endif // pMR_PROVIDERS_OFI_RDM_GLOBALENDPOINT_H
