#-----------------------------------------------------------
#
# SSE2033: System Software Experiment 2 (Spring 2017)
#
# Skeleton code for PA#3
# 
# May 2, 2017.
# CSLab, Sungkyunkwan University
#
# Student Id   :
# Student Name :
#
#-----------------------------------------------------------

CC		= gcc
CFLAGS	= -g -O -W -Wall
LDFLAGS	=
RM		= rm

BUILDER_CSRCS	= buildMain.c indexBuilder.c
BUILDER_TARGET	= indexBuilder
BUILDER_OBJECTS	= $(BUILDER_CSRCS:.c=.o)
CLIENT_CSRCS	= client.c
CLIENT_TARGET	= client
CLIENT_OBJECTS	= $(CLIENT_CSRCS:.c=.o)
SERVER_CSRCS	= server.c
SERVER_TARGET	= server
SERVER_OBJECTS	= $(SERVER_CSRCS:.c=.o)
TARGETS	= $(BUILDER_TARGET) $(CLIENT_TARGET) $(SERVER_TARGET)
OBJECTS	= $(CLIENT_OBJECTS) $(SERVER_OBJECTS)

all: c $(TARGETS)

$(BUILDER_TARGET): $(BUILDER_OBJECTS) $(HEADERS)
		$(CC) $(LDFLAGS) $(BUILDER_OBJECTS) -o $@

$(CLIENT_TARGET): $(CLIENT_OBJECTS) $(HEADERS)
		$(CC) $(LDFLAGS) $(CLIENT_OBJECTS) -o $@

$(SERVER_TARGET): $(SERVER_OBJECTS) $(HEADERS)
		$(CC) $(LDFLAGS) $(SERVER_OBJECTS) -o $@

.c.o:
		$(CC) $(CFLAGS) -c $< -o $@

clean:
		$(RM) -f $(OBJECTS) $(TARGETS) *~ *_* bible.txt

c:
		$(RM) -f $(OBJECTS) $(TARGETS) *~ 
