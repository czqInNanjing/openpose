SET(MY_SPINNAKER_LIBRARIES -Wl,-Bdynamic -lSpinnaker${D})
get_filename_component(SOURCE_NAME openpose_camera.cpp NAME_WE)
SET(GCC_COVERAGE_COMPILE_FLAGS "-I/usr/include/python2.7 -I/usr/include/x86_64-linux-gnu/python2.7  -fno-strict-aliasing -Wdate-time -D_FORTIFY_SOURCE=2 -g -fstack-protector-strong -Wformat -Werror=format-security  -DNDEBUG -g -fwrapv -O2 -Wall -Wstrict-prototypes")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS} ")
add_subdirectory(src)
SET(GCC_COVERAGE_LINK_FLAGS    "-L/usr/lib/python2.7/config-x86_64-linux-gnu -L/usr/lib -lpython2.7 -lpthread -ldl  -lutil -lm  -Xlinker -export-dynamic -Wl,-O1 -Wl,-Bsymbolic-functions")

get_filename_component(SOURCE_NAME openpose_camera.cpp NAME_WE)
message(STATUS "Adding Example ${SOURCE_NAME}")
add_executable(${SOURCE_NAME}.bin openpose_camera.cpp)
target_link_libraries( ${SOURCE_NAME}.bin
     openpose openpose_camera ${GLOG_LIBRARY} ${GFLAGS_LIBRARY} ${Caffe_LIBS}  ${MY_SPINNAKER_LIBRARIES} ${GCC_COVERAGE_LINK_FLAGS}
)

