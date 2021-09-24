using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Mirror;

public class Structure : NetworkBehaviour
{
    public bool activated = false;
    public int identifier = -1;

    public int player_number = -1;
    public Sprite red_activated;
    public Sprite red_deactivated;
    public Sprite blue_activated;
    public Sprite blue_deactivated;
    public Sprite red_occupied;
    public Sprite blue_occupied;
    public Sprite activated_spr;
    public Sprite deactivated_spr;
    public Sprite occupancy_spr;
    public Sprite[] construction_sprites;

    [Command(ignoreAuthority = true)]
    public void CmdActivate()
    {
        RpcActivate();
    }

    [ClientRpc]
    public void RpcActivate()
    {
        Activate();
    }

    [Command(ignoreAuthority = true)]
    public void CmdDeactivate()
    {
        RpcDeactivate();
    }

    public void Activate()
    {
        activated = true;
        GetComponent<SpriteRenderer>().sprite = activated_spr;
    }

    [ClientRpc]
    public void RpcDeactivate()
    {
        Deactivate();
    }

    public void Deactivate()
    {
        activated = false;
        GetComponent<SpriteRenderer>().sprite = deactivated_spr;
    }

    [Command]
    public void CmdColor(int color)
    {
        RpcColor(color);
    }

    [ClientRpc]
    public void RpcColor(int colorr)
    {
        color(colorr);
    }

    public void color(int color)
    {
        player_number = color;
        if (color == 0)
        {
            activated_spr = red_activated;
            deactivated_spr = red_deactivated;
            occupancy_spr = red_occupied;
            GetComponent<SpriteRenderer>().sprite = deactivated_spr;
            return;
        }

        activated_spr = blue_activated;
        deactivated_spr = blue_deactivated;
        occupancy_spr = blue_occupied;
        GetComponent<SpriteRenderer>().sprite = deactivated_spr;
    }


    public virtual void StartTurn()
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
        }
    }

    [Command(ignoreAuthority = true)]
    public virtual void CmdDestroy()
    {
        RpcDestroy();
    }

    [ClientRpc]
    public virtual void RpcDestroy()
    {
        Destroy(gameObject);
    }

    public virtual void LandingFunction(ShipScript ship)
    {

    }
}
