using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Mirror;
using UnityEditor;
using UnityEngine.Tilemaps;
using Steamworks;
using UnityEngine.SceneManagement;


public class BoardScript : NetworkBehaviour
{
    enum Team
    {
        RED,
        BLUE,
        NONE
    }

    public Vector2Int boardSize;
    public bool isGameOver;
    public int player_count;
    public int structure_count;
    public int moving_ship_semaphore;
    public int kicked_ship_semaphore;
    public GameObject[] meteors;
    public GameObject[] structures;
    private GameObject[] goal_squares = new GameObject[22];
    public GameObject[] ships = new GameObject[10]; //This is the ships in the shop, not the ships in the game
    public Sprite[] blueSprites = new Sprite[10];
    public GameObject meteorPrefab;
    public GameObject planetPrefab;
    public GameObject end_turn_button_prefab;
    public GameObject reset_button_prefab;
    public GameObject surrender_button_prefab;
    public GameObject[] bases = new GameObject[2];
    public GameObject[] planets = new GameObject[6];
    public Tilemap tilemap;
    public Tile redBase;
    public Tile blueBase;
    public Tile planet;
    public Tile meteor;
    public Tile background_0;
    public Tile background_1;
    public Tile border;
    public GameObject basePrefab;
    public GameObject shopPrefab;
    public GameObject scorePrefab;
    public GameObject end_turn;
    public GameObject resetButton;
    public GameObject shop;
    public GameObject[] scoreBoards = new GameObject[3];
    public GameObject[] metalBoards = new GameObject[2];
    public int starting_metal;
    public int remainingMeteors;
    public int number_of_ships;

    public GameObject ship_tracker;
    public GameObject cargo_tracker;
    public GameObject background_tracker;
    public GameObject tile_tracker_prefab;
    public GameObject theCrown;
    public GameObject goalPrefab;
    public GameObject surrender_button;

    //MINING SPRITES, PERHAPS A BAD PLACE TO STORE THEM
    public Sprite red_mine;
    public Sprite blue_mine;
    public Sprite cash_in;
    public Sprite red_cash;
    public Sprite blue_cash;
    //MINING SPRITES, PERHAPS A BAD PLACE TO STORE THEM
    public int player_turn = 0; //Change this so it comes from the auction result
    public int M_PER_METEOR = 4; //For some reason this is one off

    public GameObject[] shop_prefabs;


    public void initialize(GameObject[] bases) //This will be where the board gets created/decided
    {
        resetButton = Instantiate(reset_button_prefab, new Vector3(-10, (float)-3.5, 0), Quaternion.identity);
        end_turn = Instantiate(end_turn_button_prefab, new Vector3((float)14.2, 4, 0), Quaternion.identity);
        surrender_button = Instantiate(surrender_button_prefab, new Vector3((float)15.77, (float).35, 0), Quaternion.identity);
        NetworkServer.Spawn(surrender_button);
        NetworkServer.Spawn(end_turn);
        NetworkServer.Spawn(resetButton);
        SetSurrenderButtonParent(surrender_button);
        shop = Instantiate(shopPrefab, new Vector3((float)0,(float)0,0), Quaternion.identity);
        NetworkServer.Spawn(shop);
        isGameOver = false;
        meteors = new GameObject[100];
        tilemap = GetComponent(typeof(Tilemap)) as Tilemap;
        tilemap.origin = new Vector3Int(-10, -10, 0);
        tilemap.size = new Vector3Int(20, 20, 1);
        RpcMakeBoard(bases, end_turn, resetButton, starting_metal);
        SetBoards();
        GenerateShips();
        Shop shopScript = shop.GetComponent(typeof(Shop)) as Shop;
        shopScript.FillWithShips(ships, transform);
        GenerateMeteors();
        Rpc_first_turn();
        RpcSetShopParent(shop);


    }

    [ClientRpc]
    public void SetSurrenderButtonParent(GameObject surrender_button_local)
    {
        surrender_button_local.transform.SetParent(transform, false);
    }

    [ClientRpc]
    public void RpcSetShopParent(GameObject shop)
    {
        remainingMeteors = meteors.Length; //Holy shit clean up your code you stinky motherfucker
        shop.transform.SetParent(transform, false);
        UpdateButton();
    }

    [Command (ignoreAuthority = true)]
    public void CmdUpdateMeteorCount(int update)
    {
        UpdateMeteorCount(update);
    }

    public void UpdateMeteorCount(int update)
    {

    }

    /*[ClientRpc]
    public void UpdateMeteorCount(int update)
    {
        remainingMeteors = remainingMeteors - update;

        for (int i = 0; i < bases.Length; i++)
        {
            int carrying = 0;
            ShipScript[] shipCargo = bases[i].GetComponentsInChildren<ShipScript>();

            for(int j = 0; j < shipCargo.Length; j++)
            {
                carrying = carrying + shipCargo[j].platCount;
            }
            
            if (2*bases[i].GetComponent<BaseScript>().captured_platinum > remainingMeteors - carrying) //Test number
            {
                GameObject crown = Instantiate(theCrown, bases[i].transform.position + new Vector3(0, 1, 0), Quaternion.identity);
                crown.transform.SetParent(bases[i].transform, false);
                crown.transform.position = bases[i].transform.position + new Vector3(0, 1, 0);
                isGameOver = false;
            }
        }
    }*/

    //RPC space
    public void EndGame()
    {
        for (int i = 0; i < player_count; i++)
        {
            if (scoreBoards[i].GetComponent<ScoreKeeper>().score >= scoreBoards[(i + 1) % 2].GetComponent<ScoreKeeper>().score)
            {
                GameObject crown = Instantiate(theCrown, bases[i].transform.position + new Vector3(0, 1, 0), Quaternion.identity);
                crown.transform.SetParent(bases[i].transform, false);
                crown.transform.position = bases[i].transform.position + new Vector3(0, 1, 0);
                bases[i].GetComponent<BaseScript>().my_turn = false;
            }
        }
        //GetComponentInChildren<EndTurnButton>().EndGame();
        PlayerPrefs.SetInt("red_score", scoreBoards[0].GetComponent<ScoreKeeper>().score);
        PlayerPrefs.SetInt("blue_score", scoreBoards[1].GetComponent<ScoreKeeper>().score);
        PlayerPrefs.SetInt("my_color", get_player_number());
        if (get_player_number() == 0)
        {
            ((NetworkManagerExample)(GetComponentInParent<NetworkManager>())).DoShutdown();
        }
        SceneManager.LoadScene(2);
    }

    public void Surrender(int player_number)
    {
        scoreBoards[player_number].GetComponent<ScoreKeeper>().score = -1;
        EndGame();
    }

    public Vector2 GetBoardSize()
    {
        return boardSize;
    }

    public void SetBoards()
    {
        scoreBoards[0] = Instantiate(scorePrefab, new Vector3((float)24.9, (float).8, 0), Quaternion.identity);
        scoreBoards[1] = Instantiate(scorePrefab, new Vector3((float)24.9, (float)-.2, 0), Quaternion.identity);
        scoreBoards[2] = Instantiate(scorePrefab, new Vector3((float)24.9, (float)-1.2, 0), Quaternion.identity);
        NetworkServer.Spawn(scoreBoards[0]);
        NetworkServer.Spawn(scoreBoards[1]);
        NetworkServer.Spawn(scoreBoards[2]);
        RpcSetBoardOnClient(scoreBoards);

    }

    [ClientRpc]
    public void RpcSetBoardOnClient(GameObject[] score)
    {
        scoreBoards[0] = score[0];
        scoreBoards[1] = score[1];
        scoreBoards[2] = score[2];
        scoreBoards[0].transform.SetParent(transform, false);
        scoreBoards[1].transform.SetParent(transform, false);
        scoreBoards[2].transform.SetParent(transform, false);
        scoreBoards[0].GetComponent<ScoreKeeper>().setUp(0);
        scoreBoards[1].GetComponent<ScoreKeeper>().setUp(1);
        scoreBoards[2].GetComponent<ScoreKeeper>().setUp(2);
    }


    [ClientRpc]
    public void RpcMakeBoard(GameObject[] bases, GameObject end_turn_button, GameObject reset, int starting_metal_value) //This must happen before meteors are generated
    {
        boardSize.x = 11;
        boardSize.y = 9;
        ship_tracker = Instantiate(tile_tracker_prefab, new Vector3(-12, 2, 0), Quaternion.identity);
        cargo_tracker = Instantiate(tile_tracker_prefab, new Vector3(-12, 0, 0), Quaternion.identity);
        background_tracker = Instantiate(tile_tracker_prefab, new Vector3(-12, -2, 0), Quaternion.identity);
        meteors = new GameObject[100];
        structures = new GameObject[meteors.Length];
        for(int i = 0; i < structures.Length; i++)
        {
            structures[i] = null;
        }
        end_turn_button.transform.SetParent(transform, false);
        reset.transform.SetParent(transform, false);
        this.bases = bases;
        boardSize.x = 20;
        boardSize.y = 20;
        for(int i = 0; i < 12; i++)
        {
            for(int j = 0; j < 9; j++)
            {
                if((i+j) % 2 == 0)
                    tilemap.SetTile(new Vector3Int(i, j, 0), background_0);
                else
                {
                    tilemap.SetTile(new Vector3Int(i, j, 0), background_1);
                }
            }
        }
        for (int i = 0; i < bases.Length; i++)
        {
            bases[i].GetComponent<BaseScript>().enemy_squares_displayed = new bool[boardSize.x,boardSize.y];
            bases[i].GetComponent<BaseScript>().avaliable_metal = starting_metal_value;
            bases[i].transform.parent = transform;
            bases[i].GetComponent<BaseScript>().player_number = i;
            GetComponentsInChildren<BaseScript>()[i].SetUpMetalCount(starting_metal_value);
        }


    }

    public void GenerateShips()
    {
        System.Random rand = new System.Random();
        rand.Next(0, shop_prefabs.Length);
        ships = new GameObject[8];
        int[] blockers = new int[ships.Length];
        for(int i = 1; i < blockers.Length; i++)
        {
            blockers[i] = -1;
        }

        for(int i = 0; i < ships.Length; i++){
            ships[i] = shop_prefabs[randomByBlocker(blockers, i)];
        }

        for(int i = 0; i < ships.Length; i++)
        {
            int price = 1000;
            int pos = -1;
            for (int j = i; j < ships.Length; j++)
            {
                int cur_price = ships[j].GetComponent<ShipInShop>().price;
                if (ships[j].GetComponent<ShipInShop>().price < price)
                {
                    price = cur_price;
                    pos = j;
                }
            }
            GameObject ship = ships[i];
            ships[i] = ships[pos];
            ships[pos] = ship;
        }

        for(int i = 0; i < 4; i++)
        {
            ships[i] = Instantiate(ships[i], new Vector3((float)(12.1 + i), (float)5.3, 0), Quaternion.identity);

        }
        for(int i = 0; i < 4; i++)
        {
            ships[i + 4] = Instantiate(ships[i + 4], new Vector3((float)(12.1 + i), (float)6.9, 0), Quaternion.identity);
        }
        //ships[ships.Length - 2] = Instantiate(shop_prefabs[9], new Vector3((float)(12.1 + 2), 6, 0), Quaternion.identity); Here lie the ship testing comments
        //ships[ships.Length - 1] = Instantiate(shop_prefabs[0], new Vector3((float)(12.1 + 3), 6, 0), Quaternion.identity);
    }

    public int randomByBlocker(int[] blockers, int pos)
    {
        System.Random rand = new System.Random();
        while (true)
        {
            bool taken = false;
            int attempt = rand.Next(0, shop_prefabs.Length);
            for (int i = 0; i < blockers.Length; i++)
            {
                if (blockers[i] == attempt)
                {
                    taken = true;
                    break;
                }
            }
            if (!taken)
            {
                blockers[pos] = attempt;
                return attempt;
            }
        }
    }

    /*public void GenerateShips()
    {
        System.Random rand = new System.Random();
        int[] chosen = new int[10];
        for(int i = 0; i < 10; i++)
        {
            chosen[i] = i;
        }

        for(int i = 0; i < 5; i++)
        {
            int choice = -1;
            while(choice == -1)
            {
                choice = chosen[rand.Next(0, 10)];
            }

            ships[i] = Instantiate(shopPrefabs[choice], new Vector3((float)(-5 + i * 2.25), 6, 0), Quaternion.identity);
            chosen[choice] = -1;
        }
    }*/

    public Vector3 SetMeteorLocation(Vector3[] takenPositions, int x_min, int x_max, int y_min, int y_max, System.Random rand)
    {
        Vector3 mLocation = new Vector3(0, 0, 0);
        bool unique = false;
        while (!unique)
        {
            unique = true;
            mLocation = new Vector3(rand.Next(x_min, x_max), rand.Next(y_min, y_max), 0);
            for (int j = 0; j < takenPositions.Length; j++)
                if (mLocation.Equals(takenPositions[j]))
                {
                    unique = false;
                }
        }
        return mLocation;
    }

    public void GenerateMeteors()
    {
        System.Random rand = new System.Random();
        Vector3[] takenPositions = new Vector3[28];
        takenPositions[0] = new Vector3(-4, -4, 0);
        takenPositions[1] = new Vector3(3, 3, 0);
        for (int i = 0; i < 12; i++) //magic numbers here
        {
            takenPositions[i + 2] = SetMeteorLocation(takenPositions, 5 + (i % 2), 6 + (i % 2), 0, 9, rand);
            meteors[i] = Instantiate(meteorPrefab, takenPositions[i + 2], Quaternion.identity);
            NetworkServer.Spawn(meteors[i]);
            SetMeteorParents(meteors[i], i);
        }
        for (int i = 0; i < 13; i++) //magic numbers here
        {
            takenPositions[i + 14] = SetMeteorLocation(takenPositions, 2 + ((i + 1) % 2) * 7, 3 + ((i + 1) % 2) * 7, 0, 9, rand);
            meteors[i + 12] = Instantiate(meteorPrefab, takenPositions[i + 14], Quaternion.identity);
            NetworkServer.Spawn(meteors[i + 12]);
            SetMeteorParents(meteors[i + 12], i + 12);
        }
        /*takenPositions = new Vector3[6];
        for (int i = 0; i < 6; i++) 
        {
            Vector3 mLocation = new Vector3(0, 0, 0);
            bool unique = false;
            while (!unique)
            {
                unique = true;
                mLocation = new Vector3(((i % 2) * 5 + 3), rand.Next(0, 9), 0);
                for (int j = 0; j < takenPositions.Length; j++)
                    if (mLocation.Equals(takenPositions[j]))
                    {
                        unique = false;
                    }
            }
            takenPositions[i] = new Vector3(mLocation.x, mLocation.y, mLocation.z);
            planets[i] = Instantiate(planetPrefab, mLocation, Quaternion.identity);
            NetworkServer.Spawn(planets[i]);
            SetPlanetParents(planets[i], i);
        }*/
        for (int i = 0; i < 9; i++)
        {
            goal_squares[i] = Instantiate(goalPrefab, new Vector3(11, i, 0), Quaternion.identity);
            NetworkServer.Spawn(goal_squares[i]);
            SetGoalParents(goal_squares[i], i);
        }
        for (int i = 11; i < 20; i++)
        {
            goal_squares[i] = Instantiate(goalPrefab, new Vector3(0, i - 11, 0), Quaternion.identity);
            NetworkServer.Spawn(goal_squares[i]);
            SetGoalParentsBlue(goal_squares[i], i);
        }
    }

    [ClientRpc]
    public void SetPlanetParents(GameObject planet, int i)
    {
        planets[i] = planet;
        Vector3 pos = planet.transform.position;
        planet.transform.SetParent(transform, false);
        planet.transform.position = pos;
    }

    [ClientRpc]
    public void SetMeteorParents(GameObject meteor, int i)
    {
        meteors[i] = meteor;
        Vector3 pos = meteor.transform.position;
        meteor.transform.SetParent(transform, false);
        meteor.transform.position = pos;
    }

    [ClientRpc]
    public void SetGoalParents(GameObject goal_square, int i)
    {
        goal_squares[i] = goal_square;
        Vector3 pos = goal_square.transform.position;
        goal_square.transform.SetParent(transform, false);
        goal_square.transform.position = pos;
        goal_square.GetComponent<GoalSquare>().color((int)Team.RED);
    }

    [ClientRpc]
    public void SetGoalParentsBlue(GameObject goal_square, int i)
    {
        goal_squares[i] = goal_square;
        Vector3 pos = goal_square.transform.position;
        goal_square.transform.SetParent(transform, false);
        goal_square.transform.position = pos;
        goal_square.GetComponent<GoalSquare>().color((int)Team.BLUE);
    }

    [Command (ignoreAuthority = true)]
    public void Cmd_update_turn()
    {
        Server_update_turn();
    }

    [ClientRpc]
    public void Rpc_first_turn()
    {
        bases[player_turn].GetComponent<BaseScript>().my_turn = true;
        bases[player_turn].GetComponent<BaseScript>().Activate();
        GetComponentInChildren<Shop>().UpdateShops();
    }

    [Server]
    public void Server_update_turn()
    {
        ShipScript[] ships = GetComponentsInChildren<ShipScript>();
        for(int i = 0; i < ships.Length; i++)
        {
            if(ships[i].isMining && ships[i].building_cycle > 1 && ships[i].GetComponentInParent<BaseScript>().player_number != player_turn)
            {
                GameObject structure = ships[i].SpawnStructure();
                AddStructure(structure);
            }
        }
        Rpc_update_turn();
    }

    [ClientRpc]
    public void Rpc_update_turn()
    {
        GetComponentInChildren<Shop>().StopShopping();
        bases[player_turn].GetComponent<BaseScript>().my_turn = false;
        ShipScript[] ships = GetComponentsInChildren<ShipScript>();
        for(int i = 0; i < ships.Length; i++)
        {
            ships[i].StopMoving();
        }
        ShipScript[] ships_local = bases[player_turn].GetComponentsInChildren<ShipScript>();
        for (int i = 0; i < ships_local.Length; i++)
        {
            if (ships_local[i] != null)
            {
                ships_local[i].EndTurn();
            }
        }
        player_turn = (player_turn + 1) % player_count;
        bases[player_turn].GetComponent<BaseScript>().Activate();
        ships_local = bases[player_turn].GetComponentsInChildren<ShipScript>();
        for (int i = 0; i < ships_local.Length; i++)
        {
            if (ships_local[i] != null)
            {
                ships_local[i].StartTurnPriority();
            }
        }
        for (int i = 0; i < ships_local.Length; i++)
        {
            if (ships_local[i] != null)
            {
                ships_local[i].StartTurn();
            }
        }
        for (int i = 0; i < ships_local.Length; i++)
        {
            if (ships_local[i] != null)
            {
                ships_local[i].StartTurnDelayed();
            }
        }

        GetComponentInChildren<Shop>().UpdateShops();
        bases[player_turn].GetComponent<BaseScript>().my_turn = true;
        GetComponentInChildren<EndTurnButton>().UpdateButton();
        for (int i = 0; i < structure_count; i++)
        {
            if (structures[i] != null)
            {
                structures[i].GetComponent<Structure>().StartTurn();
            }
        }
        for (int i = 0; i < bases.Length; i++)
        {
            bases[i].GetComponent<BaseScript>().ResetMovement();
        }
    }

    public void CheckAllShipPositions()
    {
        ShipScript[] ships = GetComponentsInChildren<ShipScript>();
        for (int i = 0; i < ships.Length; i++)
        {
            ships[i].findSpaceType();
        }
    }

    [Command (ignoreAuthority = true)]
    public void CmdReset()
    {
        Reset();
    }

    [Server]
    public void Reset()
    {
        //GetComponentInParent<NetworkManagerExample>().reset();
    }

    /*[ClientRpc]
    public void RpcReset()
    {
        for(int i = 0; i < meteors.Length; i++)
        {
            if(meteors[i] != null)
                Destroy(meteors[i]);
        }

    }*/

    //ALL FUNCTIONS BELOW ARE RUN LOCALLY ON CLIENT
    public int get_player_number()
    {
        return bases[0].GetComponent<NetworkIdentity>().isLocalPlayer ? 0 : 1;
    }

    public bool my_turn()
    {
        return get_player_number() == player_turn;
    }

    public Vector3[] shipPositions()
    {
        ShipScript[] shipScripts = GetComponentsInChildren<ShipScript>();
        Vector3[] positions = new Vector3[shipScripts.Length];
        for (int i = 0; i < shipScripts.Length; i++)
            positions[i] = shipScripts[i].getPosition();
        return positions;
    }

    public ShipScript GetShipByPosition(Vector3 given)
    {
        ShipScript[] shipScripts = GetComponentsInChildren<ShipScript>();
        Vector3[] positions = new Vector3[shipScripts.Length];
        for (int i = 0; i < shipScripts.Length; i++)
            if (shipScripts[i].getPosition().Equals(given))
            {
                return shipScripts[i];
            }
        return null;
    }

    public GameObject GetStructureByPosition(Vector3 given)
    {
        for (int i = 0; i < structures.Length; i++)
            if (structures[i] != null && structures[i].transform.position.Equals(given))
            {
                return structures[i];
            }
        return null;
    }

    public int LowestPrice() //Optimize this maybe
    {
        Shop shop = GetComponentInChildren<Shop>();
        int lowest = shop.ships[0].GetComponent<ShipInShop>().price;
        for(int i = 1; i < ships.Length; i++)
        {
            if (shop.ships[i] != null && shop.ships[i].GetComponent<ShipInShop>().price < lowest)
            {
                lowest = shop.ships[i].GetComponent<ShipInShop>().price;
            }
        }
        return lowest;
    }

    public void UpdateButton()
    {
        GetComponentInChildren<EndTurnButton>().UpdateButton();
    }

    public void StopShopping()
    {
        GetComponentInChildren<Shop>().StopShopping();
    }

    public GameObject GetMeteorByPosition(Vector3 pos)
    {
        for (int i = 0; i < meteors.Length; i++)
            if (meteors[i] != null && meteors[i].transform.position.Equals(pos))
                return meteors[i];
        return null;
    }

    public GameObject GetPlanetByPosition(Vector3 pos)
    {
        for (int i = 0; i < planets.Length; i++)
            if (planets[i] != null && planets[i].transform.position.Equals(pos))
                return planets[i];
        return null;
    }

    public GameObject GetGoalByPosition(Vector3 pos)
    {
        for (int i = 0; i < goal_squares.Length; i++)
            if (goal_squares[i] != null && goal_squares[i].transform.position.Equals(pos))
                return goal_squares[i];
        return null;
    }

    public GameObject GetCelestialObjectByPosition(Vector3 pos)
    {
        GameObject celestial = GetMeteorByPosition(pos);

        if(celestial != null)
        {
            return celestial;
        }
        celestial = GetStructureByPosition(pos);
        if (celestial != null)
        {
            return celestial;
        }
        return null;
    }
    [ClientRpc]
    public void AddStructure(GameObject structure)
    {
        structures[structure_count++] = structure;
    }


    public int GetSpaceType(Vector3 pos)
    {
        for (int i = 0; i < meteors.Length; i++)
        {
            if (meteors[i] != null && meteors[i].transform.position.Equals(pos))
            {
                return (int)ShipScript.Spaces.METEOR;
            }
        }
        /*for (int i = 0; i < planets.Length; i++)
        {
            if (planets[i] != null && planets[i].transform.position.Equals(pos))
            {
                return (int)ShipScript.Spaces.PLANET;
            }
        }*/

        for (int i = 0; i < structures.Length; i++)
        {
            if (structures[i] != null && structures[i].transform.position.Equals(pos))
            {
                return (int)ShipScript.Spaces.STRUCTURE;
            }
        }

        for (int i = 0; i < bases.Length; i++)
        {
            if (bases[i].transform.position.Equals(pos))
            {
                return (int)ShipScript.Spaces.BASE;
            }
        }

        for (int i = 0; i < goal_squares.Length; i++)
        {
            if (goal_squares[i] != null && goal_squares[i].transform.position.Equals(pos))
            {
                return (int)ShipScript.Spaces.GOAL;
            }
        }
        return (int)ShipScript.Spaces.BLANK;
    }

}
