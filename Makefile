# =============================================================================
# cross-platform makefile for c/c++ projects with raylib
# based on:
#   https://makefiletutorial.com/
#   https://spin.atomicobject.com/2016/08/26/makefile-c-projects/
#
# usage:
#   make               - compile and run the project
#   make compile       - compile the project only
#   make run           - run the compiled binary only
#   make clean         - remove build artifacts
#   make cleanandcompile - clean and recompile from scratch
#
# author: prof. dr. david buzatto
# =============================================================================


# -----------------------------------------------------------------------------
# compiler
# -----------------------------------------------------------------------------
CC  := gcc
CXX := g++

# -----------------------------------------------------------------------------
# executable name
# -----------------------------------------------------------------------------
empty :=
space := $(empty) $(empty)

__escaped_path := $(subst $(space),__sp__,$(subst \,/,$(CURDIR)))

target_name := $(subst __sp__,_,$(notdir $(__escaped_path)))

target_name := $(subst á,a,$(target_name))
target_name := $(subst à,a,$(target_name))
target_name := $(subst ã,a,$(target_name))
target_name := $(subst â,a,$(target_name))
target_name := $(subst ä,a,$(target_name))
target_name := $(subst é,e,$(target_name))
target_name := $(subst è,e,$(target_name))
target_name := $(subst ê,e,$(target_name))
target_name := $(subst ë,e,$(target_name))
target_name := $(subst í,i,$(target_name))
target_name := $(subst ì,i,$(target_name))
target_name := $(subst î,i,$(target_name))
target_name := $(subst ï,i,$(target_name))
target_name := $(subst ó,o,$(target_name))
target_name := $(subst ò,o,$(target_name))
target_name := $(subst õ,o,$(target_name))
target_name := $(subst ô,o,$(target_name))
target_name := $(subst ö,o,$(target_name))
target_name := $(subst ú,u,$(target_name))
target_name := $(subst ù,u,$(target_name))
target_name := $(subst û,u,$(target_name))
target_name := $(subst ü,u,$(target_name))
target_name := $(subst ç,c,$(target_name))
target_name := $(subst ñ,n,$(target_name))

target_exec := $(target_name)


# -----------------------------------------------------------------------------
# main directories
# -----------------------------------------------------------------------------
build_dir := build
src_dirs  := src


# -----------------------------------------------------------------------------
# recursive file search
# -----------------------------------------------------------------------------
rwildcard = $(foreach d,$(wildcard $(1:=/*)),\
                $(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))


# -----------------------------------------------------------------------------
# source files
# -----------------------------------------------------------------------------
srcs := $(call rwildcard,$(src_dirs),*.c *.cpp *.s)


# -----------------------------------------------------------------------------
# object and dependency files
# -----------------------------------------------------------------------------
objs := $(srcs:%=$(build_dir)/%.o)
deps := $(objs:.o=.d)


# -----------------------------------------------------------------------------
# include directories
# -----------------------------------------------------------------------------
inc_dirs  := $(sort $(dir $(call rwildcard,$(src_dirs),*)))
inc_flags := $(addprefix -I,$(inc_dirs))


# -----------------------------------------------------------------------------
# compiler flags
# -----------------------------------------------------------------------------
cflags   := $(inc_flags) -MMD -MP -O1 -Wall -Wextra \
             -Wno-unused-parameter -pedantic-errors \
             -std=gnu99 -Wno-missing-braces

cppflags := $(inc_flags) -MMD -MP -O1 -Wall -Wextra \
             -Wno-unused-parameter -pedantic-errors \
             -std=c++20 -Wno-missing-braces


# -----------------------------------------------------------------------------
# linker flags
# -----------------------------------------------------------------------------
ldflags := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11


# =============================================================================
# targets
# =============================================================================
.phony: all compile run clean cleanandcompile

all: compile run

compile: $(build_dir)/$(target_exec)

cleanandcompile: clean compile


# -----------------------------------------------------------------------------
# final link rule
# -----------------------------------------------------------------------------
$(build_dir)/$(target_exec): $(objs)
	@mkdir -p $(build_dir)
	$(CXX) $^ -o $@ $(ldflags)


# -----------------------------------------------------------------------------
# compilation rule for c files
# -----------------------------------------------------------------------------
$(build_dir)/%.c.o: %.c
	@mkdir -p $(@D)
	$(CC) $(cflags) -c $< -o $@


# -----------------------------------------------------------------------------
# compilation rule for c++ files
# -----------------------------------------------------------------------------
$(build_dir)/%.cpp.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(cppflags) $(cxxflags) -c $< -o $@


# -----------------------------------------------------------------------------
# clean target
# -----------------------------------------------------------------------------
clean:
	@rm -rf $(build_dir)


# -----------------------------------------------------------------------------
# run target
# -----------------------------------------------------------------------------
run:
	./$(build_dir)/$(target_exec) $(args)


# -----------------------------------------------------------------------------
# dependency file inclusion
# -----------------------------------------------------------------------------
-include $(deps)
