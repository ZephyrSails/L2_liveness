# set C++ language standard
set (CMAKE_CXX_EXTENSIONS OFF)

# add warnings
if (MSVC)
  add_compile_options (/W4 /WX /utf-8)
else ()
  add_compile_options (-pedantic -Wall -Wextra -Wshadow -Werror)
endif ()

# add tests
file (GLOB testsources *.cpp)
foreach (testsourcefile ${testsources})
  get_filename_component (exename ${testsourcefile} NAME_WE)
  add_executable (${exename} ${testsourcefile})
  target_link_libraries (${exename} pegtl)
  if (ANDROID_NDK)
    add_test (NAME ${exename} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../.. COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/run-on-android.sh ${CMAKE_CURRENT_BINARY_DIR}/${exename})
  else ()
    add_test (NAME ${exename} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../.. COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${exename})
  endif ()
endforeach (testsourcefile)
