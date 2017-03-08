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

#include "allreduce.hpp"

pMR::AllReduce::AllReduce(
    Communicator const &communicator, size_type const sizeByte)
    : mSizeByte{sizeByte}
    , mBuffer(mSizeByte)
#ifdef pMR_ALLREDUCE_MPI
    , mMPI(communicator, mBuffer.data(), mSizeByte)
#endif // pMR_ALLREDUCE_MPI
#ifdef pMR_ALLREDUCE_RECURSIVE_DOUBLING
    , mRecursiveDoubling(communicator, mBuffer.data(), mSizeByte)
#endif // pMR_ALLREDUCE_RECURSIVE_DOUBLING
{
    if(std::numeric_limits<size_type>::is_signed)
    {
        if(mSizeByte < 0)
        {
            throw std::logic_error("pMR: Negative message size.");
        }
    }
}

pMR::size_type pMR::AllReduce::size() const
{
    return {mSizeByte};
}
