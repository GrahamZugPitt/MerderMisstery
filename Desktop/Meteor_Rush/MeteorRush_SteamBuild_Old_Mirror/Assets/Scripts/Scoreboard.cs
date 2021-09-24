using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Scoreboard : MonoBehaviour
{
    public int number_of_cashing_ships = 5;
    public int number_of_cashed_ships = 0;
    public GameObject scoreboard_ship_holder;
    public GameObject[] cashed_ships;

    public void MakeBlanks()
    {
        for(int i = 0; i < number_of_cashing_ships; i++)
        {
            cashed_ships[i] = Instantiate(scoreboard_ship_holder, transform.position + new Vector3((float)(i + .5), 0, 0), Quaternion.identity);
        }
    }

    public void AddShip(GameObject ship)
    {
        Vector3 position = cashed_ships[number_of_cashed_ships].transform.position;
        Destroy(cashed_ships[number_of_cashed_ships]);
        cashed_ships[number_of_cashed_ships++] = Instantiate(ship, position, Quaternion.identity);
    }
}
