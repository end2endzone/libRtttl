void play16BitsRtttl(int pin, const unsigned char * iBuffer, int numNotes) {
  // Absolutely no error checking in here

  RTTTL_DEFAULT_VALUE_SECTION * defaultSection = (RTTTL_DEFAULT_VALUE_SECTION *)iBuffer;
  RTTTL_NOTE * notesBuffer = (RTTTL_NOTE *)iBuffer;

  RTTTL_BPM bpm = defaultSection->bpm;

  #ifdef RTTTL_SERIAL_DEBUG
  Serial.print("numNotes=");
  Serial.println(numNotes);
  // format: d=N,o=N,b=NNN:
  Serial.print("d=");
  Serial.print(durationValues[defaultSection->durationIdx]);
  Serial.print(",o=");
  Serial.print(octaveValues[defaultSection->octaveIdx]);
  Serial.print(",b=");
  Serial.println(bpm);
  #endif
  
  TONE_DURATION duration;
  byte note;
  RTTTL_OCTAVE scale;

  // BPM usually expresses the number of quarter notes per minute
  TONE_DURATION wholenote = (60 * 1000L / bpm) * 4;  // this is the time for whole note (in milliseconds)

  // now begin note loop
  for(int i=0; i<numNotes; i++) {
    const RTTTL_NOTE & n = notesBuffer[i+1]; //offset by 16 bits for RTTTL_DEFAULT_VALUE_SECTION

    // first, get note duration, if available
    duration = wholenote / durationValues[n.durationIdx];

    // now get the note
    note = noteCharacterCodes[n.noteIdx];

    // now, get optional '#' sharp
    if(n.pound)
    {
      note++;
    }

    // now, get optional '.' dotted note
    if(n.dotted)
    {
      duration += duration/2;
    }

    // now, get scale
    scale = octaveValues[n.octaveIdx];

    if(note)
    {
      #ifdef RTTTL_SERIAL_DEBUG
      Serial.print(durationValues[n.durationIdx]);
      static const char noteCharacterValues[] =   {'c','d','e','f','g','a','b','p'};
      Serial.print(noteCharacterValues[n.noteIdx]);
      Serial.print( (n.pound ? "#" : "") );
      Serial.print( (n.dotted ? "." : "") );
      Serial.println(octaveValues[n.octaveIdx]);
      #endif
      
      tone(pin, rtttlNotes[(scale - 4) * 12 + note], duration);
      delay(duration+1);
      noTone(pin);
    }
    else
    {
      #ifdef RTTTL_SERIAL_DEBUG
      Serial.print(durationValues[n.durationIdx]);
      static const char noteCharacterValues[] =   {'c','d','e','f','g','a','b','p'};
      Serial.print(noteCharacterValues[n.noteIdx]);
      Serial.print( (n.pound ? "#" : "") );
      Serial.print( (n.dotted ? "." : "") );
      Serial.println();
      #endif

      delay(duration);
    }
  }
}
