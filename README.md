# ft_irc

This project is an implementation of a basic IRC server, adhering to a subset of the IRC RFC specifications (primarily RFC 2810, 2811, 2812, 2813, and 7194). It's part of the 42 School curriculum, designed to teach network programming, concurrent handling, and server-side development.

## Features

*   **Basic Connection Handling:** Accepts and manages client connections using `socket`, `bind`, `listen`, `accept`, and `poll`.
*   **User Authentication:** Requires a server password for initial connection and supports `NICK` and `USER` commands for registration.
*   **Channel Management:**
    *   Creation and joining of channels (`JOIN`).
    *   Listing available channels (`LIST`).
    *   Leaving channels (`PART`).
    *   Sending messages to channels (`PRIVMSG`).
    *   Channel topics (`TOPIC`).
    *   Channel modes (invite-only, key, user limit, topic-setting restrictions).
*   **Message Handling:**
    *   Sending private messages to other users (`PRIVMSG`).
    *   Basic command parsing and execution.
*   **Operator Privileges:**  Channel operators can kick users (`KICK`), invite users (`INVITE`) and change channel modes (`MODE`).
*   **Information Commands:**
    *   `WHOIS`:  Retrieves information about a specific user.
    *   `INFO`: Displays server information.
    *   `ADMIN`: Displays administrative information about the server.
    *   `VERSION`: Displays server version.
*   **Utility Commands:**
    *   `PING`/`PONG`:  Keeps the connection alive.
    *   `QUIT`: Disconnects the client.
    *   `NAMES`:  Lists users in a channel.
*   **Logging:** Logs server activity to a file.
*   **Error Handling:** Implements several error replies as defined in the IRC specifications.

## Project Structure

The project is organized into the following directories:

*   **`tham-le-ft_irc/`**
    *   **`Makefile`**:  The makefile for compiling the project.
    *   **`includes/`**:  Contains all the header files (`.hpp`).
        *   `Channel.hpp`:  Definition of the `Channel` class.
        *   `Command.hpp`:  Definition of the `Command` class.
        *   `Config.hpp`:  Definition of the `Config` class.
        *   `Ircserv.hpp`:  Definition of the `Ircserv` class.
        *   `ReplyCommand.hpp`: Defines reply codes and messages.
        *   `User.hpp`:  Definition of the `User` class.
    *   **`srcs/`**:  Contains all the source files (`.cpp`).
        *   `Channel.cpp`:  Implementation of the `Channel` class.
        *   `Config.cpp`:  Implementation of the `Config` class.
        *   `Ircserv.cpp`:  Implementation of the `Ircserv` class.
        *   `User.cpp`:  Implementation of the `User` class.
        *   `User_printMessage.cpp`: Implementation of User's message handling.
        *   `main.cpp`:  The main entry point of the server.
        *   **`srcs/Commands/`**:  Contains the implementation of the IRC commands.
            *   `ADMIN.cpp`
            *   `CAP.cpp`
            *   `Command.cpp`:  Base class for all commands.
            *   `INFO.cpp`
            *   `INVITE.cpp`
            *   `JOIN.cpp`
            *   `KICK.cpp`
            *   `LIST.cpp`
            *   `MODE.cpp`
            *   `NAMES.cpp`
            *   `NICK.cpp`
            *   `PART.cpp`
            *   `PASS.cpp`
            *   `PINGPONG.cpp`
            *   `PRIVMSG.cpp`
            *   `QUIT.cpp`
            *   `TOPIC.cpp`
            *   `USER.cpp`
            *   `VERSION.cpp`
            *   `WHOIS.cpp`

## Compilation

To compile the project, use the following command:

```bash
make
```

This will create an executable file named `ircserv` in the main directory.

## Usage

To run the IRC server, use the following command:

```bash
./ircserv <port> <password>
```

*   `<port>`:  The port number the server will listen on.
*   `<password>`: The server password required for clients to connect.

Example:

```bash
./ircserv 6667 mysecretpassword
```

## Classes Description

*   **`Ircserv`**: Represents the IRC server itself. Manages client connections, channels, and the main event loop.
*   **`User`**: Represents a connected client. Stores user information (nickname, username, realname, etc.) and manages communication with the client.
*   **`Channel`**: Represents an IRC channel. Stores channel information (name, topic, mode, etc.) and manages users within the channel.
*   **`Command`**: Base class for handling IRC commands. Parses commands and calls the appropriate function.
*   **`Config`**: Holds the server configuration parameters (port, password, etc.).

## Implemented Commands

The following IRC commands are implemented:

*   `PASS`: Sets the connection password.
*   `NICK`: Sets the user's nickname.
*   `USER`: Sets the user's username, hostname, and realname.
*   `JOIN`: Joins a channel.
*   `PART`: Leaves a channel.
*   `PRIVMSG`: Sends a private message to a user or channel.
*   `PING`: Sends a ping to the server.
*   `PONG`: Responds to a ping from the server.
*   `QUIT`: Disconnects from the server.
*   `LIST`: Lists available channels.
*   `NAMES`: Lists users in a channel.
*   `WHOIS`: Retrieves information about a user.
*   `ADMIN`: Retrieves information about the server administrator.
*   `INFO`: Retrieves server information.
*   `VERSION`: Retrieves server version information.
*   `KICK`: Kicks a user from a channel.
*   `INVITE`: Invites a user to a channel.
*   `TOPIC`: Sets or displays the channel topic.
*   `MODE`: Sets or displays channel modes.
*   `CAP`: Capability negotiation (minimal support).

## Notes

*   This project is a simplified implementation of an IRC server and does not implement the full IRC specification.
*   Error handling is limited.
*   The project adheres to the C++98 standard.

## Authors

*   [Christine Qin](https://github.com/cqin42) (cqin)
*   [Tham Le](https://github.com/thile42) (thi-le)
*   [Yulia Boktaeva](https://github.com/yuboktae42) (yuboktae)

