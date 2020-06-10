[![Build status](https://ci.appveyor.com/api/projects/status/e0ds2msby5qwwaqm?svg=true)](https://ci.appveyor.com/project/ameya-bhurke/torcd)

# What is torcd?
`torcd` is a simple non-secure internet based throttling transport orchestrator program. 

It is pronounced as `torque d`. 

All right. It's just some name (that sounded cool to me) and seemed like a good short form of transport orchestration, something (though not very accurate) this program does.

# What does torcd do?
`torcd` enables a client to execute commands on the server over a `TCP INET` stream socket. There are a lot of programs that do that, then, why another one? 

`torcd` is -
* simple and easy to configure using a single [libcconfig](https://hyperrealm.github.io/libconfig/) empowered configuration file. 
* lightweight
* memory efficient
* cloud ready <sub>(Dislaimer: pending tests)</sub>

# Sample configuration
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

# Is torcd platform independent?
No. It uses POSIX api. It should work on any posix compliant system.

# How can I make torcd secure? 
You can consider using a TLS proxy like [hitch](https://hitch-tls.org/).