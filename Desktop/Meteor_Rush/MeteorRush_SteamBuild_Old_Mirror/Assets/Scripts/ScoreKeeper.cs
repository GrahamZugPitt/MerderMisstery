using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;
using Mirror;

public class ScoreKeeper : NetworkBehaviour
{
    public int score = 0;
    public Color32 thisColor;
    public Color32 red = new Color32(255, 50, 50, 255);
    public Color32 blue = new Color32(50, 50, 255, 255);
    public Color32 yellow = new Color32(25, 255, 255, 255);
    public Color32 white = new Color32(255, 255, 255, 255);

    [Command (ignoreAuthority = true)]
    public void CmdAddToScore(int newScore)
    {
        RpcAddToScore(newScore);
    }

    [ClientRpc]
    public void RpcAddToScore(int scoreToAdd)
    {
        //GetComponent<TextMeshPro>().color = yellow;
        score = score + scoreToAdd;
        GetComponent<TextMeshPro>().SetText(score.ToString());
        //GetComponent<TextMeshPro>().color = thisColor;
    }

    public void AddToScore(int scoreToAdd)
    {
        score = score + scoreToAdd;
        GetComponent<TextMeshPro>().SetText(score.ToString());
    }

    [Command(ignoreAuthority = true)]
    public void CmdSetScore(int newScore)
    {
        RpcSetScore(newScore);
    }

    [ClientRpc]
    public void RpcSetScore(int newScore)
    {
        //GetComponent<TextMeshPro>().color = yellow;
        score = newScore;
        GetComponent<TextMeshPro>().SetText(score.ToString());
        //GetComponent<TextMeshPro>().color = thisColor;
    }

    public void setUp(int isRed)
    {
        if (isRed == 0)
        {
            GetComponent<TextMeshPro>().SetText(score.ToString());
            GetComponent<TextMeshPro>().color = red;
            thisColor = red;
            return;
        }
        if (isRed == 1)
        {
            GetComponent<TextMeshPro>().SetText(score.ToString());
            GetComponent<TextMeshPro>().color = blue;
            thisColor = blue;
        }
        if (isRed == 2)
        {
            score = 5; //Make this a global or something keeping it here is really bad
            GetComponent<TextMeshPro>().SetText(score.ToString());
            GetComponent<TextMeshPro>().color = white;
            thisColor = white;
        }

    }

    public void DecrementShipBoard()
    {
        score = score - 1;
        GetComponent<TextMeshPro>().SetText(score.ToString());
        if(score == 0)
        {
            GetComponentInParent<BoardScript>().EndGame();
        }
    }

}
