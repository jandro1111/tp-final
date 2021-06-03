#pragma once
#include "Header.h"
#include "blockchain.h"
class server
{
public:
	server(boost::asio::io_context& context,int port);
	~server();
	void start();
private:
	void start_waiting_connection();
	void start_answering(int);
	void connection_received_cb(const boost::system::error_code& error);
	void response_sent_cb(const boost::system::error_code& error, size_t bytes_sent);
	std::string msg;

	boost::asio::io_context& context_;
	boost::asio::ip::tcp::socket socket_;
	boost::asio::ip::tcp::acceptor acceptor_;

	nlohmann::json block;
	nlohmann::json tx;
	nlohmann::json merkleblock;
	nlohmann::json filster;
};

