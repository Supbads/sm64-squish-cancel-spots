
gcc ^
  -DWIN32 ^
  -mwindows ^
  -mconsole ^
  -std=c99 ^
  -O3 ^
  -Wall -Wextra ^
  -Wno-missing-braces ^
  -Wno-incompatible-pointer-types ^
  -Wno-unused-parameter ^
  -Wno-unused-variable ^
  -Wno-unused-function ^
  -Wno-comment ^
  -Wno-maybe-uninitialized ^
  -Wno-uninitialized ^
  -Wno-unused-but-set-variable ^
  -fwrapv ^
  -fno-strict-aliasing ^
  main.c ^
  sm64\*.c ^
  -L.\\dev\\GLFW\\lib ^
  -lopengl32 ^
  -lglfw3 ^
  -I.\\dev\\GLFW\\include ^
  -o pendulum.exe