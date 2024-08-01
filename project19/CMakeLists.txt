cmake_minimum_required (VERSION 3.8)

# include_directories(C:\\VulkanSDK\\1.3.204.1\\Third-Party\\Include\\glm)

# include_directories(${CMAKE_GLM_INCLUDE})

#include_directories($ENV{VULKAN_SDK}\\Third-Party\\Include\\glm)

project (h265_nal)

AUX_SOURCE_DIRECTORY(. DIR_SRCS)

add_executable(h265_nal ${DIR_SRCS})