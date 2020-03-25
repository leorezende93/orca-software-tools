# Do not modify the lines below
APP_CUSTOM_INST_NAME  := custom-inst
APP_CUSTOM_INST_DIR   := ./applications/$(APP_CUSTOM_INST_NAME)
APP_CUSTOM_INST_SRC   := $(APP_CUSTOM_INST_DIR)/src
APP_CUSTOM_INST_INC   := $(APP_CUSTOM_INST_DIR)/include
APP_CUSTOM_INST_LIB   := app-$(APP_CUSTOM_INST_NAME).a

INC_DIRS += -I$(APP_CUSTOM_INST_INC)
 
CFLAGS += 

# Update these lines with your source code
APP_CUSTOM_INST_SRCS := $(wildcard $(APP_CUSTOM_INST_SRC)/*.c)
APP_CUSTOM_INST_OBJS :=  $(APP_CUSTOM_INST_SRCS:.c=.o)

$(APP_CUSTOM_INST_LIB) : $(APP_CUSTOM_INST_OBJS)
	$(Q)$(AR) rcs $(APP_CUSTOM_INST_LIB) $(APP_CUSTOM_INST_OBJS) 
