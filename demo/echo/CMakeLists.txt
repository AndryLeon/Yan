add_executable(echoserver echoserver.cc; ${SOURCE_FILES})
target_link_libraries(echoserver Yan)

add_executable(echoclient echoclient.cc; ${SOURCE_FILES})
target_link_libraries(echoclient Yan)