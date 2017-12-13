#include <vuelib.h>
#include <dlfcn.h>

/**
 * print banner
 */
void banner() {
  CUIUtils::show_message(MSG_INFO, "VUE - valerino's universal emulator\n"
                                   "(c)opyleft, 2k17\n\n");
}

/**
 * print usage
 */
void usage(char **argv) {
  banner();
  CUIUtils::show_message(
      MSG_INFO,
      "\tusage: %s -e <emu_core> -o [core_options] -r <image>\n"
      "\tusage: %s -l to list available cores\n",
      argv[0], argv[0]);
}

/**
 * load emulator
 * @param name the core name
 * @param emu on successfule return, pointer to the IEmulator interface
 * @return
 */
CDynModule *load_emu(const char *name, IEmulator **emu) {
  char emu_path[260];
  snprintf(emu_path, sizeof(emu_path), "./emulators/%s/lib%s", name, name);
  *emu = NULL;
  CDynModule *mod = NULL;
  try {
    mod = new CDynModule(emu_path);
    IEmulator *e = (IEmulator *)mod->getIEmulator();
    if (!e) {
      CUIUtils::show_message(MSG_ERROR, "%s is not a VUE core: %s\n", emu_path);
      delete mod;
      mod = NULL;
    }

    // loaded
    *emu = e;
  } catch (std::system_error ex) {
    CUIUtils::show_message(MSG_ERROR, "cannot load module: %s (%d)\n", emu_path,
                           ex.code());
  }
  return mod;
}
/**
 * walk emulators directory and list cores
 * @return 0 on success
 */
int list_cores() {
  banner();
  // open directory
  tinydir_dir *d = CFile::open_directory("./emulators");
  if (!d) {
    return errno;
  }

  // walk directory
  std::string cores("available cores:\n");
  while (d->has_next) {
    tinydir_file file;
    tinydir_readfile(d, &file);
    if (file.is_dir) {
      if ((strcmp(file.name, ".") == 0) || (strcmp(file.name, "..") == 0)) {
        // skip these
        tinydir_next(d);
        continue;
      }

      // found an emulator dir, load emulator and get info
      IEmulator *e;
      CDynModule *emu = load_emu(file.name, &e);
      if (e) {
        char buffer[1024];
        snprintf(
            buffer, sizeof(buffer),
            "\tcore: %s\n \t\tinfo: %s\n\t\tversion: %s\n\t\toptions: %s\n",
            e->name().data(), e->info().data(), e->version().data(),
            e->options().data());
        cores += buffer;
        delete emu;
      }
    }

    // next
    tinydir_next(d);
  }
  CFile::close_directory(d);
  CUIUtils::show_message(MSG_INFO, cores.data());
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

  // default debug level set to error
  CDbg::set_debug_level(DBG_ERROR);

  // load configuration
  CConfiguration::init("core.json","{}");

  // load emulator library
  CUIUtils::show_toast_message(MSG_INFO, "loading emulator core: %s\n",
                               emu_core);
  IEmulator *emu;
  CDynModule *mod = load_emu(emu_core, &emu);
  if (emu && mod) {
    // will run until quitting
    int res = emu->start(game_rom);
    delete emu;
    delete mod;
    return res;
  }

  return 1;
}