using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Mirror;

public class SurrenderButton : NetworkBehaviour
{
    public Sprite MouseOn;
    public Sprite MouseOff;

    [Command (ignoreAuthority = true)]
    public void CmdSurrender(int player_surrendering)
    {
        RpcSurrender(player_surrendering);
    }

    [ClientRpc]
    public void RpcSurrender(int player_surrendering)
    {
        GetComponentInParent<BoardScript>().Surrender(player_surrendering);
    }

    public void OnMouseDown()
    {
        int player_surrendering = GetComponentInParent<BoardScript>().get_player_number();
        CmdSurrender(player_surrendering);
    }

    public void OnMouseOver()
    {
        GetComponent<SpriteRenderer>().sprite = MouseOn;
    }

    public void OnMouseExit()
    {
        GetComponent<SpriteRenderer>().sprite = MouseOff;
    }
}
