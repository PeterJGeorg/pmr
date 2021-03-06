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

#include "hints.hpp"

pMR::OFI::Hints::Hints()
{
    mInfo = fi_allocinfo();

    auto info = mInfo;

#ifdef OFI_EP_MSG
    info->ep_attr->type = FI_EP_MSG;
#elif defined OFI_EP_RDM
    info->ep_attr->type = FI_EP_RDM;
#endif // OFI_EP

    info->mode |= FI_CONTEXT;
    info->mode |= FI_LOCAL_MR;

#ifdef OFI_RMA_TARGET_RX
    info->mode |= FI_RX_CQ_DATA;
#endif // OFI_RMA_TARGET_RX

    info->caps |= FI_SEND | FI_RECV;
#ifdef OFI_EP_MSG
    info->caps |= FI_MSG;
#endif // OFI_EP_MSG
#ifdef OFI_EP_RDM
    info->caps |= FI_TAGGED;
#endif // OFI_EP_RDM

#ifdef OFI_RDM_DIRECTED_RECEIVE
    info->caps |= FI_DIRECTED_RECV;
#endif // OFI_RDM_DIRECTED_RECEIVE

#ifdef OFI_RMA
    info->caps |= FI_RMA | FI_WRITE | FI_REMOTE_WRITE;
#endif // OFI_RMA

#ifdef OFI_RMA_EVENT
    info->caps |= FI_RMA_EVENT;
#endif // OFI_RMA_EVENT

#ifdef OFI_RDM_GLOBAL
#ifdef OFI_RDM_GLOBAL_THREADSAFE
    info->domain_attr->threading = FI_THREAD_SAFE;
#else
    info->domain_attr->threading = FI_THREAD_FID;
#endif // OFI_RDM_GLOBAL_THREADSAFE
#else
    info->domain_attr->threading = FI_THREAD_ENDPOINT;
#endif // OFI_RDM_GLOBAL

#ifdef OFI_CONTROL_PROGRESS_AUTO
    info->domain_attr->control_progress = FI_PROGRESS_AUTO;
#else
    info->domain_attr->control_progress = FI_PROGRESS_MANUAL;
#endif // OFI_CONTROL_PROGRESS_AUTO

#ifdef OFI_DATA_PROGRESS_AUTO
    info->domain_attr->data_progress = FI_PROGRESS_AUTO;
#else
    info->domain_attr->data_progress = FI_PROGRESS_MANUAL;
#endif // OFI_DATA_PROGRESS_AUTO

#ifdef OFI_RM
    info->domain_attr->resource_mgmt = FI_RM_ENABLED;
#else
    info->domain_attr->resource_mgmt = FI_RM_DISABLED;
#endif // OFI_RM

#ifdef OFI_MR_SCALABLE
    info->domain_attr->mr_mode = FI_MR_SCALABLE;
#else
    info->domain_attr->mr_mode = FI_MR_BASIC;
#endif // OFI_MR_SCALABLE
}

fi_info *pMR::OFI::Hints::get()
{
    return mInfo;
}

fi_info const *pMR::OFI::Hints::get() const
{
    return mInfo;
}

std::string pMR::OFI::Hints::getStruct() const
{
    return fi_tostr(mInfo, FI_TYPE_INFO);
}
