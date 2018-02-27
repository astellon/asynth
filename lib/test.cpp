#include <iostream>

#include "synthesizer.h"

int main() {
  astellon::Synthesizer synth = astellon::Synthesizer();
  synth.setPlayer(44000.0, 256);
  synth.noteOn(16, 127);
  synth.process();
  for (int i; i < 256; i++) {
    std::cout << synth.getOutput(0)->at(i) << std::endl;
  }
  return 0;
}