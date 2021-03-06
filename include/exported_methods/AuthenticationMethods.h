#ifndef MODIO_AUTHENTICATION_H
#define MODIO_AUTHENTICATION_H

#include "wrappers/CurlWrapper.h"
#include "wrappers/MinizipWrapper.h"
#include "Globals.h"
#include <thread>

extern "C"
{
  void MODIO_DLL modioEmailRequest(char* email, void (*callback)(ModioResponse* response, char* message));
  void MODIO_DLL modioEmailExchange(char* security_code, void (*callback)(ModioResponse* response));
  bool MODIO_DLL modioIsLoggedIn();
  void MODIO_DLL modioLogout();
}

#endif
