using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Mirror;

public class SpawnShip : NetworkBehaviour
{
    public GameObject shipPrefab;
    public GameObject playerBase;
    public int shipNumber;

    public void OnMouseDown()
    {
        BaseScript baseScript = playerBase.GetComponent(typeof(BaseScript)) as BaseScript;
        baseScript.CmdSpawnShip(transform.position, shipNumber);
    }

    public void setShip(GameObject chosenShip)
    {
        shipPrefab = chosenShip;
    }
    public void setBase(GameObject playerBaseGiven)
    {
        playerBase = playerBaseGiven;
    }
    public void setShipNumber(int number)
    {
        shipNumber = number;
    }
    public void DestroyThis()
    {
        Destroy(gameObject);
    }
}
