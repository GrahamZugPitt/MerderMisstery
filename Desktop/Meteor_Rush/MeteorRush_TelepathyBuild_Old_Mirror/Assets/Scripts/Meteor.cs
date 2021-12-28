using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Mirror;

public class Meteor : NetworkBehaviour
{
    public int size = 1;
    public Sprite two_sprite;
    public Sprite one_sprite;
    public bool added_to_stack = false;

    public void Mine(int miningSpeed)
    {
        if(size <= miningSpeed)
        {
            Destroy(gameObject);
            return;
        }
        size = size - miningSpeed;
        /*if(size == 2)
        {
            GetComponent<SpriteRenderer>().sprite = two_sprite;
        }
        if (size == 1)
        {
            GetComponent<SpriteRenderer>().sprite = one_sprite;
        }*/
        return;

    }

    public int mineValue(int miningSpeed)
    {
        if (size <= miningSpeed)
        {
            return size;
        }
        return miningSpeed;
    }
}
