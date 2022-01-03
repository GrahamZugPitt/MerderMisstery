using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using Mirror;


public class Turtle : ShipScript
{

    [ClientRpc]
    override public void RpcStartMining()
    {
        building_cycle = (1 + building_cycle) % (buildings.Length + 2);
        isMining = true;
        isKickable = false;
        GetComponent<SpriteRenderer>().sprite = miningSprite;
        UpdateSprite();
        UpdateButton();
    }

    [ClientRpc]
    override public void RpcStopMining()
    {
        isMining = false;
        isKickable = true;
        UpdateSprite();
        UpdateButton();
    }

    override public void Mine()
    {
        int mine_value = Math.Min(getRemainingCargoSpace(), miningSpeed);
        if (isMining)
        {
            isMining = false;
            isKickable = true;
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
        return;
    }
}
