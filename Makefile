CC      := x86_64-w64-mingw32-gcc
TARGET  := arithmetic.dll

SRCS    := arithmetic.c include/trapno.c
OBJS    := $(SRCS:.c=.o)

CFLAGS  := -O2 -I.

LDFLAGS := -shared -s -static-libgcc -Wl,-Bstatic -lwinpthread -Wl,-Bdynamic

.PHONY: release clean prelude

release: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

prelude:
	@bear -- make

clean:
	$(RM) $(OBJS) $(TARGET)
	$(RM) compile_commands.json
