#ifndef MODIO_MODFILE_METHODS_H
#define MODIO_MODFILE_METHODS_H

#include "wrappers/CurlWrapper.h"
#include "wrappers/MinizipWrapper.h"
#include "data_containers/Mod.h"
#include "Globals.h"
#include "handlers/ModfileHandler.h"

extern "C"
{
  void MODIO_DLL modioAddModfile(int mod_id, ModioModfileHandler* modfile_handler, void (*callback)(ModioResponse* response, ModioModfile* modfile));
  void MODIO_DLL modioEditModfile(int mod_id, int modfile_id, ModioModfileHandler* modfile_handler, void (*callback)(ModioResponse* response, int modfile_id));
  void MODIO_DLL modioInstallModfile(ModioModfile *modfile, char* destination_path, void (*callback)(ModioResponse* response, char* path));
  int MODIO_DLL modioGetModfileState(int modfile_id);
  double MODIO_DLL modioGetModfileDownloadPercentage(int modfile_id);
}

#endif
