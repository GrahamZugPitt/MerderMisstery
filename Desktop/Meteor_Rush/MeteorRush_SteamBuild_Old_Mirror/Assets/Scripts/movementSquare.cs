using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class movementSquare : MonoBehaviour
{
    public bool isEmemySquare = false;
    public int dist;
    public ShipScript ship;

    public void OnMouseDown()
    {
        if(!isEmemySquare)
            ship.ChooseDest(transform.position);
    }
}
   
