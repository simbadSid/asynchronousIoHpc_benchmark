/*
 * testHelloWorld.cc
 *
 *  Created on: May 3, 2017
 *      Author: kassuskley
 */




#include <boost/lambda/lambda.hpp>
#include <iostream>
#include <iterator>
#include <algorithm>

int main()
{
    using namespace boost::lambda;
    typedef std::istream_iterator<int> in;

    std::cout << "Begining\n";
    std::for_each(
        in(std::cin), in(), std::cout << (_1 * 3) << " " );
    std::cout << "End\n";
}

void test()
{
	socket.async_connect(server_endpoint, [&](std::error_code ec)
	{
		if (!ec)
		{
			socket.async_read_some(asio::buffer(data), [&](std::error_code ec, std::size_t length)
			{
				if (!ec)
				{
					async_write(socket, asio::buffer(data, length), [&](std::error_code ec, std::size_t length)
					{
						// ...
					});
				}
			});
		}
	});
}
