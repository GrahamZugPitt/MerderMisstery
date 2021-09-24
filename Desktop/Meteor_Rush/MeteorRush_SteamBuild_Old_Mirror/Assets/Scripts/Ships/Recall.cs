using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using Mirror;
using UnityEngine.Tilemaps;


public class Recall : ShipScript
{
    [ClientRpc]
    override public void RpcCashIn(int player_number)
    {
        bool base_occupied = !(GetComponentInParent<BoardScript>().GetShipByPosition(GetComponentInParent<BaseScript>().transform.position) == null);
        cashingIn = true;
        GetComponentInParent<BoardScript>().cargo_tracker.GetComponent<TileTracker>().unload();
        GetComponentInParent<BoardScript>().CmdUpdateMeteorCount(0);
        GetComponentInParent<BaseScript>().captured_platinum = GetComponentInParent<BaseScript>().captured_platinum + platCount;
        GetComponentInParent<BoardScript>().scoreBoards[player_number].GetComponent<ScoreKeeper>().AddToScore(platCount);
        GetComponentInParent<BoardScript>().scoreBoards[2].GetComponent<ScoreKeeper>().DecrementShipBoard();
        UpdatePlatCount(0);
        if (!base_occupied)
        {
            transform.position = GetComponentInParent<BaseScript>().transform.position;
        }
        else
        {
            Destroy();
        }
    }
}
