#include <iostream>
#include <string>
#include <unistd.h>
#include "ParamTemplateThreadPool.h"

using namespace fpnn;
using std::cout;
using std::endl;

struct Params
{
	int times;
	std::string name;
};

class Dealer: public ParamTemplateThreadPool<struct Params *>::IProcessor
{
public:
	virtual void run(struct Params * param)
	{
		sleep(param->times);
		cout<<"Hello, "<<param->name<<". Times: "<<param->times<<endl;

		delete param;
	}

	virtual ~Dealer()	{}
};

int main()
{
	ParamTemplateThreadPool<struct Params *> tp(std::make_shared<Dealer>());
	tp.init(4, 3, 20, 60);

	{
		int32_t normal, temp, busy, queue, min, max, maxQueue;
		tp.status(normal, temp, busy, queue, min, max, maxQueue);
		cout<<"[Status] normal: "<<normal<<", temp: "<<temp<<", busy: "<<busy<<", queue: "<<queue<<endl;
		sleep(1);
	}

	for (int i = 0; i < 100; i++)
	{
		Params *p = new Params;
		p->times = i;
		p->name = (i%2) ? "Kittly" : "Dreamon";

		tp.wakeUp(p);
	}

	for (int i = 0; i < 100; i++)
	{
		int32_t normal, temp, busy, queue, min, max, maxQueue;
		tp.status(normal, temp, busy, queue, min, max, maxQueue);
		cout<<"[Status] normal: "<<normal<<", temp: "<<temp<<", busy: "<<busy<<", queue: "<<queue<<endl;
		sleep(1);
	}
	//cout<<"will exiting ..."<<endl;
	return 0;
}
