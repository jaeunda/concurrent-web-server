CC = gcc
BASE_CFLAGS = -Wall -Wextra -g -I$(SRCDIR)/include -MMD -MP
LDFLAGS = -pthread

# -Isrc/include: 헤더 파일 경로
# -MMD -MP: 의존성 파일(.d) 자동 생성, 누락된 헤더에 대한 경고 방지

TARGET = c-web-server
SRCDIR = src
OBJDIR = obj

SOURCES := $(shell find $(SRCDIR) -name '*.c')
OBJECTS := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))
DEPS := $(patsubst $(OBJDIR)/%.o, $(OBJDIR)/%.d, $(OBJECTS))

MODEL_FLAGS = -D_MODE_MAIN

.PHONY: all clean run

all: CFLAGS = $(BASE_CFLAGS)
all: $(TARGET)

blocking: MODEL_FLAGS = -D_MODE_BLOCKING
blocking : all run

multithread: MODEL_FLAGS = -D_MODE_MULTITHREAD
multithread: all run

threadpool: MODEL_FLAGS = -D_MODE_THREADPOOL
threadpool: all run

epoll: MODEL_FLAGS = -D_MODE_EPOLL
epoll: all run

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(TARGET): $(OBJECTS)
	@echo "Linking..."
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Build complete. Executable: $(TARGET)"

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(@D)
	@echo "Compiling $<..."
	$(CC) $(BASE_CFLAGS) $(MODEL_FLAGS) -c $< -o $@

clean:
	@echo "Cleaning up..."
	rm -f $(TARGET)
	rm -rf $(OBJDIR)
	@echo "Clean complete."

run: all
	@echo "Running $(TARGET)..."
	./$(TARGET)

-include $(DEPS)
