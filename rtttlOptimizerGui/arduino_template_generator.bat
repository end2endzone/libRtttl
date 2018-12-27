@echo off
"C:\Program Files\bin2cpp\bin2cpp.exe" arduino_binrtttl_library.template.ino . arduinoBinRtttl.h        BinRtttl        1000 -override
"C:\Program Files\bin2cpp\bin2cpp.exe" arduino_play10bitsRtttl.template.ino  . arduinoPlay10BitsRtttl.h Play10BitsRtttl 1000 -override
"C:\Program Files\bin2cpp\bin2cpp.exe" arduino_play16bitsRtttl.template.ino  . arduinoPlay16BitsRtttl.h Play16BitsRtttl 1000 -override
pause
