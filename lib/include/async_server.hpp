#ifndef ASYNC_SERVER
#define ASYNC_SERVER


//importing libraries
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace SERVER
{

using namespace boost::asio;
using ip::tcp;
using std::cout;
using std::endl;

class con_handler;
typedef boost::shared_ptr<con_handler> sp_con_handler;

class con_handler: public boost::enable_shared_from_this<con_handler>
{
private:
  tcp::socket sock;
  std::string message;
  enum { max_length = 1024 };
  char IncomStr[max_length];
  static const std::string BasePath;
  static const std::string RegexFilter;


public:


  con_handler(boost::asio::io_service& io_service):
  sock(io_service), message("Hello From Server!")
  {

  };
// creating the pointer
  static sp_con_handler create(boost::asio::io_service& io_service);

 
  void start();


  void handle_read(const boost::system::error_code& err,
                   size_t bytes_transferred);

  void handle_write(const boost::system::error_code& err, size_t bytes_transferred);

  tcp::socket& get_socket();


};

class Server 
{
private:
   tcp::acceptor acceptor_;
   void start_accept();
   
public:
//constructor for accepting connection from client
  Server(boost::asio::io_service& io_service);

  void handle_accept(sp_con_handler connection,
                     const boost::system::error_code& err);
  
};

}
#endif
