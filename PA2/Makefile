#------------------------------------------------------------------
# SWE2007: Software Experiment II (2018 Fall)
#
# Skeleton code for PA #2
# October 10, 2018
#
# Somm Kim, Sooyun Lee
# Embedded Software Laboratory
# Sungkyunkwan University
#
# Student Name  :
# Student ID No.:
#------------------------------------------------------------------

CC		= gcc
CFLAGS	= 
LDFLAGS	=
RM		= rm
TAR     = tar

CSRCS	= collect.c
TARGET	= collect
HEADER  = 
OBJECTS	= $(CSRCS:.c=.o)

# Use your own student ID
STUDENT_ID = 2018310000

all: $(TARGET)

$(TARGET): $(OBJECTS)
		$(CC) $(LDFLAGS) $(OBJECTS) $(HEADER) -o $@

.c.o:
		$(CC) $(CFLAGS) -c $< -o $@

tar:
		$(TAR) -cvzf $(STUDENT_ID).tar.gz Makefile $(CSRCS) $(HEADER) $(STUDENT_ID).pdf 

clean:
		$(RM) -f $(OBJECTS) $(TARGET) *~
