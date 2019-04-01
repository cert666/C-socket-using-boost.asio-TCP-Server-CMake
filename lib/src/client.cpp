#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;


class Client
{
private:
   tcp::socket socket;

public:

   Client(boost::asio::io_service& io_service):socket(io_service)
   {}


   void start_client(std::string ip_addres, unsigned int port_num)
   {
      //connection
     socket.connect( tcp::endpoint( 
                    boost::asio::ip::address::from_string(ip_addres), port_num ));
   }

   void write_message(std::string msg)
   {
      boost::system::error_code error;
      boost::asio::write( socket, boost::asio::buffer(msg), error );
      if( !error ) {
        cout << "Client sent hello message!" << endl;
      }
      else {
        cout << "send failed: " << error.message() << endl;
      }
   }

   void get_response(const char* response)
   {
      boost::system::error_code error;
      // getting response from server
      boost::asio::streambuf receive_buffer;
      boost::asio::read(socket, receive_buffer, boost::asio::transfer_all(), error);
      if( error && error != boost::asio::error::eof ) {
        cout << "receive failed: " << error.message() << endl;
      }
      else {
        response = boost::asio::buffer_cast<const char*>(receive_buffer.data());
        cout << response << endl;
      }
   }


};

/*
int main() {
     boost::asio::io_service io_service;
//socket creation
     tcp::socket socket(io_service);
//connection
     socket.connect( tcp::endpoint( boost::asio::ip::address::from_string("127.0.0.1"), 1234 ));
// request/message from client
     const string msg = "Hello from Client!\n";
     boost::system::error_code error;
     boost::asio::write( socket, boost::asio::buffer(msg), error );
     if( !error ) {
        cout << "Client sent hello message!" << endl;
     }
     else {
        cout << "send failed: " << error.message() << endl;
     }
 // getting response from server
    boost::asio::streambuf receive_buffer;
    boost::asio::read(socket, receive_buffer, boost::asio::transfer_all(), error);
    if( error && error != boost::asio::error::eof ) {
        cout << "receive failed: " << error.message() << endl;
    }
    else {
        const char* data = boost::asio::buffer_cast<const char*>(receive_buffer.data());
        cout << data << endl;
    }
    return 0;
}*/
