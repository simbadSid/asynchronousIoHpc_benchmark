CC     	= g++# scorep-g++
DIR_AUX	= ../srcBenchmark/auxiliary
CFLAGS 	= -Wall -g -pthread -Werror -lrt
EXEC   	=	test1Write							\
			testNWrite_Naiocb_Nbuffer			\
			testNWrite_1aiocb_1buffer			\
			testNWrite_1aiocb_Nbuffer			\
			testNWrite_Naiocb_Nbuffer_Handler	\
			testNWrite_1aiocb_Nbuffer_Handler

all   	: $(EXEC)





#-----------------------------------------------------------------------------------------------------------
# --------------------------------------- Executables ------------------------------------------------------
#-----------------------------------------------------------------------------------------------------------
test1Write					: test1Write.o util.o
							$(CC) $(CFLAGS) -o $@ $^ ;	\
							./$(@)


testNWrite_Naiocb_Nbuffer	: testNWrite_Naiocb_Nbuffer.o util.o
							$(CC) $(CFLAGS) -o $@ $^ ;	\
							./$(@)


testNWrite_1aiocb_1buffer	: testNWrite_1aiocb_1buffer.o util.o
							$(CC) $(CFLAGS) -o $@ $^ ;	\
							./$(@)


testNWrite_1aiocb_Nbuffer	: testNWrite_1aiocb_Nbuffer.o util.o
							$(CC) $(CFLAGS) -o $@ $^ ;	\
							./$(@)


testNWrite_1aiocb_Nbuffer_Handler	: testNWrite_1aiocb_Nbuffer_Handler.o util.o
							$(CC) $(CFLAGS) -o $@ $^ ;	\
							./$(@)


testNWrite_Naiocb_Nbuffer_Handler	: testNWrite_Naiocb_Nbuffer_Handler.o util.o
							$(CC) $(CFLAGS) -o $@ $^ ;	\
							./$(@)


#-----------------------------------------------------------------------------------------------------------
# --------------------------------------- Binaries ---------------------------------------------------------
#-----------------------------------------------------------------------------------------------------------
%.o				: %.cc
				$(CC) $(CFLAGS) -c -o $@ $^

%.o				: $(DIR_AUX)/%.cc
				$(CC) $(CFLAGS) -c -o $@ $^


#-----------------------------------------------------------------------------------------------------------
#---------------------------------------- General Methodes -------------------------------------------------
#-----------------------------------------------------------------------------------------------------------
.PHONY	: clean mrproper cleanJube

clean:
	rm -f *.o

mrproper: clean
	rm -rf $(EXEC) pts_aio_write.data
