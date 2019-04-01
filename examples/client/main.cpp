//importing libraries
#include "client.hpp"
#include <iostream>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>


//using namespace CLIENT;

int main()
{
  try
    {
    boost::asio::io_service io_service;  
    CLIENT::Client CLIENTclient(io_service);
    io_service.run();
    }
  catch(std::exception& e)
    {
    std::cerr << e.what() << std::endl;
    }
  return 0;
}
