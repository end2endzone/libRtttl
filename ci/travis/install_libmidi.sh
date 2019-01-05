# Any commands which fail will cause the shell script to exit immediately
set -e

# Validate Travis CI environment
if [ "$TRAVIS_BUILD_DIR" = "" ]; then
  echo "Please define 'TRAVIS_BUILD_DIR' environment variable.";
  exit 1;
fi

export GTEST_ROOT=$TRAVIS_BUILD_DIR/third_parties/googletest/install
export rapidassist_DIR=$TRAVIS_BUILD_DIR/third_parties/RapidAssist/install
export libmidi_DIR=$TRAVIS_BUILD_DIR/third_parties/libMidi/install
echo libmidi_DIR=$libmidi_DIR
export INSTALL_LOCATION=$TRAVIS_BUILD_DIR/install

echo ============================================================================
echo Cloning libMidi into $TRAVIS_BUILD_DIR/third_parties/libMidi
echo ============================================================================
mkdir -p $TRAVIS_BUILD_DIR/third_parties
cd $TRAVIS_BUILD_DIR/third_parties
git clone "https://github.com/end2endzone/libMidi.git"
cd libMidi
echo

echo Checking out version 2.0.0...
git checkout 2.0.0
echo

#  echo ============================================================================
#  echo Compiling...
#  echo ============================================================================
#  mkdir -p build
#  cd build
#  cmake -DCMAKE_INSTALL_PREFIX=$INSTALL_LOCATION ..
#  cmake --build .
#  echo

#  echo ============================================================================
#  echo Installing into $libmidi_DIR
#  echo ============================================================================
#  make install
#  echo
