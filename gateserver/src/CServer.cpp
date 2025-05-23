#include <iostream>
#include "gateserver/HttpConnection.h"
#include "gateserver/AsioIOServicePool.h"
#include "gateserver/CServer.h"

namespace gateserver {

CServer::CServer(boost::asio::io_context& ioc, unsigned short& port) :_ioc(ioc),
_acceptor(ioc, tcp::endpoint(tcp::v4(), port)) {}

void CServer::Start()
{	
	auto self = shared_from_this();
	auto& io_context = AsioIOServicePool::GetInstance()->GetIOService();
	std::shared_ptr<HttpConnection> new_con = std::make_shared<HttpConnection>(io_context);
	_acceptor.async_accept(new_con->GetSocket(), [self, new_con](beast::error_code ec) {
		try {
			if (ec) {
				self->Start();
				return;
			}
			new_con->Start();
			self->Start();
		}
		catch (std::exception& exp) {
			std::cout << "exception is " << exp.what() << std::endl;
			self->Start();
		}
	});
}

}
