# RegisterClustering
This repository implements our ISPD'19 paper: **Ya-Chu Chang, Tung-Wei Lin, Iris Hui-Ru Jiang, and Gi-Joon Nam. "Graceful Register Clustering by Effective Mean Shift Algorithm for Power and Timing Balancing."**  

# Dependecies
c++11  
[boost >= 1.58.0](https://www.boost.org/users/download/ "boost download page")  

# Units
Please be noted that the units throughout this implementation are as follows:
- Timing unit: ps
- Layout unit: 5e-4 nm  

# How to Compile 
- Download the required dependencies.
- Declare the following variables in your ```.zshrc/.bashrc``` file. (```LD_LIBRARY_PATH``` is optional.)

```
export BOOSTDIR="/home/waynelin567/boost_1_61_0"  
export LD_LIBRARY_PATH="$BOOSTDIR/stage/lib:$LD_LIBRARY_PATH"   
 ```
- Compile files:
```
make -j8 -s
```
- By default, the memory allocation algorithm used is ```jemalloc```, which has an edge over standard c++11 malloc in terms of runtime. If ```jemalloc``` is not available on your platform, you can always fall back to standard malloc by changing Makefile.  
    - By default, ```jemalloc```:  
    ```CFLAGS = -O3 $(DEPENDDIR) -fopenmp -fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free -ljemalloc -lboost_program_options```
    - Change to standard malloc:  
    ```CFLAGS = -O3 $(DEPENDDIR) -fopenmp -lboost_program_options```

# How to Run
- `./bin/clustering <input file> <output file>`
- Arguments
    - To see argument options, type `./bin/clustering -h`  
    - The full list of arguments are as follows  
    
| Argument         | Default   | Type    | Description                                                                                                                                                                                          |
|        :--:      |  :--:     | :--:    |---------------------------------------------------------|
| --M              | 4         | Integer | The M-th nearest neighbor on which the bandwidth selection will be based. |
| --K              | 140       | Integer | The number of nearest neighbors that will be taken into account when calculating the effective mean shift vector. |
| --ThreadNum      | 8         | Integer | The number of threads for parallel computing.|
| --Tol            | 0.0001    | Double  | The convergence criterion for the effective mean shift algorithm. When the shifted distance of a flip-flop across two consecutive iterations is smaller than this value, it is considered converged. |
| --Epsilon        | 5000      | Double  | The merging criterion to compensate for approximation error. The larger the epsilon, the fewer the clusters. |
| --MaxDisp        | 300000    | Double  | The maximum allowed displacement.|
| &#x2011;&#x2011;MaxBandwidth   | MaxDisp/3 | Double  | The maximum cutoff bandwidth in effective mean shift. When a flip-flop has a bandwidth larger than this value, it will be cut off to be MaxBandwidth.|
| &#x2011;&#x2011;MaxClusterSize | 80        | Integer | The maximum number of flip-flops a multi-bit flip-flop can accomodate.|
| --help           |           |         |     Show this message.                                                 |

# Quality and Performance
The reported results are tested on a workstation with 197GB memory and 2 Intel Xeon E5-2650 v2 @ 2.6GHz CPUs. All with default arguments listed above.

| Benchmark   | #Clusters | Cluster Size (Min/Max) | #Clusters of Size (1/80) | Displacement (Max/Avg) | Power Ratio | Runtime(s) |
|:-----------:|:---------:|:----------------------:|:------------------------:|:----------------------:|:-----------:|:----------:|
| superblue18 | 5794      | 1/62                   | 10/0                     | 294880/23667           | 0.7425      | 17.63      |
| superblue16 | 7976      | 1/56                   | 16/0                     | 275120/25502           | 0.7423      | 23.39      |
| superblue4  | 9378      | 1/66                   | 17/0                     | 236360/22334           | 0.7421      | 33.95      |
| superblue5  | 6307      | 1/56                   | 35/0                     | 272840/27890           | 0.7424      | 16.36      |
| superblue3  | 9221      | 1/61                   | 23/0                     | 270240/24162           | 0.7424      | 27.92      |
| superblue1  | 7883      | 1/60                   | 15/0                     | 221920/27944           | 0.7427      | 21.80      |
| superblue7  | 15013     | 1/67                   | 42/0                     | 261440/24496           | 0.7427      | 46.52      |
| superblue10 | 12870     | 1/64                   | 51/0                     | 274360/26339           | 0.7420      | 38.81      |

# Input File Format
The testcases are derived from the ICCAD 2015 Contest. If in any case you would like to use your own benchmark, please comply with the following input file format.
```
DIEAREA ( 0 0 )(1.12837e+07 6.1218e+06 )
Register_Name                                                   X              Y  Max_Rise  Max_Fall
A1_B1_C1_D1_E14_F2_G1_o491856                         8.08678e+06    2.36493e+06   3387.42   3374.43
A1_B1_C1_D1_E20_F4_o429284                            8.29882e+06    1.39365e+06   5479.04   5478.04
A1_B3_C4_D14_E23_H3_o423199                           2.38982e+06    4.13991e+06         *         *
```
- The first line states the die area.   
    - The first set of numbers is the origin coordinate of the die.  
    - The second set is the upper right corner of the die.   
    - Please note that the unit is in 5e-4 nm.   
- The second line explains the meaning of each column.   
- Starting from the third line, is the information of each register.
    - The first column is the name of the register.   
    - The second/third column is the x/y coordinate.  
    - The fourth/fifth column is the set-up slack of rise/fall signal.  
    - Sometimes, timing information is not always available. Hence the star symbol ```*``` acts as a placeholder indicating the unavailability.  

# Output File Format  
The meaning of the output file is described below.
```
DIEAREA ( 0 0 )(1.12837e+07 6.1218e+06 )
Register_Name                                                   X              Y   LABEL
A1_B1_C1_D1_E14_F2_G1_o491856                         8.08678e+06    2.36403e+06   547
A1_B1_C1_D1_E20_F4_o429284                            8.29882e+06    1.39360e+06   18
A1_B3_C4_D14_E23_H3_o423199                           8.08678e+06    2.36403e+06   547
```
- The first line is the same as the input file.
- The second line explains the meaning of each column.   
- Starting from the third line, is the clustered result of each register.
    - The first column is the name of the register.
    - The second/third column is the clustered coordinate of the register.
    - The fourth column is the cluster label to which the register belongs to. The registers with the same label will have the same XY coordinate.  
# Contacts  
Please direct any questions to this email address: ```waynelin567@gmail.com```. I will reply as soon as possible.   
