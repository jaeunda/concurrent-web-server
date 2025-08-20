CC = gcc
# -Isrc/include: 헤더 파일 경로
CFLAGS = -Wall -Wextra -g -Iinclude
LDFLAGS = 
# -MMD -MP: 의존성 파일(.d) 자동 생성, 누락된 헤더에 대한 경고 방지
DEPFLAGS = -MMD -MP

SRCDIR = src
OBJDIR = obj
BINDIR = bin

TARGETS := $(addprefix $(BINDIR)/, \
	c-web-server \
	server-blocking \
	server-multi-thread \
	server-thread-pool \
	server-epoll \
)

COMMON_SOURCES := $(shell find $(SRCDIR)/common $(SRCDIR)/util -name '*.c')
COMMON_OBJS := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(COMMON_SOURCES))

LAUNCHER_OBJ := $(OBJDIR)/launcher.o

SERVER_SOURCES := $(shell find $(SRCDIR)/servers -name '*.c')
SERVER_OBJS := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SERVER_SOURCES))

# 왜 이렇게 쓰는지 사실 모름
DEPS := $(LAUNCHER_OBJ:.o=.d) $(SERVER_OBJS:.o=.d) $(COMMON_OBJS:.o=.d)

# Conditional Flags (pthread)
THREADED_TARGETS := $(BINDIR)/server-multi-thread $(BINDIR)/server-thread-pool
$(THREADED_TARGETS): LDFLAGS += -pthread

.PHONY: all clean run help block multi pool epoll

all: $(TARGETS)

run: $(BINDIR)/c-web-server
	@echo "Running Launcher ..."
	./$<

clean:
	@echo "Cleaning up ..."
	@rm -rf $(BINDIR) $(OBJDIR)
	@echo "Clean Complete."

help:
	@echo "Usage: make [target]"
	@echo ""
	@echo "== MAIN TARGETS =="
	@echo "  all		Build all executables (default)"
	@echo "  run		Run the main launcher"
	@echo "  clean		Remove all build artifacts"
	@echo "  help		Show this help message"
	@echo "== DEBUGGING TARGETS =="
	@echo "  block		Run the blocking server"
	@echo "  multi		Run the multi-thread server"
	@echo "  pool		Run the thread-pool server"
	@echo "  epoll		Run the epoll server"
	@echo ""
	@echo "  You can override arguments like this:"
	@echo "   make epoll PORT=9090 IP=0.0.0.0"
	@echo ""

# DEBUG
PORT ?= 8080
IP ?= 127.0.0.1
ROOT ?= ../public
THREAD_NUM ?= 4

block: $(BINDIR)/server-blocking
	./$< $(PORT) $(IP) $(ROOT) $(THREAD_NUM)

multi: $(BINDIR)/server-multi-thread
	./$< $(PORT) $(IP) $(ROOT) $(THREAD_NUM)

pool: $(BINDIR)/server-thread-pool
	./$< $(PORT) $(IP) $(ROOT) $(THREAD_NUM)

epoll: $(BINDIR)/server-epoll
	./$< $(PORT) $(IP) $(ROOT) $(THREAD_NUM)

# BUILD
$(BINDIR) $(OBJDIR):
	@mkdir -p $@

$(BINDIR)/c-web-server: $(LAUNCHER_OBJ) $(COMMON_OBJS) | $(BINDIR)
	@echo "Linking $@ ..."
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BINDIR)/server-%: $(OBJDIR)/servers/server-%.o $(COMMON_OBJS) | $(BINDIR)
	@echo "Linking $@ ..."
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@mkdir -p $(dir $@)
	@echo "Compiling $< ..."
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

-include $(DEPS)
