using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Viper : ShipScript
{
    override public void OnArrival()
    {
        Debug.Log(GetComponentInParent<BoardScript>().GetStructureByPosition(dest) != null);
        if (GetComponentInParent<BoardScript>().GetStructureByPosition(dest) != null && GetComponentInParent<BoardScript>().GetStructureByPosition(dest).GetComponent<Structure>().player_number == GetComponentInParent<BaseScript>().player_number)
        {
            temp_movement = true;
            temp_movement_number = 4;
            kickingDistance = 4;
        }
        else
        {
            temp_movement = false;
            kickingDistance = 2;
        }
    }

    override public void StartTurnDelayed()
    {
        if (GetComponentInParent<BoardScript>().GetStructureByPosition(dest) != null && GetComponentInParent<BoardScript>().GetStructureByPosition(dest).GetComponent<Structure>().player_number == GetComponentInParent<BaseScript>().player_number)
        {
            temp_movement = true;
            temp_movement_number = 4;
            kickingDistance = 4;
        }
        else
        {
            temp_movement = false;
            kickingDistance = 2;
        }
    }
}
