#!/bin/bash

PROJECT_ROOT=$(pwd)
BOOST_PATH="${PROJECT_ROOT}/boost"

PYTHON_INCLUDE_DIR=$(python3 -c "from distutils.sysconfig import get_python_inc; print(get_python_inc())")
PYTHON_LIBRARY=$(python3 -c "import distutils.sysconfig as sysconfig; import os; print(os.path.join(sysconfig.get_config_var('LIBDIR'), sysconfig.get_config_var('LDLIBRARY')))")
if [[ "${PYTHON_LIBRARY}" != *.so ]]
then
    echo "Wrong Python library, for pyenv try: env PYTHON_CONFIGURE_OPTS=\"--enable-shared\" pyenv install 3.x.x"
    echo "where x is your python version."
    exit 1
fi

# For an unknown reason it can happen that BOOST uses another compiler name in
# its library filenames as FindBoost and FindBoost can't find the BOOST
# libraries. As work-around the filename lookup is done here and feeded to
# FindBoost.
echo "*************************************************************************"
echo "* Looking for BOOST library files"
echo "*************************************************************************"
LIBBOOST=$(find ${BOOST_PATH}/lib -type f -name "libboost*" -print -quit)
COMPILER=$(echo ${LIBBOOST} | sed -r "s|.*(gcc[0-9]*).*|\1 |g")
if [ -z "${COMPILER}" ]
then
    echo "Couldn't find BOOST library files on path ${BOOST_PATH}"
    exit 1
else
    echo "Library files found. Compiled with compiler '${COMPILER}'"
fi

echo "*************************************************************************"
echo "* Generates Makefile - Release variant"
echo "*************************************************************************"
CMAKE_OPTIONS="-DCMAKE_BUILD_TYPE=Release"
CMAKE_OPTIONS+=" -DCMAKE_INSTALL_PREFIX=../install_release"
CMAKE_OPTIONS+=" -DENABLE_BUILD_EXAMPLES=On"
CMAKE_OPTIONS+=" -D64BIT_SUPPORT=ON"
CMAKE_OPTIONS+=" -DBOOST_ROOT=${BOOST_PATH}"
CMAKE_OPTIONS+=" -DBOOST_LIBRARYDIR=${BOOST_PATH}/lib"
CMAKE_OPTIONS+=" -DBOOST_INCLUDEDIR=${BOOST_PATH}/include/boost-1_65_1"
CMAKE_OPTIONS+=" -DBoost_COMPILER=-${COMPILER}"
CMAKE_OPTIONS+=" -DBOOST_VERSION=1.65.1"


CMAKE_OPTIONS+=" -DGLOG_INCLUDEDIR=${PROJECT_ROOT}/glog/include"
CMAKE_OPTIONS+=" -DGLOG_LIBRARYDIR=${PROJECT_ROOT}/glog/lib_x64_r"


mkdir -p build_gcc_release
cd build_gcc_release
mkdir -p lib
cd lib
mkdir -p include
cd include
touch build_info.hpp
cd ../../
cmake ${CMAKE_OPTIONS} ..
cd ${PROJECT_ROOT}

echo "*************************************************************************"
echo "* Generates Makefile - Debug variant"
echo "*************************************************************************"
CMAKE_OPTIONS="-DCMAKE_BUILD_TYPE=Debug"
CMAKE_OPTIONS+=" -DCMAKE_INSTALL_PREFIX=../install_debug"
CMAKE_OPTIONS+=" -DENABLE_BUILD_EXAMPLES=On"
CMAKE_OPTIONS+=" -D64BIT_SUPPORT=ON"
CMAKE_OPTIONS+=" -DBOOST_ROOT=${BOOST_PATH}"
CMAKE_OPTIONS+=" -DBOOST_LIBRARYDIR=${BOOST_PATH}/lib"
CMAKE_OPTIONS+=" -DBOOST_INCLUDEDIR=${BOOST_PATH}/include/boost-1_65_1"
CMAKE_OPTIONS+=" -DBoost_COMPILER=-${COMPILER}"
CMAKE_OPTIONS+=" -DBOOST_VERSION=1.65.1"

CMAKE_OPTIONS+=" -DGLOG_INCLUDEDIR=${PROJECT_ROOT}/glog/include"
CMAKE_OPTIONS+=" -DGLOG_LIBRARYDIR=${PROJECT_ROOT}/glog/lib_x64_d"

mkdir -p build_gcc_debug
cd build_gcc_debug
mkdir -p lib
cd lib
mkdir -p include
cd include
touch build_info.hpp
cd ../../
cmake ${CMAKE_OPTIONS} ..
cd ${PROJECT_ROOT}
