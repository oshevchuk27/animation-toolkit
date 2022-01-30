set(LIBS ${CORE})
add_executable(linear_blend_skinning linear_blend_skinning.cpp)
target_link_libraries(linear_blend_skinning ${LIBS})
