
int main()
{
	int type;
	int id= nextClientID;
	char name[NAME_LENGTH], password[NAME_LENGTH];
	client_t* newClient = malloc(sizeof(client_t*));
	newClient->ID=id;
	insert(clients, newClient);
	connect(id);
	sndMsg(DEFAULTID, (void*)id, sizeof(int));
	rcvMsg(id,(void*)&type, sizeof(int));
	rcvMsg(id, (void*)name, NAME_LENGTH);
	rcvMsg(id, (void*)password, NAME_LENGTH);
	if(type==LOGIN)
	{
		if(logIn(name,password)!=0)
		{
			//error
			sndMsg(id, (void*)&aux, sizeof(int));
		}
	}
	else if(type==SIGNUP)
	{
		if(signUp(name, password)!=0)
		{
			//error
			sndMsg(id, (void*)&aux, sizeof(int));
		}
		logIn(name, password);
	}
}