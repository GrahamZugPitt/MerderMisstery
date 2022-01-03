using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Mirror;
using TMPro;

public class BaseScript : NetworkBehaviour
{
    public float avaliable_metal = 3;
    public int captured_platinum = 0;
    public GameObject next_ship;
    public GameObject graveyard;
    public ShipScript kicking_ship = null;
    public int player_number;
    public bool my_turn;
    public bool has_a_moving_ship = false;
    public Sprite platinum;
    public Sprite cargo_counter_display;
    public Sprite base_cargo_counter_spr;
    public GameObject displayPrefab;
    public GameObject metalDisplay;
    public bool[,] enemy_squares_displayed;
    public bool displaying_to_enemy;

    public Sprite activated_spr;
    public Sprite deactivated_spr;

    public bool activated;

    [Command]
    public void CmdSpawnShip(Vector3 location, int shipNumber)
    {
        RpcSpawnShip(location, shipNumber);
    }
    
    [Server]
    public void RpcSpawnShip(Vector3 location, int shipNumber)
    {
        BoardScript board = transform.GetComponentInParent(typeof(BoardScript)) as BoardScript;
        board.GetComponentInChildren<Shop>().isShopping = false;
        ShipInShop ship = board.ships[shipNumber].GetComponent(typeof(ShipInShop)) as ShipInShop;
        GameObject theShip = Instantiate(ship.ship, location, Quaternion.identity);
        NetworkServer.Spawn(theShip, connectionToClient);
        updateBoard(theShip, ship.price, location);
    }

    [ClientRpc]
    public void updateBoard(GameObject theShip, int price, Vector3 location)
    {
        theShip.GetComponent<ShipScript>().isDeactivated = false;
        theShip.transform.SetParent(transform);
        theShip.GetComponent<ShipScript>().color();
        theShip.GetComponent<ShipScript>().UpdateSprite();
        theShip.GetComponent<ShipScript>().player_number = player_number;
        GameObject movement_value_display = theShip.GetComponent<ShipScript>().movement_value_display;
        CreateDisplay(theShip, movement_value_display);
        GameObject cargo_value_display = theShip.GetComponent<ShipScript>().cargo_value_display;
        CreateDisplayCargo(theShip, cargo_value_display);
        
        BoardScript board = theShip.GetComponentInParent<BoardScript>();
        avaliable_metal = avaliable_metal - price;
        UpdateMetalCount(avaliable_metal);
        if(location.x == transform.position.x && location.y == transform.position.y)
        {
            Deactivate();
        }
        if (board.GetStructureByPosition(location) != null)
        {
            board.GetStructureByPosition(location).GetComponent<Structure>().Deactivate();
        }
        BaseScript enemy;
        UpdateButton();
        if (hasAuthority)
        {
            enemy = GetComponentInParent<BoardScript>().bases[(player_number + 1) % 2].GetComponent<BaseScript>();
        }
        else
        {
            enemy = GetComponentInParent<BoardScript>().bases[player_number].GetComponent<BaseScript>();
        }
        enemy.GetComponent<BaseScript>().ResetMovement();
        if (enemy.displaying_to_enemy && !theShip.GetComponent<ShipScript>().hasAuthority)
        {
            theShip.GetComponent<ShipScript>().ShowNextTurnRange();
        }

    }

    public void updateBoardGraveyard(GameObject theShip, int price, Vector3 location)
    {
        theShip.GetComponent<ShipScript>().UpdateSprite();
        BoardScript board = theShip.GetComponentInParent<BoardScript>();
        avaliable_metal = avaliable_metal - price;
        UpdateMetalCount(avaliable_metal);
        if (location.x == transform.position.x && location.y == transform.position.y)
        {
            Deactivate();
        }
        if (board.GetStructureByPosition(location) != null)
        {
            board.GetStructureByPosition(location).GetComponent<Structure>().Deactivate();
        }
        BaseScript enemy;
        UpdateButton();
        if (hasAuthority)
        {
            enemy = GetComponentInParent<BoardScript>().bases[(player_number + 1) % 2].GetComponent<BaseScript>();
        }
        else
        {
            enemy = GetComponentInParent<BoardScript>().bases[player_number].GetComponent<BaseScript>();
        }
        enemy.GetComponent<BaseScript>().ResetMovement();
        if (enemy.displaying_to_enemy && !theShip.GetComponent<ShipScript>().hasAuthority)
        {
            theShip.GetComponent<ShipScript>().ShowNextTurnRange();
        }

    }

    public void ResetMovement()
    {
        ShipScript[] ships = GetComponentsInChildren<ShipScript>();
        if (ships != null)
        {
            for (int i = 0; i < ships.Length; i++)
            {
                if (ships[i].isShowingRangeToEnemy)
                {
                    ships[i].KillSquares();
                    ships[i].ShowNextTurnRange();
                }
            }
        }
    }

    public void Update()
    {
        /*bool hit_this_time = false;
        if(Input.GetKeyDown("space") && !displaying_to_enemy && !hasAuthority){
            ShowMovementToEnemy();
            displaying_to_enemy = true;
            hit_this_time = true;
        }

        if (Input.GetKeyDown("space") && displaying_to_enemy && !hasAuthority && !hit_this_time)
        {
            HideMovementFromEnemy();
            displaying_to_enemy = false;
        }*/
    }

    public void ShowMovementToEnemy()
    {
        ShipScript[] ships = GetComponentsInChildren<ShipScript>();
        for(int i = 0; i < ships.Length; i++)
        {
            ships[i].ShowNextTurnRange();
        }
    }

    public void HideMovementFromEnemy()
    {
        ShipScript[] ships = GetComponentsInChildren<ShipScript>();
        for (int i = 0; i < ships.Length; i++)
        {
            ships[i].isShowingRangeToEnemy = false;
            ships[i].KillSquares();
        }
    }

    public void CreateDisplay(GameObject theShip, GameObject display)
    {
        display = Instantiate(theShip.GetComponent<ShipScript>().text_display_prefab, theShip.transform.position, Quaternion.identity);
        display.transform.SetParent(theShip.transform, false);
        display.transform.position = new Vector3((float)(theShip.transform.position.x - .25), (float)(theShip.transform.position.y - .25), 0);
        display.GetComponent<StatHolder>().text_display = Instantiate(display.GetComponent<StatHolder>().text_display_prefab, display.transform.position - new Vector3((float)+.04,0,0), Quaternion.identity);
        //display.GetComponent<StatHolder>().text_display.GetComponent<TextMeshPro>().color = white;
        display.GetComponent<StatHolder>().UpdateValue(theShip.GetComponent<ShipScript>().movement);
        display.GetComponent<StatHolder>().text_display.transform.SetParent(display.transform);
        theShip.GetComponent<ShipScript>().movement_value_display = display;
    }

    public void CreateDisplayCargo(GameObject theShip, GameObject display)
    {
        display = Instantiate(theShip.GetComponent<ShipScript>().text_display_prefab, theShip.transform.position, Quaternion.identity);
        display.GetComponent<SpriteRenderer>().sprite = cargo_counter_display;
        display.transform.SetParent(theShip.transform, false);
        display.transform.position = new Vector3((float)(theShip.transform.position.x + .3), (float)(theShip.transform.position.y - .25), 0);
        display.GetComponent<StatHolder>().text_display = Instantiate(display.GetComponent<StatHolder>().text_display_prefab, display.transform.position, Quaternion.identity);
        //display.GetComponent<StatHolder>().text_display.GetComponent<TextMeshPro>().color = white;
        display.GetComponent<StatHolder>().UpdateValue(theShip.GetComponent<ShipScript>().attackScore);
        display.GetComponent<StatHolder>().text_display.transform.SetParent(display.transform);
        theShip.GetComponent<ShipScript>().cargo_value_display = display;
    }


    public void StartTurn()
    {
        GetComponent<YourTurn>().StartTurn();
    }

    public void StopMovement()
    {
        ShipScript[] ships = GetComponentsInChildren<ShipScript>();
        for(int i = 0; i < ships.Length; i++)
        {
            ships[i].StopMoving();
        }
    }

    public bool[] ThingsLeftToDo()
    {
        ShipScript[] ships = GetComponentsInChildren<ShipScript>();
        bool activated_ship = false;
        bool meteor_action = false;
        bool can_build = false;
        for (int i = 0; i < ships.Length; i++)
        {
            if (ships[i] != null &&
                (!ships[i].isDeactivated))
            {
                activated_ship = true;
                break;
            }
        }

        for (int i = 0; i < ships.Length; i++)
        {
            if (ships[i] != null && ships[i].findSpaceType() == (int)ShipScript.Spaces.METEOR && !ships[i].isMining){
                meteor_action = true;
                break;
            }
        }

            if (avaliable_metal >= GetComponentInParent<BoardScript>().LowestPrice() && activated)
            {
                can_build = true;
            }
        Structure[] structures = GetComponentsInChildren<Structure>();
        for (int i = 0; i < structures.Length; i++)
        {
            if(structures[i] != null && structures[i].activated && structures[i].identifier == 1)
            {
                can_build = true;
                break;
            }
        }
        bool[] things_left = new bool[3];
        things_left[0] = activated_ship;
        things_left[1] = meteor_action;
        things_left[2] = can_build;
        return things_left;
    }

    public void UpdateButton()
    {
        GetComponentInParent<BoardScript>().UpdateButton();
    }

    public Vector3 GetPosition()
    {
        return transform.position;
    }


    public void SetUpMetalCount(float metal)
    {
        metalDisplay = Instantiate(displayPrefab, transform.position + new Vector3(0, (float).5, 0), Quaternion.identity);
        metalDisplay.transform.SetParent(transform);
        metalDisplay.GetComponent<StatHolder>().UpdateValue(metal);
        metalDisplay.GetComponent<StatHolder>().text_display.transform.SetParent(metalDisplay.transform);
        metalDisplay.GetComponent<SpriteRenderer>().sprite = base_cargo_counter_spr;
    }

    public void UpdateMetalCount(float metal)
    {
        avaliable_metal = metal;
        metalDisplay.GetComponent<StatHolder>().UpdateValue(metal);
        GetComponentInParent<BoardScript>().GetComponentInChildren<Shop>().UpdateShops();
    }

    [Command]
    public void CmdUpdateMetalCount(float metal)
    {
        RpcUpdateMetalCount(metal);
    }

    [ClientRpc]
    public void RpcUpdateMetalCount(float metal)
    {
        UpdateMetalCount(metal);
    }

    public void Activate()
    {
        GetComponent<SpriteRenderer>().sprite = activated_spr;
        activated = true;
    }

    public void Deactivate()
    {
        GetComponent<SpriteRenderer>().sprite = deactivated_spr;
        activated = false;
    }

    [Command]
    public void CmdPop(Vector3 pos)
    {
        RpcPop(pos);
    }

    [ClientRpc]
    public void RpcPop(Vector3 pos)
    {
        GetComponentInChildren<Graveyard>().Pop(pos);
    }
}
