# Any commands which fail will cause the shell script to exit immediately
set -e

# Validate Travis CI environment
if [ "$TRAVIS_BUILD_DIR" = "" ]; then
  echo "Please define 'TRAVIS_BUILD_DIR' environment variable.";
  exit 1;
fi

export GTEST_ROOT=$TRAVIS_BUILD_DIR/third_parties/googletest/install
export rapidassist_DIR=$TRAVIS_BUILD_DIR/third_parties/RapidAssist/install
export bitreader_DIR=$TRAVIS_BUILD_DIR/third_parties/BitReader/install
echo bitreader_DIR=$bitreader_DIR

echo ============================================================================
echo Cloning BitReader into $TRAVIS_BUILD_DIR/third_parties/BitReader
echo ============================================================================
mkdir -p $TRAVIS_BUILD_DIR/third_parties
cd $TRAVIS_BUILD_DIR/third_parties
git clone "https://github.com/end2endzone/BitReader.git"
cd BitReader
echo

echo Checking out version 1.3.0...
git checkout 1.3.0
echo

#  echo ============================================================================
#  echo Compiling...
#  echo ============================================================================
#  mkdir -p build
#  cd build
#  cmake -DCMAKE_INSTALL_PREFIX=$bitreader_DIR ..
#  cmake --build .
#  echo

#  echo ============================================================================
#  echo Installing into $bitreader_DIR
#  echo ============================================================================
#  make install
#  echo
