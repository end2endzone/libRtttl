# Any commands which fail will cause the shell script to exit immediately
set -e

# Validate Travis CI environment
if [ "$TRAVIS_BUILD_DIR" = "" ]; then
  echo "Please define 'TRAVIS_BUILD_DIR' environment variable.";
  exit 1;
fi

export GTEST_ROOT=$TRAVIS_BUILD_DIR/third_parties/googletest/install
export rapidassist_DIR=$TRAVIS_BUILD_DIR/third_parties/RapidAssist/install
export anyrtttl_DIR=$TRAVIS_BUILD_DIR/third_parties/AnyRtttl/install
echo anyrtttl_DIR=$anyrtttl_DIR

echo ============================================================================
echo Cloning AnyRtttl into $TRAVIS_BUILD_DIR/third_parties/AnyRtttl
echo ============================================================================
mkdir -p $TRAVIS_BUILD_DIR/third_parties
cd $TRAVIS_BUILD_DIR/third_parties
git clone "https://github.com/end2endzone/AnyRtttl.git"
cd AnyRtttl
echo

echo Checking out version 2.2.0...
git checkout 2.2.0
echo

#  echo ============================================================================
#  echo Compiling...
#  echo ============================================================================
#  mkdir -p build
#  cd build
#  cmake -DCMAKE_INSTALL_PREFIX=$anyrtttl_DIR ..
#  cmake --build .
#  echo

#  echo ============================================================================
#  echo Installing into $anyrtttl_DIR
#  echo ============================================================================
#  make install
#  echo
