#include "server.h"


using boost::asio::ip::tcp;
std::string make_response_string(int request,std::string id,int cant);


server::server(boost::asio::io_context& io_context,int port)
	: context_(io_context),
	acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),//ese de ahi es el numero del puerto
	socket_(io_context)
{
}

server::~server()
{
}

void server::start()
{
	if (socket_.is_open())
	{
		socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
		socket_.close();
	}
	start_waiting_connection();
}

void server::start_waiting_connection()
{
	std::cout << "start_waiting_connection()" << std::endl;
	if (socket_.is_open())
	{
		std::cout << "Error: Can't accept new connection from an open socket" << std::endl;
		return;
	}
	acceptor_.async_accept(			//solo recibe socket que va a administrar la nueva conexion y el callback
		socket_,
		boost::bind(
			&server::connection_received_cb,
			this,
			boost::asio::placeholders::error
		)
	);
}

void server::start_answering(int aux,std::string id,int cant)
{
	std::cout << "start_answering()" << std::endl;
	msg = make_response_string(aux, id,cant);
	boost::asio::async_write(//make_daytime_string()
		socket_,
		boost::asio::buffer(msg),
		boost::bind(
			&server::response_sent_cb,
			this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
	socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
	socket_.close();
}


void server::connection_received_cb(const boost::system::error_code& error)//aca hacemos lo de buscar e interpretar el mensaje
{
	std::ofstream handler;//abro/creo si no esta/ el archivo para poner lo que reciba del server
	handler.open("handler.txt", std::ios::trunc);//borro lo que habia antes
	for (;;)//recibo lo del cliente y lo interpreto
	{
		boost::array<char,12800> buf;
		boost::system::error_code error;

		size_t len = socket_.read_some(boost::asio::buffer(buf), error);

		if (error == boost::asio::error::eof)
			break; // Connection closed cleanly by peer.
		else if (error)
			throw boost::system::system_error(error); // Some other error.

		std::cout.write(buf.data(), len);
		handler.write(buf.data(), len);//guardo en el archivo que se llama handler, para interpretar mas adelante
		handler.close();
		break;
	}
	using namespace std;
	string nombreArchivo = "handler.txt";
	ifstream archivo(nombreArchivo.c_str());
	string linea;
	string aux = "";
	// Obtener línea de archivo, y almacenar contenido en "linea"
	while (getline(archivo, linea)) {
					aux += linea;
	}
	int cant = 2;
	std::string id = "534F219B";
	std::cout << aux << std::endl;
	int option=parse_request(aux,cant,id);
	
	handler.close();
	//en aux tengo el path a buscar


	std::cout << "connection_received_cb()" << std::endl;
	if (!error) {
		start_answering(option,id,cant);
		start_waiting_connection();
	}
	else {
		std::cout << error.message() << std::endl;
	}
}

void server::response_sent_cb(const boost::system::error_code& error,
	size_t bytes_sent)
{
	std::cout << "response_sent_cb()" << std::endl;
	if (!error) {
		std::cout << "Response sent. " << bytes_sent << " bytes." << std::endl;
	}

}


std::string make_response_string(int request,std::string id,int cant)//aca armamos el mensaje en aux tengo el path a buscar
{
#pragma warning(disable : 4996)
	std::string aux ="";
	int header = 0;//sacar cuando tenga blockheader
	blockchain algo("ejemplo.json");
	std::string res;
	res += "HTTP/1.1 200 OK";
	res += "\r\n";
	res += "Content-Length : ";
	switch (request) {
	case 5:
		if (algo.searchid(id) < 0) {
			aux = "{\"status\": false,\"result\":1}";
		}
		else {
			if (id == "00000000") {
				cant=algo.getcantblock();
			}
			aux = str(boost::format("{\"status\": true,\"result\":%1%}") % algo.getblocks(cant,algo.searchid(id)));
		}
		break;
	case 6:
		if (algo.searchid(id) < 0) {
			aux = "{\"status\": false,\"result\":1}";
		}
		else {
			if (id == "00000000") {
				cant = algo.getcantblock();
			}
			aux = str(boost::format("{\"status\": true,\"result\":%1%}") % algo.getheader(cant,algo.searchid(id)));
		}
		break;
	default:
		aux = "{\"status\": true,\"result\":null}";
		break;
	}
	res += std::to_string(aux.size());
	res += "\r\n";
	res += "Content-Type : text / html; charset = iso - 8859 - 1";
	res += "\r\n";
	res += aux;
	return res;
}
int server::parse_request(std::string request, int& cant, std::string& id) {
	size_t offset;
	if ((offset = request.find("send_block")) != std::string::npos) { //Si encuentro el pedido recorto tal que solo quede el json de "request", y lo guardo.
		request = request.substr((offset + strlen("send_block/")));
		request = request.substr(0, (int)request.find("HTTP"));
		std::cout << std::endl<<request << std::endl;
		block = nlohmann::json::parse(request);
		return SEND_BLOCK;
	}
	else if ((offset = request.find("send_tx")) != std::string::npos) {
		request = request.substr(offset + strlen("send_tx/"));
		request = request.substr(0, (int)request.find("HTTP"));
		tx = nlohmann::json::parse(request);
		return SEND_TX;
	}
	else if ((offset = request.find("send_merkle_block")) != std::string::npos) {
		request = request.substr(offset + strlen("send_merkle_block/"));
		request = request.substr(0, (int)request.find("HTTP"));
		merkleblock = nlohmann::json::parse(request);
		return SEND_MERKLE_ROOT;
	}
	else if ((offset = request.find("send_filter")) != std::string::npos) {
		request = request.substr(offset + strlen("send_filter/"));
		request = request.substr(0, (int)request.find("HTTP"));
		filster = nlohmann::json::parse(request);
		return SEND_FILTER;
	}
	else if ((offset = request.find("get_blocks")) != std::string::npos) {
		std::string auxiliaryStr;
		offset += strlen("get_blocks?block_id=");//getblocks?block_id=00000000&count=2
		auxiliaryStr = request.substr(offset);
		auxiliaryStr = request.substr(0, BLOCK_ID_LENGTH);
		id = auxiliaryStr;

		auxiliaryStr = request.substr(offset + BLOCK_ID_LENGTH + strlen("&count="));
		cant = std::atoi(auxiliaryStr.c_str());
		
		return GET_BLOCKS;
	}
	else if ((offset = request.find("get_block_header")) != std::string::npos) {
		std::string auxiliaryStr;
		offset += strlen("get_block_header?block_id=");
		auxiliaryStr = request.substr(offset);
		auxiliaryStr = request.substr(0, BLOCK_ID_LENGTH);
		id = auxiliaryStr;
		
		auxiliaryStr = request.substr(offset + BLOCK_ID_LENGTH + strlen("&count="));
		cant = std::atoi(auxiliaryStr.c_str());

		return GET_BLOCK_HEADER;
	}
	else {
		std::cerr << "Header request not valid!" << std::endl;
		return INVALID_REQUEST;
	}
}
