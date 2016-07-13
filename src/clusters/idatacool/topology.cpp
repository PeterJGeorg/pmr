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

#include "connection.hpp"
#include "node.hpp"
#include "../../providers/verbs/connection.hpp"
#include "../../providers/verbs/topology.hpp"
#include "../../misc/singleton.hpp"
#include "../../backends/backend.hpp"

void pMR::Connection::connectVerbs(Target const &target)
{
    verbs::Devices devices;
    auto device = verbs::getIBAdapter(devices);

    mVerbs = std::make_shared<verbs::Connection>(target, device);
}

pMR::Provider pMR::Connection::detectProvider(Target const &target) const
{
    auto originNode = Singleton<Node>::Instance();
    decltype(originNode) targetNode;

    backend::exchange(target, originNode, targetNode);
    
    if(originNode.getHostID() == targetNode.getHostID())
    {
        return Provider::cma;
    }
    else
    {
        return Provider::verbs;
    }
}
