using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Mirror;

public class ShipInShop : NetworkBehaviour
{
    public GameObject highlighter;
    public GameObject ship; //Given
    public Sprite blueSprite;
    public int shop_number;
    public int price; //TODO: Fix this
    public bool isConsideringBuying = false;
    public SpawnShip spawner;

    public void OnMouseDown()
    {
        if (!GetComponentInParent<BoardScript>().my_turn())
        {
            return;
        }

        if (spawner == null)
        {
            isConsideringBuying = false;
        }

        if (GetComponentInParent<Shop>().isShopping && isConsideringBuying)
        {
            StopShopping();
            return;
        }

        if (GetComponentInParent<Shop>().isShopping)
        {
            ShipInShop[] spawners = GetComponentInParent<Shop>().GetComponentsInChildren<ShipInShop>();
            for(int i = 0; i < spawners.Length; i++)
            {
                if(spawners[i].spawner != null)
                {
                    spawners[i].spawner.DestroyThis();
                }
            }
        }

        GetComponentInParent<Shop>().isShopping = true;
        GameObject[] bases = GetComponentInParent<Transform>().GetComponentInParent<BoardScript>().bases;
        for(int i = 0; i < bases.Length; i++)
        {
            bases[i].GetComponent<BaseScript>().StopMovement();
        }
        isConsideringBuying = true;
        Transform pTransform = transform.parent.GetComponentInParent(typeof(Transform)) as Transform;
        BoardScript board = pTransform.parent.GetComponentInParent(typeof(BoardScript)) as BoardScript;
        GameObject player = board.bases[0].GetComponent<NetworkIdentity>().isLocalPlayer ? board.bases[0] : board.bases[1];
        findValidPlacements(player, board.planets);
    }


    public void findValidPlacements(GameObject playerBase, GameObject[] planets)
    {
        BaseScript count = playerBase.GetComponent(typeof(BaseScript)) as BaseScript;
        if (count.avaliable_metal >= price)
        {
            GameObject theHighlighter = Instantiate(highlighter, playerBase.transform.position, Quaternion.identity);
            spawner = theHighlighter.GetComponent(typeof(SpawnShip)) as SpawnShip;
            spawner.setShip(ship);
            spawner.setBase(playerBase);
            spawner.setShipNumber(shop_number);
        }
        /*
        for (int i = 0; i < planets.Length; i++)
        {
            PlanetScript counts = planets[i].GetComponent(typeof(PlanetScript)) as PlanetScript;
            if(counts.avaliable_metal >= price)
            {
                GameObject theHighlighterP = Instantiate(highlighter, planets[i].transform.position, Quaternion.identity);
                SpawnShip spawnerP = theHighlighterP.GetComponent(typeof(SpawnShip)) as SpawnShip;
                spawnerP.setShip(ship);
                spawnerP.setBase(playerBase);
            }
        }*/
    }

    public void color()
    {
        Transform pTransform = transform.parent.GetComponentInParent(typeof(Transform)) as Transform;
        BoardScript board = pTransform.parent.GetComponentInParent(typeof(BoardScript)) as BoardScript;
        if(board.get_player_number() == 1) //Magic number
        {
            GetComponent<SpriteRenderer>().sprite = blueSprite;
        }
    }

    public void StopShopping()
    {
        GetComponentInParent<Shop>().isShopping = false;
        isConsideringBuying = false;
        if(spawner != null)
            spawner.DestroyThis();
    }

}
