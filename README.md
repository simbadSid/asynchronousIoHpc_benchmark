# AIO implementation assessment

This project gathers a list of benchmark pattern that allows to assess different AIO implementations (including a custom implementation).   The objective beeing to determine the best strategy to optimize the time/memory footprint of a basic scientific computation application (loop arround 2 operations: compute and save in file).   Each benchmark maybe tuned according to the following parameters (non exhaustive list):</br>
  1. Parallel thread for the computation</br>
  2. Signal-based interruption of the main thread at each effective write in memory </br>
  3. Mutex-based synchronization between the main thread and the asynchronous writing thread</br>
  4. Group the events (end of effective write in memory) to be treated asynchronously</br>
Different adaptations of each strategy have been implemented to be portable on UNIX/Windows/MacOsX platforms (the process/synchronization mechanisms used by each platform is fundamentally different).</br>
More information about this project maybe found at my personal notes <a>https://www.overleaf.com/8742826mfxxwnbysvwh#/31188663/</a> or on my thesis <a>https://www.overleaf.com/9761147nyqtxfwyvncp#/35622564/</a></br>
The project is made of three main parts:

## The benchmark implementation (directory ./srcBenchmark)
This directory contains a list of benchmark pattern.
Each pattern is implemented in the directory src/unitPerformanceAssess and extends the C++ class "Worker".
Each implementation has a specific "Compute method".  It also has a specific "save" method that allows to write in a given file (using different AIO implementations).</br>
A main programs (file src/main.cc) allows to run one of this pattern in a loop after fixing the benchmark parameters (using the command line). </br>
Each implemented pattern may be generated from the root directory using the commend: "make pattern_<pattern name>"
Then the pattern may be tested using the command.</br>
"./pattern_<patter name> [optional parameters]" </br>
The different parameters may be see by using the option "-help".



## The benchmark pattern instrumentation(./jubeTester)
The Jube file "jubeTester/benchmarkInstrumentation.xml" runs the different pattern benchmarks previously described and assess them.
The comand for that is: "jube run benchmarkInstrumentation.xml".</br>
In order to print all the defined benchmark, one can simply go to the root directory and run the command</br>
"make run<pattern name>"



## The result printer
Once the desired test has been succesfully run (see previous command), one can print the result by runing "make plot<pattern name>"


