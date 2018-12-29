// testRtttl.cpp : Defines the entry point for the console application.
//

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#include <stdio.h>
#include <tchar.h>
#include <vector>
#include <string>
#include "notes.h"
#include "tone.h"
#include "rtttl.h"
#include "bitreader.h"
#include "gtesthelper.h"
#include "utils.h"
#include "ImperialMarch.h"
#include "generator.h"
#include "counter5bits.h"
#include "mario.h"
#include "testLibRtttl.h"

#define BUZZER_OUT_PIN 0

typedef unsigned char uint8_t;
typedef unsigned char byte;
typedef std::vector<int> IntVector;

using namespace librtttl;

/*************************************************
 * RTTTL Melodies
 *************************************************/
//See also http://ez4mobile.com/nokiatone/rtttf.htm
const char *zelda = "Zelda1:d=4,o=5,b=125:a#,f.,8a#,16a#,16c6,16d6,16d#6,2f6,8p,8f6,16f.6,16f#6,16g#.6,2a#.6,16a#.6,16g#6,16f#.6,8g#.6,16f#.6,2f6,f6,8d#6,16d#6,16f6,2f#6,8f6,8d#6,8c#6,16c#6,16d#6,2f6,8d#6,8c#6,8c6,16c6,16d6,2e6,g6,8f6,16f,16f,8f,16f,16f,8f,16f,16f,8f,8f,a#,f.,8a#,16a#,16c6,16d6,16d#6,2f6,8p,8f6,16f.6,16f#6,16g#.6,2a#.6,c#7,c7,2a6,f6,2f#.6,a#6,a6,2f6,f6,2f#.6,a#6,a6,2f6,d6,2d#.6,f#6,f6,2c#6,a#,c6,16d6,2e6,g6,8f6,16f,16f,8f,16f,16f,8f,16f,16f,8f,8f";
const char *simpsons = "Simpsons:d=4,o=5,b=160:c.6,e6,f#6,8a6,g.6,e6,c6,8a,8f#,8f#,8f#,2g,8p,8p,8f#,8f#,8f#,8g,a#.,8c6,8c6,8c6,c6";

void TestRTTTL::SetUp()
{
  gTestHelper & helper = gTestHelper::getInstance();
}

void TestRTTTL::TearDown()
{
}

void testRtttlConversion(const char * iRtttlString)
{
  setRtttlSettings(STRICT_RTTTL_SETTINGS);

  //read song name
  const std::string rtttlString = iRtttlString;
  std::string songName = std::string(iRtttlString).substr(0, rtttlString.find(':'));

  SCOPED_TRACE(songName.c_str());

  //extract tones calls from RTTTL data
  dumpString(iRtttlString, "testRtttlEncoder.rtttl.input.txt");
  dumpRtttlToneCalls(iRtttlString, "testRtttlEncoder.rtttl.input.tonecalls.txt");

  //try to convert tones back to RTTTL
  std::string rtttlCode;
  RTTTL_RESULT result = calls2Rtttl(songName.c_str(), rtttlCode, NULL, NULL, NULL);
  dumpString(rtttlCode.c_str(), "testRtttlEncoder.rtttl.output1.txt");
  dumpRtttlToneCalls(rtttlCode.c_str(), "testRtttlEncoder.rtttl.output.tonecalls.txt");

  //assert both tone calls are equals.
  ASSERT_TRUE( isFileEquals( "testRtttlEncoder.rtttl.input.tonecalls.txt",
                             "testRtttlEncoder.rtttl.output.tonecalls.txt") );

  //use same properties as the input rtttl string
  RTTTL_DURATION forcedDuration = 4;
  RTTTL_OCTAVE_VALUE forcedOctave = 5;
  result = calls2Rtttl(songName.c_str(), rtttlCode, &forcedDuration, &forcedOctave, NULL);
  dumpString(rtttlCode.c_str(), "testRtttlEncoder.rtttl.output2.txt");

  ASSERT_TRUE( isFileEquals(  "testRtttlEncoder.rtttl.input.txt",
                              "testRtttlEncoder.rtttl.output2.txt") );
}

TEST_F(TestRTTTL, testRtttlConversion)
{
  testRtttlConversion(zelda);
  if (HasFatalFailure())
    return;
  testRtttlConversion(simpsons);
  if (HasFatalFailure())
    return;
}

void testParseRTTTL(const char * iRtttlString)
{
  setRtttlSettings(STRICT_RTTTL_SETTINGS);

  //read song name
  const std::string rtttlString = iRtttlString;
  std::string songName = std::string(iRtttlString).substr(0, rtttlString.find(':'));

  SCOPED_TRACE(songName.c_str());

  //parse content
  RTTTL_SONG song = parseRtttlString(iRtttlString);

  //convert back to string
  std::string rtttlCopy = toString(song, true);

  //dumb both string
  dumpString(iRtttlString, "testParseRTTTL.rtttl.input.txt");
  dumpString(rtttlCopy.c_str(), "testParseRTTTL.rtttl.output.txt");

  //assert both rtttl code are equals.
  ASSERT_TRUE( isFileEquals(  "testParseRTTTL.rtttl.input.txt",
                              "testParseRTTTL.rtttl.output.txt") );
}

TEST_F(TestRTTTL, testParseRTTTL)
{
  testParseRTTTL(zelda);
  if (HasFatalFailure())
    return;
  testParseRTTTL(simpsons);
  if (HasFatalFailure())
    return;
}

TEST_F(TestRTTTL, testMarioThemeSongs)
{
  dumpMarioThemeToneCalls(0, theme, "mario_theme.txt");
  dumpMarioThemeToneCalls(0, life, "mario_life.txt");
  dumpMarioThemeToneCalls(0, flagpole, "mario_flagpole.txt");
  dumpMarioThemeToneCalls(0, death, "mario_death.txt");
  dumpMarioThemeToneCalls(0, gameover, "mario_gameover.txt");
}

TEST_F(TestRTTTL, testImperialMarchRTTTLConvertion)
{
  setRtttlSettings(RELAXED_RTTTL_SETTINGS);

  clearToneCalls();
  playImperialMarch(0);
  dumpRecordedToneCalls("testRtttlEncoder.impmarch.input.tonecalls.txt");
  TONE_CALL_INFO_LIST officialCalls = getRecordedToneCalls(); //make a copy

  //try to convert tones back to RTTTL
//do not force default section ? ***************************************************************************************************************
  std::string rtttlCode;
  RTTTL_RESULT rtttlEncodeResult = calls2Rtttl("impmarch",rtttlCode , NULL, NULL, NULL);
  ASSERT_TRUE( rtttlEncodeResult == SUCCESS );
  dumpString(rtttlCode.c_str(), "testRtttlEncoder.impmarch.rtttl.output.txt");
  dumpRtttlToneCalls(rtttlCode.c_str(), "testRtttlEncoder.impmarch.output.tonecalls.txt");
  TONE_CALL_INFO_LIST convertedCalls = getRecordedToneCalls(); //make a copy
  
  //assert same number of calls.
  ASSERT_TRUE( officialCalls.size() == convertedCalls.size() );

  //assert duration are equals
  for(size_t i=0; i<officialCalls.size(); i++)
  {
    const TONE_CALL_INFO & c1 = officialCalls[i];
    const TONE_CALL_INFO & c2 = convertedCalls[i];
    //printf("ASSERT TONE_CALL_INFO[%d]\n", i);

    //sometimes delays might differ from 1 ms
    ASSERT_NEAR( c1.delay, c2.delay, 3 );

    //assert same duration for notes
    ASSERT_TRUE( c1._tone.duration == c2._tone.duration );
  }
}

TEST_F(TestRTTTL, testImperialMarchRTTTLConvertionFailure)
{
  setRtttlSettings(STRICT_RTTTL_SETTINGS);

  clearToneCalls();
  playImperialMarch(0);
  dumpRecordedToneCalls("testRtttlEncoder.impmarch.input.tonecalls.txt");
  TONE_CALL_INFO_LIST officialCalls = getRecordedToneCalls(); //make a copy

  //try to convert tones back to RTTTL
  std::string rtttlCode;
  RTTTL_RESULT rtttlEncodeResult = calls2Rtttl("impmarch",rtttlCode , NULL, NULL, NULL);
  ASSERT_TRUE( rtttlEncodeResult != SUCCESS );
}

TEST_F(TestRTTTL, testMaximumMinimum)
{
  char     tmp1_min;
  char     tmp1_max;
  minimize(tmp1_min);
  maximize(tmp1_max);

  unsigned char tmp2_min;
  unsigned char tmp2_max;
  minimize(tmp2_min);
  maximize(tmp2_max);

  short    tmp3_min;
  short    tmp3_max;
  minimize(tmp3_min);
  maximize(tmp3_max);

  unsigned short tmp4_min;
  unsigned short tmp4_max;
  minimize(tmp4_min);
  maximize(tmp4_max);

  int tmp5_min;
  int tmp5_max;
  minimize(tmp5_min);
  maximize(tmp5_max);

  unsigned int tmp6_min;
  unsigned int tmp6_max;
  minimize(    tmp6_min);
  maximize(    tmp6_max);

  //float    tmp7_min;
  //float    tmp7_max;
  //minimize(tmp7_min);
  //maximize(tmp7_max);

  //double   tmp8_min;
  //double   tmp8_max;
  //minimize(tmp8_min);
  //maximize(tmp8_max);
}

TEST_F(TestRTTTL, testAssertNear)
{
  double myValue = 0.3;
  double myEpsilon = 0.0001;

  ASSERT_NEAR(myValue, 0.30000001, myEpsilon);
}

TEST_F(TestRTTTL, testComputeEpsilonValues)
{
  {
    unsigned char value = 250;
    unsigned char epsilon = 10;
    unsigned char valueMin = 0;
    unsigned char valueMax = 0;
    computeEpsilonValues(value, epsilon, valueMin, valueMax);
    ASSERT_TRUE(valueMin == 240);
    ASSERT_TRUE(valueMax == 255);
  }
  {
    unsigned char value = 5;
    unsigned char epsilon = 10;
    unsigned char valueMin = 0;
    unsigned char valueMax = 0;
    computeEpsilonValues(value, epsilon, valueMin, valueMax);
    ASSERT_TRUE(valueMin == 0);
    ASSERT_TRUE(valueMax == 15);
  }
  {
    char value = 120;
    char epsilon = 10;
    char valueMin = 0;
    char valueMax = 0;
    computeEpsilonValues(value, epsilon, valueMin, valueMax);
    ASSERT_TRUE(valueMin == 110);
    ASSERT_TRUE(valueMax == 127);
  }
  {
    char value = -120;
    char epsilon = 10;
    char valueMin = 0;
    char valueMax = 0;
    computeEpsilonValues(value, epsilon, valueMin, valueMax);
    ASSERT_TRUE(valueMin == -128);
    ASSERT_TRUE(valueMax == -110);
  }
}

void testPlayRTTTL(const char * iRtttlString)
{
  setRtttlSettings(BPM_FRIENDLY_RTTTL_SETTINGS);

  //read song name
  const std::string rtttlString = iRtttlString;
  std::string songName = std::string(iRtttlString).substr(0, rtttlString.find(':'));

  SCOPED_TRACE(songName.c_str());

  //extract tones calls from RTTTL data
  dumpString(iRtttlString, "testPlayRTTTL.rtttl.input.txt");
  dumpRtttlToneCalls(iRtttlString, "testPlayRTTTL.rtttl.input.tonecalls.txt");

  //parse content
  RTTTL_SONG song = parseRtttlString(iRtttlString);
  const TONE_CALL_INFO_LIST toneCalls = playRTTTL(song);
  dumpToneCalls(toneCalls, "testPlayRTTTL.rtttl.output.tonecalls.txt");

  //assert both tones are equals.
  ASSERT_TRUE( isFileEquals(  "testPlayRTTTL.rtttl.input.tonecalls.txt",
                              "testPlayRTTTL.rtttl.output.tonecalls.txt") );
}

TEST_F(TestRTTTL, testPlayRTTTL)
{
  testPlayRTTTL(zelda);
  if (HasFatalFailure())
    return;
  testPlayRTTTL(simpsons);
  if (HasFatalFailure())
    return;
}

TEST_F(TestRTTTL, testParseToneCallsFile)
{
  const char * inputFile  = "testParseToneCallsFile.input.tonecalls.txt";
  const char * outputFile = "testParseToneCallsFile.output.tonecalls.txt";

  const char * rtttlString = zelda;
  dumpRtttlToneCalls(rtttlString, inputFile);
  
  TONE_CALL_INFO_LIST parsedCalls;
  bool success = parseToneCallsFile(inputFile, parsedCalls);
  ASSERT_TRUE(success);

  dumpToneCalls(parsedCalls, outputFile);

  //assert both tones are equals.
  ASSERT_TRUE( isFileEquals(inputFile, outputFile) );
}

TEST_F(TestRTTTL, testToCppArray)
{
  setRtttlSettings(STRICT_RTTTL_SETTINGS);

  const unsigned char inputData[] = {0x00, 0x34, 0x46, 0xf2, 0xff, 0x23, 0xaa};
  STREAM::STREAM_SIZE inputDataSize = sizeof(inputData)/sizeof(inputData[0]);

  //allocate a stream
  STREAM inputStream;
  inputStream.alloc(inputDataSize);
  ASSERT_TRUE(inputStream.buffer != NULL);
  //copy inputData into inputStream
  memcpy(inputStream.buffer, inputData, inputDataSize);

  //convert to code
  std::string cpp = toCppArray(inputStream);
  const char * expectedCode = "{0x00, 0x34, 0x46, 0xF2, 0xFF, 0x23, 0xAA}";
  ASSERT_TRUE(cpp == expectedCode);
}

TEST_F(TestRTTTL, testParseRtttlStream)
{
  setRtttlSettings(STRICT_RTTTL_SETTINGS);

  //parse content
  RTTTL_SONG song1 = parseRtttlString(zelda);
  
  //convert to stream
  STREAM stream1;
  toStream(song1, stream1);
  ASSERT_TRUE(stream1.buffer != NULL);

  //back to song
  RTTTL_SONG song2;
  reset(song2);
  bool success = parseStream(stream1, song2);
  ASSERT_TRUE(success);
  
  //assert both songs are equals
  ASSERT_TRUE(song1.defaults.raw == song2.defaults.raw);
  ASSERT_TRUE(song1.notes.size() == song2.notes.size());
  for(size_t i=0; i<song1.notes.size(); i++)
  {
    const RTTTL_NOTE & note1 = song1.notes[i];
    const RTTTL_NOTE & note2 = song2.notes[i];
    ASSERT_TRUE(note1.raw == note2.raw);
  }
}

TEST_F(TestRTTTL, testSizeof)
{
  //test size
  char buffer[1024];
  sprintf(buffer, "sizeof(RTTTL_DEFAULT_VALUE_SECTION)=%d, sizeof(RTTTL_NOTE)=%d. \n", 
    sizeof(RTTTL_DEFAULT_VALUE_SECTION),
    sizeof(RTTTL_NOTE)
    );
  SCOPED_TRACE(buffer);
  ASSERT_TRUE( sizeof(RTTTL_DEFAULT_VALUE_SECTION) == 2 );
  ASSERT_TRUE( sizeof(RTTTL_NOTE) == 2 );

  //fill structures
  RTTTL_DEFAULT_VALUE_SECTION d;
  memset(&d, 0, sizeof(d));
  d.durationIdx = 0x07;
  d.octaveIdx = 0x03;
  d.bpm = 0x03FF;
  //d.padding = true;
  ASSERT_TRUE( d.raw == 0x7FFF );

  RTTTL_NOTE n;
  memset(&n, 0, sizeof(n));
  n.durationIdx = 0x07;
  n.noteIdx = 0x07;
  n.pound = true;
  n.dotted = true;
  n.octaveIdx = 0x03;
  n.padding = 0x3f;
  ASSERT_TRUE( n.raw == 0xFFFF );
}

TEST_F(TestRTTTL, DISABLED_testBitReaderCopy8)
{
#ifdef USE_BITADDRESS_READ_WRITE
  printf("SKIPPED\n");
  return;
#else

  int bitSizes[] = {1,6,2,5,3,5,4,2,1,7};
  size_t numBitSizes = sizeof(bitSizes)/sizeof(bitSizes[0]);

  const int MEM_SIZE = 100;
  char memsource[MEM_SIZE];
  char memtarget[MEM_SIZE + 1]; //1 byte (8 bits) more to allow a little bit copy overflow protection

  //randomize source  
  for(int i=0; i<MEM_SIZE; i++)
  {
    //memsource[i] = (i%54)+(i%6)*50;
    memsource[i] = (char)0xFF;
  }

  BitReader r;
  r.setBuffer(memsource);

  BitWriter w;
  w.setBuffer(memtarget);

  int numBitCopied = 0;
  const int totalBitCopy = MEM_SIZE * BITS_PER_BYTE;
  int copyIterationCount = 0;
  while(numBitCopied < totalBitCopy)
  {
    //get num bit to copy in this pass
    int numBits = bitSizes[copyIterationCount%numBitSizes];
    unsigned char c = r.read8(numBits);
    w.write8(c, numBits);

    copyIterationCount++;
    numBitCopied += numBits;
  }

  //assert both memories are equals
  for(int i=0; i<MEM_SIZE; i++)
  {
    ASSERT_TRUE( memsource[i] == memtarget[i] );
  }
#endif
}

TEST_F(TestRTTTL, DISABLED_testBitReaderCopy16)
{
#ifdef USE_BITADDRESS_READ_WRITE
  printf("SKIPPED\n");
  return;
#else

  int bitSizes[] = {10,6,12,15,3,5,14,2,11,7};
  size_t numBitSizes = sizeof(bitSizes)/sizeof(bitSizes[0]);

  const int MEM_SIZE = 100;
  char memsource[MEM_SIZE];
  char memtarget[MEM_SIZE + 1]; //1 byte (8 bits) more to allow a little bit copy overflow protection

  //randomize source  
  for(int i=0; i<MEM_SIZE; i++)
  {
    //memsource[i] = (i%54)+(i%6)*50;
    memsource[i] = (char)0xFF;
  }

  BitReader r;
  r.setBuffer(memsource);

  BitWriter w;
  w.setBuffer(memtarget);

  int numBitCopied = 0;
  const int totalBitCopy = MEM_SIZE * BITS_PER_BYTE;
  int copyIterationCount = 0;
  while(numBitCopied < totalBitCopy)
  {
    //get num bit to copy in this pass
    int numBits = bitSizes[copyIterationCount%numBitSizes];
    unsigned short c = r.read16(numBits);
    w.write16(c, numBits);

    copyIterationCount++;
    numBitCopied += numBits;
  }

  //assert both memories are equals
  for(int i=0; i<MEM_SIZE; i++)
  {
    ASSERT_TRUE( memsource[i] == memtarget[i] );
  }

#endif
}

TEST_F(TestRTTTL, testBitAddress)
{
#ifndef USE_BITADDRESS_SETTER_GETTER
  printf("SKIPPED\n");
  return;
#endif
#ifdef USE_BITADDRESS_SETTER_GETTER

  const int BUFFER_SIZE = 300;
  unsigned char buffer[BUFFER_SIZE] = {0};

  BitAddress addr;

  //write all 8 bits (b7 b6 b5 b4 b3 b2 b1 b0) values into buffer
  for(int b7=0;b7<=1;b7++)
  {
    for(int b6=0;b6<=1;b6++)
    {
      for(int b5=0;b5<=1;b5++)
      {
        for(int b4=0;b4<=1;b4++)
        {
          for(int b3=0;b3<=1;b3++)
          {
            for(int b2=0;b2<=1;b2++)
            {
              for(int b1=0;b1<=1;b1++)
              {
                for(int b0=0;b0<=1;b0++)
                {
                  addr.set(buffer, b0 == 1); addr.next();
                  addr.set(buffer, b1 == 1); addr.next();
                  addr.set(buffer, b2 == 1); addr.next();
                  addr.set(buffer, b3 == 1); addr.next();
                  addr.set(buffer, b4 == 1); addr.next();
                  addr.set(buffer, b5 == 1); addr.next();
                  addr.set(buffer, b6 == 1); addr.next();
                  addr.set(buffer, b7 == 1); addr.next();
                }
              }
            }
          }
        }
      }
    }
  }

  //expect char increasing
  for(unsigned int i=0; i<=255; i++)
  {
    ASSERT_TRUE( buffer[i] == i );
  }
#endif
}

TEST_F(TestRTTTL, test5BitCounterWrite)
{
  #pragma pack(push, 1) // exact fit - no padding
  union FiveBitBlock
  {
    unsigned char raw;
    struct
    {
      unsigned char bits: 5;
      unsigned char reserved: 3;
    };
  };
  #pragma pack(pop) //back to whatever the previous packing mode was


  const int BUFFER_SIZE = 1024;
  unsigned char buffer[BUFFER_SIZE] = {0};
#ifndef USE_BITADDRESS_READ_WRITE
  BitWriter w;
#else
  BitAddress w;
#endif
  w.setBuffer(buffer);
  for(unsigned int i=0; i<=70; i++)
  {
    FiveBitBlock b;
    b.bits = i;
    w.write(&b.raw, 5);
  }

  //assert buffer == fiveBitCounters
  for(size_t i=0; i<fiveBitCountersSize; i++)
  {
    ASSERT_TRUE( buffer[i] == fiveBitCounters[i]);
  }
}

TEST_F(TestRTTTL, test5BitCounterRead)
{
  #pragma pack(push, 1) // exact fit - no padding
  union FiveBitBlock
  {
    unsigned char raw;
    struct
    {
      unsigned char bits: 5;
      unsigned char padding: 3;
    };
  };
  #pragma pack(pop) //back to whatever the previous packing mode was

  FiveBitBlock expected;
  expected.raw = 0;
#ifndef USE_BITADDRESS_READ_WRITE
  BitReader w;
#else
  BitAddress w;
#endif
  w.setBuffer(fiveBitCounters);
  for(unsigned int i=0; i<=70; i++)
  {
    unsigned char actualCounter = 0;
    w.read(5, &actualCounter);

    ASSERT_TRUE(actualCounter == expected.bits );

    expected.bits++;
  }
}

TEST_F(TestRTTTL, testFindAlternateRtttlEncoding)
{
  StringVector alternates;
  RTTTL_RESULT result = findAlternateRtttlEncoding(zelda, alternates);
  ASSERT_TRUE(result == SUCCESS);

  ASSERT_TRUE(alternates.size() > 0);
}

TEST_F(TestRTTTL, testOptimizeEncoding)
{
  std::string optimizedCode;
  RTTTL_RESULT optimzeResult = optimizeEncoding(zelda, optimizedCode);
  ASSERT_TRUE( optimzeResult == SUCCESS );

  size_t oldZeldaLength = std::string(zelda).size();
  size_t newZeldaLength = optimizedCode.size();

  ASSERT_TRUE(newZeldaLength < oldZeldaLength);

  //assert that optimized code is still identical to original
  dumpRtttlToneCalls(zelda,                 "testOptimizeEncoding.input.tonecalls.txt");
  dumpRtttlToneCalls(optimizedCode.c_str(), "testOptimizeEncoding.output.tonecalls.txt");

  //assert both tone calls are equals.
  ASSERT_TRUE( isFileEquals( "testOptimizeEncoding.input.tonecalls.txt",
                             "testOptimizeEncoding.output.tonecalls.txt") );
}
