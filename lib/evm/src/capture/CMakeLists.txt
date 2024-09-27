target_sources(${PROJECT_NAME}
        PUBLIC
        ../../include/evm/capture/capture.hpp
        ../../include/evm/capture/selector.hpp
        ../../include/evm/capture/source.hpp
        ../../include/evm/capture/stabilizer.hpp
        ../../include/evm/capture/device_source.hpp

        PRIVATE
        capture.cpp
        selector.cpp
        source.cpp
        stabilizer.cpp
        device_source.cpp)