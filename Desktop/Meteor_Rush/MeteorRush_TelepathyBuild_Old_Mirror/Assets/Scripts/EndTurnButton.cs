using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Mirror;

public class EndTurnButton : NetworkBehaviour
{
    public Sprite EndTurnOn;
    public Sprite EndTurnOff;
    public Sprite EndTurnOnYellow;
    public Sprite EndTurnOffYellow;
    public Sprite NotMyTurn;
    private bool mouseIsOver;

    public void OnMouseOver()
    {
        mouseIsOver = true;

        if (transform.GetComponentInParent<BoardScript>().my_turn() && !GetComponentInParent<BoardScript>().bases[GetComponentInParent<BoardScript>().get_player_number()].GetComponent<BaseScript>().ThingsLeftToDo())
        {
            transform.GetComponent<SpriteRenderer>().sprite = EndTurnOn;
            return;
        }

        if (transform.GetComponentInParent<BoardScript>().my_turn())
        {
            transform.GetComponent<SpriteRenderer>().sprite = EndTurnOnYellow;
            return;
        }

        if (!transform.GetComponentInParent<BoardScript>().my_turn())
            transform.GetComponent<SpriteRenderer>().sprite = NotMyTurn;

    }

    public void OnMouseExit()
    {
        mouseIsOver = false;
    }

    public void OnMouseDown()
    {
        if (transform.GetComponentInParent<BoardScript>().my_turn())
        {
            transform.GetComponentInParent<BoardScript>().Cmd_update_turn();
            transform.GetComponent<SpriteRenderer>().sprite = NotMyTurn;
        }
    }

    public void UpdateButton()
    {
        if (transform.GetComponentInParent<BoardScript>().my_turn() && !GetComponentInParent<BoardScript>().bases[GetComponentInParent<BoardScript>().get_player_number()].GetComponent<BaseScript>().ThingsLeftToDo())
        {
            transform.GetComponent<SpriteRenderer>().sprite = EndTurnOff;
            return;
        }
        if (transform.GetComponentInParent<BoardScript>().my_turn())
        {
            transform.GetComponent<SpriteRenderer>().sprite = EndTurnOffYellow;
            return;
        }
        if (!transform.GetComponentInParent<BoardScript>().my_turn())
            transform.GetComponent<SpriteRenderer>().sprite = NotMyTurn;
    }

    public void Update()
    {
        if (!mouseIsOver)
        {
            UpdateButton();
        }
    }

}
