#include "MIDIinput.h"
int value = 0;
unsigned long int timer = 0;
bool waiting = false;
unsigned int waitTime = 0;
bool touched = false;

bool chord[12] = {
  false,false,false,false,false,false,false,false,false,false,false,false
};

int chaos(byte p, int inLo, int threshold, int inHi, byte outLo, byte outHi){
  int newValue = touchRead(p);
  if (waiting){ // Wait briefly to make notes audible.
    if (millis() - timer > waitTime){
      waiting = false;
    }
  }
  else {
    if (newValue >= threshold && touched == false){ // rising edge
      touched = true;
      newValue = -1;
    }
    else if (newValue <= inLo && touched == true){ // falling edge
      touched = false;
      usbMIDI.sendNoteOn(value, 0, MIDIchannel);
      timer = millis();
      waiting = true;
      newValue = 0;
    }
    else if (newValue > inLo && touched == true){ // send MIDI
      newValue = map(newValue, inLo, inHi, outLo, outHi);
      newValue = constrain(newValue, outLo, outHi);
      if (chord[newValue % 12] == true && newValue != value){
        usbMIDI.sendNoteOn(value, 0, MIDIchannel); // we don't want TOTAL chaos
        usbMIDI.sendNoteOn(newValue, outHi, MIDIchannel);
        waitTime = 127 - newValue; // not necessary. just adds flavor.
        value = newValue;
        timer = millis();
        waiting = true;
      }
    }
    else {newValue = -1;}
  }
  return newValue;
};

//MIDI EVENT HANDLERS///////////////////////////////////////////////////////////
void onNoteOff(byte channel, byte note, byte velocity){
  usbMIDI.sendNoteOff(note, 0, channel);
  chord[note % 12] = false;
}
  
void onNoteOn(byte channel, byte note, byte velocity){
  usbMIDI.sendNoteOn(note, velocity, channel);
  if (velocity == 0){
    chord[note % 12] = false;    
  }
  else {chord[note % 12] = true;}
}

void onPolyPressure(byte channel, byte note, byte pressure){
	usbMIDI.sendPolyPressure(note, pressure, channel);
}

void onControl(byte channel, byte control, byte value){
	usbMIDI.sendControlChange(control, value, channel);
}

void onProgram(byte channel, byte program){
	usbMIDI.sendProgramChange(program, channel);
}

void onAfterTouch(byte channel, byte pressure){
	usbMIDI.sendAfterTouch(pressure, channel);
}

void onPitchBend(byte channel, int bend){
	usbMIDI.sendPitchBend(bend, channel);
}

//USB MIDI EVENT HANDLERS///////////////////////////////////////////////////////
void onUSBNoteOff(byte channel, byte note, byte velocity){
  usbMIDI.sendNoteOff(note, 0, channel);
}
  
void onUSBNoteOn(byte channel, byte note, byte velocity){
  usbMIDI.sendNoteOn(note, velocity, channel);
}

void onUSBPolyPressure(byte channel, byte note, byte pressure){
	usbMIDI.sendPolyPressure(note, pressure, channel);
}

void onUSBControl(byte channel, byte control, byte value){
	usbMIDI.sendControlChange(control, value, channel);
}

void onUSBProgram(byte channel, byte program){
	usbMIDI.sendProgramChange(program, channel);
}

void onUSBAfterTouch(byte channel, byte pressure){
	usbMIDI.sendAfterTouch(pressure, channel);
}

void onUSBPitchBend(byte channel, int bend){
	usbMIDI.sendPitchBend(bend, channel);
}