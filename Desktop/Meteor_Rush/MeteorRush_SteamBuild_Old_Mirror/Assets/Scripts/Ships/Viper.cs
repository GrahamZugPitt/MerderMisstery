using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Viper : ShipScript
{
    override public void OnArrival()
    {
        if (GetComponentInParent<BoardScript>().GetStructureByPosition(dest) != null && GetComponentInParent<BoardScript>().GetStructureByPosition(dest).GetComponent<Structure>().player_number == GetComponentInParent<BaseScript>().player_number)
        {
            temp_movement = true;
            temp_movement_number = 4;
            attackScore = 4;
        }
        else
        {
            temp_movement = false;
            attackScore = 2;
        }
    }

    override public void StartTurnDelayed()
    {
        if (GetComponentInParent<BoardScript>().GetStructureByPosition(dest) != null && GetComponentInParent<BoardScript>().GetStructureByPosition(dest).GetComponent<Structure>().player_number == GetComponentInParent<BaseScript>().player_number)
        {
            temp_movement = true;
            temp_movement_number = 4;
            attackScore = 4;
        }
        else
        {
            temp_movement = false;
            attackScore = 2;
        }
    }
}
