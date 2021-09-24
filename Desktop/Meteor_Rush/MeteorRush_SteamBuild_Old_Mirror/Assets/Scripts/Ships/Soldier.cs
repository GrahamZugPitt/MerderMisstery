using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using Mirror;

public class Soldier : ShipScript
{
    public int friendlyKickingDistance = 2;
    override public void Kick()
    {
        int trueDistance = kickingDistance;
        ShipScript ship = GetComponentInParent<BoardScript>().GetShipByPosition(dest);
        for (int j = 0; j < length; j++)
            if (squares[j] != null)
                Destroy(squares[j]);
        if (ship.hasAuthority)
        {
            ship.kickedByFriendly = true;
            trueDistance = friendlyKickingDistance;
        }
        ship.isBeingKicked = true;
        ship.showRange(trueDistance, movementSquare, enemySquare);
        ship.isMoving = true;
        ship.origin = GetComponentInParent<BoardScript>().GetShipByPosition(dest).GetPosition();
        hasChosenKickingTarget = true;
    }
}

