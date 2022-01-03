using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;
using System;
using Mirror;
using UnityEngine.Tilemaps;
using System.Linq;


public class Momentum : ShipScript
{
    [ClientRpc]
    override public void RpcStopMining()
    {
        isMining = false;
        UpdateSprite();
        UpdateButton();
        if (isShowingRangeToEnemy)
        {
            ShowNextTurnRange();
        }
    }

    [ClientRpc]
    override public void RpcStartMining()
    {
        building_cycle = (1 + building_cycle) % (buildings.Length + 2);
        isMining = true;
        UpdateSprite();
        UpdateButton();
        if (isShowingRangeToEnemy)
        {
            ShowNextTurnRange();
        }
    }

    override public void ShowNextTurnRange()
    {
        KillSquares();
        isShowingRangeToEnemy = true;
        if (isMining)
        {
            if (!isDeactivated)
                showRange(movement + 1, enemy_movement_square_prefab, enemy_movement_square_prefab);
        }
        else
        {
            if (!isDeactivated)
                showRange(movement, enemy_movement_square_prefab, enemy_movement_square_prefab);
        }

    }

    /*[ClientRpc]
    override public void RpcCashIn(int player_number)
    {
        cashingIn = true;
        GetComponentInParent<BoardScript>().cargo_tracker.GetComponent<TileTracker>().unload();
        GetComponentInParent<BoardScript>().CmdUpdateMeteorCount(0);
        GetComponentInParent<BaseScript>().captured_platinum = GetComponentInParent<BaseScript>().captured_platinum + platCount;

        if (hasAuthority)
            GetComponentInParent<BoardScript>().scoreBoards[player_number].GetComponent<ScoreKeeper>().CmdAddToScore(platCount); //This is a weird way to do this
        UpdatePlatCount(0);
        UpdateMovement(1 + platCount + metalCount);
    }*/

    override public void Mine()
    {
        int mine_value = Math.Min(getRemainingCargoSpace(), miningSpeed);
        if (isMining)
        {
            isMining = false;
            switch (building_cycle)
            {
                case 0:
                    UpdatePlatCount(attackScore + GetComponentInParent<BoardScript>().GetMeteorByPosition(transform.position).GetComponent<Meteor>().Mine(mine_value, this));
                    spaceType = (int)Spaces.BLANK;
                    break;
                //GetComponent<SpriteRenderer>().sprite = isDeactivated ? deactivated : activated; Very Strange you have to do this
                case 1:
                    spaceType = (int)Spaces.BLANK;
                    GetComponentInParent<BoardScript>().GetMeteorByPosition(transform.position).GetComponent<Meteor>().CmdDestroy();
                    float metal = GetComponentInParent<BaseScript>().avaliable_metal + GetComponentInParent<BoardScript>().M_PER_METEOR;
                    GetComponentInParent<BaseScript>().UpdateMetalCount(metal);
                    break;
                default:
                    spaceType = (int)Spaces.BLANK;
                    GameObject structure = GetComponentInParent<BoardScript>().GetStructureByPosition(transform.position);
                    GetComponentInParent<BoardScript>().GetMeteorByPosition(transform.position).GetComponent<Meteor>().CmdDestroy();
                    structure.GetComponent<Structure>().color(GetComponentInParent<BaseScript>().player_number);
                    structure.transform.SetParent(GetComponentInParent<BaseScript>().transform);
                    break;
            }
        }
        UpdateSprite();
        UpdateMovement(attackScore);
        return;
    }
    override public void UpdatePlatCount(int newPlatValue)
    {
        attackScore = newPlatValue;
        cargo_value_display.GetComponent<StatHolder>().UpdateValue(newPlatValue.ToString());
    }
}
