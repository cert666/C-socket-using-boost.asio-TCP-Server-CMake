#!/bin/bash

# Path 
PRJROOT=$(pwd)
INSTALL_PATH="${PRJROOT}/boost"
BOOST_INSTALL_PATH="${PRJROOT}/boost"
GLOG_PATH="${PRJROOT}/glog"

BOOTSTRAP_BOOST="Yes"
BOOTSTRAP_GLOG="Yes"

echo ------------------------------------------------------
echo Bootstrapper script BOOST and GLOG
echo this will usually take more than 20 minutes
echo ... so take a big cup of coffee and some be patient
echo ""
echo "Bootstrapping BOOST: ${BOOTSTRAP_BOOST}"
echo "Bootstrapping GLOG: ${BOOTSTRAP_GLOG}"
echo ""
echo "Press <ENTER> to start"
read x
echo ------------------------------------------------------

###############################################################################
### TRY function
###
### Tries to execute statement and interrupts script if statement returns
### non-zero exit code
###############################################################################
function try {
    $*
    local exitcode=$?
    if [ ${exitcode} -ne 0 ]
    then
        echo "ERROR: '$1' returned exit code: ${exitcode}"
        exit $?
    fi
}

###############################################################################
###   BOOST
###############################################################################
if [[ "${BOOTSTRAP_BOOST}" == "Yes" ]]
then
    path_pyenv=$(which pyenv)
    if [[ -x "${path_pyenv}" ]]
    then
        pyenv_path=$(pyenv which python )
        pyenv_path="$(echo ${pyenv_path} | sed 's/bin\/python//g')"
        pyconfig=$(find ${pyenv_path} -type f -name "pyconfig.h" -print -quit)
        echo "pyconfig path: ${pyconfig}"
    else
        pyconfig=$(find /usr/include -type f -name "pyconfig.h" -print -quit)
    fi

    if [[ "${pyconfig}" != '' ]]
    then
        export CPLUS_INCLUDE_PATH="${pyconfig%/pyconfig.h}"
    else
        echo "Unable to find pyconfig.h. Install Python development packages."
        echo ""
        echo "For example: sudo apt install python3.5-dev libpython3.5-dev"
        echo ""
        exit 1
    fi

    cd boost
    mkdir -p tmp
    mkdir -p license
    try unzip boost_1_65_1.zip -d tmp
    cd tmp/boost_1_65_1
    cp -rv ./LICENSE_1_0.txt ${BOOST_INSTALL_PATH}/license

    echo Bootstraping boost
    try ./bootstrap.sh gcc

    echo Compiling boost \(debug,gcc\)

    CXXFLAGS=-fPIC
    BOOST_OPTIONS="variant=debug,release toolset=gcc"
    BOOST_OPTIONS+=" link=static runtime-link=static cxxflags=${CXXFLAGS}"
    BOOST_OPTIONS+=" --layout=versioned --without-context --without-chrono"
    BOOST_OPTIONS+=" --without-graph_parallel --without-locale  --without-mpi"
    BOOST_OPTIONS+=" --without-serialization --without-signals --without-wave"
    BOOST_OPTIONS+=" --prefix=${BOOST_INSTALL_PATH}"
    try ./b2 debug ${BOOST_OPTIONS} install -j4

    cd ${PRJROOT}
fi


###############################################################################
###   GOOGLE LOG
###############################################################################
if [[ "${BOOTSTRAP_GLOG}" == "Yes" ]]
then
    GLOG_INC_PATH="${GLOG_PATH}/include"
    mkdir -p ${GLOG_INC_PATH}

    cd ${GLOG_PATH}
    mkdir -p tmp
    try unzip glog-master.zip -d tmp

    ####################
    ###   Debug
    ####################
    CMAKE_OPTIONS="-DCMAKE_BUILD_TYPE=Debug"
    CMAKE_OPTIONS+=" -DCMAKE_INSTALL_PREFIX=${ROOT_PATH}/install_debug"
    CMAKE_OPTIONS+=" -DBUILD_TESTING=OFF"

    GLOG_LIB_PATH="${GLOG_PATH}/lib_x64_d"    

    mkdir -p ${GLOG_LIB_PATH}
    cd "tmp/glog-master"
    mkdir -p gcc_build_debug
    cd gcc_build_debug

    try cmake ..
    try make -j4

    cp -rv *.a ${GLOG_LIB_PATH}
    cp -rv glog ${GLOG_INC_PATH}

    cd ${PRJROOT}

    ####################
    ###   Release
    ####################
    CMAKE_OPTIONS="-DCMAKE_BUILD_TYPE=Release"
    CMAKE_OPTIONS+=" -DCMAKE_INSTALL_PREFIX=${ROOT_PATH}/install_release"
    CMAKE_OPTIONS+=" -DBUILD_TESTING=OFF"

    GLOG_LIB_PATH="${GLOG_PATH}/lib_x64_r"

    mkdir -p ${GLOG_LIB_PATH}
    cd ${GLOG_PATH}
    cd "tmp/glog-master"
    mkdir -p gcc_build_release
    cd gcc_build_release

    try cmake ..
    try make -j4

    cp -rv *.a ${GLOG_LIB_PATH}
    cp -rv glog ${GLOG_INC_PATH}

    
    cp -rv ${GLOG_PATH}/tmp/glog-master/src/glog ${GLOG_INC_PATH}/glog
    cp -rv ${GLOG_PATH}/tmp/glog-master/COPYING ${GLOG_INC_PATH}/glog/license

    cd ${PRJROOT}



fi


echo Bootstrap is done 
echo Have a nice day :\)
