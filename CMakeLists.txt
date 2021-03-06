if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source build disabled")
endif()

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(color)

cmake_minimum_required(VERSION 2.8.11)
if(POLICY CMP0054)
    cmake_policy(SET CMP0054 NEW)
endif()

include(config)

project(pMR)

set(CLUSTER "" CACHE STRING "Cluster to build for.")
include(cluster)

set(BACKEND MPI CACHE STRING "Backend to use.")
set(THREAD Serialized CACHE STRING "Threading model to use.")
set(CAPI OFF CACHE BOOL "Build C API.")
set(MIC OFF CACHE BOOL "Build for for first generation Intel Xeon Phi.")
set(PROFILING OFF CACHE BOOL "Enable profiling.")

set(ALIGNMENT 64 CACHE STRING "Set preferred memory alignment.")
set(PADDING 1 CACHE STRING "Set preferred memory allocation padding.")
set(CACHELINESIZE 64 CACHE STRING "Set cache line size.")
set(WARN.ZERO ON CACHE BOOL "Warn when using zero sized windows.")


set(MPI.PERSISTENT OFF CACHE BOOL "Enable persistent communication.")
set(MPI.TAG.NARROW OFF CACHE BOOL "Enable narrow MPI tags to range [0,max(uint16_t))")

set(NULL.WARN ON CACHE BOOL "Warn when using provider null.")

set(OFI.EP RDM CACHE STRING "Specify endpoint type (MSG | RDM).")
set(OFI.RMA OFF CACHE BOOL "Use RMA capability.")
set(OFI.RMA.EVENT OFF CACHE BOOL "Use RMA Events for synchronization.")
set(OFI.RMA.EVENT.NONZERO ON CACHE BOOL "RMA Events require non-zero size receive memory region.")
set(OFI.RMA.TARGET.RX OFF CACHE BOOL "RMA with immediate data consumes receive buffer.")
set(OFI.RMA.CONTROL OFF CACHE BOOL "Also use RMA capability for control messages.")
set(OFI.POLL.SPIN ON CACHE BOOL "Use spin method to poll for completion/counter.")
set(OFI.CONTROL.PROGRESS.AUTO OFF CACHE BOOL "Automatic progess of control.")
set(OFI.DATA.PROGRESS.AUTO OFF CACHE BOOL "Automatic progess of data.")
set(OFI.RM OFF CACHE BOOL "Enable Resource Management.")
set(OFI.MR.SCALABLE ON CACHE BOOL "Scalable Memory Registration Mode.")
set(OFI.RDM.DIRECTED.RECEIVE OFF CACHE BOOL "Enable directed receive capability for RDM endpoints.")
set(OFI.RDM.GLOBAL ON CACHE BOOL "Use one global endpoint for RDM.")
set(OFI.RDM.GLOBAL.THREADSAFE OFF CACHE BOOL "Require the provider to be thread safe.")
set(OFI.NO.CONTROL ON CACHE BOOL "Disable any control mechanism by pMR, rely on provider to manage all resources. Only works for non-RMA transfers.")

set(PSM2.THREAD.SAFE OFF CACHE STRING "Pretend PSM2 to be thread safe (experimental).")
set(PSM2.WRAPPER OFF CACHE STRING "Compile PSM2 wrapper for LD_PRELOAD to allow pMR use PSM2 next to OFI (experimental).")
set(PSM2.WRAPPER.BLOCK OFF CACHE STRING "Block others from calling peek while data operations are in progress. Otherwise hide own requests in peek.")
set(PSM2.WAIT.BLOCK OFF CACHE STRING "Using blocking wait instead of polling and testing.")
set(PSM2.PRINT.STATS OFF CACHE STRING "Print PSM2 Matched Queue stats.")
set(PSM2.EAGER.THRESHOLD 64000 CACHE STRING "Set eager rendezvous threshold. For messages below threshold PSM2 decides which protocol to use.")

set(SELF.WARN ON CACHE BOOL "Warn when using provider self.")

set(VERBS.RDMA ON CACHE BOOL "Use RDMA capability.")
set(VERBS.RDMA.CONTROL OFF CACHE BOOL "Also use RDMA capability for control messages.")
set(VERBS.MTU 1024 CACHE STRING "Set MTU size.")
set(VERBS.INLINE 128 CACHE STRING "Set max inline data size.")
set(VERBS.ODP OFF CACHE BOOL "Use ODP capability.")


set(RECURSIVE.DOUBLING.BLOCKING 1 CACHE STRING "Set Recursive Doubling blocking factor.")
set(RECURSIVE.DOUBLING.DOMAIN.SIZE 1 CACHE STRING "Set Recursive Doubling pre reduction domain size.")
set(RECURSIVE.DOUBLING.FBT OFF CACHE BOOL "Enable optimization for 1D FBT with four blocks.")


set(QPACE2.WARN.TOPOLOGY ON CACHE BOOL "Warning about misuse of topology.")
set(QPACE3.PSM2 OFF CACHE BOOL "Use PSM2 directly instead of OFI.")

add_subdirectory(src)
add_subdirectory(doxygen)
