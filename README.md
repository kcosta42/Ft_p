# Ft_p

Disclaimer
----------
> In School 42, almost every project must be written according to a coding syntax standard (also known as "[Norme](./docs/norme.fr.pdf)").
>
> That's why some parts may be strange

About
-----
> This introductory project on UNIX network programming invites us to realize a client-server offering identical services to ftp.

This is the first project of the Network & System Administration branch at School 42 Paris

Installation
------------
Run `make fclean && make`

Usage
-----
`./server <port>`

`./client <addr> <port>`

### Example
`./server 4242 & ./client localhost 4242`

Server Commands:
- `ls`: List directory contents
- `cp`: Copy files
- `mv`: Move files
- `cd`: Change current directory
- `get`: Retrieve a file previously store
- `put`: Store a local file into the server
- `del`: Remove a file
- `pwd`: Show current directory
- `quit`: Exit client
- `mkdir`: Make directories
- `rmdir`: Remove directories

Local Commands:
- `lls`: List local directory contents
- `lcd`: Change local current directory
- `lpwd`: Show local current directory

##### Project done in 2018
