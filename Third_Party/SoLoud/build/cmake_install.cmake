# Install script for directory: C:/Users/toddy/Downloads/soloud20200207/contrib

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/SoLoud")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/toddy/Downloads/soloud20200207/build/Debug/soloud.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/toddy/Downloads/soloud20200207/build/Release/soloud.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/toddy/Downloads/soloud20200207/build/MinSizeRel/soloud.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/toddy/Downloads/soloud20200207/build/RelWithDebInfo/soloud.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/soloud" TYPE FILE FILES
    "C:/Users/toddy/Downloads/soloud20200207/contrib/../include/soloud.h"
    "C:/Users/toddy/Downloads/soloud20200207/contrib/../include/soloud_audiosource.h"
    "C:/Users/toddy/Downloads/soloud20200207/contrib/../include/soloud_bassboostfilter.h"
    "C:/Users/toddy/Downloads/soloud20200207/contrib/../include/soloud_biquadresonantfilter.h"
    "C:/Users/toddy/Downloads/soloud20200207/contrib/../include/soloud_bus.h"
    "C:/Users/toddy/Downloads/soloud20200207/contrib/../include/soloud_c.h"
    "C:/Users/toddy/Downloads/soloud20200207/contrib/../include/soloud_dcremovalfilter.h"
    "C:/Users/toddy/Downloads/soloud20200207/contrib/../include/soloud_echofilter.h"
    "C:/Users/toddy/Downloads/soloud20200207/contrib/../include/soloud_error.h"
    "C:/Users/toddy/Downloads/soloud20200207/contrib/../include/soloud_fader.h"
    "C:/Users/toddy/Downloads/soloud20200207/contrib/../include/soloud_fft.h"
    "C:/Users/toddy/Downloads/soloud20200207/contrib/../include/soloud_fftfilter.h"
    "C:/Users/toddy/Downloads/soloud20200207/contrib/../include/soloud_file.h"
    "C:/Users/toddy/Downloads/soloud20200207/contrib/../include/soloud_file_hack_off.h"
    "C:/Users/toddy/Downloads/soloud20200207/contrib/../include/soloud_file_hack_on.h"
    "C:/Users/toddy/Downloads/soloud20200207/contrib/../include/soloud_filter.h"
    "C:/Users/toddy/Downloads/soloud20200207/contrib/../include/soloud_flangerfilter.h"
    "C:/Users/toddy/Downloads/soloud20200207/contrib/../include/soloud_internal.h"
    "C:/Users/toddy/Downloads/soloud20200207/contrib/../include/soloud_lofifilter.h"
    "C:/Users/toddy/Downloads/soloud20200207/contrib/../include/soloud_monotone.h"
    "C:/Users/toddy/Downloads/soloud20200207/contrib/../include/soloud_openmpt.h"
    "C:/Users/toddy/Downloads/soloud20200207/contrib/../include/soloud_queue.h"
    "C:/Users/toddy/Downloads/soloud20200207/contrib/../include/soloud_robotizefilter.h"
    "C:/Users/toddy/Downloads/soloud20200207/contrib/../include/soloud_sfxr.h"
    "C:/Users/toddy/Downloads/soloud20200207/contrib/../include/soloud_speech.h"
    "C:/Users/toddy/Downloads/soloud20200207/contrib/../include/soloud_tedsid.h"
    "C:/Users/toddy/Downloads/soloud20200207/contrib/../include/soloud_thread.h"
    "C:/Users/toddy/Downloads/soloud20200207/contrib/../include/soloud_vic.h"
    "C:/Users/toddy/Downloads/soloud20200207/contrib/../include/soloud_vizsn.h"
    "C:/Users/toddy/Downloads/soloud20200207/contrib/../include/soloud_wav.h"
    "C:/Users/toddy/Downloads/soloud20200207/contrib/../include/soloud_waveshaperfilter.h"
    "C:/Users/toddy/Downloads/soloud20200207/contrib/../include/soloud_wavstream.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/soloud-config.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/soloud-config.cmake"
         "C:/Users/toddy/Downloads/soloud20200207/build/CMakeFiles/Export/cmake/soloud-config.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/soloud-config-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/soloud-config.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "C:/Users/toddy/Downloads/soloud20200207/build/CMakeFiles/Export/cmake/soloud-config.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "C:/Users/toddy/Downloads/soloud20200207/build/CMakeFiles/Export/cmake/soloud-config-debug.cmake")
  endif()
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "C:/Users/toddy/Downloads/soloud20200207/build/CMakeFiles/Export/cmake/soloud-config-minsizerel.cmake")
  endif()
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "C:/Users/toddy/Downloads/soloud20200207/build/CMakeFiles/Export/cmake/soloud-config-relwithdebinfo.cmake")
  endif()
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "C:/Users/toddy/Downloads/soloud20200207/build/CMakeFiles/Export/cmake/soloud-config-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "C:/Users/toddy/Downloads/soloud20200207/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
