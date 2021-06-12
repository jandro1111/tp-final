#pragma once
// INCLUDES	//
#include<iostream>
#include<string>
#include <exception>
#include <list>
#include <curl/curl.h>
#include "json.hpp"
#include<ostream>
#include<fstream>
#include<sstream>
#include<boost/format.hpp>
#include<iomanip>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include<curl/curl.h>
#include <boost\bind.hpp>
#include <math.h>
//	DEFINES	//
const nlohmann::json nulljson ;//supongamos que es un define
#define SERVERP 80
#define CLIENTEP 81
#define BLOCK_ID_LENGTH 8
enum REQUESTTYPE { INVALID_REQUEST = 0, SEND_BLOCK, SEND_TX, SEND_MERKLE_ROOT, SEND_FILTER, GET_BLOCKS, GET_BLOCK_HEADER };//
enum IMGUIREF {CONECTIONOK, RESPONSEOK, RESPONSEFAIL, CANTCONECT, CANCONECT, BADPARAMETERS, CONNECTIONFINISHED};
//	STRUCT	//
