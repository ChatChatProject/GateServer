﻿#include <iostream>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include "gateserver/CServer.h"
#include "gateserver/ConfigMgr.h"
#include "gateserver/RedisMgr.h"
#include "gateserver/MysqlMgr.h"

using namespace gateserver;

int main()
{
	try
	{
		MysqlMgr::GetInstance();
		RedisMgr::GetInstance();
		auto & gCfgMgr = ConfigMgr::Inst();
		std::string gate_port_str = gCfgMgr["GateServer"]["Port"];
		unsigned short gate_port = atoi(gate_port_str.c_str());
		net::io_context ioc{ 1 };
		boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
		signals.async_wait([&ioc](const boost::system::error_code& error, int signal_number) {
			if (error) {
				return;
			}
			ioc.stop();
			});
		std::make_shared<CServer>(ioc, gate_port)->Start();
		std::cout << "Gate Server listen on port: " << gate_port << std::endl;
		ioc.run();
		RedisMgr::GetInstance()->Close();
	}
	catch (std::exception const& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		RedisMgr::GetInstance()->Close();
		return EXIT_FAILURE;
	}
}

