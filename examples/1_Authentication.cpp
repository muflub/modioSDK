#include "ModIOSDK.h"

#include <iostream>
#include <string>

int main()
{
  modio::Instance mod(7, "e91c01b8882f4affeddd56c96111977b");
  
  volatile bool finished = false;
  
  auto wait = [&]()
  {
    while (!finished)
    {
      Sleep(10);
    }
  };
  
  auto finish = [&]()
  {
    finished = true;
  };
  
  // Check to see if we have a cookie and are already logged in
  if (!mod.IsLoggedIn())
  {
    std::string email;
    
    std::cout << "Enter your email: " << std::endl;
    std::cin >> email;
    
    // Auth works by sending an email with a code. Lets trigger that now
    mod.emailRequest(email, [&](const ModioResponse& response, const std::string& message)
    {
      std::cout << "Response code: " << response.code << std::endl;
      
      if (response->code == 200)
      {
        std::cout << "Message: " << message << std::endl;
        
        std::string securityCode;
        std::cout << "Please enter the 5 digit security code: ";
        std::cin >> securityCode;
        
        // Finish the auth process by entering the security code
        mod.emailExchange(securityCode, [&](const ModioResponse& response)
        {
          std::cout << "Response code: " << response.code << std::endl;
  
          if (response->code == 200)
          {
            std::cout << "Code exchanged!" << std::endl;
          }
          else
          {
            std::cout << "Error exchanging code" << std::endl;
          }
          
          finish();
        });
      }
      else
      {
        std::cout << "Error sending code" << endl;
        finish();
      }
    });
    
    wait();
  }
  else
  {
    std::cout << "You are already logged in. Do you want to logout? (y/n)" << std::endl;
    std::string userOption;
    std::cin >> userOption;
    
    if (userOption == "y")
    {
      mod.logout();
    }
  }

  std::cout << "Process finished" << std::endl;
  return 0;
}
