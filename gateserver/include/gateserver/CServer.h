#pragma once
#include <string>
#include <memory>
#include <boost/asio.hpp>

namespace gateserver {

namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

class CServer:public std::enable_shared_from_this<CServer>
{
public:
	CServer(boost::asio::io_context& ioc, unsigned short& port);
	void Start();
private:
	tcp::acceptor  _acceptor;
	net::io_context& _ioc;
};

}
