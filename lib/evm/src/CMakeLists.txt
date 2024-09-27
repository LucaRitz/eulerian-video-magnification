target_sources(${PROJECT_NAME}
        PUBLIC
        ../include/evm/evm.hpp
        ../include/evm/data.hpp
        ../include/evm/worker.hpp
        ../include/evm/color_converter.hpp

        PRIVATE
        evm.cpp
        color_converter.cpp)

add_subdirectory(capture)
add_subdirectory(evm)
