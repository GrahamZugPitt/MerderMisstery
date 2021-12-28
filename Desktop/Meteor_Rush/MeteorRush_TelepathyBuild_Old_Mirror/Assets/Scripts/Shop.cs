using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Mirror;

public class Shop : NetworkBehaviour
{
    public GameObject[] ships = new GameObject[10];
    public bool isShopping = false;
    
    public void FillWithShips(GameObject[] givenShips, Transform board_transform)
    {
        for (int i = 0; i < givenShips.Length; i++)
        {
            if (givenShips[i] != null)
            {
                NetworkServer.Spawn(givenShips[i]);
            }
        }
        RpcSetParent(givenShips, board_transform);
    }

    [ClientRpc]
    public void RpcSetParent(GameObject[] givenShips, Transform board_transform)
    {
        transform.SetParent(board_transform);
        for (int i = 0; i < givenShips.Length; i++)
        {
            if (givenShips[i] != null)
            {
                ships[i] = givenShips[i];
            }
        }
        for(int i = 0; i < ships.Length; i++)
        {
            if (ships[i] != null)
            {
                Vector3 prev = new Vector3(ships[i].transform.position.x, ships[i].transform.position.y, ships[i].transform.position.z);
                ships[i].transform.SetParent(transform, false);
                ships[i].transform.position = prev;
                ships[i].GetComponent<ShipInShop>().color();
                ships[i].GetComponent<ShipInShop>().shop_number = i;
            }
            
        }
    }

    public void StopShopping()
    {
        ShipInShop[] ships = GetComponentsInChildren<ShipInShop>();
        for(int i = 0; i < ships.Length; i++)
        {
            if(ships[i] != null)
                ships[i].StopShopping();
        }
    }
}
