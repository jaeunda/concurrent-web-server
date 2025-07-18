# concurrent-web-serever
A high-performance web server implemented with C, supporting multiple concurrency models (epoll, thread, process) - build from scratch with no external libraries.


### Project Structure
```
concurrent-web-server
├── Makefile      		# Build System
├── README.md     		# Project Overview
├── docs          		# Documentation and Reports
│   ├── troubleshooting
│   └── notes
├── public        		# Static files served by the web server
├── src           		# Source Code
│   ├── http      		    # HTTP protocol handling code
│   ├── include       		# Header files
│   ├── server        		# Core server logic
│   ├── socket        		# Low-level socket and network-related functions
│   └── util          		# Utility functions
└── tests         		# Test code and scripts
```

### Models & Progress

| Module                                      | Status        |
|--------------------------------------------|---------------|
| Blocking TCP Server                        | Planned        |
| Multi-threaded Server                      | Planned        |
| Epoll-based Concurrent Server              | Planned        |
| Benchmarking & Load Testing                | Planned        |
| Performance Analysis & Visualization       | Planned       |


- See [`docs/project-progress.md`](docs/project-progress.md) for the full checklist.

### Build & Run
will be added after initial implemented is complete.
