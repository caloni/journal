#include <iostream>
#include <stdlib.h>

enum MSG_TYPE
{
	MSG_TYPE_NONE = 0,
	MSG_TYPE1 = 1,
	MSG_TYPE2 = 2, // � okay terminar com v�rgula uma enumera��o
};

void MsgType1()
{
	std::cout << "MsgType is ONE\n";
}

void MsgType2()
{
	std::cout << "MsgType is TWO\n";
}

int main(int argc, char* argv[])
{
	while( --argc )
	{
		MSG_TYPE msgType = (MSG_TYPE) atoi(argv[argc]);

		switch( msgType )
		{
		case MSG_TYPE1:
			MsgType1();
			break;

		case MSG_TYPE2:
			MsgType2();
			break;
		}
	}
}
