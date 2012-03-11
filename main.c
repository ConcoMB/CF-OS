#include "list.h"
#include "cmp.h"
#include <stdio.h>
#include "league.h"

int main()
{
    listADT list = newList(cmpTeam);
    team_t t1,t2,t3;
    t1.points =100;
    t2.points=3;
    t3.points=12;
    strcpy(t1.name,"ala");
    strcpy(t2.name,"alasds");
    strcpy(t3.name,"sadla");
    t1.user=NULL;
    t2.user=NULL;
    t3.user=NULL;
    insert(list, &t1);
    
    insert(list, &t2);
    
    insert(list, &t3);
    
    displayLeagues(list);
    return 0;
}