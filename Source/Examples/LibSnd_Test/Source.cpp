#include <iostream>
using namespace std;


#include <sndfile.h>


void main()
{
	

	SF_INFO sf_info;
	{
		sf_info.format = 0;
	}
	auto sf_file = sf_open("../../../../../Media/Sounds/test.wav", SFM_READ, &sf_info);

	if(sf_file == NULL)
	{
		throw std::exception("failed to open file");
	}

	sf_close(sf_file);

	system("pause");
}