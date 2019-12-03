#include "dialogue.cpp"
int main(){
	NPClite jarrett("Benedict", MERCHANT); //0
	NPClite kim("Liam", MERCHANT); //1
	NPClite pope("Micheal", MERCHANT); //2
	NPClite gaben("Kyle", MERCHANT); //3
	NPClite marie("David", WORKER); //4
	NPClite lary("Erick", WORKER); //5
	NPClite luigi("Frank", WORKER); //6
	NPClite albert("Gail", WORKER); //7
	NPClite dennis("Henry", MAYOR); //8
	NPClite helen("Isaac", POLICE); //9
	NPClite merge("Jake", INNKEEPER); //10
	NPClite sigmund("Charles", PRIEST); //11
	NPClite town[TOWN_SIZE] = {jarrett, kim, pope, gaben, marie, lary, luigi, albert, dennis, helen, merge, sigmund};
	simulation(town, time(NULL));
	while(!goodMurder(town)){
		NPClite jarrett("Benedict", MERCHANT); //0
		NPClite kim("Liam", MERCHANT); //1
		NPClite pope("Micheal", MERCHANT); //2
		NPClite gaben("Kyle", MERCHANT); //3
		NPClite marie("David", WORKER); //4
		NPClite lary("Erick", WORKER); //5
		NPClite luigi("Frank", WORKER); //6
		NPClite albert("Gail", WORKER); //7
		NPClite dennis("Henry", MAYOR); //8
		NPClite helen("Isaac", POLICE); //9
		NPClite merge("Jake", INNKEEPER); //10
		NPClite sigmund("Charles", PRIEST); //11
		town[0] = jarrett;
		town[1] = kim;
		town[2] = pope;
		town[3] = gaben;
		town[4] = marie;
		town[5] = lary;
		town[6] = luigi;
		town[7] = albert;
		town[8] = dennis;
		town[9] = helen;
		town[10] = merge;
		town[11] = sigmund;
	simulation(town, time(NULL));
	}
	dialogue(town);
	return 0;
}
