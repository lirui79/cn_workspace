KVERS = $(shell uname -r)

# 生成模块名
MODULE_NAME := mycdev

# 源文件目录
SRC_PATH := ./

# 作为一个模块进行编译
CONFIG_RUNYEE_CAMVIB=m

# Kernel modules
obj-$(CONFIG_RUNYEE_CAMVIB) := $(SRC_PATH)$(MODULE_NAME).o

# 指定模块的一些编译选项
EXTRA_CFLAGS=-g -O1

build: kernel_modules
kernel_modules:
	make -C /lib/modules/$(KVERS)/build M=$(CURDIR) modules

clean:
	make -C /lib/modules/$(KVERS)/build M=$(CURDIR) clean