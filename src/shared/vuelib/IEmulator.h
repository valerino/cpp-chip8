//
// Created by valerino on 08/11/2017.
//

#ifndef PROJECT_CEMULATOR_H
#define PROJECT_CEMULATOR_H

#include <string>

/**
 * interface for emulator plugins
 */
class IEmulator {
public:
    /**
     *  get the emulator name
     *  @return std::string
     */
    virtual std::string name() = 0;

    /**
     * start emulator
     * @param rom_path path to a rom file
     * @param extras_path optional, path to a folder containing bios, etc....
     * @return 0 on success
     */
    virtual int start(const char* rom_path, const char* extras_path=NULL) = 0;

    /**
     * stop emulator and release resources, pointer to object will become invalid!
     */
    virtual void stop() = 0;

    /**
     * pause emulator
     */
    virtual void pause() = 0;

    /**
     * show emulator-specific configuration options
     */
    virtual void configure() = 0;

};

/**
 * every emulator plugin must export IEmulator* get_emulator_interface()
 */
typedef IEmulator* (*GET_EMULATOR_INTERFACE)();

#endif //PROJECT_CEMULATOR_H
