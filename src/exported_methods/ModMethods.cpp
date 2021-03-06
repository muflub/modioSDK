#include "exported_methods/ModMethods.h"

extern "C"
{
  struct GetModsParams
  {
    void (*callback)(ModioResponse* response, ModioMod* mods, int mods_size);
  };

  struct AddModParams
  {
    void (*callback)(ModioResponse* response, ModioMod* mod);
  };

  struct DeleteModParams
  {
    ModioMod* mod;
    void (*callback)(ModioResponse* response, ModioMod* mod);
  };

  struct SetUserModVoteParams
  {
    int mod_id;
    void (*callback)(ModioResponse* response, int mod_id);
  };

  map< int,AddModParams* > add_mod_callback;
  map< int,DeleteModParams* > delete_mod_callbacks;
  map< int,GetModsParams* > get_mods_callbacks;
  map< int,SetUserModVoteParams* > set_user_mod_vote_callbacks;

  void onGetMods(int call_number, ModioResponse* response, json response_json)
  {
    ModioMod* mods = NULL;
    int mods_size = 0;
    if(response->code == 200)
    {
      mods_size = (int)response_json["data"].size();
      mods = new ModioMod[mods_size];
      for(int i=0;i<mods_size;i++)
      {
        modioInitMod(&mods[i], response_json["data"][i]);
      }
    }
    get_mods_callbacks[call_number]->callback(response, mods, mods_size);
    get_mods_callbacks.erase(call_number);
  }

  void modioGetMods(ModioFilter* filter, void (*callback)(ModioResponse* response, ModioMod* mods, int mods_size))
  {
    string filter_string = modio::getFilterString(filter);
    vector<string> headers;
    headers.push_back("Authorization: Bearer " + modio::ACCESS_TOKEN);
    string url = modio::MODIO_URL + modio::MODIO_VERSION_PATH + "games/" + modio::toString(modio::GAME_ID) + "/mods?" + filter_string + "&api_key=" + modio::API_KEY;

    int call_number = modio::curlwrapper::getCallCount();
    modio::curlwrapper::advanceCallCount();

    get_mods_callbacks[call_number] = new GetModsParams;
    get_mods_callbacks[call_number]->callback = callback;

    std::thread get_mods_thread(modio::curlwrapper::get, call_number, url, headers, &onGetMods);
    get_mods_thread.detach();
  }

  void onModAdded(int call_number, ModioResponse* response, json response_json)
  {
    ModioMod* mod = new ModioMod;
    modioInitMod(mod, response_json);
    add_mod_callback[call_number]->callback(response, mod);
    add_mod_callback.erase(call_number);
  }

  void modioEditMod(int mod_id, ModioModHandler* mod_handler, void (*callback)(ModioResponse* response, ModioMod* mod))
  {
    vector<string> headers;
    headers.push_back("Authorization: Bearer " + modio::ACCESS_TOKEN);

    int call_number = modio::curlwrapper::getCallCount();
    modio::curlwrapper::advanceCallCount();

    add_mod_callback[call_number] = new AddModParams;
    add_mod_callback[call_number]->callback = callback;

    string url = modio::MODIO_URL + modio::MODIO_VERSION_PATH + "games/" + modio::toString(modio::GAME_ID) + "/mods/" + modio::toString(mod_id);

    std::thread email_exchage_thread(modio::curlwrapper::put, call_number, url, headers, modio::getModfileCurlFormCopyContentsParams(mod_handler), &onModAdded);
    email_exchage_thread.detach();
  }

  void modioAddMod(ModioModHandler* mod_handler, void (*callback)(ModioResponse* response, ModioMod* mod))
  {
    vector<string> headers;
    headers.push_back("Authorization: Bearer " + modio::ACCESS_TOKEN);

    int call_number = modio::curlwrapper::getCallCount();
    modio::curlwrapper::advanceCallCount();

    add_mod_callback[call_number] = new AddModParams;
    add_mod_callback[call_number]->callback = callback;

    string url = modio::MODIO_URL + modio::MODIO_VERSION_PATH + "games/" + modio::toString(modio::GAME_ID) + "/mods";

    std::thread add_mod_thread(modio::curlwrapper::postForm, call_number, url, headers, modio::getModfileCurlFormCopyContentsParams(mod_handler), modio::getModfileCurlFormFilesParams(mod_handler), &onModAdded);
    add_mod_thread.detach();
  }

  void onModDeleted(int call_number, ModioResponse* response, json response_json)
  {
    ModioMod* mod = new ModioMod;
    modioInitMod(mod, response_json);
    delete_mod_callbacks[call_number]->callback(response, mod);
    delete_mod_callbacks.erase(call_number);
  }

  void modioDeleteMod(int mod_id, void (*callback)(ModioResponse* response, ModioMod* mod))
  {
    vector<string> headers;
    headers.push_back("Authorization: Bearer " + modio::ACCESS_TOKEN);

    int call_number = modio::curlwrapper::getCallCount();
    modio::curlwrapper::advanceCallCount();

    delete_mod_callbacks[call_number] = new DeleteModParams;
    delete_mod_callbacks[call_number]->callback = callback;

    string url = modio::MODIO_URL + modio::MODIO_VERSION_PATH + "games/" + modio::toString(modio::GAME_ID) + "/mods/" + modio::toString(mod_id);

    std::thread delete_mod_thread(modio::curlwrapper::deleteCall, call_number, url, headers, &onModDeleted);
    delete_mod_thread.detach();
  }

  void onSetUserModVote(int call_number, ModioResponse* response, json response_json)
  {
    set_user_mod_vote_callbacks[call_number]->callback(response, set_user_mod_vote_callbacks[call_number]->mod_id);
    set_user_mod_vote_callbacks.erase(call_number);
  }

  void modioSetUserModVote(int mod_id, bool vote_up, void (*callback)(ModioResponse* response, int mod_id))
  {
    map<string, string> data;
    data["rating"] = "-1";
    if(vote_up)
      data["rating"] = "1";

    vector<string> headers;
    headers.push_back("Authorization: Bearer " + modio::ACCESS_TOKEN);

    int call_number = modio::curlwrapper::getCallCount();
    modio::curlwrapper::advanceCallCount();

    set_user_mod_vote_callbacks[call_number] = new SetUserModVoteParams;
    set_user_mod_vote_callbacks[call_number]->callback = callback;

    string url = modio::MODIO_URL + modio::MODIO_VERSION_PATH + "games/" + modio::toString(modio::GAME_ID) + "/mods/" + modio::toString(mod_id) + "/ratings";

    std::thread set_user_mod_vote_thread(modio::curlwrapper::post, call_number, url, headers, data, &onSetUserModVote);
    set_user_mod_vote_thread.detach();
  }
}
