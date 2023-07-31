/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmelard <lmelard@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 17:36:40 by cgaillag          #+#    #+#             */
/*   Updated: 2023/07/31 15:35:09 by lmelard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "defines.hpp"
#include "numericReplies.hpp"
#include "utils.hpp"

/*
	// syntax: PRIVMSG <msgtarget> <message>

	Numeric Replies:

		x	ERR_NOSUCHNICK (401)
			ERR_NOSUCHSERVER (402)
		x	ERR_CANNOTSENDTOCHAN (404)
			ERR_TOOMANYTARGETS (407)
		x	ERR_NORECIPIENT (411)
		x	ERR_NOTEXTTOSEND (412)
			ERR_NOTOPLEVEL (413)
			ERR_WILDTOPLEVEL (414)
			RPL_AWAY (301)
*/

void	Server::handlePrivmsg( int clientSocket, std::string param )
{
	Client				*client = &_clients.at( clientSocket );
	const std::string&	source = client->getSource();
	const std::string&	nickname = client->getNickname();

	std::string 		targetList = "", textToBeSent = "";

	if ( param.empty() || splitStringInTwo( param, ' ', &targetList, &textToBeSent ) == false ) {
		replyMsg( clientSocket, ERR_NORECIPIENT( source, nickname, "PRIVMSG" ) );
		replyMsg( clientSocket, ERR_NOTEXTTOSEND( source, nickname ) );
		return ;
	}
	else if ( targetList.empty() || textToBeSent.empty() ) {
		replyMsg( clientSocket, ERR_NEEDMOREPARAMS( source, nickname, "PRIVMSG" ) );
		return ;
	}

	if ( textToBeSent.at( 0 ) == ':' )
		textToBeSent.erase( 0, 1 );
	std::vector< std::string >	targetNames = splitString( targetList, ',' );
	// mettre un commentaire si targetNames.size() > TARGMAXMSG?
	// à voir et tester à 42 car irssi différent
	for ( size_t i = 0; i < targetNames.size() && i < TARGMAXMSG ; ++i ) {
		std::string	target = targetNames[ i ];
		size_t		sharp = target.find("#");

		if ( !target.empty() ) {
			std::string reply = RPL_PRIVMSG( source, nickname, target, textToBeSent );

			// if CHANNEL (#)
			if ( sharp != std::string::npos && ( sharp == 0 || sharp == 1 )) {

				std::string	channelName = target.substr( sharp );
				if ( channelName.size() > CHANNELLEN )
					channelName = channelName.substr(0, CHANNELLEN);
				// if channel does not exist
				if ( !existingChannel( channelName ) ) {
					replyMsg( clientSocket, ERR_NOSUCHCHANNEL( source, nickname, channelName ) );
					continue ;
				}

				Channel	channel = _channels.at( channelName );

				// if client is on Channel
				if (  channel.checkChannelMembers( nickname ) ) {
					// it start with '@#' --> text to be sent to chanops only
					if ( target.find( "@#" ) == 0 ) {
						channelMsgToChanOps( clientSocket, channelName, reply );
						continue ;
					}
					// start with '#' --> text to be sent to channel members
					else if ( target.find( "#" ) == 0 ) {
						channelMsgNotClient( clientSocket, target, reply );
						continue ;
					}
				}
				// client is not on the channel or channel prefixes is not '#' neither "@#"
				replyMsg( clientSocket, ERR_CANNOTSENDTOCHAN( source, nickname, target  ));
			}
			// if CLIENT
			else if ( existingClient( target ) )
				replyMsg( findClientFd( target ), reply );
			else
				replyMsg( clientSocket, ERR_NOSUCHNICK( source, target ) );
		}
		else {
			replyMsg( clientSocket, ERR_NOSUCHNICK( source, "" ) );
		}
	}
}
