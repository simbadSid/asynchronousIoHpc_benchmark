CC						= scorep-g++ -std=c++11
#CC						= g++ -std=c++11
CC_windows				= x86_64-w64-mingw32-g++ -std=c++11
#CC_windows				= g++
CC_MPI					= mpic++ -std=c++11 -fopenmp #scorep-mpic++ -std=c++11 -fopenmp

SRC_DIR					= srcBenchmark/
BIN_DIR					= bin/
RESOURCE_DIR			= ../resource
RESOURCE_IO				= $(RESOURCE_DIR)/ioFiles
RESOURCE_DOC			= $(RESOURCE_DIR)/documentation
RESOURCE_STAT			= $(RESOURCE_DIR)/statistic
RESOURCE_SCOREP			= scorep-*
JUBE_DIR				= jubeTester
JUBE_OUTPUT				=	$(JUBE_DIR)/bench_run \
							$(JUBE_DIR)/jube-parse.log
CFLAGS_UNIX				= -Wall -g -Werror -lrt -pthread
CFLAGS_WINDOWS			= -Wall -g -Werror -static-libstdc++ -static-libgcc #-ILIBSTDC++-6.DLL

EXEC_UNIX				=	posixGlibcIO_sleep					\
							posixGlibcAIO_sleep					\
							posixGlibcAIO_sleep_noSignal		\
							posixGlibcIO_sleep_memoryFootprint	\
							posixGlibcAIO_sleep_memoryFootprint
#							posixGlibcIO_sleep_mpi				\
#							posixGlibcAIO_sleep_mpi
EXEC_WINDOWS			=	windowsIO_sleep.exe					\
							windowsAIO_sleep.exe

all_unix   				: $(EXEC_UNIX)
all_windows				: $(EXEC_WINDOWS)
all		   				: $(EXEC_UNIX)



hackForWine				:
						ln -s /usr/x86_64-w64-mingw32/sys-root/mingw/bin/libwinpthread-1.dll ./libwinpthread-1.dll


#-----------------------------------------------------------------------------------------------------------
# --------------------------------------- Run tests --------------------------------------------------------
#-----------------------------------------------------------------------------------------------------------
runAllBenchmark						: all
									cd $(JUBE_DIR);																	\
									jube run benchmarkInstrumentation.xml --only-bench init_outputFile;				\
									jube run benchmarkInstrumentation.xml --only-bench run_benchmark


runAllBenchmarkHPC					: all
									cd $(JUBE_DIR);																	\
									jube run benchmarkInstrumentation_hpc.xml --only-bench init_outputFile;			\
									jube run benchmarkInstrumentation_hpc.xml --only-bench run_benchmark


runWriteTimeBenchmark				: all
									cd $(JUBE_DIR);																	\
									jube run benchmarkInstrumentation_writeTime.xml --only-bench init_outputFile;	\
									jube run benchmarkInstrumentation_writeTime.xml --only-bench run_benchmark


runWriteTimeBenchmark_row			: all
									cd $(JUBE_DIR);																	\
									jube run benchmarkInstrumentation_writeTime.xml --only-bench init_outputFile;	\
									jube run benchmarkInstrumentation_writeTime.xml --only-bench run_benchmark_row


runRequestBenchmark					: all
									cd $(JUBE_DIR);																	\
									jube run benchmarkInstrumentation_requestTime.xml --only-bench init_outputFile;	\
									jube run benchmarkInstrumentation_requestTime.xml --only-bench run_benchmark


runRequestBenchmark_row				: all
									cd $(JUBE_DIR);																	\
									jube run benchmarkInstrumentation_requestTime.xml --only-bench init_outputFile;	\
									jube run benchmarkInstrumentation_requestTime.xml --only-bench run_benchmark_row


runMemoryFootprintBenchmark			: all
									cd $(JUBE_DIR);																	\
									jube run benchmarkInstrumentation_memoryFootprint.xml --only-bench init_outputFile;	\
									jube run benchmarkInstrumentation_memoryFootprint.xml --only-bench run_benchmark


#-----------------------------------------------------------------------------------------------------------
# --------------------------------------- Plot results -----------------------------------------------------
#-----------------------------------------------------------------------------------------------------------
plotSurface							:
									cd $(JUBE_DIR);														\
									jube run benchmarkInstrumentation.xml --only-bench plotSurface;


plotCloud							:
									cd $(JUBE_DIR);														\
									jube run benchmarkInstrumentation.xml --only-bench plotCloud;


plotBar								:
									cd $(JUBE_DIR);														\
									jube run benchmarkInstrumentation.xml --only-bench plotBar;


plotPoint							:
									cd $(JUBE_DIR);														\
									jube run benchmarkInstrumentation.xml --only-bench plotPoint;


plotBarCompare						:
									cd $(JUBE_DIR);														\
									jube run benchmarkInstrumentation.xml --only-bench plotBarCompare;


plotPointCompare					:
									cd $(JUBE_DIR);														\
									jube run benchmarkInstrumentation.xml --only-bench plotPointCompare;


plotWriteTime						:
									cd $(JUBE_DIR);														\
									jube run benchmarkInstrumentation_writeTime.xml --only-bench plotWriteTime;


plotRequestTime						:
									cd $(JUBE_DIR);														\
									jube run benchmarkInstrumentation_requestTime.xml --only-bench plotRequestTime;


plotMemoryFootprint					:
									cd $(JUBE_DIR);														\
									jube run benchmarkInstrumentation_memoryFootprint.xml --only-bench plotMemoryFootprint;


plotMemoryFootprintCompare			:
									cd $(JUBE_DIR);														\
									jube run benchmarkInstrumentation_memoryFootprint.xml --only-bench plotMemoryFootprintCompare;


plotPointHPC							:
									cd $(JUBE_DIR);														\
									jube run benchmarkInstrumentation_hpc.xml --only-bench plotPoint;


plotPointCompareHPC					:
									cd $(JUBE_DIR);														\
									jube run benchmarkInstrumentation_hpc.xml --only-bench plotPointCompare;


#-----------------------------------------------------------------------------------------------------------
# --------------------------------------- Executables Unix -------------------------------------------------
#-----------------------------------------------------------------------------------------------------------
posixGlibcIO_sleep					: $(BIN_DIR)main.o $(BIN_DIR)util.o $(BIN_DIR)worker.o $(BIN_DIR)worker_posixGlibcIO_sleep.o $(BIN_DIR)queue.o $(BIN_DIR)queue_cs.o $(BIN_DIR)handlerWriteParam.o $(BIN_DIR)synchronization_unix.o
									$(CC) $(CFLAGS_UNIX) -o $@ $^


posixGlibcAIO_sleep					: $(BIN_DIR)main.o $(BIN_DIR)util.o $(BIN_DIR)worker.o $(BIN_DIR)worker_posixGlibcAIO_sleep.o $(BIN_DIR)queue.o $(BIN_DIR)queue_cs.o $(BIN_DIR)handlerWriteParam.o $(BIN_DIR)synchronization_unix.o
									$(CC) $(CFLAGS_UNIX) -o $@ $^ 


posixGlibcIO_sleep_mpi				: $(BIN_DIR)main_mpi.o $(BIN_DIR)util_mpi.o $(BIN_DIR)worker_mpi.o $(BIN_DIR)worker_posixGlibcIO_sleep_mpi.o $(BIN_DIR)queue_mpi.o $(BIN_DIR)queue_cs_mpi.o $(BIN_DIR)handlerWriteParam_mpi.o $(BIN_DIR)synchronization_unix_mpi.o
									$(CC_MPI) $(CFLAGS_UNIX) -o $@ $^


posixGlibcAIO_sleep_mpi				: $(BIN_DIR)main_mpi.o $(BIN_DIR)util_mpi.o $(BIN_DIR)worker_mpi.o $(BIN_DIR)worker_posixGlibcAIO_sleep_mpi.o $(BIN_DIR)queue_mpi.o $(BIN_DIR)queue_cs_mpi.o $(BIN_DIR)handlerWriteParam_mpi.o $(BIN_DIR)synchronization_unix_mpi.o
									$(CC_MPI) $(CFLAGS_UNIX) -o $@ $^


posixGlibcAIO_sleep_noSignal		: $(BIN_DIR)main_noSignal.o $(BIN_DIR)util.o $(BIN_DIR)worker_noSignal.o $(BIN_DIR)worker_posixGlibcAIO_sleep_noSignal.o $(BIN_DIR)queue.o $(BIN_DIR)queue_cs.o $(BIN_DIR)handlerWriteParam.o $(BIN_DIR)synchronization_unix.o
									$(CC) $(CFLAGS_UNIX) -DNO_HANDLER_SIGNAL -o $@ $^


posixGlibcIO_sleep_memoryFootprint	: $(BIN_DIR)main_memoryFootprint.o $(BIN_DIR)util.o $(BIN_DIR)worker_memoryFootprint.o $(BIN_DIR)worker_posixGlibcIO_sleep_memoryFootprint.o $(BIN_DIR)queue.o $(BIN_DIR)queue_cs.o $(BIN_DIR)handlerWriteParam.o $(BIN_DIR)synchronization_unix.o
									$(CC) $(CFLAGS_UNIX) -o $@ $^


posixGlibcAIO_sleep_memoryFootprint	: $(BIN_DIR)main_memoryFootprint.o $(BIN_DIR)util.o $(BIN_DIR)worker_memoryFootprint.o $(BIN_DIR)worker_posixGlibcAIO_sleep_memoryFootprint.o $(BIN_DIR)queue.o $(BIN_DIR)queue_cs.o $(BIN_DIR)handlerWriteParam.o $(BIN_DIR)synchronization_unix.o
									$(CC) $(CFLAGS_UNIX) -DMEMORY_FOOTPRINT -o $@ $^


#-----------------------------------------------------------------------------------------------------------
# --------------------------------------- Executables Windows ----------------------------------------------
#-----------------------------------------------------------------------------------------------------------
windowsIO_sleep.exe					: $(BIN_DIR)main_windows.o $(BIN_DIR)util_windows.o $(BIN_DIR)worker_windows.o $(BIN_DIR)worker_windowsIO_sleep_windows.o $(BIN_DIR)queue_windows.o $(BIN_DIR)queue_cs_windows.o $(BIN_DIR)handlerWriteParam_windows.o $(BIN_DIR)synchronization_windows.o
									$(CC_windows) $(CFLAGS_WINDOWS) -DWINDOWS_PLATFORM -o $@ $^


windowsAIO_sleep.exe				: $(BIN_DIR)main_windows.o $(BIN_DIR)util_windows.o $(BIN_DIR)worker_windows.o $(BIN_DIR)worker_windowsAIO_sleep_windows.o $(BIN_DIR)queue_windows.o $(BIN_DIR)queue_cs_windows.o $(BIN_DIR)handlerWriteParam_windows.o $(BIN_DIR)synchronization_windows.o
									$(CC_windows) $(CFLAGS_WINDOWS) -DWINDOWS_PLATFORM -o $@ $^


#-----------------------------------------------------------------------------------------------------------
# --------------------------------------- Binaries Unix ----------------------------------------------------
#-----------------------------------------------------------------------------------------------------------
$(BIN_DIR)%.o						: $(SRC_DIR)%.cc
									$(CC) $(CFLAGS_UNIX) -c -o $@ $^


$(BIN_DIR)%.o						: $(SRC_DIR)auxiliary/%.cc
									$(CC) $(CFLAGS_UNIX) -c -o $@ $^


$(BIN_DIR)%.o						: $(SRC_DIR)unitPerformanceAssess/%.cc
									$(CC) $(CFLAGS_UNIX) -c -o $@ $^


$(BIN_DIR)synchronization_unix.o	: $(SRC_DIR)auxiliary/synchronization_unix.cc
									$(CC) $(CFLAGS_UNIX) -pthread -c -o $@ $^


$(BIN_DIR)%_noSignal.o				: $(SRC_DIR)unitPerformanceAssess/%.cc
									$(CC) $(CFLAGS_UNIX) -DNO_HANDLER_SIGNAL -c -o $@ $^


$(BIN_DIR)%_noSignal.o				: $(SRC_DIR)%.cc
									$(CC) $(CFLAGS_UNIX) -DNO_HANDLER_SIGNAL -c -o $@ $^


$(BIN_DIR)%_memoryFootprint.o		: $(SRC_DIR)unitPerformanceAssess/%.cc
									$(CC) $(CFLAGS_UNIX) -DMEMORY_FOOTPRINT -c -o $@ $^


$(BIN_DIR)%_memoryFootprint.o		: $(SRC_DIR)%.cc
									$(CC) $(CFLAGS_UNIX) -DMEMORY_FOOTPRINT -c -o $@ $^


$(BIN_DIR)%_mpi.o					: $(SRC_DIR)%.cc
									$(CC_MPI) $(CFLAGS_UNIX) -DMPI_PARADIGM -c -o $@ $^


$(BIN_DIR)%_mpi.o					: $(SRC_DIR)auxiliary/%.cc
									$(CC_MPI) $(CFLAGS_UNIX) -DMPI_PARADIGM -c -o $@ $^


$(BIN_DIR)%_mpi.o					: $(SRC_DIR)unitPerformanceAssess/%.cc
									$(CC_MPI) $(CFLAGS_UNIX) -DMPI_PARADIGM -c -o $@ $^


$(BIN_DIR)synchronization_unix_mpi.o: $(SRC_DIR)auxiliary/synchronization_unix.cc
									$(CC_MPI) $(CFLAGS_UNIX) -DMPI_PARADIGM -pthread -c -o $@ $^


#-----------------------------------------------------------------------------------------------------------
# --------------------------------------- Binaries Windows -------------------------------------------------
#-----------------------------------------------------------------------------------------------------------
$(BIN_DIR)%_windows.o				: $(SRC_DIR)%.cc
									$(CC_windows) $(CFLAGS_WINDOWS) -DWINDOWS_PLATFORM -c -o $@ $^


$(BIN_DIR)%_windows.o				: $(SRC_DIR)auxiliary/%.cc
									$(CC_windows) $(CFLAGS_WINDOWS) -DWINDOWS_PLATFORM -c -o $@ $^


$(BIN_DIR)%_windows.o				: $(SRC_DIR)unitPerformanceAssess/%.cc
									$(CC_windows) $(CFLAGS_WINDOWS) -DWINDOWS_PLATFORM -c -o $@ $^


$(BIN_DIR)%_windows.o				: $(SRC_DIR)unitPerformanceAssess/%.cc
									$(CC_windows) $(CFLAGS_WINDOWS) -DWINDOWS_PLATFORM -c -o $@ $^


$(BIN_DIR)%_windows.o				: $(SRC_DIR)auxiliary/%.cc
									$(CC_windows) $(CFLAGS_WINDOWS) -DWINDOWS_PLATFORM -c -o $@ $^


$(BIN_DIR)synchronization_windows.o	: $(SRC_DIR)auxiliary/synchronization_windows.cc
									$(CC_windows) $(CFLAGS_WINDOWS) -DWINDOWS_PLATFORM -c -o $@ $^


#-----------------------------------------------------------------------------------------------------------
#---------------------------------------- General Methodes -------------------------------------------------
#-----------------------------------------------------------------------------------------------------------
.PHONY								: clean mrproper cleanJube

clean								:
									rm -f $(BIN_DIR)*.o


cleanJube							:
									rm -rf $(JUBE_OUTPUT)


mrproper							: clean cleanJube
									rm -rf $(EXEC_UNIX) $(EXEC_WINDOWS) $(RESOURCE_DOC)/* $(RESOURCE_IO)/* ./libwinpthread-1.dll scorep* #$(RESOURCE_STAT)/*
