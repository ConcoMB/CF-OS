
int main()
{
	int type;
	int id= nextClientID;
	char name[NAME_LENGTH], password[NAME_LENGTH];

	connect(id);
	rcvMsg(id,(void*)&type, sizeof(int));
	rcvMsg(id, (void*)name, NAME_LENGTH);
	rcvMsg(id, (void*)password, NAME_LENGTH);
	if(type==LOGIN)
	{
		if(logIn(name,password)!=0)
		{
			sendMsg(id, (void*)&aux, sizeof(int));
		}
	}
	else if(type==SIGNUP)
	{
		if(signUp(name, password)!=0)
		{
			sendMsg(id, (void*)&aux, sizeof(int));
		}
		logIn(name, password);
	}
}