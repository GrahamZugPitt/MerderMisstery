using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Mirror;

public class BaseScript : NetworkBehaviour
{
    public int avaliable_metal = 100;
    public int captured_platinum = 0;
    public GameObject next_ship;
    public int player_number;
    public bool my_turn;
    public bool has_a_moving_ship = false;
    public Sprite platinum;

    [Command]
    public void CmdSpawnShip(Vector3 location, int shipNumber)
    {
        RpcSpawnShip(location, shipNumber);
    }
    
    [Server]
    public void RpcSpawnShip(Vector3 location, int shipNumber)
    {
        BoardScript script = transform.GetComponentInParent(typeof(BoardScript)) as BoardScript;
        script.GetComponentInChildren<Shop>().isShopping = false;
        ShipInShop ship = script.ships[shipNumber].GetComponent(typeof(ShipInShop)) as ShipInShop;
        GameObject theShip = Instantiate(ship.ship, location, Quaternion.identity);
        NetworkServer.Spawn(theShip, connectionToClient);
        updateBoard(theShip, ship.price);
        script.metalBoards[player_number].GetComponent<ScoreKeeper>().CmdAddToScore(-ship.price); //maybe put this in update board
    }

    [ClientRpc]
    public void updateBoard(GameObject theShip, int price)
    {
        avaliable_metal = avaliable_metal - price;
        theShip.transform.SetParent(transform);
        theShip.GetComponent<ShipScript>().color();
        theShip.GetComponent<ShipScript>().findSpaceType();
        UpdateButton();
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

    public bool ThingsLeftToDo()
    {
        ShipScript[] ships = GetComponentsInChildren<ShipScript>();
        for (int i = 0; i < ships.Length; i++)
        {
            if (ships[i] != null && (!ships[i].hasActed || (ships[i].findSpaceType() == (int)ShipScript.spaces.METEOR && !ships[i].isMining)))
            {
                return true;
            }
        }

            if (avaliable_metal > GetComponentInParent<BoardScript>().LowestPrice())
            {
                return true;
            }
        return false;
    }

    public void UpdateButton()
    {
        GetComponentInParent<BoardScript>().UpdateButton();
    }

}
