/*
Compile instructions:
[e.g.]
gcc -o binaryFile sourceFile1.c [sourceFile2.c sourceFileNNN.c] -lwiriingPi
[actual]
gcc -o blink blink.c -lwiriingPi
Needed arguments:
-lwiriingPi: Link 'Wiring for Pi' library
*/
#include <wiringPi.h>
int main (void)
{
  wiringPiSetup () ;
  pinMode (0, OUTPUT) ;
  for (;;)
  {
    digitalWrite (0, HIGH) ; delay (500) ;
    digitalWrite (0,  LOW) ; delay (500) ;
  }
  return 0 ;
}
