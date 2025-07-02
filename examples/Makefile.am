# Examples Makefile.am

if ENABLE_EXAMPLES

bin_PROGRAMS = basic_example fifo_example config_example

# Basic example
basic_example_SOURCES = basic_example.c
basic_example_LDADD = ../libxensiv_bgt60trxx.a
basic_example_CPPFLAGS = -I$(top_srcdir)

# FIFO example
fifo_example_SOURCES = fifo_example.c
fifo_example_LDADD = ../libxensiv_bgt60trxx.a
fifo_example_CPPFLAGS = -I$(top_srcdir)

# Config example
config_example_SOURCES = config_example.c
config_example_LDADD = ../libxensiv_bgt60trxx.a
config_example_CPPFLAGS = -I$(top_srcdir)

# Compiler flags for examples
AM_CFLAGS = -Wall -Wextra -std=c99

endif