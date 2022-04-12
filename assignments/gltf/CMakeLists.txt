add_executable(AGLTFSkin AGLTFSkin.cpp AGLTFGeometry.cpp)
target_link_libraries(AGLTFSkin ${LIBS})

add_executable(AGLTFSkin_Compare AGLTFSkin_Compare.cpp AGLTFGeometry.cpp)
target_link_libraries(AGLTFSkin_Compare ${LIBS})

add_executable(ASkeletonTest ASkeletonTest.cpp ASkeletonDrawer.cpp)
target_link_libraries(ASkeletonTest ${LIBS})

add_executable(AGLTFSimple AGLTFSimple.cpp ASkeletonDrawer.cpp AGLTFGeometry.cpp)
target_link_libraries(AGLTFSimple ${LIBS})

add_executable(AGLTFPipeLB AGLTFPipeLB.cpp ASkeletonDrawer.cpp AGLTFGeometry.cpp)
target_link_libraries(AGLTFPipeLB ${LIBS})

add_executable(AGLTFPipeDLB AGLTFPipeDLB.cpp ASkeletonDrawer.cpp AGLTFGeometry.cpp)
target_link_libraries(AGLTFPipeDLB ${LIBS})

add_executable(AGLTFPipe AGLTFPipe.cpp ASkeletonDrawer.cpp AGLTFGeometry.cpp)
target_link_libraries(AGLTFPipe ${LIBS})

add_executable(AGLTFThread AGLTFThread.cpp ASkeletonDrawer.cpp AGLTFGeometry.cpp)
target_link_libraries(AGLTFThread ${LIBS})

add_executable(AGLTFThread_Shader AGLTFThread_Shader.cpp ASkeletonDrawer.cpp AGLTFGeometry.cpp)
target_link_libraries(AGLTFThread_Shader ${LIBS})

add_executable(AGLTFThreadLB AGLTFThreadLB.cpp ASkeletonDrawer.cpp AGLTFGeometry.cpp)
target_link_libraries(AGLTFThreadLB ${LIBS})

add_executable(AGLTFThreadDLB AGLTFThreadDLB.cpp ASkeletonDrawer.cpp AGLTFGeometry.cpp)
target_link_libraries(AGLTFThreadDLB ${LIBS})

#add_executable(AGLTFHandLB AGLTFHandLB.cpp ASkeletonDrawer.cpp AGLTFGeometry.cpp)
#target_link_libraries(AGLTFHandLB ${LIBS})

#add_executable(AGLTFHandDLB AGLTFHandDLB.cpp ASkeletonDrawer.cpp AGLTFGeometry.cpp)
#target_link_libraries(AGLTFHandDLB ${LIBS})

add_executable(AGLTFMia AGLTFMia.cpp ASkeletonDrawer.cpp AGLTFGeometry.cpp)
target_link_libraries(AGLTFMia ${LIBS})
