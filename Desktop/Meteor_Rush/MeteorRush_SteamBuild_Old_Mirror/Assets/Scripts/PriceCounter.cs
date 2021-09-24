using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;
using Mirror;

public class PriceCounter : NetworkBehaviour
{
    public int score = 0;
    public string thisPlayer;
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
        GetComponent<TextMeshPro>().SetText(thisPlayer + ": " + score);
        //GetComponent<TextMeshPro>().color = thisColor;
    }

    [Command(ignoreAuthority = true)]
    public void CmdSetPrice(int newScore)
    {
        RpcSetPrice(newScore);
    }

    [ClientRpc]
    public void RpcSetPrice(int newScore)
    {
        //GetComponent<TextMeshPro>().color = yellow;
        score = newScore;
        GetComponent<TextMeshPro>().SetText(thisPlayer + ": " + score);
        //GetComponent<TextMeshPro>().color = thisColor;
    }

    public void SetUp(int price)
    {
        GetComponent<TextMeshPro>().SetText("" + price);
        GetComponent<TextMeshPro>().color = white;
    }

    /*public void setUp(bool isRed, string pColor)
    {
        if (isRed)
        {
            GetComponent<TextMeshPro>().SetText(pColor + ": " + score);
            GetComponent<TextMeshPro>().color = red;
            thisPlayer = pColor;
            thisColor = red;
            return;
        }
        GetComponent<TextMeshPro>().SetText(pColor + ": " + score);
        GetComponent<TextMeshPro>().color = blue;
        thisPlayer = pColor;
        thisColor = blue;

    }*/

}
