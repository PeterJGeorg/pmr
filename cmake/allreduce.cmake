if(ALLREDUCE.MPI)
    set(pMR_ALLREDUCE_MPI ALLREDUCE.MPI)
    if(NOT ${BACKEND} STREQUAL "MPI")
        ERROR("MPI provider only available for backend MPI!")
    endif()

    add_subdirectory(allreduce/mpi)
    list(APPEND ALLREDUCE $<TARGET_OBJECTS:ALLREDUCE_MPI>)
    set(ALLREDUCE_STRING "${ALLREDUCE_STRING} MPI")
endif()

if(ALLREDUCE.RECURSIVE.DOUBLING)
    set(pMR_ALLREDUCE_RECURSIVE_DOUBLING ALLREDUCE.RECURSIVE.DOUBLING)

    set(RECURSIVE_DOUBLING_CAPS "${RECURSIVE_DOUBLING_CAPS} Blocking=${RECURSIVE.DOUBLING.BLOCKING}")
    add_definitions(-DRECURSIVE_DOUBLING_BLOCKING=${RECURSIVE.DOUBLING.BLOCKING})

    set(RECURSIVE_DOUBLING_CAPS "${RECURSIVE_DOUBLING_CAPS} DomainSize=${RECURSIVE.DOUBLING.DOMAIN.SIZE}")
    add_definitions(-DRECURSIVE_DOUBLING_DOMAIN_SIZE=${RECURSIVE.DOUBLING.DOMAIN.SIZE})

    if(RECURSIVE.DOUBLING.FBT)
        set(RECURSIVE_DOUBLING_CAPS "${RECURSIVE_DOUBLING_CAPS} FBT")
        add_definitions(-DRECURSIVE_DOUBLING_FBT)
    endif()

    add_subdirectory(allreduce/recursivedoubling)
    list(APPEND ALLREDUCE $<TARGET_OBJECTS:ALLREDUCE_RECURSIVE_DOUBLING>)
    set(ALLREDUCE_STRING "${ALLREDUCE_STRING} RecursiveDoubling")
    INFO("RecursiveDoubling options:${RECURSIVE_DOUBLING_CAPS}")
endif()

INFO("Enabled AllReduce algorithm(s):${ALLREDUCE_STRING}")
