using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Monster : ShipScript
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
        if (!ship.hasAuthority)
        {
            ship.kickedByFriendly = true;
            ship.CmdDestroy();
            CmdUpdateMovement(movement + 1);
            //CmdUpdateCargo(cargoSpace + 1);
            return;
        }
        ship.isBeingKicked = true;
        ship.showRange(kickingDistance, movementSquare, enemySquare);
        ship.isMoving = true;
        ship.origin = GetComponentInParent<BoardScript>().GetShipByPosition(dest).GetPosition();
        hasChosenKickingTarget = true;
    }
}
