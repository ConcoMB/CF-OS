#include "league.h"
#include "files.h"
#include "list.h"

int main(void){
	leagues=newList(cmpLeague);
	users=newList(cmpUser);
	league_t league1={1,1,"liga1","",NULL,NULL,NULL}, league2={2,1,"liga","8484",NULL,NULL,NULL};
	user_t user1={1,NULL,"Fede","4848"}, user2={2,NULL,"Conrad","3535"}, user3={3,NULL,"Tomy","9898"};
	
	insert(users,(listElementT)&user1);
	insert(users,(listElementT)&user2);
	insert(users,(listElementT)&user3);
	
	insert(leagues,(listElementT)&league1);
	insert(leagues,(listElementT)&league2);
	league1.sportists=newList(cmpSportist);
	league2.sportists=newList(cmpSportist);
	league1.teams=newList(cmpTeam);
	league2.teams=newList(cmpTeam);
	league1.trades=newList(cmpTrade);
	league2.trades=newList(cmpTrade);
	
	saveAll();
}
