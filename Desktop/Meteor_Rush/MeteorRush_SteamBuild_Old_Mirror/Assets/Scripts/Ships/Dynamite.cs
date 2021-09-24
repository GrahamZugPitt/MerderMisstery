using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Mirror;
using UnityEditor;
using UnityEngine.Tilemaps;

public class Dynamite : ShipScript
{
    override public void Kick()
    {
        ShipScript ship = GetComponentInParent<BoardScript>().GetShipByPosition(dest);
        for (int j = 0; j < length; j++)
            if (squares[j] != null)
            {
                GetComponentInParent<BaseScript>().enemy_squares_displayed[(int)squares[j].transform.position.x, (int)squares[j].transform.position.y] = false;
                Destroy(squares[j]);
            }
        if (ship.hasAuthority)
        {
            ship.kickedByFriendly = true;
        }
        if (!ship.hasAuthority)
        {
            ship.CmdDestroy();
            CmdDestroy();
            return;
        }
        ship.isBeingKicked = true;
        ship.showRange(kickingDistance, movementSquare, enemySquare);
        ship.isMoving = true;
        ship.origin = GetComponentInParent<BoardScript>().GetShipByPosition(dest).GetPosition();
        hasChosenKickingTarget = true;
    }

    /*override public void StartTurnDelayed()
    {
        Explode();
    }*/

    public void Explode()
    {
        for(int i = 0; i < 4; i++)
        {
            GameObject celestial = GetComponentInParent<BoardScript>().GetMeteorByPosition(transform.position + new Vector3(stupidTrig(i), stupidTrig(i + 1), 0));
            ShipScript ship = GetComponentInParent<BoardScript>().GetShipByPosition(transform.position + new Vector3(stupidTrig(i), stupidTrig(i + 1), 0));
            if(ship != null)
                ship.CmdDestroy();

            if (celestial != null)
            {
                CmdDestroy(celestial);
            }
        }
        //Doesn't matter unless you let people use abilites on planets
        GameObject celestial_on_me = GetComponentInParent<BoardScript>().GetMeteorByPosition(transform.position);
        if (celestial_on_me != null)
        {
            CmdDestroy(celestial_on_me);
        }
        CmdDestroy();
        isDeactivated = true;
        UpdateSprite();
        UpdateButton();
    }

    [Command (ignoreAuthority = true)]
    public void CmdDestroy(GameObject target)
    {
        RpcDestroy(target);
    }

    [ClientRpc]
    public void RpcDestroy(GameObject target)
    {
        Destroy(target);
        target = null;
    }

    public int stupidTrig(int dumb)
    {
        switch (dumb){
            case 1:
                return 1;
            case 3:
                return -1;
            default:
                return 0;
        }
    }
}
