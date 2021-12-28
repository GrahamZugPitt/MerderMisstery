using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Mirror;
using UnityEditor;
using UnityEngine.Tilemaps;


public class BoardScript : NetworkBehaviour
{

    public Vector2 boardSize;
    public bool isGameOver;
    public int player_count = 2;
    public GameObject[] meteors;
    public GameObject[] ships = new GameObject[10]; //This is the ships in the shop, not the ships in the game
    public GameObject[] shopPrefabs = new GameObject[10];
    public Sprite[] blueSprites = new Sprite[10];
    public GameObject meteorPrefab;
    public GameObject end_turn_button_prefab;
    public GameObject reset_button_prefab;
    public GameObject[] bases = new GameObject[2];
    public GameObject[] planets = new GameObject[6];
    public Tilemap tilemap;
    public Tile redBase;
    public Tile blueBase;
    public Tile planet;
    public Tile meteor;
    public Tile background;
    public Tile border;
    public GameObject basePrefab;
    public GameObject shopPrefab;
    public GameObject scorePrefab;
    public GameObject end_turn;
    public GameObject resetButton;
    public GameObject shop;
    public GameObject[] scoreBoards = new GameObject[2];
    public GameObject[] metalBoards = new GameObject[2];
    public int starting_metal = 3;

    public GameObject ship_tracker;
    public GameObject cargo_tracker;
    public GameObject background_tracker;
    public GameObject tile_tracker_prefab;

    //MINING SPRITES, PERHAPS A BAD PLACE TO STORE THEM
    public Sprite red_mine;
    public Sprite blue_mine;
    public Sprite cash_in;
    //MINING SPRITES, PERHAPS A BAD PLACE TO STORE THEM
    public int player_turn = 0; //Change this so it comes from the auction result
    //List<Material> tiles;

    public GameObject turtle_shop;
    public GameObject antelope_shop;


    void Start()
    {
        //Remove this once you have a system for initializing the game
        //Home home = GetComponent("Home") as Home;
    }

    // Update is called once per frame
    void Update()
    {

    }


    public void initialize(GameObject[] bases) //This will be where the board gets created/decided
    {
        resetButton = Instantiate(reset_button_prefab, new Vector3(-10, (float)-3.5, 0), Quaternion.identity);
        end_turn = Instantiate(end_turn_button_prefab, new Vector3(10, (float)-3.5, 0), Quaternion.identity);
        NetworkServer.Spawn(end_turn);
        NetworkServer.Spawn(resetButton);
        shop = Instantiate(shopPrefab, new Vector3(0,0,0), Quaternion.identity);
        NetworkServer.Spawn(shop);
        isGameOver = false;
        meteors = new GameObject[16];
        tilemap = GetComponent(typeof(Tilemap)) as Tilemap;
        tilemap.origin = new Vector3Int(-10, -10, 0);
        tilemap.size = new Vector3Int(20, 20, 1);
        SetBoards();
        RpcMakeBoard(bases, end_turn, resetButton);
        GenerateShips();
        Shop shopScript = shop.GetComponent(typeof(Shop)) as Shop;
        shopScript.FillWithShips(ships, transform);
        GenerateMeteors();
        Rpc_update_turn();
        RpcSetShopParent(shop);


    }

    [ClientRpc]
    public void RpcSetShopParent(GameObject shop)
    {
        shop.transform.SetParent(transform, false);
    }

    public void GameOver()
    {
        isGameOver = true;
    }
    public Vector2 GetBoardSize()
    {
        return boardSize;
    }

    public void SetBoards()
    {
        scoreBoards[0] = Instantiate(scorePrefab, new Vector3(17, 3, 0), Quaternion.identity);
        scoreBoards[1] = Instantiate(scorePrefab, new Vector3(17, (float)-.5, 0), Quaternion.identity);
        metalBoards[0] = Instantiate(scorePrefab, new Vector3(17, (float)1.5, 0), Quaternion.identity);
        metalBoards[1] = Instantiate(scorePrefab, new Vector3(17, -2, 0), Quaternion.identity);
        NetworkServer.Spawn(scoreBoards[0]);
        NetworkServer.Spawn(scoreBoards[1]);
        NetworkServer.Spawn(metalBoards[0]);
        NetworkServer.Spawn(metalBoards[1]);
        RpcSetBoardOnClient(scoreBoards, metalBoards);
        metalBoards[0].GetComponent<ScoreKeeper>().CmdSetScore(starting_metal);
        metalBoards[1].GetComponent<ScoreKeeper>().CmdSetScore(starting_metal);


    }

    [ClientRpc]
    public void RpcSetBoardOnClient(GameObject[] score, GameObject[] metal)
    {
        scoreBoards[0] = score[0];
        scoreBoards[1] = score[1];
        metalBoards[0] = metal[0];
        metalBoards[1] = metal[1];
        scoreBoards[0].transform.SetParent(transform, false);
        scoreBoards[1].transform.SetParent(transform, false);
        scoreBoards[0].GetComponent<ScoreKeeper>().setUp(true, "RED");
        scoreBoards[1].GetComponent<ScoreKeeper>().setUp(false, "BLUE");
        metalBoards[0].transform.SetParent(transform, false);
        metalBoards[1].transform.SetParent(transform, false);
        metalBoards[1].transform.SetParent(transform, false);
        metalBoards[0].GetComponent<ScoreKeeper>().setUp(true, "METAL");
        metalBoards[1].GetComponent<ScoreKeeper>().setUp(false, "METAL");
    }


    [ClientRpc]
    public void RpcMakeBoard(GameObject[] bases, GameObject end_turn_button, GameObject reset) //This must happen before meteors are generated
    {
        ship_tracker = Instantiate(tile_tracker_prefab, new Vector3(-12, 2, 0), Quaternion.identity);
        cargo_tracker = Instantiate(tile_tracker_prefab, new Vector3(-12, 0, 0), Quaternion.identity);
        background_tracker = Instantiate(tile_tracker_prefab, new Vector3(-12, -2, 0), Quaternion.identity);
        meteors = new GameObject[16];
        end_turn_button.transform.SetParent(transform, false);
        reset.transform.SetParent(transform, false);
        this.bases = bases;
        boardSize.x = 20;
        boardSize.y = 20;
        for(int i = -5; i < 5; i++)
        {
            for(int j = -5; j < 5; j++)
            {
                tilemap.SetTile(new Vector3Int(i, j, 0), background);
            }
        }
        for (int i = 0; i < bases.Length; i++)
        {
            bases[i].transform.parent = transform;
            bases[i].GetComponent<BaseScript>().player_number = i;
        }


    }

    public void GenerateShips()
    {
        ships = new GameObject[2];
        ships[0] = Instantiate(antelope_shop, new Vector3((float)(-5 + 1 * 2.25), 6, 0), Quaternion.identity);
        ships[1] = Instantiate(turtle_shop, new Vector3((float)(-5 + 3 * 2.25), 6, 0), Quaternion.identity);
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

    public void GenerateMeteors()
    {
        System.Random rand = new System.Random();
        Vector3[] takenPositions = new Vector3[18];
        takenPositions[0] = new Vector3(-4, -4, 0);
        takenPositions[1] = new Vector3(3, 3, 0);
        for (int i = 0; i < 16; i++) //magic numbers here
        {
            Vector3 mLocation = new Vector3(0,0,0);
            bool unique = false;
            while (!unique) {
                unique = true;
                mLocation = new Vector3((i % 8) - 4, rand.Next(-4, 4), 0);
                for(int j = 0; j < takenPositions.Length; j++)
                    if (mLocation.Equals(takenPositions[j]))
                    {
                        unique = false;
                    }
            }
            takenPositions[i + 2] = new Vector3(mLocation.x, mLocation.y, mLocation.z);
            meteors[i] = Instantiate(meteorPrefab, mLocation, Quaternion.identity);
            NetworkServer.Spawn(meteors[i]);
            setMeteorParents(meteors[i], i);
        }
    }

    [ClientRpc]
    public void setMeteorParents(GameObject meteor, int i)
    {
        meteors[i] = meteor;
        Vector3 pos = meteor.transform.position;
        meteor.transform.SetParent(transform, false);
        meteor.transform.position = pos;
    }

    [Command (ignoreAuthority = true)]
    public void Cmd_update_turn()
    {
        Rpc_update_turn();
    }

    [ClientRpc]
    public void Rpc_update_turn()
    {
        bases[player_turn].GetComponent<BaseScript>().my_turn = false;
        bases[player_turn].GetComponent<BaseScript>().StopMovement();
        player_turn = (player_turn + 1) % player_count;
        ShipScript[] ships_local = bases[player_turn].GetComponentsInChildren<ShipScript>();
        for(int i = 0; i < ships_local.Length; i++)
        {
            if (ships_local[i] != null)
            {
                ships_local[i].StartTurn();
            }
        }
        metalBoards[player_turn].GetComponent<ScoreKeeper>().CmdSetScore(bases[player_turn].GetComponent<BaseScript>().avaliable_metal++);
        bases[player_turn].GetComponent<BaseScript>().my_turn = true;
        GetComponentInChildren<EndTurnButton>().UpdateButton();
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
        RpcReset();
        GenerateMeteors();
    }

    [ClientRpc]
    public void RpcReset()
    {
        for(int i = 0; i < meteors.Length; i++)
        {
            if(meteors[i] != null)
                Destroy(meteors[i]);
        }
        ShipScript[] ships = GetComponentsInChildren<ShipScript>();
        for (int i = 0; i < ships.Length; i++)
        {
            ships[i].Destroy();
        }
        BaseScript[] baseScripts = GetComponentsInChildren<BaseScript>();
        for (int i = 0; i < bases.Length; i++)
        {
            baseScripts[i].avaliable_metal = 3; //Magic Number
        }

    }

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

    public ShipScript FindShipByPosition(Vector3 given)
    {
        ShipScript[] shipScripts = GetComponentsInChildren<ShipScript>();
        Vector3[] positions = new Vector3[shipScripts.Length];
        for (int i = 0; i < shipScripts.Length; i++)
            if (shipScripts[i].getPosition().Equals(given))
                return shipScripts[i];
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

}
