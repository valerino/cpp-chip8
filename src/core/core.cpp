#include <vuelib.h>
#include <dlfcn.h>

int main(int argc, char** argv) {
  // load emulator library
  void* emu = dlopen("emulators/chip8/libchip8.dylib",RTLD_LAZY);
  GET_EMULATOR_INTERFACE p=(GET_EMULATOR_INTERFACE)dlsym(emu,"get_emulator_interface");

  // instantiate emulator
  IEmulator* chip8 = (IEmulator*)p();
  printf("emu: %s\n", chip8->name().c_str());

  // start emulator with invaders rom
  chip8->start("/Users/valerino/Downloads/invaders.bin");

  // done, exit
  chip8->stop();
  return 0;
}