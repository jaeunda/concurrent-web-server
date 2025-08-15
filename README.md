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
| [Core Module Development](https://github.com/jaeunda/epoll-web-server/milestone/1)            | In Progress |
| Config Module Development          | Planned |
| Socket Module Implementation       | Planned |
| HTTP Module Implementation         | Planned |
| Client Module Development          | Planned |
| Server Module Implementation       | Planned |
| Thread Module Implementation       | Planned |
| Thread Pool Module Implementation  | Planned |
| Epoll Module Implementation        | Planned |
| Benchmarking Module Implementation | Planned |
| [DevOps and Automation](https://github.com/jaeunda/epoll-web-server/milestone/11)              | In Progress |



### Build & Run
will be added after initial implemented is complete.



