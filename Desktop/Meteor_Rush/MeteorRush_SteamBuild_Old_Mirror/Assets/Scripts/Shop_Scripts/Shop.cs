using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Mirror;

public class Shop : NetworkBehaviour
{
    public GameObject[] ships = new GameObject[10];
    public bool isShopping = false;
    public GameObject displayPrefab;
    public GameObject mcPrefab;
    public Sprite cargo_sprite;
    public Sprite coin_sprite;

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
        RpcSetShopNumbers();
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
        for (int i = 0; i < 4; i++)
        {
            GameObject display = Instantiate(ships[i].GetComponent<ShipInShop>().ship.GetComponent<ShipScript>().mining_symbol_prefab, new Vector3((float)(12.1 + i), (float)4.72, 0), Quaternion.identity);
            if (ships[i].GetComponent<ShipInShop>().color_int == 0)
            {
                display.GetComponent<SpriteRenderer>().sprite = ships[i].GetComponent<ShipInShop>().ship.GetComponent<ShipScript>().buildings[0].GetComponent<Structure>().red_occupied;
            }
            else
            {
                display.GetComponent<SpriteRenderer>().sprite = ships[i].GetComponent<ShipInShop>().ship.GetComponent<ShipScript>().buildings[0].GetComponent<Structure>().blue_occupied;
            }
            display.transform.SetParent(ships[i].transform);

        }
        for (int i = 0; i < 4; i++)
        {
            GameObject display = Instantiate(ships[i + 4].GetComponent<ShipInShop>().ship.GetComponent<ShipScript>().mining_symbol_prefab, new Vector3((float)(12.1 + i), (float)6.32, 0), Quaternion.identity);
            if (ships[i + 4].GetComponent<ShipInShop>().color_int == 0)
            {
                display.GetComponent<SpriteRenderer>().sprite = ships[i + 4].GetComponent<ShipInShop>().ship.GetComponent<ShipScript>().buildings[0].GetComponent<Structure>().red_occupied;
            }
            else
            {
                display.GetComponent<SpriteRenderer>().sprite = ships[i + 4].GetComponent<ShipInShop>().ship.GetComponent<ShipScript>().buildings[0].GetComponent<Structure>().blue_occupied;
            }
            display.transform.SetParent(ships[i].transform);
        }
        transform.position = new Vector3(0, (float).5, 0);
    }

    [ClientRpc]
    public void RpcSetShopNumbers()
    {
        for(int i = 0; i < ships.Length; i++)
        {
            if (ships[i] != null)
            {
                ships[i].GetComponent<ShipInShop>().priceDisplay = Instantiate(displayPrefab, ships[i].transform.position + new Vector3(0, (float).45, 0), Quaternion.identity);
                ships[i].GetComponent<ShipInShop>().priceDisplay.transform.SetParent(ships[i].transform);
                ships[i].GetComponent<ShipInShop>().priceDisplay.GetComponent<StatHolder>().UpdateValue(ships[i].GetComponent<ShipInShop>().price);
                ships[i].GetComponent<ShipInShop>().priceDisplay.GetComponent<SpriteRenderer>().sprite = coin_sprite;
                ships[i].GetComponent<ShipInShop>().priceDisplay.GetComponent<StatHolder>().text_display.transform.SetParent(ships[i].GetComponent<ShipInShop>().priceDisplay.transform);

                ships[i].GetComponent<ShipInShop>().movement_display = Instantiate(displayPrefab, ships[i].transform.position + new Vector3((float)-.25, (float)-.25, 0), Quaternion.identity);
                ships[i].GetComponent<ShipInShop>().movement_display.transform.SetParent(ships[i].transform);
                ships[i].GetComponent<ShipInShop>().movement_display.GetComponent<StatHolder>().MakeTextDisplay(new Vector3((float)-.04,0,0));
                ships[i].GetComponent<ShipInShop>().movement_display.GetComponent<StatHolder>().UpdateValue(ships[i].GetComponent<ShipInShop>().ship.GetComponent<ShipScript>().movement);
                ships[i].GetComponent<ShipInShop>().movement_display.GetComponent<StatHolder>().text_display.transform.SetParent(ships[i].GetComponent<ShipInShop>().movement_display.transform);

                /*ships[i].GetComponent<ShipInShop>().cargo_display = Instantiate(displayPrefab, ships[i].transform.position + new Vector3((float).3, (float)-.25, 0), Quaternion.identity);
                ships[i].GetComponent<ShipInShop>().cargo_display.transform.SetParent(ships[i].transform);
                ships[i].GetComponent<ShipInShop>().cargo_display.GetComponent<StatHolder>().UpdateValue(ships[i].GetComponent<ShipInShop>().ship.GetComponent<ShipScript>().cargoSpace);
                ships[i].GetComponent<ShipInShop>().cargo_display.GetComponent<SpriteRenderer>().sprite = cargo_sprite;
                ships[i].GetComponent<ShipInShop>().cargo_display.GetComponent<StatHolder>().text_display.transform.SetParent(ships[i].GetComponent<ShipInShop>().cargo_display.transform);*/

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

    public void UpdateShops()
    {
        ShipInShop[] ships = GetComponentsInChildren<ShipInShop>();
        for (int i = 0; i < ships.Length; i++)
        {
            if (ships[i] != null)
                ships[i].UpdatePanel();
        }
    }
}
