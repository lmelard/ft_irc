/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaillag <cgaillag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 15:18:54 by lmelard           #+#    #+#             */
/*   Updated: 2023/07/10 18:55:00 by cgaillag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"
#include "Socket.hpp"
#include "defines.hpp"
#include "utils.hpp"

// /**
//  * @brief       Forbidden inet_ntop implementation
//  */

// std::string ntop( const struct sockaddr_storage& socket ) {
//   std::stringstream ss;

//   if( socket.ss_family == AF_INET ) {
//     const struct sockaddr_in* sockaddr;
//     const uint8_t*            addr;
//     sockaddr = reinterpret_cast<const struct sockaddr_in*>( &socket );
//     addr = reinterpret_cast<const uint8_t*>( &sockaddr->sin_addr.s_addr );
//     ss << static_cast<int>( addr[0] ) << ".";
//     ss << static_cast<int>( addr[1] ) << ".";
//     ss << static_cast<int>( addr[2] ) << ".";
//     ss << static_cast<int>( addr[3] ) << ".";
//     return ss.str();
//   } else if( socket.ss_family == AF_INET6 ) {
//     const struct sockaddr_in6* sockaddr;
//     const uint8_t*             addr;
//     sockaddr = reinterpret_cast<const struct sockaddr_in6*>( &socket );
//     addr = sockaddr->sin6_addr.s6_addr;
//     for( int i = 0; i < 16; ++i ) {
//       if( i > 0 && i % 2 == 0 ) {
//         ss << "::";
//       }
//       ss << static_cast<int>( addr[i] );
//     }
//     return ss.str();
//   } else {
//     return std::string( "UNKN_ADDR" );
//   }
// }

#include <cstdlib>		// std::atoi()
#include <iostream>		// std::cout
#include <stdexcept>	// std::invalid_argument, std::runtime_error...

void	checkArgs(int argc, char **argv)
{
	/* check number of arguments */
	if (argc != 3)
		throw std::invalid_argument("incorrect number of arguments");

	/* check <port> is a port number */
	std::string	port(argv[1]);
	if (port.find_first_not_of("0123456789") != std::string::npos \
		|| port.empty())
		throw std::invalid_argument("wrong port number");

	long val = std::atol(argv[1]);
	if (val < 0 || val > 65535)
		throw std::invalid_argument("wrong port number");

	/* check <password> is existing & valid */
	std::string	password = argv[2];

	if (password.empty())
		throw std::invalid_argument("a password is required");
	else if (password.length() < PASSMINLEN || password.length() > PASSMAXLEN)
		throw std::invalid_argument("password length is out of bounds");
	else if (!isValidUser(password))
		throw std::invalid_argument("a password contains invalid characters");

	return ;
}

int	main(int argc, char **argv)
{
	try
	{
		/* ***** CHECK NB of ARG and VALUES OF ARGV[1] & ARGV[2] ***** */
		checkArgs(argc, argv);
		/* if incorrect --> throw an exception invalid_argument */

		/* ***** INIT REQUIRED PARAMETERS FOR THE SERVER CLASS ***** */
		/* Classe Server avec 2 params: port et password, garder password en char* */
		std::size_t	port = std::atoi(argv[1]);
		const char	*password = argv[2];

		Server server(port, password, "tiny_server");
		server.run();
	}
	catch (const std::invalid_argument& ia)
	{
        std::cerr << "Invalid argument: " << ia.what() << std::endl;
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return EXIT_FAILURE;
	}
	catch(std::runtime_error &e)
	{
		std::cout << "Exception: " << e.what() << std::endl;
	}
	return EXIT_SUCCESS;
}
