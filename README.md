# concurrent-web-serever
A high-performance web server implemented with C, supporting multiple concurrency models (epoll, thread, process) - build from scratch with no external libraries.


### Project Structure
```
epoll-web-server
├── Makefile            # Build System
├── README.md           # Project Overview
├── docs/               # Documentation and Reports
│   ├── notes/
│   └── troubleshooting/
├── include/            # Shared Header Files
├── public/             # Static files served by the web server
├── src/
│   ├── common/         # Shared Source Code
│   │   ├── http/          # HTTP Protocol Handling
│   │   ├── socket/        # Low-Level Socket and Network-Related Functions
│   │   └── util/          # Utility Functions
│   ├── launcher.c      # Main Program
│   └── servers/        # Core Server Logic
└── tests/              # Test Code and Scripts
```


### Modules & Progress

| Module                             | Status  |
| ---------------------------------- | ------- |
| [Core Module Development](https://github.com/jaeunda/epoll-web-server/milestone/1)            | Completed |
| Config Module Development          | Planned |
| [Socket Module Implementation](https://github.com/jaeunda/epoll-web-server/milestone/3)       | In Progress |
| HTTP Module Implementation         | Planned |
| Client Module Development          | Planned |
| Server Module Implementation       | Planned |
| Thread Module Implementation       | Planned |
| Thread Pool Module Implementation  | Planned |
| Epoll Module Implementation        | Planned |
| Benchmarking Module Implementation | Planned |
| [DevOps and Automation](https://github.com/jaeunda/epoll-web-server/milestone/11)              | Completed |



### Build & Run
```
$ git clone https://github.com/jaeunda/epoll-web-server.git
$ make run

$ make clean
```
#### Build Commands
```
$ make help
Usage: make [target]

== MAIN TARGETS ==
  all           Build all executables (default)
  run           Run the main launcher
  clean         Remove all build artifacts
  help          Show this help message
== DEBUGGING TARGETS ==
  block         Run the blocking server
  multi         Run the multi-thread server
  pool          Run the thread-pool server
  epoll         Run the epoll server

  You can override arguments like this:
   make epoll PORT=9090 IP=0.0.0.0
```




