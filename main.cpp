#include <iostream>
#include "segy.h"

int main () {

	segy sgy;
	//	sgy.OpenFile("velocity.segy");
	sgy.OpenFile("shot.segy");
	sgy.PrintTextHeader();
	sgy.PrintBinaryHeader();
	sgy.PrintTraceHeader();

}