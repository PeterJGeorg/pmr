list(APPEND OFI_COMMON
    common/addressvector.cpp
    common/completionqueue.cpp
    common/counter.cpp
    common/domain.cpp
    common/endpoint.cpp
    common/eventqueue.cpp
    common/fabric.cpp
    common/hints.cpp
    common/info.cpp
    common/memoryaddress.cpp
    common/memoryregion.cpp
    common/message.cpp
    common/passiveendpoint.cpp
    common/rma.cpp
    common/tagged.cpp
    recvmemorywindow.cpp
    sendmemorywindow.cpp)

list(APPEND OFI_MSG
    msg/connection.cpp
    msg/softendpoint.cpp)

list(APPEND OFI_RDM
    rdm/connection.cpp
    rdm/softendpoint.cpp)

list(APPEND OFI_GRDM
    grdm/connection.cpp
    grdm/globalendpoint.cpp
    grdm/softendpoint.cpp)

if(OFI.EP STREQUAL "MSG")
    add_library(PROVIDER_OFI OBJECT ${OFI_COMMON} ${OFI_MSG})
elseif(OFI.EP STREQUAL "RDM")
    if(OFI.RDM.GLOBAL)
        add_library(PROVIDER_OFI OBJECT ${OFI_COMMON} ${OFI_GRDM})
    else()
        add_library(PROVIDER_OFI OBJECT ${OFI_COMMON} ${OFI_RDM})
    endif()
endif()
