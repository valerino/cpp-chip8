#include <vuelib.h>
#include <dlfcn.h>

/**
 * print usage
 */
void usage(char **argv) {
  printf("[.] vue : valerino universal emulator\n");
  printf("[.] (c)opyleft, 2k17\n");
  printf("usage: %s -e <emu_core> -o [core_options] -r <image>\n",argv[0]);
  printf("usage: %s -l to list available cores\n",argv[0]);
}

/**
 * walk emulators directory and list cores
 * @return 0 on success
 */
int list_cores() {
  // open directory
  tinydir_dir* d = CFile::open_directory("./emulators");
  if (!d) {
    int e = errno;
    return errno;
  }

  // walk directory
  while (d->has_next) {
    tinydir_file file;
    tinydir_readfile(d, &file);
    if (file.is_dir) {
      if ((strcmp(file.name,".") == 0) || (strcmp(file.name,"..") == 0)) {
        // skip these
        tinydir_next(d);
        continue;
      }
      // found an emulator dir
      char path[260];
#if defined(TARGET_OS_MAC) || defined(TARGET_OS_IOS)
      sprintf(path,"./emulators/%s/lib%s.dylib",file.name, file.name);
#elif TARGET_OS_UNIX
      // add others....
      sprintf(path,"./emulators/%s/lib%s.so",file.name, file.name);
#elif TARGET_OS_WIN32
      sprintf(path,"./emulators/%s/lib%s.dll",file.name, file.name);
#endif

      // load library and call the info functions
#ifdef WIN32
      HANDLE* emu = LoadLibraryA(path);
      GET_EMULATOR_INTERFACE p =
          (GET_EMULATOR_INTERFACE)GetProcAddress(emu, "get_emulator_interface");
#else
      void *emu = dlopen(path, RTLD_LAZY);
      GET_EMULATOR_INTERFACE p =
          (GET_EMULATOR_INTERFACE)dlsym(emu, "get_emulator_interface");
#endif
      // instantiate emulator
      IEmulator *e = (IEmulator *)p();
      printf("[.] core: %s, info: %s, version: %s, options: %s\n", e->name().data(), e->info().data(), e->version().data(), e->options().data());

#ifdef WIN32
      FreeLibrary(emu);
#else
      dlclose(emu);
#endif
    }

    tinydir_next(d);
  }
  CFile::close_directory(d);
  return 0;
}

/**
 * main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char **argv) {
  int c;
  const char *emu_core = NULL;
  const char *game_rom = NULL;
  const char *options = NULL;
  while ((c = getopt(argc, argv, "e:o:r:l")) != -1) {
    switch (c) {
    case 'e':
      emu_core = optarg;
      break;
    case 'o':
      options = optarg;
      break;
    case 'r':
      game_rom = optarg;
      break;
    case 'l':
      // list cores mode
      list_cores();
      return 0;
      break;
    case '?':
      // print usage
      usage(argv);
      return 1;

    default:
      break;
    }
  }
  if (!emu_core || !game_rom) {
    // print usage
    usage(argv);
    return 1;
  }

  // load configuration
  CConfiguration cfg("core.json");
  cfg.read("{}");

  // load emulator library
  void *emu = dlopen("emulators/chip8/libchip8.dylib", RTLD_LAZY);
  GET_EMULATOR_INTERFACE p =
      (GET_EMULATOR_INTERFACE)dlsym(emu, "get_emulator_interface");

  // instantiate emulator
  IEmulator *chip8 = (IEmulator *)p();
  CDbg::notify("running emulator: %s\n", chip8->name().c_str());

  // start emulator with invaders rom
  chip8->start("emulators/chip8/invaders.ch8");

  // done, exit
  chip8->stop();
  delete chip8;
  return 0;
}