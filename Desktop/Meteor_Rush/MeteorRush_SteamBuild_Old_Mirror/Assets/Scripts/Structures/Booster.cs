using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Mirror;

public class Booster : Structure
{
    public int temp_movement;

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
            if (ship != null && ship.player_number == player_number)
            {
                ship.temp_movement = true;
                ship.temp_movement_number = temp_movement;
                if (ship.identifier == 9)
                {
                    ship.temp_movement_number = 20;
                }
                Deactivate();
            }
        }
    }

    override public void LandingFunction(ShipScript ship)
    {
        if (player_number == ship.GetComponentInParent<BaseScript>().player_number && activated && GetComponentInParent<BaseScript>().player_number == GetComponentInParent<BoardScript>().player_turn)
        {

            ship.isDeactivated = false;
            ship.temp_movement = true;
            ship.temp_movement_number = temp_movement;
            if (ship.identifier == 9)
            {
                ship.temp_movement_number = 20;
            }
            Deactivate();
        }
    }
}
