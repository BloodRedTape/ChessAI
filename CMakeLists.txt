cmake_minimum_required(VERSION 3.10)

project(Sample)

add_subdirectory(libs/base)
add_subdirectory(libs/2d)

set(SAMPLE_SOURCES
    "sources/main.cpp"
    "sources/pixel_engine.cpp"
 "sources/chess.cpp" "sources/board.cpp" "sources/moves_builder.cpp")

set(SAMPLE_INCLUDE
    "./sources"
)

add_executable(Sample ${SAMPLE_SOURCES})
target_link_libraries(Sample StraitXBase StraitX2D)
target_include_directories(Sample
    PUBLIC ${SAMPLE_INCLUDE}
)

target_compile_features(Sample PRIVATE cxx_std_17)