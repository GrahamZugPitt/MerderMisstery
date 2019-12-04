#include "dialogue.cpp"
int main(){
	unsigned int initialSeed = time(NULL);
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
	simulation(town, initialSeed);
	std::cout << initialSeed  << "\n";
	while(!goodMurder(town)){
		initialSeed = time(NULL);
		jarrett = *(new NPClite ("Benedict", MERCHANT)); //0
		kim = *(new NPClite("Liam", MERCHANT)); //1
		pope = *(new NPClite("Micheal", MERCHANT)); //2
		gaben =  *(new NPClite("Kyle", MERCHANT)); //3
		marie =  *(new NPClite ("David", WORKER)); //4
		lary =  *(new NPClite ("Erick", WORKER)); //5
		luigi =  *(new NPClite ("Frank", WORKER)); //6
		albert =  *(new NPClite ("Gail", WORKER)); //7
		dennis =  *(new NPClite ("Henry", MAYOR)); //8
		helen =  *(new NPClite ("Isaac", POLICE)); //9
		merge =  *(new NPClite ("Jake", INNKEEPER)); //10
		sigmund =  *(new NPClite ("Charles", PRIEST)); //11
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
	simulation(town, initialSeed);
	std::cout << initialSeed << "\n";
	}
	dialogue(town);
	return 0;
}
