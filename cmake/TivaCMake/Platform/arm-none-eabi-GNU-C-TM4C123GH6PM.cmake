# Setup compile flags for a TM4C123GH6PM
# It is likely that other tiva microcontroller use similar flags, but it is currently the clearest approach to
# write out all the flags for each individual processor in a file named Generic-GNU-C-<Processor>.cmake
# Build a list of flags used for C and C++
# Note: the space in the string after each option is important since this forms the string of options and
# each option should be separated by a space. 
string(CONCAT
  MCU_FLAGS
  "-mthumb "                 # use thumb mode code
  "-mcpu=cortex-m4 "         # The CPU used by the microcontroller
  "-march=armv7e-m "         # The architecture to optimize for
  "-mfloat-abi=hard "        # use hardware floating point since the microcontroller has an FPU
  "-mfpu=fpv4-sp-d16 "       # the version of the floating point unit to use
  "-ffunction-sections "     # seperate function sections
  "-fdata-sections "         # separate data sections
  "-gdwarf-3 "               # Used for debugging symbol format
  "-gstrict-dwarf "          # Used for debugging symbol format
  )

# Implicitly, since compiler ID is GNU-C the C and CXX flags for gcc has been set.
# We need to augment them with our microcontroller-specific options
string(APPEND CMAKE_C_FLAGS_INIT ${MCU_FLAGS})
string(APPEND CMAKE_CXX_FLAGS_INIT ${MCU_FLAGS})
unset(MCU_FLAGS)

# We also need to add --specs=nosys.specs to configure how the newlib standard C library handles syscalls
string(APPEND CMAKE_EXE_LINKER_FLAGS_INIT "--specs=nosys.specs ") 
