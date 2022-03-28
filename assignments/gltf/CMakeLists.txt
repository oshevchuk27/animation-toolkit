add_executable(AGLTFSkin AGLTFSkin.cpp AGLTFGeometry.cpp)
target_link_libraries(AGLTFSkin ${LIBS})

add_executable(AGLTFSkin_Compare AGLTFSkin_Compare.cpp AGLTFGeometry.cpp)
target_link_libraries(AGLTFSkin_Compare ${LIBS})

add_executable(ASkeletonTest ASkeletonTest.cpp ASkeletonDrawer.cpp)
target_link_libraries(ASkeletonTest ${LIBS})

add_executable(AGLTFSimple AGLTFSimple.cpp ASkeletonDrawer.cpp AGLTFGeometry.cpp)
target_link_libraries(AGLTFSimple ${LIBS})

add_executable(AGLTFPipe AGLTFPipe.cpp ASkeletonDrawer.cpp AGLTFGeometry.cpp)
target_link_libraries(AGLTFPipe ${LIBS})

add_executable(AGLTFThread AGLTFThread.cpp ASkeletonDrawer.cpp AGLTFGeometry.cpp)
target_link_libraries(AGLTFThread ${LIBS})

add_executable(AGLTFThread_Shader AGLTFThread_Shader.cpp ASkeletonDrawer.cpp AGLTFGeometry.cpp)
target_link_libraries(AGLTFThread_Shader ${LIBS})

add_executable(AGLTFHand AGLTFHand.cpp ASkeletonDrawer.cpp AGLTFGeometry.cpp)
target_link_libraries(AGLTFHand ${LIBS})

add_executable(AGLTFMia AGLTFMia.cpp ASkeletonDrawer.cpp AGLTFGeometry.cpp)
target_link_libraries(AGLTFMia ${LIBS})
