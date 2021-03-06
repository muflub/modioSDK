#ifndef MODIO_MOD_METHODS_H
#define MODIO_MOD_METHODS_H

#include "wrappers/CurlWrapper.h"
#include "wrappers/MinizipWrapper.h"
#include "data_containers/Mod.h"
#include "Globals.h"
#include "Filter.h"
#include "handlers/ModHandler.h"

extern "C"
{
  void MODIO_DLL modioGetMods(ModioFilter* filter, void (*callback)(ModioResponse* response, ModioMod* mods, int mods_size));
  void MODIO_DLL modioAddMod(ModioModHandler* mod_handler, void (*callback)(ModioResponse* response, ModioMod* mod));
  void MODIO_DLL modioEditMod(int mod_id, ModioModHandler* mod_handler, void (*callback)(ModioResponse* response, ModioMod* mod));
  void MODIO_DLL modioDeleteMod(int mod_id, void (*callback)(ModioResponse* response, ModioMod* mod));
  void MODIO_DLL modioSetUserModVote(int mod_id, bool vote_up, void (*callback)(ModioResponse* response, int mod_id));
}

#endif
