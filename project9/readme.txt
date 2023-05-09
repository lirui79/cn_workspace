1.  make
2.  insmod mycdev.ko
3.  ls /dev  
4.  find   mycdev  file node
5. gcc testcdev.c -o testcdev
6.  ./testcdev



 128  make clean
  129  make
  130  apt-get install linux-source
  131  ls /usr/src/linux-source-5.15.0
  132  ls /usr/local/src/
  133  ls /usr/include/linux/
  134  make clean
  135  make
  136  ls
  137  cat Makefile 
  138  make clean
  139  make oldconfig && make prepare
  140  apt install flex
  141  make oldconfig && make prepare
  142  apt install bison
  143  make oldconfig && make prepare
  144  apt install openssl
  145  apt search openssl
  146  make oldconfig && make prepare
  147  apt-get install libssl-dev
  148  make oldconfig && make prepare
  149  apt install libelf-dev
  150  history
