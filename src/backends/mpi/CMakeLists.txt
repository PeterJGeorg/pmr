add_library(BACKEND OBJECT
    communicator.cpp
    exchange.cpp
    local.cpp
    sync.cpp
    target.cpp
    threadsupport.cpp)

install(FILES
    communicator.hpp
    target.hpp DESTINATION include/pmr)
