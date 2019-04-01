
./init_cmake.sh

if [ -d "build_gcc_debug" ]; then
  # Control will enter here if DIRECTORY exists.
  cd build_gcc_debug
  make install
  cd -
fi

if [ -d "build_gcc_release" ]; then
  # Control will enter here if DIRECTORY exists.
  cd build_gcc_release
  make install
  cd -
fi

