# CMakeList.txt : CMake project for sheet-hero, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(INCLUDE_DIR "include")
set(SOURCE_DIR "src")

set(SOURCE_FILES
    ${SOURCE_DIR}/sheet-hero.cpp
    ${SOURCE_DIR}/App.cpp
    ${SOURCE_DIR}/AppImgui.cpp
    ${SOURCE_DIR}/LineShape.cpp
    ${SOURCE_DIR}/Measure.cpp
    ${SOURCE_DIR}/MidiEngine.cpp
    ${SOURCE_DIR}/MusicalData.cpp
    ${SOURCE_DIR}/MusicalSymbol.cpp
    ${SOURCE_DIR}/Note.cpp
    ${SOURCE_DIR}/NoteGroup.cpp
    ${SOURCE_DIR}/Scale.cpp
    ${SOURCE_DIR}/SheetMusicSettings.cpp
    ${SOURCE_DIR}/Song.cpp
    ${SOURCE_DIR}/SongGenerator.cpp
    ${SOURCE_DIR}/SongPlayer.cpp
    ${SOURCE_DIR}/SongRenderer.cpp
    ${SOURCE_DIR}/TimeSignature.cpp
)
set(HEADER_FILES
    ${INCLUDE_DIR}/App.hpp
    ${INCLUDE_DIR}/GrandMeasure.hpp
    ${INCLUDE_DIR}/LineShape.hpp
    ${INCLUDE_DIR}/Measure.hpp
    ${INCLUDE_DIR}/MidiEngine.hpp
    ${INCLUDE_DIR}/MusicalData.hpp
    ${INCLUDE_DIR}/MusicalSymbol.hpp
    ${INCLUDE_DIR}/Note.hpp
    ${INCLUDE_DIR}/NoteGroup.hpp
    ${INCLUDE_DIR}/Scale.hpp
    ${INCLUDE_DIR}/SheetMusicSettings.hpp
    ${INCLUDE_DIR}/Song.hpp
    ${INCLUDE_DIR}/SongGenerator.hpp
    ${INCLUDE_DIR}/SongPlayer.hpp
    ${INCLUDE_DIR}/SongRenderer.hpp
    ${INCLUDE_DIR}/TimeSignature.hpp

    ${INCLUDE_DIR}/effolkronium/random.hpp
)

project (sheet-hero)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(SFML COMPONENTS system window graphics audio CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)
find_package(libremidi CONFIG REQUIRED)

# Add source to this project's executable.
add_executable (sheet-hero ${SOURCE_FILES} ${HEADER_FILES} )

target_include_directories(sheet-hero PRIVATE ${INCLUDE_DIR} ${INCLUDE_DIR}/effolkronium/)

target_link_libraries(sheet-hero PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(sheet-hero PRIVATE sfml-system sfml-network sfml-graphics sfml-window sfml-audio)
target_link_libraries(sheet-hero PRIVATE ImGui-SFML::ImGui-SFML)
target_link_libraries(sheet-hero PRIVATE libremidi)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET sheet-hero PROPERTY CXX_STANDARD 20)
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:${PROJECT_NAME}>/data)