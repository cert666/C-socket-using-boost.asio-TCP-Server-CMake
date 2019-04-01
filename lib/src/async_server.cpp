//importing libraries

#include "async_server.hpp"
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/make_shared.hpp>

using namespace SERVER;


// creating the pointer
sp_con_handler con_handler::create(boost::asio::io_service& io_service)
{
//    return sp_con_handler(new con_handler(io_service));
  return boost::make_shared<con_handler>(io_service);
}
//socket creation
tcp::socket& con_handler::get_socket()
{
 return sock;
}

void con_handler::start()
{
 sock.async_read_some(
     boost::asio::buffer(data, max_length),
     boost::bind(&con_handler::handle_read,
                 shared_from_this(),
                 boost::asio::placeholders::error,
                 boost::asio::placeholders::bytes_transferred));

 sock.async_write_some(
     boost::asio::buffer(message, max_length),
     boost::bind(&con_handler::handle_write,
               shared_from_this(),
               boost::asio::placeholders::error,
               boost::asio::placeholders::bytes_transferred));
}

void con_handler::handle_read(const boost::system::error_code& err, size_t bytes_transferred)
{
 (void)bytes_transferred;

 if (!err) {
      cout << data << endl;
 } else {
      std::cerr << "error: " << err.message() << std::endl;
      sock.close();
 }
}

void con_handler::handle_write(const boost::system::error_code& err, size_t bytes_transferred)
{
 (void)bytes_transferred;
 if (!err) {
    cout << "Server sent Hello message!"<< endl;
 } else {
    std::cerr << "error: " << err.message() << endl;
    sock.close();
 }
}




void Server::start_accept()
{
 // socket
  sp_con_handler connection = con_handler::create(acceptor_.get_io_service());

 // asynchronous accept operation and wait for a new connection.
  acceptor_.async_accept(connection->get_socket(),
     boost::bind(&Server::handle_accept, this, connection,
     boost::asio::placeholders::error));
}

//constructor for accepting connection from client
Server::Server(boost::asio::io_service& io_service): acceptor_(io_service, tcp::endpoint(tcp::v4(), 1234))
{
  start_accept();
}

void Server::handle_accept(sp_con_handler connection, const boost::system::error_code& err)
{
 if (!err) {
   connection->start();
 }
 start_accept();
}


