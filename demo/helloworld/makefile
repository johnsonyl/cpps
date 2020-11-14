#添加规则

release = true

include Rules.mk

projectname = helloworld

#引用头文件路径
INC_DIRS += -I../../cpps/
SRC_FILES += $(wildcard ./demo/$(projectname)/*.c*)

#宏定义修改
MACROS += -DREDIS_EXPORT
libpath = ./usr/lib64

GEN_EXE = ./$(BIN_DIR)/$(projectname)


$(shell mkdir -p $(OBJ_DIR)/cpps)

.PHONY: all

all: $(OBJ_FILES)
	$(CPP) $(CPPFLAGS) $(PIC) $(INC_DIRS) $(GEN_OBJS) $(LIB_DIRS) $(LNK_LIBS) -o $(GEN_EXE)
	
	
#工程生成文件清理	
clean: force
	$(RM) $(GEN_EXE)
