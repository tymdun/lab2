TARGET   = tcp_client

CC       = gcc
CFLAGS   = -std=c11 -Wall -Wextra -g -DLOG_USE_COLOR

LINKER   = gcc
LFLAGS   =

SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

$(BINDIR)/$(TARGET): $(OBJECTS)
	$(LINKER) $(OBJECTS) $(LFLAGS) -o $@

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

test:
	docker run -it --rm -v "$(PWD)":/test ecen493r_lab_tester -v

test-verbose:
	docker run -it --rm -v "$(PWD)":/test ecen493r_lab_tester -vs

clean:
	$(RM) $(OBJECTS)
	$(RM) $(BINDIR)/$(TARGET)
