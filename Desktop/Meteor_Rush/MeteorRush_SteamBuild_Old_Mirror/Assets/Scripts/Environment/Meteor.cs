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

    public int Mine(int miningSpeed, ShipScript ship)
    {
        if(size <= miningSpeed)
        {
            Destroy(gameObject);
            return size;
        }
        size = size - miningSpeed;
       
        return miningSpeed;

    }

    public int mineValue(int miningSpeed)
    {
        if (size <= miningSpeed)
        {
            return size;
        }
        return miningSpeed;
    }

    [Command (ignoreAuthority = true)]
    public void CmdDestroy()
    {
        RpcDestroy();
    }

    public void RpcDestroy()
    {
        Destroy(gameObject);
    }
}
