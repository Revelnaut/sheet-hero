# CMakeList.txt : CMake project for sheet-hero, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("sheet-hero")

find_package(nlohmann_json CONFIG REQUIRED)
find_package(SFML COMPONENTS system window graphics audio CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)

#include_directories(${SFML_INCLUDE_DIR})

# Add source to this project's executable.
add_executable (sheet-hero "sheet-hero.cpp" "Game.cpp"  "Scale.cpp" "Note.cpp" "Song.cpp" "Measure.cpp" "NoteSet.cpp" "TimeSignature.cpp")

target_link_libraries(sheet-hero PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(sheet-hero PRIVATE sfml-system sfml-network sfml-graphics sfml-window sfml-audio)
target_link_libraries(sheet-hero PRIVATE ImGui-SFML::ImGui-SFML)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET sheet-hero PROPERTY CXX_STANDARD 20)
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:${PROJECT_NAME}>/data)