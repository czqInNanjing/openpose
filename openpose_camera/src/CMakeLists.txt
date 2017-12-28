set(SOURCES
        openpose_producer.cpp
        postProcessing.cpp
        renderer.cpp
        )

add_library(openpose_camera ${SOURCES})

install(TARGETS openpose_camera
    EXPORT OpenPose
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/openpose)