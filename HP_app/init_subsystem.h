#ifndef INIT_SUBSYSTEM_H_
#define INIT_SUBSYSTEM_H_

enum SubsystemConfigError
{
	eOk = 0,
	eFail = -1,
};

void SubsystemInit(void);

#endif // INIT_SUBSYSTEM_H_
