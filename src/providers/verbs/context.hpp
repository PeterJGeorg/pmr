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

#ifndef pMR_PROVIDERS_VERBS_CONTEXT_H
#define pMR_PROVIDERS_VERBS_CONTEXT_H

extern "C"
{
#include <infiniband/verbs.h>
}
#include "device.hpp"

namespace pMR { namespace verbs
{
    class Context
    {
        public:
            Context(Device const&);
            Context(const Context&) = delete;
            Context(Context&&) = delete;
            Context& operator=(const Context&) = delete;
            Context& operator=(Context&&) = delete;
            ~Context() noexcept;
            ibv_context* get();
            ibv_context const* get() const;
        private:
            ibv_context *mContext = nullptr;
    };
}}
#endif // pMR_PROVIDERS_VERBS_PROTECTIONDOMAIN_H
