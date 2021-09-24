using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Mirror;

public class Factory : Structure
{
    override public void StartTurn()
    {
        ShipScript ship = GetComponentInParent<BoardScript>().GetShipByPosition(transform.position);
        if (ship != null && ship.player_number != player_number)
        {
            Deactivate();
            return;
        }
        if (GetComponentInParent<BaseScript>().my_turn)
        {
            Activate();
            GetComponentInParent<BaseScript>().UpdateMetalCount((float)(GetComponentInParent<BaseScript>().avaliable_metal + .5));
        }
    }
}
