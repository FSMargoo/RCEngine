cmake_minimum_required(VERSION 3.28)
project(RCEngine)

set(CMAKE_CXX_STANDARD 20)

add_compile_definitions(UNICODE _UNICODE)

include_directories(./)
include_directories(./thirdparty/vecmath)

set(RCEngineSource
        main.cpp
        include/RCEngine.h
        include/RCMap.h
        include/RCContext.h
        source/RCContext.cpp
        include/RCException.h
        include/RCRenderTarget.h
        source/RCRenderTarget.cpp
        include/RCColor.h
        source/RCColor.cpp
        include/RCVideoWindow.h
        source/RCVideoWindow.cpp
        include/RCTexture.h
        source/RCTexture.cpp
        source/RCMap.cpp
        include/RCScene.h
        source/RCScene.cpp
        include/RCCamera.h
        source/RCCamera.cpp
        include/RCRenderer.h
        source/RCRenderer.cpp
        include/RCInteractor.h
        source/RCInteractor.cpp
        include/RCSprite.h
        source/RCSprite.cpp
        thirdparty/MemoryPool/C-11/MemoryPool.tcc)

add_executable(RCEngine
        ${RCEngineSource}
)
add_library(RCEngineLib ${RCEngineSource})