using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Mirror;

public class Chest : ShipScript
{
    [ClientRpc]
    public override void RpcKick(Vector3 finish, bool friendlyKick)
    {
        if (friendlyKick) {
            ShipScript[] ships = GetComponentInParent<BoardScript>().bases[GetComponentInParent<BaseScript>().player_number].GetComponentsInChildren<ShipScript>();
            ShipScript kicking_ship = GetComponentInParent<BoardScript>().bases[GetComponentInParent<BaseScript>().player_number].GetComponent<BaseScript>().kicking_ship;

            int remaining_space = cargoSpace - platCount;
            if (remaining_space > kicking_ship.platCount)
            {
                UpdatePlatCount(platCount + kicking_ship.platCount);
                kicking_ship.UpdatePlatCount(0);
            }
            else
            {
                UpdatePlatCount(platCount + remaining_space);
                kicking_ship.UpdatePlatCount(kicking_ship.platCount - remaining_space);
            }
        }

        if (!friendlyKick)
        {
            GetComponentInParent<BoardScript>().moving_ship_semaphore++;
        }
        isLiterallyMoving = true;
        StopMining();
        dest = finish;
        for (int i = 0; i < GetComponentInParent<BoardScript>().bases.Length; i++)
        {
            GetComponentInParent<BoardScript>().bases[i].GetComponent<BaseScript>().kicking_ship = null;
        }
    }
}
