[![Build status](https://ci.appveyor.com/api/projects/status/e0ds2msby5qwwaqm?svg=true)](https://ci.appveyor.com/project/ameya-bhurke/torcd)

# What is torcd?
`torcd` is a simple posix compliant internet super server program that executes commands(program binaries existing on server disk) on the server based on client input. The server process executes as a daemon on the server. For every incoming request, the server spawns a new thread which forks a process as per process configuration. `torcd` is lightweight and doesn't consume a lot of resources itself and can be used in resource constrained environments.

It is pronounced as `torque d`. 

# Why torcd?
It's just some name (that sounded cool to me) and seemed like a good short form of transport orchestration, something (though not very accurate) this program does.

# What does torcd do?
`torcd` enables a client to execute commands on the server over a `TCP INET` stream socket. There are a lot of programs that do that, then, why another one? 

`torcd` is -
* simple and easy to configure using a single [libcconfig](https://hyperrealm.github.io/libconfig/) empowered configuration file. 
* lightweight
* memory efficient
* performance overhead is minimal, ~20-30 millis while executing a command directly in the shell vs torcd.

# Sample server configuration
Here's a sample `torc.cfg` file - 
```
// port number on which torcd server listens to incoming requests
port = 3699;

// process configuration
processes = (
        {
            name = "mycmdname";
            threads = 50;
            cmd = "/usr/bin/mycmdname";
            argc = 2; // optional
        }
);
```
* `port` is the server port number on which the daemon will listen for incoming requests.
* `processes` is a list of processes/ commands that `torcd` can support. 
    * `name` is the (file)name of the command. The client must pass this name to identify which process needs to be executed.
    * `threads` is max count of simultaneous processes that can be executed.
    * `cmd` is the path to where the process/ command exists on disk. e.g. for a `process` `name` `echo`, the `cmd` is `/usr/bin/echo`.
    * `argc` is the count of arguments that the command expects the client to send.

# How does the client communicate with the server? a.k.a. protocol
`torcd` uses a simple text based protocol. Let's use the example of a command called `hosttoip` which returns the ip for a host on the www. This command takes one argument.

Let's assume that the server is running on `localhost` port `3699`.
```sh
$ nc localhost 3699
$ hosttoip # new line
$ torcd> #prompt from the server confirming it's ready to receive an argument.
$ torcd>www.somevalidhost.com # new line, client sends argument
$ 99.99.99.99 # response from the server
```

The client connects to the server on a well known `host:port` and then sends the following on a newline - 
* proc/ cmd name as configured in the name param of a process in config.
* the server will then respond with a prompt confirming it's ready to receive an argument.
* 1 or more arguments separated by command line. **The exact count of arguments should be configured in the configuration file.**

The server won't execute the command until it receives the exact number of arguments.

## Input output

When the server executes a command it reroutes the `stdin` and `stdout`/`stderr` to the client socket. The client program can then effectively use the socket to interact with the command.

# Test client
There's a test client available in python [here](./client/test.py)

# Is torcd platform independent?
No. It uses POSIX api. It should work on any posix compliant system.

# How can I make torcd secure? 
You can consider using a TLS proxy like [hitch](https://hitch-tls.org/).
