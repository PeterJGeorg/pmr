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

#include "target.hpp"
#include "connection.hpp"
#include "sendwindow.hpp"
#include "recvwindow.hpp"
#include "../../misc/print.hpp"

extern "C"
{
#include "pmr.h"

    pMR_Connection* pMR_CreateConnection(MPI_Comm communicator, int targetRank,
            int uniqueSendID, int uniqueRecvID)
    {
        try
        {
            return reinterpret_cast<pMR_Connection*>(new pMR::Connection(
                        pMR::Target(communicator, targetRank,
                            uniqueSendID, uniqueRecvID)));
        }
        catch(const std::exception &e)
        {
            pMR::print(e.what());
            MPI_Abort(MPI_COMM_WORLD, 1);
            return nullptr;
        }
    }
    
    void pMR_DestroyConnection(pMR_Connection *connection)
    {
        try
        {
            delete reinterpret_cast<pMR::Connection*>(connection);
        }
        catch(const std::exception &e)
        {
            pMR::print(e.what());
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    pMR_SendWindow* pMR_CreateSendWindow(pMR_Connection *connection,
            void *buffer, uint32_t sizeByte)
    {
        try
        {
            return reinterpret_cast<pMR_SendWindow*>(
                    new pMR::SendWindow<unsigned char>(
                        *reinterpret_cast<pMR::Connection*>(connection),
                        static_cast<unsigned char*>(buffer), sizeByte));
        }
        catch(const std::exception &e)
        {
            pMR::print(e.what());
            MPI_Abort(MPI_COMM_WORLD, 1);
            return nullptr;
        }
    }

    void pMR_DestroySendWindow(pMR_SendWindow *sendWindow)
    {
        try
        {
            delete
                reinterpret_cast<pMR::SendWindow<unsigned char>*>(sendWindow);
        }
        catch(const std::exception &e)
        {
            pMR::print(e.what());
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    int pMR_IsSameSendWindow(pMR_SendWindow *sendWindow,
            void *buffer, uint32_t sizeByte)
    {
        try
        {
            return static_cast<int>(reinterpret_cast
                    <pMR::SendWindow<unsigned char>*>(sendWindow)
                    ->isSame(static_cast<unsigned char*>(buffer), sizeByte));
        }
        catch(const std::exception &e)
        {
            pMR::print(e.what());
            MPI_Abort(MPI_COMM_WORLD, 1);
            return 0;
        }
    }

    void pMR_InitSend(pMR_SendWindow *sendWindow)
    {
        try
        {
            return reinterpret_cast<pMR::SendWindow<unsigned char>*>(sendWindow)
                ->init();
        }
        catch(const std::exception &e)
        {
            pMR::print(e.what());
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    void pMR_PostSend(pMR_SendWindow *sendWindow)
    {
        try
        {
            return reinterpret_cast<pMR::SendWindow<unsigned char>*>(sendWindow)
                ->post();
        }
        catch(const std::exception &e)
        {
            pMR::print(e.what());
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    void pMR_WaitSend(pMR_SendWindow *sendWindow)
    {
        try
        {
            return reinterpret_cast<pMR::SendWindow<unsigned char>*>(sendWindow)
                ->wait();
        }
        catch(const std::exception &e)
        {
            pMR::print(e.what());
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    pMR_RecvWindow* pMR_CreateRecvWindow(pMR_Connection *connection,
            void *buffer, uint32_t sizeByte)
    {
        try
        {
            return reinterpret_cast<pMR_RecvWindow*>(
                    new pMR::RecvWindow<unsigned char>(
                        *reinterpret_cast<pMR::Connection*>(connection),
                        static_cast<unsigned char*>(buffer), sizeByte));
        }
        catch(const std::exception &e)
        {
            pMR::print(e.what());
            MPI_Abort(MPI_COMM_WORLD, 1);
            return nullptr;
        }
    }

    void pMR_DestroyRecvWindow(pMR_RecvWindow *recvWindow)
    {
        try
        {
            delete
                reinterpret_cast<pMR::RecvWindow<unsigned char>*>(recvWindow);
        }
        catch(const std::exception &e)
        {
            pMR::print(e.what());
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    int pMR_IsSameRecvWindow(pMR_RecvWindow *recvWindow,
            void *buffer, uint32_t sizeByte)
    {
        try
        {
            return static_cast<int>(reinterpret_cast
                    <pMR::RecvWindow<unsigned char>*>(recvWindow)
                    ->isSame(static_cast<unsigned char*>(buffer), sizeByte));
        }
        catch(const std::exception &e)
        {
            pMR::print(e.what());
            MPI_Abort(MPI_COMM_WORLD, 1);
            return 0;
        }
    }

    void pMR_InitRecv(pMR_RecvWindow *recvWindow)
    {
        try
        {
            return reinterpret_cast<pMR::RecvWindow<unsigned char>*>(recvWindow)
                ->init();
        }
        catch(const std::exception &e)
        {
            pMR::print(e.what());
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    void pMR_PostRecv(pMR_RecvWindow *recvWindow)
    {
        try
        {
            return reinterpret_cast<pMR::RecvWindow<unsigned char>*>(recvWindow)
                ->post();
        }
        catch(const std::exception &e)
        {
            pMR::print(e.what());
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    void pMR_WaitRecv(pMR_RecvWindow *recvWindow)
    {
        try
        {
            return reinterpret_cast<pMR::RecvWindow<unsigned char>*>(recvWindow)
                ->wait();
        }
        catch(const std::exception &e)
        {
            pMR::print(e.what());
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }
}
