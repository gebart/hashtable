# Build test
ELFS = $(notdir $(CURDIR)).elf
DEPS = $(notdir $(CURDIR)).d

.PHONY: all
all: $(ELFS)

LDPREFIX = -Wl,

LINKFLAGS += $(LDPREFIX)--gc-sections

CFLAGS += -g3 -ggdb -O2 -fno-common -ffunction-sections -fdata-sections
CFLAGS += -Wall -Wextra -Wpedantic -Werror -Wno-error=pedantic -MD -MP

-include $(DEPS)
%.c : Makefile

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.elf : %.o
	$(CC) $(LINKFLAGS) -o $@ $<
