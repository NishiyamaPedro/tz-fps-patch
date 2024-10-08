SRC = dllmain.c xinput1_3.c memory.c fps.c xinput1_3.def
CC = i686-w64-mingw32-gcc
LINKER_FLAGS = -Wl,--enable-stdcall-fixup
OBJ_NAME = xinput1_3.dll

all:
	$(CC) -o $(OBJ_NAME) --shared $(SRC) $(LINKER_FLAGS) 