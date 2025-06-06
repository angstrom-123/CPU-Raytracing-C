#include "my_assert.h"

void my_assert(bool expression, char* description)
{
	if (!expression)
	{
		fprintf(stderr, "\n===============\nAssertion Failure\n< %s >\n===============\n", 
				description);
	}
}
