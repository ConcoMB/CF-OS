int cmpTeam(void* team1, void* team2)
{
    return ((team_t*)team1->points)-((team_t*)team2->points);
}
