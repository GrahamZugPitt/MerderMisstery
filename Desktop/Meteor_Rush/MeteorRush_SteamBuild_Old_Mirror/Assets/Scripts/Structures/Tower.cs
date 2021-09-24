using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Tower : Structure
{
    override public void LandingFunction(ShipScript ship)
    {
        if (player_number != ship.GetComponentInParent<BaseScript>().player_number && activated && ship.GetComponentInParent<BaseScript>().player_number != GetComponentInParent<BoardScript>().player_turn)
        {
            Deactivate();
            ship.Destroy();
        }
    }
}
