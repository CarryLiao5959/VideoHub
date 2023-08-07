# 编译器
CC = g++

# 源文件
SRCS := $(shell find ./* -type f | grep '\.cpp' | grep -v 'main\.cpp')
$(warning SRCS is ${SRCS})

# 目标文件
OBJS := $(patsubst %.cpp, %.o, $(filter %.cpp, $(SRCS)))
$(warning OBJS is ${OBJS})
ALL_OBJS := $(shell find . -name "*.o")

# 头文件
INCLUDE_TEMP = $(shell find ./* -type d | grep -v '\./config')
INCLUDE = $(patsubst %,-I %, $(INCLUDE_TEMP))
$(warning INCLUDE is ${INCLUDE})

# 编译选项
CFLAGS = -g -O0 -Wall -Werror -Wno-unused -fPIC
$(warning CFLAGS is ${CFLAGS})

# 链接选项
LDFLAG = -lpthread -std=c++11 -ldl
$(warning LDFLAG is ${LDFLAG})

# 主程序
SRC_MAIN = main.cpp
OBJ_MAIN = ${SRC_MAIN:%.cpp=%.o}
EXE_MAIN = main

target: clean ${EXE_MAIN}
$(EXE_MAIN): $(OBJ_MAIN) $(OBJS)
	$(CC) -o $@ $^ $(LDFLAG) $(CFLAGS) $(INCLUDE)

%.o: %.cpp
	${CC} ${CFLAGS} ${INCLUDE} -c $< -o $@

clean:
	rm -f ${ALL_OBJS} ${OBJ_MAIN} ${EXE_MAIN}
