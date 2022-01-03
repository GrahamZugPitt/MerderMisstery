using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Mirror;

public class ShipInShop : NetworkBehaviour
{
    public GameObject highlighter;
    public GameObject ship; //Given
    public Sprite redSprite_on;
    public Sprite redSprite_off;
    public Sprite blueSprite_on;
    public Sprite blueSprite_off;
    public int shop_number;
    public int price; //TODO: Fix this
    public int color_int;
    public bool isConsideringBuying = false;
    public SpawnShip spawner;
    public GameObject priceDisplay;
    public GameObject movement_display;
    public GameObject cargo_display;
    public GameObject card_prefab;
    public SpawnShip[] spawners;

    public void OnMouseOver()
    {
        if (Input.GetMouseButtonDown(1))
        {
            GameObject card = Instantiate(card_prefab, transform.position, Quaternion.identity);
            card.transform.SetParent(GetComponentInParent<BoardScript>().transform);
            card.GetComponent<ShipCard>().SetShip(ship, priceDisplay, movement_display, cargo_display);
        }
    }

    public void OnMouseDown()
    {
        if (!GetComponentInParent<BoardScript>().my_turn())
        {
            return;
        }

        if (GetComponentInParent<Shop>().isShopping && isConsideringBuying)
        {
            StopShopping();
            return;
        }

        if (GetComponentInParent<Shop>().isShopping)
        {
            GetComponentInParent<Shop>().StopShopping();
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
        GoalSquare[] goal_squares = board.GetComponentsInChildren<GoalSquare>();
        findValidPlacements(player, board.structures, goal_squares);
    }


    public void findValidPlacements(GameObject playerBase, GameObject[] planets, GoalSquare[] goal_squares)
    {
        spawners = new SpawnShip[planets.Length + 1 + goal_squares.Length];
        BaseScript theBase = playerBase.GetComponent(typeof(BaseScript)) as BaseScript;
        /*if (theBase.avaliable_metal >= price && GetComponentInParent<BoardScript>().GetShipByPosition(theBase.transform.position) == null && theBase.activated)
        {
            GameObject theHighlighter = Instantiate(highlighter, playerBase.transform.position, Quaternion.identity);
            spawner = theHighlighter.GetComponent(typeof(SpawnShip)) as SpawnShip;
            spawners[0] = spawner;
            spawner.setShip(ship);
            spawner.setBase(playerBase);
            spawner.setShipNumber(shop_number);
            spawner.SetSpawnerParent(transform);
        }*/
        for (int i = 0; i < planets.Length; i++)
        {
            if(planets[i] == null)
            {
                continue;
            }

            Structure structure = planets[i].GetComponent<Structure>();
            if (theBase.avaliable_metal >= price && structure.player_number == theBase.player_number && structure.identifier == 1 && GetComponentInParent<BoardScript>().GetShipByPosition(planets[i].transform.position) == null && structure.activated) //may implicitly use a magic number
            {
                GameObject theHighlighterP = Instantiate(highlighter, planets[i].transform.position, Quaternion.identity);
                SpawnShip spawnerP = theHighlighterP.GetComponent(typeof(SpawnShip)) as SpawnShip;
                spawners[i + 1] = spawnerP;
                spawnerP.SetSpawnerParent(transform);
                spawnerP.setShip(ship);
                spawnerP.setBase(playerBase);
                spawnerP.setShipNumber(shop_number);
            }
        }
        for(int i = planets.Length; i < planets.Length + goal_squares.Length; i++)
        {
            if(goal_squares[i - planets.Length].square_color != playerBase.GetComponent<BaseScript>().player_number)
            {
                continue;
            }
            GameObject theHighlighterP = Instantiate(highlighter, goal_squares[i - planets.Length].transform.position, Quaternion.identity);
            SpawnShip spawnerP = theHighlighterP.GetComponent(typeof(SpawnShip)) as SpawnShip;
            spawners[i - planets.Length] = spawnerP;
            spawnerP.SetSpawnerParent(transform);
            spawnerP.setShip(ship);
            spawnerP.setBase(playerBase);
            spawnerP.setShipNumber(shop_number);
        }
    }

    public void color()
    {
        Transform pTransform = transform.parent.GetComponentInParent(typeof(Transform)) as Transform;
        BoardScript board = pTransform.parent.GetComponentInParent(typeof(BoardScript)) as BoardScript;
        if(board.get_player_number() == 1) //Magic number
        {
            color_int = 1;
        }
    }

    public void StopShopping()
    {
        GetComponentInParent<Shop>().isShopping = false;
        isConsideringBuying = false;
        if(spawner != null)
            spawner.DestroyThis();
        for (int i = 0; i < spawners.Length; i++)
        {
            if (spawners[i] != null)
            {
                spawners[i].DestroyThis();
            }
        }
        
    }

    public void UpdatePanel()
    {
        float metal_count = GetComponentInParent<BoardScript>().bases[color_int].GetComponent<BaseScript>().avaliable_metal;
        for(int i = 0; i < GetComponentInParent<BoardScript>().planets.Length; i++)
        {
            if(GetComponentInParent<BoardScript>().planets[i] == null)
            {
                continue;
            }
            Planet planet = GetComponentInParent<BoardScript>().planets[i].GetComponent<Planet>();

            if (planet.player_number == color_int && planet.metal > metal_count)
            {
                metal_count = planet.metal;
            }
        }
        if(color_int == 1)
        {
            if (metal_count < price) {
                GetComponent<SpriteRenderer>().sprite = blueSprite_off;
            }
            else
            {
                GetComponent<SpriteRenderer>().sprite = blueSprite_on;
            }
            return;
        }

        if (metal_count < price)
        {
            GetComponent<SpriteRenderer>().sprite = redSprite_off;
        }
        else
        {
            GetComponent<SpriteRenderer>().sprite = redSprite_on;
        }

    }

}
