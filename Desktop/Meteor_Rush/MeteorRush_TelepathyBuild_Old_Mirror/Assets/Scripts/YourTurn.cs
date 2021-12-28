using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Mirror;

public class YourTurn : NetworkBehaviour
{
    public Sprite myTurn;
    public Sprite empty;

    public void StartTurn()
    {
        Debug.Log("I get here");
        if (transform.GetComponentInParent<BoardScript>().my_turn())
            transform.GetComponent<SpriteRenderer>().sprite = myTurn;
        if (!transform.GetComponentInParent<BoardScript>().my_turn())
            transform.GetComponent<SpriteRenderer>().sprite = empty;
    }

}
