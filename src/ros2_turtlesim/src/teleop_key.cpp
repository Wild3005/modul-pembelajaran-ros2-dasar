#include <cstdio>
#ifdef PLATFORM_WINDOWS
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

int main(int argc, char ** argv)
{
  (void) argc;
  (void) argv;

  printf("hello world ros2_turtlesim package\n");
  return 0;
}
