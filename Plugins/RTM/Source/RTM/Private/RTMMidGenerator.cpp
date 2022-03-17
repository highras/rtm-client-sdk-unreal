#include "RTMMidGenerator.h"

namespace rtm
{
	using namespace std;

	int RTMMidGenerator::_orderMideGenertor = 0;
	mutex RTMMidGenerator::_mutexMidGenrator;
}
