SET(MY_SPINNAKER_LIBRARIES -Wl,-Bdynamic -lSpinnaker${D})

add_subdirectory(src)


get_filename_component(SOURCE_NAME openpose_camera.cpp NAME_WE)
message(STATUS "Adding Example ${SOURCE_NAME}")
add_executable(${SOURCE_NAME}.bin openpose_camera.cpp)
target_link_libraries( ${SOURCE_NAME}.bin
     openpose openpose_camera ${GLOG_LIBRARY} ${GFLAGS_LIBRARY} ${Caffe_LIBS}  ${MY_SPINNAKER_LIBRARIES}
)
