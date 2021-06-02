#include "server.h"


using boost::asio::ip::tcp;
std::string make_response_string(int request);


server::server(boost::asio::io_context& io_context)
	: context_(io_context),
	acceptor_(io_context, tcp::endpoint(tcp::v4(), CLIENTEP)),//ese de ahi es el numero del puerto
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

void server::start_answering(int aux)
{
	std::cout << "start_answering()" << std::endl;
	msg = make_response_string(aux);
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
		boost::array<char, 128> buf;
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
		// Lo vamos imprimiendo
		bool fin = false;
		bool ini = false;
		for (int i = 0; fin == false; ++i) {//aislo el archivo a buscar
			if (linea[i] == '/' || ini == true) {//encontre el inicio
				if (ini == false) {
				}
				else {
					aux += linea[i];
				}
				ini = true;
			}
			if (ini == true) {
				if (linea[i] == ' ') {//encontre el final
					fin = true;
				}
			}
		}
		if (fin == true) {
			break;
		}
	}
	int option=0;
	handler.close();
	//en aux tengo el path a buscar


	std::cout << "connection_received_cb()" << std::endl;
	if (!error) {
		start_answering(option);
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


std::string make_response_string(int request)//aca armamos el mensaje en aux tengo el path a buscar
{
#pragma warning(disable : 4996)
	std::string aux ="";
	int header = 0;//sacar cuando tenga blockheader
	int cant = 2;//sacar cuando tenga parser
	blockchain algo("ejemplo.json");
	std::string res;
	res += "HTTP/1.1 200 OK";
	res += "\r\n";
	res += "Content-Length : ";
	switch (request) {
	case 0:
		aux = str(boost::format("{\"status\": true,\"result\":%1%}")%algo.getblocks(cant));
		break;
	case 1:
		aux = str(boost::format("{\"status\": true,\"result\":%1%}")%header);
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

