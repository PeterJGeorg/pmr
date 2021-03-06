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

#ifndef pMR_SENDMEMORYWINDOW_H
#define pMR_SENDMEMORYWINDOW_H

#include <memory>
#include "config.hpp"
#include "provider.hpp"

namespace pMR
{
    class Connection;

    class SendMemoryWindow
    {
    public:
        SendMemoryWindow(
            Connection const &, void *buffer, size_type const sizeByte);
        SendMemoryWindow(SendMemoryWindow const &) = delete;
        SendMemoryWindow(SendMemoryWindow &&) = default;
        SendMemoryWindow &operator=(SendMemoryWindow const &) = delete;
        SendMemoryWindow &operator=(SendMemoryWindow &&) = default;
        ~SendMemoryWindow();
        void init();
        void post();
        void post(size_type const sizeByte);
        void wait();
        void *data();
        void const *data() const;
        size_type size() const;

    private:
        void *mBuffer;
        size_type const mSizeByte;
        Provider mProvider;

#ifdef pMR_PROVIDER_CMA
        std::unique_ptr<CMA::SendMemoryWindow, CMA::SendMemoryWindowDeleter>
            mCMA;
#endif // pMR_PROVIDER_CMA

#ifdef pMR_PROVIDER_MPI
        std::unique_ptr<MPI::SendMemoryWindow, MPI::SendMemoryWindowDeleter>
            mMPI;
#endif // pMR_PROVIDER_MPI

#ifdef pMR_PROVIDER_NULL
        std::unique_ptr<Null::SendMemoryWindow, Null::SendMemoryWindowDeleter>
            mNull;
#endif // pMR_PROVIDER_NULL

#ifdef pMR_PROVIDER_OFI
        std::unique_ptr<OFI::SendMemoryWindow, OFI::SendMemoryWindowDeleter>
            mOFI;
#endif // pMR_PROVIDER_OFI

#ifdef pMR_PROVIDER_PSM2
        std::unique_ptr<PSM2::SendMemoryWindow, PSM2::SendMemoryWindowDeleter>
            mPSM2;
#endif // pMR_PROVIDER_PSM2

#ifdef pMR_PROVIDER_SCIF
        std::unique_ptr<SCIF::SendMemoryWindow, SCIF::SendMemoryWindowDeleter>
            mSCIF;
#endif // pMR_PROVIDER_SCIF

#ifdef pMR_PROVIDER_SELF
        std::unique_ptr<Self::SendMemoryWindow, Self::SendMemoryWindowDeleter>
            mSelf;
#endif // pMR_PROVIDER_SELF

#ifdef pMR_PROVIDER_VERBS
        std::unique_ptr<Verbs::SendMemoryWindow, Verbs::SendMemoryWindowDeleter>
            mVerbs;
#endif // pMR_PROVIDER_VERBS
    };
}
#endif // pMR_SENDMEMORYWINDOW_H
