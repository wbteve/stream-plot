# Copyright (c) 2014, Sagar G V (sagar.writeme@gmail.com)
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation
# and/or other materials provided with the distribution.
#
# * Neither the name of the copyright holder nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

TARGET = streamplot

OBJDIR = obj
INCDIR = inc
BINDIR = bin
SRCDIR = src

CC = gcc

CFLAGS = -Wall -Wunused -std=c99 -O2 -I$(INCDIR)
LFLAGS = -O2 -Wl,-Bstatic -lSDL2 -Wl,-Bdynamic -lrt -ldl -lm -lpthread

#LFLAGS = -O2 -Wl,-Bstatic -lSDL2main -lSDL2 -Wl,-Bdynamic -lm -ldinput8 -lmingw32 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid

SRCS := $(wildcard $(SRCDIR)/*.c)
INCS := $(wildcard $(INCDIR)/*.h)
OBJS := $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

all: $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LFLAGS) -o $@

$(OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.c $(INCLUDES) Makefile
	$(CC) $(CFLAGS) -c $< -o $@

.PHONEY: clean
clean:
	rm -f $(OBJS) $(BINDIR)/$(TARGET)
