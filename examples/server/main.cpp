//importing libraries

#include "async_server.hpp"
#include <iostream>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

using namespace SERVER;

int main()
{
  try
    {
    boost::asio::io_service io_service;  
    Server server(io_service);
    io_service.run();
    }
  catch(std::exception& e)
    {
    std::cerr << e.what() << std::endl;
    }
  return 0;
}
