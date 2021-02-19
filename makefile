#start:main.o fal.o fal_flash.o fal_partition.o fal_rtt.o fal_flash_port.o
#	gcc -o main main.o fal.o fal_flash.o fal_partition.o fal_rtt.o fal_flash_port.o

#CFLAGS = -I ./inc

#main.o:main.c
#	gcc -c main.c
#fal.o:./src/fal.c
#	gcc $(CFLAGS) -c ./src/fal.c
#fal_flash.o:./src/fal_flash.c
#	gcc $(CFLAGS) -c ./src/fal_flash.c
#fal_partition.o:./src/fal_partition.c
#	gcc $(CFLAGS) -c ./src/fal_partition.c
#fal_rtt.o:./src/fal_rtt.c
#	gcc $(CFLAGS) -c ./src/fal_rtt.c
#fal_flash_port.o:./src/fal_flash_port.c
#	gcc $(CFLAGS) -c ./src/fal_flash_port.c
#clean:
#	rm -rf fal.o fal_flash.o fal_partition.o fal_rtt.o

#CUR_DIR=.

APP_DIR=./App
FAL_DIR=./Fal/src
FLASHDB_DIR=./FlashDB/src

INC_DIR= -I ./Fal/inc \
		 -I ./FlashDB/inc

SRC = ${wildcard  ${APP_DIR}/*.c} \
      ${wildcard  ${FAL_DIR}/*.c} \
	  ${wildcard  ${FLASHDB_DIR}/*.c}
      
OBJ = ${patsubst %.c, %.o, ${SRC}}

TARGET=app
CC=gcc
CCFLAGS=-g -Wall ${INC_DIR}


${TARGET}: ${OBJ}
	${CC} ${OBJ} -o $@
	@echo "Compile done."

#${OBJ}:${SRC}
#   $(CC) ${CCFLAGS} -c $? 

$(OBJ):%.o:%.c
	@echo "Compiling $< ==> $@"
	${CC} ${CCFLAGS} -c $< -o $@

clean:
	@rm -f ${OBJ}
	@echo "Clean object files done."

	@rm -f *~
	@echo "Clean tempreator files done."

	@rm -f ${TARGET}
	@echo "Clean target files done."

	@echo "Clean done."
