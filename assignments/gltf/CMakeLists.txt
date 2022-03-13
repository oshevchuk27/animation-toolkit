add_executable(AGLTFSkin AGLTFSkin.cpp AGLTFGeometry.cpp)
target_link_libraries(AGLTFSkin ${LIBS})

add_executable(AGLTFSkin_Compare AGLTFSkin_Compare.cpp AGLTFGeometry.cpp)
target_link_libraries(AGLTFSkin_Compare ${LIBS})

add_executable(ASkeletonTest ASkeletonTest.cpp ASkeletonDrawer.cpp)
target_link_libraries(ASkeletonTest ${LIBS})

add_executable(AGLTFSimple AGLTFSimple.cpp ASkeletonDrawer.cpp AGLTFGeometry.cpp)
target_link_libraries(AGLTFSimple ${LIBS})
