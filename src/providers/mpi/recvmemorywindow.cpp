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

#include "recvmemorywindow.hpp"
#include <stdexcept>
#include <config.hpp>
#include "../../backends/mpi/threadsupport.hpp"
#include "connection.hpp"

pMR::MPI::RecvMemoryWindow::RecvMemoryWindow(
    std::shared_ptr<Connection> const connection, void *buffer,
    int const sizeByte)
    : mConnection(connection), mBuffer(buffer), mSizeByte{sizeByte}
{
#ifdef MPI_PERSISTENT
    if(mConnection->getThreadLevel() >= ThreadLevel::Multiple ||
        cThreadLevel <= ThreadLevel::Serialized)
    {
        initRecv();
    }
    else
    {
        std::lock_guard<std::mutex> lock(Backend::ThreadSupport::Mutex);
        initRecv();
    }
#endif // MPI_PERSISTENT
}

pMR::MPI::RecvMemoryWindow::~RecvMemoryWindow()
{
    if(mConnection->getThreadLevel() >= ThreadLevel::Multiple ||
        cThreadLevel <= ThreadLevel::Serialized)
    {
        freeRequest();
    }
    else
    {
        std::lock_guard<std::mutex> lock(Backend::ThreadSupport::Mutex);
        freeRequest();
    }
}

void pMR::MPI::RecvMemoryWindow::init()
{
    if(mConnection->getThreadLevel() >= ThreadLevel::Multiple ||
        cThreadLevel <= ThreadLevel::Serialized)
    {
        recv();
    }
    else
    {
        std::lock_guard<std::mutex> lock(Backend::ThreadSupport::Mutex);
        recv();
    }
}

void pMR::MPI::RecvMemoryWindow::post()
{
}

void pMR::MPI::RecvMemoryWindow::wait()
{
    if(mConnection->getThreadLevel() >= ThreadLevel::Multiple ||
        cThreadLevel <= ThreadLevel::Serialized)
    {
        if(MPI_Wait(&mRequest, MPI_STATUS_IGNORE) != MPI_SUCCESS)
        {
            throw std::runtime_error("pMR: Unable to (wait) receive data.");
        }
    }
    else
    {
        int flag = static_cast<int>(false);
        while(!flag)
        {
            std::lock_guard<std::mutex> lock(Backend::ThreadSupport::Mutex);
            if(MPI_Test(&mRequest, &flag, MPI_STATUS_IGNORE) != MPI_SUCCESS)
            {
                throw std::runtime_error("pMR: Unable to (test) receive data.");
            }
        }
    }
}

#ifdef MPI_PERSISTENT
void pMR::MPI::RecvMemoryWindow::initRecv()
{
    if(MPI_Recv_init(mBuffer, {mSizeByte}, MPI_BYTE,
           {mConnection->getTargetRank()}, {mConnection->getRecvTag()},
           {mConnection->getCommunicator()}, &mRequest) != MPI_SUCCESS)
    {
        throw std::runtime_error("pMR: Unable to init receive data.");
    }
}
#endif // MPI_PERSISTENT

void pMR::MPI::RecvMemoryWindow::recv()
{
#ifdef MPI_PERSISTENT
    if(MPI_Start(&mRequest) != MPI_SUCCESS)
#else
    if(MPI_Irecv(mBuffer, {mSizeByte}, MPI_BYTE, {mConnection->getTargetRank()},
           {mConnection->getRecvTag()}, {mConnection->getCommunicator()},
           &mRequest) != MPI_SUCCESS)
#endif // MPI_PERSISTENT
    {
        throw std::runtime_error("pMR: Unable to receive data.");
    }
}

void pMR::MPI::RecvMemoryWindow::freeRequest()
{
    if(mRequest != MPI_REQUEST_NULL)
    {
        if(MPI_Request_free(&mRequest) != MPI_SUCCESS)
        {
            throw std::runtime_error("pMR: Unable to free MPI request.");
        }
    }
}
