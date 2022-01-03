using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;
using System;
using Mirror;
using UnityEngine.Tilemaps;
using System.Linq;


public class Butterfly : ShipScript
{
    /*public int butterfly_normal_movement = 4;
    public int butterfly_slowed_movement = 1;

    override public void Mine()
    {
        int mine_value = Math.Min(getRemainingCargoSpace(), miningSpeed);
        if (isMining)
        {
            isMining = false;
            switch (findSpaceType())
            {
                case (int)Spaces.METEOR:
                    UpdatePlatCount(platCount + GetComponentInParent<BoardScript>().GetMeteorByPosition(transform.position).GetComponent<Meteor>().Mine(mine_value, this));
                    spaceType = (int)Spaces.BLANK;
                    GetComponent<SpriteRenderer>().sprite = isDeactivated ? deactivated : activated; //Very Strange you have to do this
                    if(platCount > 0 || metalCount > 0)
                    {
                        UpdateMovement(butterfly_slowed_movement);
                    }
                    else
                    {
                        UpdateMovement(butterfly_default_movement);
                    }
                    return;
            }
        }
        UpdateSprite();
        
        if (platCount > 0 || metalCount > 0)
        {
            UpdateMovement(butterfly_slowed_movement);
        }
        else
        {
            UpdateMovement(butterfly_default_movement);
        }
        return;
    }

    override public void ShowNextTurnRange()
    {
        KillSquares();
        isShowingRangeToEnemy = true;
        if (isMining || platCount > 0 || metalCount > 0)
        {
            if (!isDeactivated)
                showRange(butterfly_slowed_movement, enemy_movement_square_prefab, enemy_movement_square_prefab);
        }
        else
        {
            if (!isDeactivated)
                showRange(butterfly_normal_movement, enemy_movement_square_prefab, enemy_movement_square_prefab);
        }

    }

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

    override public void UpdatePlatCount(int newPlatValue)
    {
        if (newPlatValue == 0)
        {
            UpdateMovement(butterfly_normal_movement);
        }
        else
        {
            UpdateMovement(butterfly_slowed_movement);
        }
        if (meteor_text == null)
        {
            meteor_text = Instantiate(text_display, transform.position, Quaternion.identity);
            meteor_text.transform.SetParent(transform);
        }
        platCount = newPlatValue;
        if (newPlatValue == 0)
        {
            meteor_text.GetComponent<TextMeshPro>().SetText("");
            return;
        }
        meteor_text.GetComponent<TextMeshPro>().SetText(platCount.ToString());
    }*/
}
