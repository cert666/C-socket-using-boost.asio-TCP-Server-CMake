//importing libraries

#include "async_server.hpp"
#include <iostream>
#include <fstream>      // std::ifstream
#include <boost/regex.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/make_shared.hpp>


using namespace SERVER;

const std::string con_handler::BasePath="../../lib/src/";
std::string PathToHtml("");
const std::string con_handler::RegexFilter="(?<=GET /)(.*)(?= HTTP)";

// creating the pointer
sp_con_handler con_handler::create(boost::asio::io_service& io_service)
{
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
                 boost::asio::buffer(IncomStr),
                 boost::bind(&con_handler::handle_read,
                 shared_from_this(),
                 boost::asio::placeholders::error,
                 boost::asio::placeholders::bytes_transferred));


   // load binary file for fw update
   std::ifstream file;
   std::cout << "Openning file: " << PathToHtml;
   file.open(PathToHtml );

   file.seekg (0, file.end);// set pointer to the end
   auto bin_file_length = file.tellg(); // get size of file
   file.seekg (0, file.beg);// set pointer back to the begining

   cout<< "Size of web page is: " << bin_file_length << endl;

   std::stringstream strStream;
   strStream << file.rdbuf(); //read the file
   file.close();


   cout<< "Size of string of the web page is: " << strStream.str().size() <<endl;


   sock.async_write_some(
     boost::asio::buffer(strStream.str(), strStream.str().size()),
     boost::bind(&con_handler::handle_write,
               shared_from_this(),
               boost::asio::placeholders::error,
               boost::asio::placeholders::bytes_transferred));


}

void con_handler::handle_read(const boost::system::error_code& err, size_t bytes_transferred)
{
   (void)bytes_transferred;

   if (!err) {      



      std::string InComeStr;

      for(int i=0;i<max_length;i++) {
         InComeStr+=IncomStr[i];
      }
      std::cout << "\nIncomming message: \n"; 
      cout<<InComeStr;
      std::cout << "\n*******************************************\n"; 

      // filter regular expresion pattern
      const boost::regex filter(RegexFilter, boost::regex_constants::perl);


      auto words_begin = 
         boost::sregex_iterator(InComeStr.begin(),
                          InComeStr.end(), filter);

      auto words_end = boost::sregex_iterator();

      std::cout << "Found " 
           << std::distance(words_begin, words_end) 
           << " words:\n";


      // check if there is a some "xxxx" after "/" character "GET /xxxx HTTP/1.1
      if( words_begin != words_end )
      {
         std::cout << "Found " ;
         std::string match_str;
         for (boost::sregex_iterator i = words_begin; i != words_end; ++i) {
            boost::smatch match = *i;                                                 
            match_str = match.str();
            
         }   

         PathToHtml.clear();
         std::cout << match_str << '\n';
         std::cout << BasePath << '\n';
         PathToHtml = BasePath + match_str;
         std::cout << PathToHtml << '\n';



      }
      else
      {
         std::cout << "Didnt found matching patterm! \n";             
      }


   } 
   else
   {
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


