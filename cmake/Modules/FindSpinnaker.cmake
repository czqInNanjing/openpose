unset(Spinnaker_FOUND)
unset(Spinnaker_INCLUDE_DIRS)
unset(Spinnaker_LIBS)

set(Spinnaker_INCLUDE_DIRS /usr/include/spinnaker)

find_library(Spinnaker_LIBS NAMES Spinnaker
    HINTS
    /usr/lib)

if (Spinnaker_LIBS AND Spinnaker_INCLUDE_DIRS)
  set(Spinnaker_FOUND 1)
endif (Spinnaker_LIBS AND Spinnaker_INCLUDE_DIRS)
