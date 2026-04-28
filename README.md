
# Password Cracker
Author: Brandon Durfee

Demo Video: https://www.youtube.com/watch?v=KtLmi5g1FrA

### About:
This is a toy pedagogical implementation of a password cracker (like hashcat or john the ripper). It provides password cracking on MD5 hashes, with three different attack types:
1. Brute force on the CPU
2. Brute force on the GPU
3. Dictionary attack on the CPU

For context, MD5 is not cryptographically secure, and modern passwords are (hopefully) hashed using Argon, or Bcrypt. These hash algorithms are also intentionally slower, reducing the efficiency of brute force attacks. Furthermore, salts are typically added, which make dictionary attacks much harder (unless you know the salt).

### Options:
1. --mode (brute or dict): Configure whether to use brute force or dictionary attacks
2. --threads: Configure number of threads
3. --gpu: Use the gpu
4. --wordlist: Set the path to the wordlist used in dictionary attacks. Wordlist must be newline separated (like ROCKYOU.txt)
5. --charset: Configure whether to use the default, advanced, ultra, or complete charsets for cracking
6. --target_digest: The target MD5 hash you are trying to crack
7. --rules: Configure dictionary rules (none or basic)
8. --length: the length of the password you are trying to crack

Charsets:
1. Default Charset: abcdefghijklmnopqrstuvwxyz
2. Advanced Charset: abcdefghijklmnopqrstuvwxyz!@*$
3. Ultra Charset: abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@*$
4. Complete Charset: abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@*$0123456789

### Build: Makefile (Linux)

```bash
module load cuda # load cuda module if necessary
make clean
make # build the cracker binary
```

### Build: Manual Compilation (Linux)

Prerequisites: NVIDIA GPU, NVCC/CUDA, openSSL, OpenMP
```bash
module load cuda # load the CUDA module if necessary
cd src
nvcc -Xcompiler -fopenmp main.cpp cuda_cracker.cu attack.cpp dict.cpp utils.cpp  -o cracker -lssl -lcrypto
```

### Examples
1. Use brute force on the CPU with length of 5 on hello's md5sum

```bash
[bdurfee3@atl1-1-03-015-2-0 passwordcracker]$ echo -n 'hello' | md5sum
5d41402abc4b2a76b9719d911017c592  -

[bdurfee3@atl1-1-03-015-2-0 passwordcracker]$ ./cracker --mode brute --target_digest 5d41402abc4b2a76b9719d911017c592 --length 5

***CONFIG***
  Mode: brute
  Threads: 1
  Length: 5
  GPU: no
  Charset: abcdefghijklmnopqrstuvwxyz
  Target Digest: 5d41402abc4b2a76b9719d911017c592

***RESULTS***
  discovered password: hello
  discovered hash: 5d41402abc4b2a76b9719d911017c592
  complexity: 11881376
  seconds to crack: 1.83837
  (ignore if early exit) hash/sec: 6.463e+06
```

2. Use dictionary attack on the CPU with rockyou.txt wordlist and basic rules
```bash
[bdurfee3@atl1-1-03-014-30-0 passwordcracker]$ echo -n 'gr8c4tch' | md5sum
9f748013af895cce7c56044e75ea3a96

[bdurfee3@atl1-1-03-015-2-0 passwordcracker]$  ./cracker --mode dict  --target_digest 9f748013af895cce7c56044e75ea3a96 --wordlist /home/hice1/bdurfee3/ECE6122/final/passwordcracker/rockyou.txt --rules basic

***CONFIG***
  Mode: dict
  Threads: 1
  Length: 4
  GPU: no
  Charset: abcdefghijklmnopqrstuvwxyz
  Wordlist: /home/hice1/bdurfee3/ECE6122/final/passwordcracker/rockyou.txt
  Rules basic
  Target Digest: 9f748013af895cce7c56044e75ea3a96

***RESULTS***
  discovered password: gr8c4tch
  discovered hash: 9f748013af895cce7c56044e75ea3a96
  complexity: 456976
  seconds to crack: 5.06381
  (ignore if early exit) hash/sec: 90243.6
```

3. Use brute force attack on GPU with the ultra charset to crack 'Bs*Z@a$'
```bash
[bdurfee3@atl1-1-03-014-30-0 passwordcracker]$ echo -n 'Bs*Z@a$' | md5sum
6ce8bf115790c2464b4d5028f264aacb 

[bdurfee3@atl1-1-03-015-2-0 passwordcracker]$ ./cracker --mode brute --target_digest 6ce8bf115790c2464b4d5028f264aacb --gpu --charset ultra --length 7

***CONFIG***
  Mode: brute
  Threads: 1
  Length: 7
  GPU: yes
  Charset: abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@*$
  Target Digest: 6ce8bf115790c2464b4d5028f264aacb

***RESULTS***
  discovered password: Bs*Z@a$
  discovered hash: 6ce8bf115790c2464b4d5028f264aacb
  complexity: 1727094849536
  seconds to crack: 134.291
  (ignore if early exit) hash/sec: 1.28609e+10
```

4. Use brute force GPU with complete charset to crack '$*3@Ba'
```bash
[bdurfee3@atl1-1-03-014-30-0 passwordcracker]$ echo -n '$*3@Ba' | md5sum
6ca49457476b776feecf461804e08464 

[bdurfee3@atl1-1-03-014-30-0 passwordcracker]$ ./cracker --mode brute --length 6 --target_digest 6ca49457476b776feecf461804e08464 --charset complete --gpu

***CONFIG***
  Mode: brute
  Threads: 1
  Length: 6
  GPU: yes
  Charset: abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@*$0123456789
  Target Digest: 6ca49457476b776feecf461804e08464

***RESULTS***
  discovered password: $*3@Ba
  discovered hash: 6ca49457476b776feecf461804e08464
  complexity: 82653950016
  seconds to crack: 7.12116
  (ignore if early exit) hash/sec: 1.16068e+10
```

### Device info
Some device info for perf comparisons

#### lscpu output (snipped)
```bash
[bdurfee3@atl1-1-01-002-8-0 passwordcracker]$ lscpu
Architecture:                x86_64
  CPU op-mode(s):            32-bit, 64-bit
  Address sizes:             46 bits physical, 48 bits virtual
  Byte Order:                Little Endian
CPU(s):                      40
  On-line CPU(s) list:       0-39
Vendor ID:                   GenuineIntel
  Model name:                Intel(R) Xeon(R) Gold 6248 CPU @ 2.50GHz
    CPU family:              6
    Model:                   85
    Thread(s) per core:      1
    Core(s) per socket:      20
    Socket(s):               2
    Stepping:                7
    CPU(s) scaling MHz:      77%
    CPU max MHz:             2500.0000
    CPU min MHz:             1000.0000
    BogoMIPS:                5000.00
<snip>
```

#### NVIDIA-SMI output:
```bash
[bdurfee3@atl1-1-01-002-8-0 passwordcracker]$ nvidia-smi
Mon Apr 27 19:14:40 2026       
+-----------------------------------------------------------------------------------------+
| NVIDIA-SMI 575.57.08              Driver Version: 575.57.08      CUDA Version: 12.9     |
|-----------------------------------------+------------------------+----------------------+
| GPU  Name                 Persistence-M | Bus-Id          Disp.A | Volatile Uncorr. ECC |
| Fan  Temp   Perf          Pwr:Usage/Cap |           Memory-Usage | GPU-Util  Compute M. |
|                                         |                        |               MIG M. |
|=========================================+========================+======================|
|   0  Tesla V100-PCIE-32GB           On  |   00000000:AF:00.0 Off |                    0 |
| N/A   48C    P0             27W /  250W |       0MiB /  32768MiB |      0%      Default |
|                                         |                        |                  N/A |
+-----------------------------------------+------------------------+----------------------+
                                                                                         
+-----------------------------------------------------------------------------------------+
| Processes:                                                                              |
|  GPU   GI   CI              PID   Type   Process name                        GPU Memory |
|        ID   ID                                                               Usage      |
|=========================================================================================|
|  No running processes found                                                             |
+-----------------------------------------------------------------------------------------+
```


### TODO:
1. [DONE] parallelize the CPU brute threading cracker using OpenMP
2. [DONE] add dictionary attack using wordlist (passed in as a file) and dictionary rules
3. [DONE] add GPU brute force attack
4. [DONE] add timing measurements (total amount of time & hashes/sec)
5. [DONE] add Makefile
6. get Benchmarks

Extensions:
1. add dictionary attack on GPUs
2. add ability for user to input a multitude of hashes in a file that they want to try to crack
3. add ability for multiple different types of hashes (sha1, sha256, sha512)
4. Multi level / advanced dictionary attacks