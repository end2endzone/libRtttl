# Any commands which fail will cause the shell script to exit immediately
set -e

# Validate Travis CI environment
if [ "$TRAVIS_BUILD_DIR" = "" ]; then
  echo "Please define 'TRAVIS_BUILD_DIR' environment variable.";
  exit 1;
fi

export GTEST_ROOT=$TRAVIS_BUILD_DIR/third_parties/googletest/install
export rapidassist_DIR=$TRAVIS_BUILD_DIR/third_parties/RapidAssist/install
echo rapidassist_DIR=$rapidassist_DIR
export INSTALL_LOCATION=$TRAVIS_BUILD_DIR/install

echo ============================================================================
echo Cloning RapidAssist into $TRAVIS_BUILD_DIR/third_parties/RapidAssist
echo ============================================================================
mkdir -p $TRAVIS_BUILD_DIR/third_parties
cd $TRAVIS_BUILD_DIR/third_parties
git clone "https://github.com/end2endzone/RapidAssist.git"
cd RapidAssist
echo

echo Checking out version v0.5.0...
git checkout 0.5.0
echo

echo ============================================================================
echo Compiling...
echo ============================================================================
mkdir -p build
cd build
cmake -DCMAKE_INSTALL_PREFIX=$INSTALL_LOCATION ..
cmake --build .
echo

echo ============================================================================
echo Installing into $rapidassist_DIR
echo ============================================================================
make install
echo
