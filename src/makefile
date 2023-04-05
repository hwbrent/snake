# Define variables
SRCDIRS := food game screen snake gui
SRCFILES := $(foreach dir,$(SRCDIRS),$(wildcard $(dir)/src/*.c))
OBJFILES := $(patsubst %.c,%.o,$(SRCFILES))
LIBS := -lmenu -lncurses -lpthread

# Define phony targets
.PHONY: all clean

# Define targets and dependencies
all: main

main: $(OBJFILES) main.c
	gcc $^ $(LIBS) -o $@
	./$@

%.o: %.c
	gcc -c $< -o $@

clean:
	rm -f $(OBJFILES) main

#Explanation:
#
#    The first section defines some variables to make the makefile more flexible: SRCDIRS lists the directories where source files can be found, SRCFILES uses wildcard and foreach to generate a list of all source files, OBJFILES lists the object files that need to be generated, and LIBS lists the libraries to link against.
#    The .PHONY target declares the targets that are not actually files, but rather operations that need to be performed.
#    The all target is the default target, which depends on main.
#    The main target depends on all the object files and main.c. It uses the automatic variables $^ and $@ to refer to the list of dependencies and the target name, respectively. It also runs the executable and then the clean target using make.
#    The % wildcard is used to define a pattern rule that can generate any object file from its corresponding source file.
#    The clean target removes all object files and the main executable.
#
#Overall, this makefile is more flexible and easier to maintain, since it can handle changes to the source file structure without needing to modify the makefile itself.