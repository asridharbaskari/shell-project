targets := shell
objects := shell.o parse.o

CC := gcc
CFLAGS := -Wall -Werror -Wextra -MMD
CFLAGS += -g

ifneq ($(V),1)
Q = @
endif

all: $(targets)

# Dependency tracking *must* be below the 'all' rule

deps := $(patsubst %.o, %.d, $(objects))
-include $(deps)

shell: $(objects) 
	@echo "CC $@"
	$(Q)$(CC) $(CFLAGS) -o $@ $^ 

%.o: %.c
	@echo "CC $@"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@echo "CLEAN"
	$(Q)rm -rf $(targets) $(objects) $(deps) 
