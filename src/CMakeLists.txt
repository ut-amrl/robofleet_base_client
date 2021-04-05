target_sources(client
PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/WsClient.hpp
    ${CMAKE_CURRENT_LIST_DIR}/ClientNode.hpp
    ${CMAKE_CURRENT_LIST_DIR}/MessageScheduler.hpp
)
