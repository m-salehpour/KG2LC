# Knowledge Graphs Meet Knowledge Compilation: A Novel, Logic-Based Approach to Processing SPARQL Queries 

Welcome! This is the codebase related to the KG2LC prototype.



# To replicate the results
 1) Clone the repository
 2) Download the datasets (download links below)
 3) Check the path of downloaded files and add to them to `KG2LC.c` file
 4) Compile the project (please see below)
 5) Ensure to run the executable as a privileged user

# How to compile
We used `gcc -O2 -std=c99 KG2LC.c -Iinclude -Llib -lsdd -lm -o KG2LC.run` to compile the KG2LC prototype under Linux. Please note that `KG2LC.c` is the source code file. This directory also contains (1) a subdirectory `include` containing the header file `sddapi.h`, and (2) a subdirectory `lib` including the library file `libsdd.a`. You can also add the following flags if you are interested to play with code and modify it based on your needs: `-Werror=vla -Wextra -Wall -Wshadow -Wswitch-default -DDEBUG=*DEBEGLEVEL* -fsanitize=address -g3`

# Benchmark Datasets
| Dataset       | Link          |
| ------------- | ------------- |
| SP2Bench      | https://zenodo.org/record/5710057/files/sp2b100k_sorted.nt?download=1 |
| LUBM          | https://zenodo.org/record/5710057/files/lubm100k_sorted.nt?download=1 |

# Authors
* Masoud Salehpour (University of Sydney)
* Joseph G. Davis  (University of Sydney)
