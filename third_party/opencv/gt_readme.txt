[VERSION]
2.4.6.1

[HOW TO BUILD]


(1) mkdir release
(2) cd release
(3) cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D BUILD_PYTHON_SUPPORT=ON ..

=========================================================================
#if you encounter a problem about freak.cpp
#them go to release/modules//features2d/CMakeFiles/opencv_features2d.dir/ to edit build.make
#Find freak.cpp in Building CXX object XXX block and add "-O0" to $(CXX_DEFINES) $(CXX_FLAGS) 
=========================================================================

(4) make

(5) make install